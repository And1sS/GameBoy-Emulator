#include "PPU.h"
#include "Memory.h"

uint8_t Memory::read_byte(uint16_t addr) const
{
	if (IN_RANGE(addr, ADDR_VRAM_START, ADDR_VRAM_END))
		return ppu->read_byte_VRAM(addr);
	else if (IN_RANGE(addr, ADDR_OAM_START, ADDR_OAM_END))
		return ppu->read_byte_OAM(addr);
	return mem[addr];
}


void Memory::write_byte(uint16_t addr, uint8_t value)
{
	if (IN_RANGE(addr, ADDR_VRAM_START, ADDR_VRAM_END))
		return ppu->write_byte_VRAM(addr, value);
	else if (IN_RANGE(addr, ADDR_OAM_START, ADDR_OAM_END))
		return ppu->write_byte_OAM(addr, value);
	mem[addr] = value;
}


void Memory::write_two_bytes(uint16_t addr, uint16_t value)
{
	write_byte(addr, value & 0xFF);
	write_byte(addr + 1, (value >> 8) & 0xFF);
}

void Memory::write_bytes(uint16_t addr, std::initializer_list<uint8_t> l)
{
	std::copy(l.begin(), l.end(), mem + addr);
}

uint8_t Memory::read_IO_byte(uint16_t addr)
{
	if (!IN_RANGE(addr, ADDR_IO_START, ADDR_IO_END))
		throw std::invalid_argument("address should be in IO range");
	return mem[addr];
}

void Memory::set_IO_flag(uint16_t addr, uint8_t index)
{
	if (!IN_RANGE(addr, ADDR_IO_START, ADDR_IO_END))
		throw std::invalid_argument("address should be in IO range");

	mem[addr] |= (1 << index);
}

void Memory::reset_IO_flag(uint16_t addr, uint8_t index)
{
	if (!IN_RANGE(addr, ADDR_IO_START, ADDR_IO_END))
		throw std::invalid_argument("address should be in IO range");

	mem[addr] &= ~(0 << index);
}

bool Memory::get_IO_flag(uint16_t addr, uint8_t index)
{
	if (!IN_RANGE(addr, ADDR_IO_START, ADDR_IO_END))
		throw std::invalid_argument("address should be in IO range");

	return mem[addr] & (1 << index);
}
