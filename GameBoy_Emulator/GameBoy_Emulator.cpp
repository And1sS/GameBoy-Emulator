#include "Memory.h"
#include "CPU.h"
#include "PPU.h"
#include "Timer.h"

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class Emulator : public olc::PixelGameEngine
{
private:
	float fTargetFrameTime = 1.0f / 59.73f;
	float fAccumulatedTime = 0.0f;

	Memory* mem;
	Timer* timer;
	CPU* cpu;
	PPU* ppu;

	PPU::Mode prev_mode = PPU::Mode::DISABLED;

	std::string rom_filename;

public:
	Emulator(std::string rom_filename) : rom_filename(rom_filename)
	{
		sAppName = "GameBoy Emulator";
	}

	bool OnUserCreate() override
	{
		try
		{
			std::ifstream in(rom_filename, std::ios::binary);

			mem = new Memory(in);
			timer = new Timer(mem);
			cpu = new CPU(mem, timer);
			ppu = new PPU(mem);
			mem->set_timer(timer);
			mem->set_PPU(ppu);
		}
		catch (const std::runtime_error& e)
		{
			std::cout << e.what() << std::endl;
			std::cout << "Enter any character to close" << std::endl;
			char c;
			std::cin >> c;
			return false;
		}

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		fAccumulatedTime += fElapsedTime;
		if (fAccumulatedTime >= fTargetFrameTime)
		{
			fAccumulatedTime -= fTargetFrameTime;
			fElapsedTime = fTargetFrameTime;
		}
		else
			return true; // Don't do anything this frame

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
			mem->execute_one_cycle();
			timer->execute_one_cycle();

			uint8_t byte = mem->read_IO_byte(0xFF00);
			uint8_t result = byte;
			if ((~byte) & (1 << 5))
			{
				result = (byte & 0xF0) | ~(
					(GetKey(olc::Key::ENTER).bHeld << 3)|
					(GetKey(olc::Key::SHIFT).bHeld << 2)|
					(GetKey(olc::Key::A).bHeld << 1)|
					(GetKey(olc::Key::S).bHeld));
			}
			else if ((~byte) & (1 << 4))
			{
				result = (byte & 0xF0) | ~(
					(GetKey(olc::Key::DOWN).bHeld << 3) |
					(GetKey(olc::Key::UP).bHeld << 2) |
					(GetKey(olc::Key::LEFT).bHeld << 1) |
					(GetKey(olc::Key::RIGHT).bHeld));
			}
			mem->write_IO_byte(0xFF00, result);
		}

		static const olc::Pixel pixels[] = { {224, 248, 208}, {136, 192, 112}, {52, 104, 86}, {8, 24, 32} };
		const auto& screen = ppu->get_screen_buffer();
		for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
				Draw(x, y, pixels[screen[y][x]]);
		return true;
	}
};


int main(int argc, char** argv)
{
	std::string rom_filename = "roms/donkey_kong_land.gb";
	if (argc > 1)
		rom_filename = std::string(argv[1]);

	Emulator emulator(rom_filename);
	if (emulator.Construct(160, 144, 4, 4))
		emulator.Start();

	return 0;
}


