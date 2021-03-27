#include "APU.h"
#include "Memory.h"
#include "Sound\Generator.h"
#include "Sound\ToneGenerator.h"
#include "Sound\NoiseGenerator.h"
#include "Sound\WaveGenerator.h"

#include <fstream>
#include <cstdlib>


APU::APU(Memory* mem) : mem(mem)
{
	generators =
	{
		std::make_unique<ToneGenerator>(this, 0, true),
		std::make_unique<ToneGenerator>(this, 1, false),
		std::make_unique<WaveGenerator>(this, 2),
		std::make_unique<MockGenerator>(this, 3)
	};
};

void APU::process_sound_IO_write(uint16_t addr, uint8_t value)
{
	size_t generator_index;
	if (IN_RANGE(addr, Memory::ADDR_IO_NR10, Memory::ADDR_IO_NR14))
		generator_index = 0;
	else if (IN_RANGE(addr, Memory::ADDR_IO_NR21, Memory::ADDR_IO_NR24))
		generator_index = 1;
	else if (IN_RANGE(addr, Memory::ADDR_IO_NR30, Memory::ADDR_IO_NR34))
		generator_index = 2;
	else if (IN_RANGE(addr, Memory::ADDR_IO_NR41, Memory::ADDR_IO_NR44))
		generator_index = 3;
	else if (IN_RANGE(addr, Memory::ADDR_IO_NR50, Memory::ADDR_IO_NR52))
	{
		process_NR5x_write(addr, value);
		return;
	}
	else
		return;

	generators[generator_index]->process_sound_IO_write(addr, value); 
}

void APU::process_NR5x_write(uint16_t addr, uint8_t value)
{
	if (addr == Memory::ADDR_IO_NR50)
	{
		int8_t volume1 = (value >> 4) & 0b111;
		int8_t volume2 = value & 0b111;
		volume = ((volume1 > volume2) ? volume1 : volume2) / 7.0;
	}
	else if (addr == Memory::ADDR_IO_NR51)
	{
		for (uint8_t i = 0; i < 4; i++)
			channel_on_off[i] = ((value >> i) | (value >> (i + 4))) & 1;
	}
	else
	{
		global_on_off = GET_BIT(value, 7);
	}
}

void APU::update_generator_status(uint8_t generator_number, bool status)
{
	uint8_t value = mem->read_IO_byte(Memory::ADDR_IO_NR52);
	if (status == false)
		value &= ~(1 << generator_number);
	else
		value |= (1 << generator_number);
	mem->write_IO_byte(Memory::ADDR_IO_NR52, value);
}

void APU::get_wave_pattern_ram(std::array<uint16_t, 32>& dest)
{
	for (uint8_t i = 0; i < 16; i++)
	{
		uint8_t temp = mem->read_IO_byte(Memory::ADDR_IO_WAVE_PATTERN_RAM_START + i);
		dest[i * 2] = temp >> 4;
		dest[i * 2 + 1] = temp & 0xF;
	}
}

void APU::execute_one_cycle()
{
	current_time += TIME_STEP;

	while (current_time > SOUND_STEP) 
	{
		// generate 1 sound sample
		current_time -= SOUND_STEP;
		int32_t sample_sum = 0;
		for (uint8_t i = 0; i < 4; i++)
			sample_sum += channel_on_off[i] * generators[i]->generate_sample(SOUND_STEP);
		   
		sound_buffer[cur_pos] = sample_sum * volume / 4;
		cur_pos = (cur_pos + 1) % sound_buffer.size();
	}

	/*if (cur_pos == 40 * BIT_RATE)
	{
		std::ofstream out("out.txt");
		for (size_t i = 0; i < cur_pos; i++)
		{
			out << sound_buffer[i] << " ";
		}
		exit(1);
	}*/
}