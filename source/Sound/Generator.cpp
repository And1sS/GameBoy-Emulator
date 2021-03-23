#include "Generator.h"
#include "..\APU.h"

Generator::Generator(APU* apu, uint8_t number) : apu(apu), number(number) {}

bool Generator::handle_sound_length(double elapsed_time)
{
	if (!infinite_sound)
	{
		sound_length_accumulated_time += elapsed_time;
		if (sound_length_accumulated_time > sound_length)
		{
			sound_length_accumulated_time = 0;
			turned_on = false;

			apu->update_generator_status(number, false);

			return false;
		}
	}
	return true;
}