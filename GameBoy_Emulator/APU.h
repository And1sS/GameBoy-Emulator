#pragma once

#include <array>
#include <atomic>

class Memory;

class APU
{
public:
	static constexpr size_t BIT_RATE = 44100;
	static constexpr size_t BUFFER_SIZE = 3 * BIT_RATE;
private:
	Memory* mem;

	uint64_t clock_cycles = 0;

	std::array<int16_t, BUFFER_SIZE> sound_buffer;
	std::atomic_size_t cur_pos = 0;
	// change constant 23 in execute_one_cycle if you want to change bit rate
public:
	APU(Memory* mem) : mem(mem) {};
	void execute_one_cycle();

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