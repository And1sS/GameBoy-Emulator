#pragma once

#include "../Cartridge.h"

class MBC1 : public Cartridge
{
private:
	uint8_t number_of_rom_banks;
	uint8_t bank_addr_bits;

	bool large_rom;
	bool large_ram;

	uint8_t lower_rom_bank_number = 1;
	uint8_t ram_or_upper_rom_bank_number = 0;
	uint8_t banking_mode = 0; // 1 - ram banking mode

	bool ram_enabled = false;
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
	large_rom = (data[0x148] >= 5); // >= 1MB
	large_ram = (data[0x149] >= 3); // >= 32kB 
}


void MBC1::write_byte(uint16_t addr, uint8_t value)
{
	if (IN_RANGE(addr, Memory::ADDR_BANK_0_START, Memory::ADDR_SWITCH_BANK_END))
	{
		if (IN_RANGE(addr, 0x0000, 0x1FFF)) // 0000-1FFF - RAM Enable (Write Only)
			ram_enabled = ((value & 0xF) == 0xA);
		else if (IN_RANGE(addr, 0x2000, 0x3FFF)) // 2000-3FFF - ROM Bank Number (Write Only)
		{
			value &= 0b11111;
			lower_rom_bank_number = value;
			if (bank_addr_bits < 5)
				lower_rom_bank_number = ((value << (8 - bank_addr_bits)) & 0xFF) >> (8 - bank_addr_bits);

			if (lower_rom_bank_number == 0)
				lower_rom_bank_number = 1;

			uint8_t upper_rom_bank_number = ram_or_upper_rom_bank_number;
			if (!large_rom || (banking_mode == 1)) // ram banking mode
				upper_rom_bank_number = 0;
			current_rom_bank = cartridge.data() + ((upper_rom_bank_number << 5)
				+ lower_rom_bank_number) * 16 * KB;
		}
		else if (IN_RANGE(addr, 0x4000, 0x5FFF)) // 4000-5FFF - RAM Bank Number - or - Upper Bits of ROM Bank Number (Write Only)
		{
			value &= 0b11;
			ram_or_upper_rom_bank_number = value;
			if (large_rom && (banking_mode == 0)) // rom banking mode
			{
				current_rom_bank = cartridge.data() + ((ram_or_upper_rom_bank_number << 5)
					+ lower_rom_bank_number) * 16 * KB;
			}
			if (large_ram && (banking_mode == 1)) // ram banking mode
				current_ram_bank = external_RAM.data() + ram_or_upper_rom_bank_number * 8 * KB;
		}
		else if (IN_RANGE(addr, 0x6000, 0x7FFF)) // 6000-7FFF - Banking Mode Select (Write Only)
		{
			if (!large_rom && !large_ram)
				return;
			value &= 1;
			banking_mode = value;
			if (banking_mode == 0) // rom banking mode
				current_ram_bank = external_RAM.data();
			else if (large_ram) // ram banking mode
				current_ram_bank = external_RAM.data() + ram_or_upper_rom_bank_number * 8 * KB;
		}
	}
	else if (ram_enabled)
		Cartridge::write_byte(addr, value);
}