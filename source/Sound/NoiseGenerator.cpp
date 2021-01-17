#include <cmath>

#include "NoiseGenerator.h"
#include "..\util.h"
#include "..\Memory.h"

void NoiseGenerator::process_sound_IO_write(uint16_t addr, uint8_t value)
{
	if (addr == Memory::ADDR_IO_NR41)
	{
		// Sound Length = (64-t1)*(1/256) seconds The Length value is used only if Bit 6 in NR44 is set.
		// Bit 5 - 0 - Sound length data (Write Only) (t1: 0 - 63)
		sound_length = (64 - (value & 0b111111)) * 1.0 / 256;
	}
	else if (addr == Memory::ADDR_IO_NR42)
	{
		envelope_initial_volume = value >> 4;				// Bit 7-4 - Initial Volume of envelope (0-0Fh) (0=No Sound)
		envelope_direction = GET_BIT(value, 3) ? 1 : -1;	// Bit 3   - Envelope Direction (0=Decrease, 1=Increase)

		// Length of 1 step = n*(1/64) seconds
		// Bit 2-0 - Number of envelope sweep (n: 0-7) (If zero, stop envelope operation.)
		envelope_step = (value & 0b111) * 1.0 / 64;
	}
	else if (addr == Memory::ADDR_IO_NR43)
	{
		uint8_t s = (value >> 4) & 0b1111;          // Bit 7-4 - Shift Clock Frequency (s)
		counter_width = GET_BIT(value, 3) ? 7 : 15; // Bit 3 - Counter Step / Width (0 = 15 bits, 1 = 7 bits)
		double r = value & 0b111;                  // Bit 2-0 - Dividing Ratio of Frequencies (r)

		if (r == 0)
			r = 0.5;

		period = 1.0 * (1 << (s + 1)) * r / 524288;
	}
	else if (addr == Memory::ADDR_IO_NR44)
	{
		turned_on = GET_BIT(value, 7);   // Bit 7 - Initial (1=Restart Sound) (Write Only);
		if (turned_on)
		{
			current_volume = envelope_initial_volume;
			accumulated_time = 0;
			envelope_accumulated_time = 0;
		}

		infinite_sound = !GET_BIT(value, 6); // Bit 6 - Counter/consecutive selection (Read/Write)
											 // (1 = Stop output when length in NR41 expires)
	}
}

int16_t NoiseGenerator::generate_sample(double elapsed_time)
{
	if (!turned_on)
		return 0;

	update_time(elapsed_time);
	envelope_accumulated_time += elapsed_time;
	shift_accumulated_time    += elapsed_time;

	if (!infinite_sound && accumulated_time > sound_length)
	{
		accumulated_time = 0;
		turned_on = false;

		return 0;
	}

	if (envelope_step != 0)
		while (envelope_accumulated_time >= envelope_step)
		{
			envelope_accumulated_time -= envelope_step;
			current_volume += envelope_direction;
			if (current_volume > 15)
				current_volume = 15;
			else if (current_volume < 0)
				current_volume = 0;
		}

	while (shift_accumulated_time >= period)
	{
		shift_accumulated_time -= period;
		uint16_t bit = (shift_register ^ (shift_register >> 1)) & 1;
		shift_register >>= 1;
		shift_register |= bit << counter_width;
	}

	int16_t sample = (~(shift_register & 1)) * current_volume * 1000;
	return sample;
}