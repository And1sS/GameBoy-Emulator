#include "Timer.h"
#include "Memory.h"
#include "util.h"

void Timer::execute_one_cycle()
{
	clock_cycle += 4;

	if (clock_cycle % 256 == 0)
		mem->write_IO_byte(Memory::ADDR_IO_DIV, clock_cycle >> 8);

	if (overflow_state == OverflowState::OVERFLOW_BEGIN)
	{
		overflow_state = OverflowState::OVERFLOW_END;
		mem->write_byte(Memory::ADDR_IO_TIMA, mem->read_IO_byte(Memory::ADDR_IO_TMA));
		mem->set_IO_flag(Memory::ADDR_IO_IF, 2); // Bit 2: Timer  Interrupt Request(INT 50h)  (1 = Request)
	}
	else if (overflow_state == OverflowState::OVERFLOW_END)
		overflow_state = OverflowState::NO_OVERFLOW;

	uint8_t TAC = mem->read_IO_byte(Memory::ADDR_IO_TAC);

	if (!GET_BIT(TAC, 2)) // Bit  2 - Timer Enable
		return;

	if (clock_cycle % clock_divs[TAC & 0b11] == 0)
		increment_timer();
}

void Timer::increment_timer()
{
	uint8_t timer_val = mem->read_IO_byte(Memory::ADDR_IO_TIMA) + 1;
	if (timer_val == 0)
		overflow_state = OverflowState::OVERFLOW_BEGIN;
	mem->write_IO_byte(Memory::ADDR_IO_TIMA, timer_val);
}

void Timer::process_timer_IO_write(uint16_t addr, uint8_t value)
{
	if (addr == Memory::ADDR_IO_TIMA)
		process_write_to_TIMA(value);
	else if (addr == Memory::ADDR_IO_TMA)
		process_write_to_TMA(value);
	else if (addr == Memory::ADDR_IO_DIV)
		process_write_to_DIV(value);
	else if (addr == Memory::ADDR_IO_TAC)
		process_write_to_TAC(value);
}

void Timer::process_write_to_TIMA(uint8_t value)
{
	switch (overflow_state)
	{
	case OverflowState::OVERFLOW_BEGIN: // break omitted intentionally
		overflow_state = OverflowState::NO_OVERFLOW;
	case OverflowState::NO_OVERFLOW:
		mem->write_IO_byte(Memory::ADDR_IO_TIMA, value);
		break;
	case OverflowState::OVERFLOW_END:
		break;
	}
}

void Timer::process_write_to_TMA(uint8_t value)
{
	mem->write_IO_byte(Memory::ADDR_IO_TMA, value);
	if (overflow_state == OverflowState::OVERFLOW_END)
		mem->write_byte(Memory::ADDR_IO_TIMA, value);
}


void Timer::process_write_to_DIV(uint8_t value)
{
	uint8_t TAC = mem->read_IO_byte(Memory::ADDR_IO_TAC);

	if (GET_BIT(TAC, 2) &&  // Bit  2 - Timer Enable
		GET_BIT(clock_cycle, clock_div_bits[TAC & 0b11]) == 1)
		increment_timer();

	clock_cycle = 0;
	mem->write_IO_byte(Memory::ADDR_IO_DIV, 0);
}

void Timer::process_write_to_TAC(uint8_t new_TAC)
{
	uint8_t old_TAC = mem->read_IO_byte(Memory::ADDR_IO_TAC);
	uint8_t old_bit = GET_BIT(clock_cycle, clock_div_bits[old_TAC & 0b11]);
	uint8_t new_bit = GET_BIT(clock_cycle, clock_div_bits[new_TAC & 0b11]);

	bool glitch;
	if (GET_BIT(old_TAC, 2) == 0) // Bit  2 - Timer Enable
		glitch = false;
	else
	{
		if (GET_BIT(new_TAC, 2) == 0)
			glitch = (old_bit == 1);
		else
			glitch = (old_bit == 1) && (new_bit == 0);
	}

	if (glitch)
		increment_timer();

	mem->write_IO_byte(Memory::ADDR_IO_TAC, new_TAC);
}