#include "ToneGenerator.h"
#include "..\Memory.h"

const std::array<uint8_t, 4> ToneGenerator::sound_data = 
	{ 0b00000001, 0b00000011, 0b00001111, 0b00111111 };

ToneGenerator::ToneGenerator(bool sweep_enabled) : sweep_enabled(sweep_enabled)
{ }

void ToneGenerator::calculate_period()
{
	// Frequency = 131072/(2048-x) Hz (frequency_specifier == x)
	period = 1.0 * (2048 - frequency_specifier) / 131072;
}

void ToneGenerator::process_sound_IO_write(uint16_t addr, uint8_t value)
{
	if (addr == Memory::ADDR_IO_NR10 && sweep_enabled)
	{
		sweep_step = ((value >> 4) & 0b111) / 128.0; // Bit 6-4 - Sweep Time
		sweep_direction = GET_BIT(value, 3) ? -1 : 1; // Bit 3 - Sweep Increase / Decrease: 0: Addition, 1 : Subtraction
		sweep_shift = value & 0b111; // Bit 2-0 - Number of sweep shift (n: 0-7)
	}
	if (addr == Memory::ADDR_IO_NR11 || addr == Memory::ADDR_IO_NR21)
	{
		duty = value >> 6; // Bit 7-6 - Wave Pattern Duty (Read/Write)

		// Sound Length = (64-t1)*(1/256) seconds The Length value is used only if Bit 6 in NR24 is set.
		// Bit 5 - 0 - Sound length data(Write Only) (t1: 0 - 63)
		sound_length = (64 - (value & 0b111111)) * 1.0 / 256;
		accumulated_time = 0;
	}
	else if (addr == Memory::ADDR_IO_NR12 || addr == Memory::ADDR_IO_NR22)
	{
		envelope_initial_volume = value >> 4;				// Bit 7-4 - Initial Volume of envelope (0-0Fh) (0=No Sound)
		envelope_direction = GET_BIT(value, 3) ? 1 : -1;	// Bit 3   - Envelope Direction (0=Decrease, 1=Increase)

		// Length of 1 step = n*(1/64) seconds
		// Bit 2-0 - Number of envelope sweep (n: 0-7) (If zero, stop envelope operation.)
		envelope_step = (value & 0b111) * 1.0 / 64;
	}
	else if (addr == Memory::ADDR_IO_NR13 || addr == Memory::ADDR_IO_NR23)
	{
		// Frequency's lower 8 bits of 11 bit data (x). Next 3 bits are in NR24 ($FF19)
		frequency_specifier = (frequency_specifier & (0b111 << 8)) | value;

		calculate_period();
	}
	else if (addr == Memory::ADDR_IO_NR14 || addr == Memory::ADDR_IO_NR24)
	{
		////////////////////
		// TODO: CHECK THIS 
		////////////////////
		turned_on = GET_BIT(value, 7);   // Bit 7 - Initial (1=Restart Sound) (Write Only);
		if (turned_on)
		{
			current_volume = envelope_initial_volume;
			accumulated_time = 0;
			envelope_accumulated_time = 0;
		}

		infinite_sound = !GET_BIT(value, 6); // Bit 6 - Counter/consecutive selection (Read/Write) (1 = Stop output when length in NR21 expires)

		// Bit 2-0 - Frequency's higher 3 bits (x) (Write Only)
		frequency_specifier = (frequency_specifier & 0xFF) | ((value & 0b111) << 8);

		calculate_period();
	}
}

int16_t ToneGenerator::generate_sample(double elapsed_time)
{
	if (!turned_on)
		return 0;

	update_time(elapsed_time);
	sweep_accumulated_time += elapsed_time;
	envelope_accumulated_time += elapsed_time;

	if (!infinite_sound && accumulated_time > sound_length)
	{
		accumulated_time = 0;
		turned_on = false;

		return 0;
	}

	if (sweep_step != 0.0)
		while (sweep_accumulated_time >= sweep_step)
		{
			sweep_accumulated_time -= sweep_step;
			frequency_specifier += sweep_direction * (frequency_specifier >> sweep_shift);
			if (frequency_specifier == 0 || frequency_specifier > 2047)
				turned_on = false;
			calculate_period();
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

	uint8_t sound_bit_index = fmod(accumulated_time, period) / period * 8;
	int16_t sample = GET_BIT(sound_data[duty], sound_bit_index) * current_volume * 1000;
	return sample;
};