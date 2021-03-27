#pragma once

#include <array>
#include <atomic>
#include <memory>

#include "Sound/Generator.h"

class Memory;

class APU
{
public:
	static constexpr size_t BIT_RATE    = 44100;
	static constexpr size_t BUFFER_SIZE = 100 * BIT_RATE;
	static constexpr double TIME_STEP   = 1.0 / 1048576;
	static constexpr double SOUND_STEP  = 1.0 / BIT_RATE;

	APU(Memory* mem);
	void execute_one_cycle();
	void process_sound_IO_write(uint16_t addr, uint8_t value);
	void update_generator_status(uint8_t generator_number, bool status);
	void get_wave_pattern_ram(std::array<uint16_t, 32>& dest);

	const std::array<int16_t, BUFFER_SIZE>& get_sound_buffer();
	size_t get_cur_pos();

private:
	Memory* mem;
	std::array<std::unique_ptr<Generator>, 4> generators;

	double current_time = 0;

	std::array<int16_t, BUFFER_SIZE> sound_buffer;
	std::atomic_size_t cur_pos = 0;

	double volume = 1; // 0 to 1
	uint16_t global_on_off = 1;
	std::array<uint16_t, 4> channel_on_off = { 1, 1, 1, 1 };

	void process_NR5x_write(uint16_t addr, uint8_t value);
};

inline const std::array<int16_t, APU::BUFFER_SIZE>& APU::get_sound_buffer()
{
	return sound_buffer;
}

inline size_t APU::get_cur_pos()
{
	return cur_pos;
}