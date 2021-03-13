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
        std::make_unique<ToneGenerator>(true),
        std::make_unique<ToneGenerator>(false),
        std::make_unique<MockedGenerator>(),
        std::make_unique<MockedGenerator>()
    };
};

void APU::process_sound_IO_write(uint16_t addr, uint8_t value)
{
    size_t generator_index;
    if (IN_RANGE(addr, Memory::ADDR_IO_NR10, Memory::ADDR_IO_NR14))
        generator_index = 0;
    else if (IN_RANGE(addr, Memory::ADDR_IO_NR21, Memory::ADDR_IO_NR24))
        generator_index = 1;
    else if (IN_RANGE(addr, Memory::ADDR_IO_NR41, Memory::ADDR_IO_NR44))
        generator_index = 3;
    else
        return;

    generators[generator_index]->process_sound_IO_write(addr, value); 
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