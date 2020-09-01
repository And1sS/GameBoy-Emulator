#pragma once

#include "../Cartridge.h"

class MBC1 : public Cartridge
{
private:
	uint8_t number_of_rom_banks;
	uint8_t bank_addr_bits;
public:
	void write_byte(uint16_t addr, uint8_t value);

	MBC1(std::vector<uint8_t> data);
};


MBC1::MBC1(std::vector<uint8_t> data) : Cartridge(data)
{
	number_of_rom_banks = data.size() / (16 * KB);
	bank_addr_bits = 0;
	uint8_t temp = number_of_rom_banks;
	while (temp > 1)
	{
		temp >>= 1;
		bank_addr_bits++;
	}
}


void MBC1::write_byte(uint16_t addr, uint8_t value)
{
	if (IN_RANGE(addr, Memory::ADDR_BANK_0_START, Memory::ADDR_SWITCH_BANK_END))
	{
		if (IN_RANGE(addr, 0x0000, 0x1FFF)) // 0000-1FFF - RAM Enable (Write Only)
			;// TODO
		else if (IN_RANGE(addr, 0x2000, 0x3FFF)) // 2000-3FFF - ROM Bank Number (Write Only)
		{
			uint8_t bank_number = ((value << (8 - bank_addr_bits)) & 0xFF) >> (8 - bank_addr_bits);
			if (bank_number == 0)
				bank_number = 1;
			current_bank = cartridge.data() + bank_number * 16 * KB;
		}
		else if (IN_RANGE(addr, 0x4000, 0x5FFF)) // 4000-5FFF - RAM Bank Number - or - Upper Bits of ROM Bank Number (Write Only)
			;// TODO
		else if (IN_RANGE(addr, 0x6000, 0x7FFF)) // 6000-7FFF - Banking Mode Select (Write Only)
			;// TODO
	}
	else
		Cartridge::write_byte(addr, value);
}