#pragma once

#include <cstdint>
#include <initializer_list>
#include <algorithm>

class Memory
{
private:
	uint8_t mem[0xFFFF];
public:
	inline uint8_t read_byte(uint16_t addr) const
	{
		return mem[addr];
	}

	void write_byte(uint16_t addr, uint8_t value)
	{
		mem[addr] = value;
	}

	void write_two_bytes(uint16_t addr, uint16_t value)
	{
		mem[addr] = value & 0xFF;
		mem[addr + 1] = (value >> 8) & 0xFF;
	}

	void write_bytes(uint16_t addr, std::initializer_list<uint8_t> l)
	{
		std::copy(l.begin(), l.end(), mem + addr);
	}
};

