#include "APU.h"
#include "Memory.h"
#include "Sound\Generator.h"
#include "Sound\ToneGenerator.h"
#include "Sound\NoiseGenerator.h"

#include <fstream>
#include <cstdlib>


APU::APU(Memory* mem) : mem(mem)
{
    generators =
    {
        std::make_unique<MockedGenerator>(),
        std::make_unique<ToneGenerator>(false),
        std::make_unique<MockedGenerator>(),
        std::make_unique<MockedGenerator>()
    };
};

void APU::process_sound_IO_write(uint16_t addr, uint8_t value)
{
    if (IN_RANGE(addr, Memory::ADDR_IO_NR50, Memory::ADDR_IO_NR52))
        return;

    // integer division used on purpose
    generators[(addr - Memory::ADDR_IO_NR10) / 5]->process_sound_IO_write(addr, value); 
}

void APU::execute_one_cycle()
{
    current_time += TIME_STEP;

	while (current_time > SOUND_STEP) 
	{
        // generate 1 sound sample
        current_time -= SOUND_STEP;
        int16_t sample_sum = 0;
        for (const auto& generator : generators)
            sample_sum += generator->generate_sample(SOUND_STEP) / 4;
           
        sound_buffer[cur_pos] = sample_sum;
        cur_pos = (cur_pos + 1) % sound_buffer.size();
	}

    /*if (cur_pos == 20 * BIT_RATE)
    {
        std::ofstream out("out.txt");
        for (size_t i = 0; i < cur_pos; i++)
        {
            out << sound_buffer[i] << " ";
        }
        exit(1);
    }*/
}