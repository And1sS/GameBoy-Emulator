#pragma once

#include <array>
#include <atomic>
#include <memory>

#include "Sound/Generator.h"

class Memory;

class APU
{
public:
	static constexpr size_t BIT_RATE = 44100;
	static constexpr size_t BUFFER_SIZE = 100 * BIT_RATE;
	static constexpr double TIME_STEP = 1.0 / 1048576;
	static constexpr double SOUND_STEP = 1.0 / BIT_RATE;

private:
	Memory* mem;
	std::array<std::unique_ptr<Generator>, 4> generators;

	double current_time = 0;

	std::array<int16_t, BUFFER_SIZE> sound_buffer;
	std::atomic_size_t cur_pos = 0;

public:
	APU(Memory* mem);
	void execute_one_cycle();
	void process_sound_IO_write(uint16_t addr, uint8_t value);

	const std::array<int16_t, BUFFER_SIZE>& get_sound_buffer();
	size_t get_cur_pos();
};

inline const std::array<int16_t, APU::BUFFER_SIZE>& APU::get_sound_buffer()
{
	return sound_buffer;
}

inline size_t APU::get_cur_pos()
{
	return cur_pos;
}