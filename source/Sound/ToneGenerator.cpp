#include <cmath>

#include "ToneGenerator.h"
#include "..\APU.h"
#include "..\Memory.h"

ToneGenerator::ToneGenerator(APU* apu, uint8_t number, bool sweep_enabled) : Generator(apu, number), sweep_enabled(sweep_enabled)
{ }

void ToneGenerator::calculate_frequency()
{
	// Frequency = 131072/(2048-x) Hz (frequency_specifier == x)
	frequency = 131072 / (2048 - frequency_specifier);
	frequency_changed = true;
}

void ToneGenerator::process_sound_IO_write(uint16_t addr, uint8_t value)
{
	if (addr == Memory::ADDR_IO_NR10 && sweep_enabled)
	{
		sweep_step = ((value >> 4) & 0b111) / 128.0; // Bit 6-4 - Sweep Time
		sweep_direction = GET_BIT(value, 3) ? -1 : 1; // Bit 3 - Sweep Increase / Decrease: 0: Addition, 1 : Subtraction
		sweep_shift = value & 0b111; // Bit 2-0 - Number of sweep shift (n: 0-7)
	}
	else if (addr == Memory::ADDR_IO_NR11 || addr == Memory::ADDR_IO_NR21)
	{
		int8_t old_duty = duty;
		duty = value >> 6; // Bit 7-6 - Wave Pattern Duty (Read/Write)
		if (duty != old_duty)
		{
			fourier_generators[duty]->reset();
			frequency_changed = true;
		}

		// Sound Length = (64-t1)*(1/256) seconds The Length value is used only if Bit 6 in NR14/NR24 is set.
		// Bit 5 - 0 - Sound length data (Write Only) (t1: 0 - 63)
		sound_length = (64 - (value & 0b111111)) / 256.0;
		sound_length_accumulated_time = 0;
	}
	else if (addr == Memory::ADDR_IO_NR12 || addr == Memory::ADDR_IO_NR22)
	{
		envelope_initial_volume = value >> 4;				// Bit 7-4 - Initial Volume of envelope (0-0Fh) (0=No Sound)
		envelope_direction = GET_BIT(value, 3) ? 1 : -1;	// Bit 3   - Envelope Direction (0=Decrease, 1=Increase)

		// Length of 1 step = n*(1/64) seconds
		// Bit 2-0 - Number of envelope sweep (n: 0-7) (If zero, stop envelope operation.)
		envelope_step = (value & 0b111) / 64.0;
	}
	else if (addr == Memory::ADDR_IO_NR13 || addr == Memory::ADDR_IO_NR23)
	{
		// Frequency's lower 8 bits of 11 bit data (x). Next 3 bits are in NR14/NR24
		frequency_specifier = (frequency_specifier & (0b111 << 8)) | value;

		calculate_frequency();
	}
	else if (addr == Memory::ADDR_IO_NR14 || addr == Memory::ADDR_IO_NR24)
	{
		bool restart = GET_BIT(value, 7);   // Bit 7 - Initial (1=Restart Sound) (Write Only);
		// initialize
		if (restart)
		{
			turned_on = true;
			current_volume = envelope_initial_volume;
			sound_length_accumulated_time = 0;
			sweep_accumulated_time = 0;
			envelope_accumulated_time = 0;

			apu->update_generator_status(number, true);
		}

		infinite_sound = !GET_BIT(value, 6); // Bit 6 - Counter/consecutive selection (Read/Write)
		                                     // (1 = Stop output when length in NR21 expires)

		// Bit 2-0 - Frequency's higher 3 bits (x) (Write Only)
		frequency_specifier = (frequency_specifier & 0xFF) | ((value & 0b111) << 8);

		calculate_frequency();
	}
}

int16_t ToneGenerator::generate_sample(double elapsed_time)
{
	if (!turned_on)
		return 0;

	if (!handle_sound_length(elapsed_time))
		return 0;

	if (sweep_step != 0)
		handle_sweep(elapsed_time);

	if (envelope_step != 0)
		handle_envelope(elapsed_time);
	
	return _generate_sample(elapsed_time);
};