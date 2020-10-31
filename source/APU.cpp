#include "APU.h"
#include "Memory.h"
#include "Sound\Generator.h"
#include "Sound\ToneGenerator.h"

APU::APU(Memory* mem) : mem(mem)
{
    for (uint8_t i = 0; i < 4; i++) 
        generators[i] = std::make_unique<MockedGenerator>();

    generators[1] = std::make_unique<ToneGenerator>();
};

void APU::process_sound_IO_write(uint16_t addr, uint8_t value)
{
    if (IN_RANGE(addr, Memory::ADDR_IO_NR50, Memory::ADDR_IO_NR52))
        return;

    generators[(addr - Memory::ADDR_IO_NR10) / 5]->process_sound_IO_write(addr, value); // integer division used on purpose
}

void APU::execute_one_cycle()
{
    static float phase = 0;
    static constexpr int w = 5000;
    current_time += TIME_STEP;

	while (current_time > SOUND_STEP) 
	{
        // generate 1 sound sample
        current_time -= SOUND_STEP;
        int16_t sample_sum = 0;
        for (const auto& generator : generators)
            sample_sum += generator->generate_sample(SOUND_STEP) / 4;
           

        sound_buffer[(cur_pos + 1) % sound_buffer.size()] = sample_sum; // 2000 * sin(phase);
        //phase += w * SOUND_STEP;
        //if (phase > 2 * 3.1415)
        //    phase -= 2 * 3.1415;
        cur_pos = (cur_pos + 1) % sound_buffer.size();
	}
}