#include <cmath>

#include "WaveGenerator.h"
#include "..\APU.h"
#include "..\Memory.h"

WaveGenerator::WaveGenerator(APU* apu, uint8_t number) : Generator(apu, number)
{ }

void WaveGenerator::calculate_frequency()
{
	// Frequency = 65536/(2048-x) Hz (frequency_specifier == x)
	frequency = 65536 / (2048 - frequency_specifier);
	period = 1 / frequency;
	period_accumulated_time = 0;
	frequency_changed = true;
}

void WaveGenerator::process_sound_IO_write(uint16_t addr, uint8_t value)
{
	if (addr == Memory::ADDR_IO_NR30)
	{
		power_on = GET_BIT(value, 7); // Bit 7 - Sound Channel 3 Off  (0=Stop, 1=Playback)  (Read/Write)
	}
	else if (addr == Memory::ADDR_IO_NR31)
	{
	    // Sound Length = (256 - t1) * (1 / 256) seconds This value is used only if Bit 6 in NR34 is set.
		// Bit 7-0 - Sound length (t1: 0 - 255)
		sound_length = (256 - value) / 256.0;
		sound_length_accumulated_time = 0;
	}
	else if (addr == Memory::ADDR_IO_NR32)
	{
		// Bits 6-5 - Select output level (Read / Write)
		static const std::array<float, 4> multipliers = { 0, 1, 0.5, 0.25 };
		volume_multiplier = multipliers[(value >> 5) & 0b11];
	}
	else if (addr == Memory::ADDR_IO_NR33)
	{
		// Frequency's lower 8 bits of 11 bit data (x). Next 3 bits are in NR34
		frequency_specifier = (frequency_specifier & (0b111 << 8)) | value;

		calculate_frequency();
	}
	else if (addr == Memory::ADDR_IO_NR34)
	{
		bool restart = GET_BIT(value, 7);   // Bit 7 - Initial (1=Restart Sound) (Write Only);
		// initialize
		if (power_on && restart)
		{
			turned_on = true;
			sound_length_accumulated_time = 0;

			apu->get_wave_pattern_ram(samples_array);

			apu->update_generator_status(number, true);
		}

		infinite_sound = !GET_BIT(value, 6); // Bit 6 - Counter/consecutive selection (Read/Write)
											 // (1 = Stop output when length in NR21 expires)

		// Bit 2-0 - Frequency's higher 3 bits (x) (Write Only)
		frequency_specifier = (frequency_specifier & 0xFF) | ((value & 0b111) << 8);

		calculate_frequency();
	}
}

int16_t WaveGenerator::generate_sample(double elapsed_time)
{
	if (!turned_on)
		return 0;

	if (!handle_sound_length(elapsed_time))
		return 0;

	return _generate_sample(elapsed_time);
};