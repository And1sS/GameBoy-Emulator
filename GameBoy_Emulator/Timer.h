#pragma once

#include <cstdint>

class Memory;

class Timer
{
private:
	Memory* mem;
	uint16_t clock_cycle = 0;

	static constexpr uint16_t clock_divs[4] = { 1024, 16, 64, 256 };
	static constexpr uint16_t clock_div_bits[4] = { 9, 3, 5, 7 };

	enum class OverflowState { NO_OVERFLOW, OVERFLOW_BEGIN, OVERFLOW_END };
	OverflowState overflow_state = OverflowState::NO_OVERFLOW;

	void increment_timer();

	void process_write_to_TIMA(uint8_t value);
	void process_write_to_TMA(uint8_t value);
	void process_write_to_DIV(uint8_t value);
	void process_write_to_TAC(uint8_t value);
public:
	Timer(Memory* mem) : mem(mem) {}
	void execute_one_cycle();

	void process_timer_IO_write(uint16_t addr, uint8_t value);
};

