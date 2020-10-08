#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <atomic>

#include "Memory.h"
#include "CPU.h"
#include "PPU.h"
#include "APU.h"
#include "Timer.h"

class Stream : public sf::SoundStream
{
private:
    std::vector<int16_t> temp;

    size_t last_read_pos = 0;
    APU* apu;

public:
    Stream(APU* apu) : apu(apu)
    {
        initialize(1, APU::BIT_RATE);
    }

    bool onGetData(Chunk& data) override
    {
        while (apu->get_cur_pos() == last_read_pos)
            sf::sleep(sf::milliseconds(10));

        size_t cur_pos = apu->get_cur_pos();
        int length = cur_pos - last_read_pos;
        if (cur_pos < last_read_pos)
            length = cur_pos + (APU::BUFFER_SIZE - last_read_pos);

        if (length > APU::BUFFER_SIZE / 2)
        {
            last_read_pos = cur_pos;

            while (apu->get_cur_pos() == last_read_pos)
                sf::sleep(sf::milliseconds(10));

            cur_pos = apu->get_cur_pos();
            int length = cur_pos - last_read_pos;
            if (cur_pos < last_read_pos)
                length = cur_pos + (APU::BUFFER_SIZE - last_read_pos);
        }

        if (length > 2000)
            length = 2000;
        int new_pos = (last_read_pos + length) % APU::BUFFER_SIZE;

        if (new_pos > last_read_pos)
        {
            data.sampleCount = new_pos - last_read_pos;
            data.samples = apu->get_sound_buffer().data() + last_read_pos;
        }
        else
        {
            temp.clear();
            temp.resize(new_pos + (APU::BUFFER_SIZE - last_read_pos));

            const auto& sound_buffer = apu->get_sound_buffer();
            std::copy(sound_buffer.begin() + last_read_pos, sound_buffer.end(), temp.begin());
            std::copy(sound_buffer.begin(), sound_buffer.begin() + new_pos,
                temp.begin() + (APU::BUFFER_SIZE - last_read_pos));

            data.sampleCount = temp.size();
            data.samples = temp.data();
        }

        last_read_pos = new_pos;
        return true;
    }

    void onSeek(sf::Time timeOffset) override {}
};

int main(int argc, char** argv)
{
    sf::RenderWindow window(sf::VideoMode(160, 144), "SFML works!");
    sf::Texture t;
    std::vector<uint8_t> screen_buffer(160 * 144 * 4);
    for (int i = 0; i < 160 * 144 * 4; i += 4)
    {
        screen_buffer[i] = 0;
        screen_buffer[i + 1] = 0;
        screen_buffer[i + 2] = 255;
        screen_buffer[i + 3] = 255;
    }
    t.create(160, 144);
    sf::Sprite sprite(t);
    t.update(screen_buffer.data());

    sf::Clock clock;
    
    int k = 0;
    int64_t elapsed_time = 0;

    float fTargetFrameTime = 1.0f / 59.73f;
    float fAccumulatedTime = 0.0f;

    std::string rom_filename = "roms/cpu_instrs.gb";
    if (argc > 1)
        rom_filename = std::string(argv[1]);

    Memory* mem;
    Timer* timer;
    CPU* cpu;
    PPU* ppu;
    APU* apu;

    PPU::Mode prev_mode = PPU::Mode::DISABLED;

    try
    {
        std::ifstream in(rom_filename, std::ios::binary);

        mem = new Memory(in);
        timer = new Timer(mem);
        cpu = new CPU(mem, timer);
        ppu = new PPU(mem);
        apu = new APU(mem);

        mem->set_timer(timer);
        mem->set_PPU(ppu);
        mem->set_APU(apu);
    }
    catch (const std::runtime_error& e)
    {
        std::cout << e.what() << std::endl;
        std::cout << "Enter any character to close" << std::endl;
        char c;
        std::cin >> c;
        return 1;
    }

    Stream stream(apu);
    stream.play();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::Resized)
            {
                if (event.size.width * 1.0 / event.size.height > 160.0 / 144)
                    ;
            }
        }

        sf::Time elapsed = clock.restart();
        elapsed_time += elapsed.asMicroseconds();
        k++;
        if (k == 1000)
        {
            // std::cout << 1.0 * k / elapsed_time * 1000000.0 << std::endl;
            k = 0;
            elapsed_time = 0;
        }

        fAccumulatedTime += elapsed.asSeconds();
        if (fAccumulatedTime >= fTargetFrameTime)
            fAccumulatedTime -= fTargetFrameTime;
        else
            continue; // Don't do anything this frame

        while (true)
        {
            if (ppu->get_mode() == PPU::Mode::V_BLANK && prev_mode != ppu->get_mode())
            {
                prev_mode = ppu->get_mode();
                break;
            }
            prev_mode = ppu->get_mode();

            cpu->execute_one_cycle();
            ppu->execute_one_cycle();
            apu->execute_one_cycle();
            mem->execute_one_cycle();
            timer->execute_one_cycle();

            uint8_t byte = mem->read_IO_byte(0xFF00);
            uint8_t result = byte;
            if ((~byte) & (1 << 5))
            {
                result = (byte & 0xF0) | ~(
                    (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) << 3) |
                    ((sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
                        sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)) << 2) |
                    (sf::Keyboard::isKeyPressed(sf::Keyboard::A) << 1) |
                    (sf::Keyboard::isKeyPressed(sf::Keyboard::S)));
            }
            else if ((~byte) & (1 << 4))
            {
                result = (byte & 0xF0) | ~(
                    (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) << 3) |
                    (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) << 2) |
                    (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) << 1) |
                    sf::Keyboard::isKeyPressed(sf::Keyboard::Right));
            }
            mem->write_IO_byte(0xFF00, result);
        }
       
        struct pixel { uint8_t r, g, b, a; };
        static const pixel pixels[] = 
        {
            {224, 248, 208, 255}, 
            {136, 192, 112, 255},
            {52, 104, 86, 255},
            {8, 24, 32, 255} 
        };
        
        const auto& screen = ppu->get_screen_buffer();
        for (int x = 0; x < 160; x++)
            for (int y = 0; y < 144; y++)
                memcpy(screen_buffer.data() + (y * 160 + x) * 4,
                    pixels + screen[y][x], sizeof(pixel));
        t.update(screen_buffer.data());


        window.clear();
        window.draw(sprite);
        window.display();
    }
}