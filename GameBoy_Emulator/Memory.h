#pragma once

#include <cstdint>

class Memory
{
private:
	uint8_t mem[0xFFFF];

public:
	inline uint8_t read_byte(uint16_t addr) const
	{
		return mem[addr];
	};

	void write_byte(uint16_t addr, uint8_t value)
	{
		mem[addr] = value;
	};
};

