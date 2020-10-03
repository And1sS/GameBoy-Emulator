#include "APU.h"
#include "Memory.h"

void APU::execute_one_cycle()
{
    static float phase = 0;
    static constexpr int w = 5000;
	clock_cycles++;
	if (clock_cycles % 23 == 0) // 1048576 / 44100 = 23
	{
        // generate 1 sound sample
        sound_buffer[(cur_pos + 1) % sound_buffer.size()] = 0; // 2000 * sin(phase);
        phase += w * 1.0 / BIT_RATE;
        if (phase > 2 * 3.1415)
            phase -= 2 * 3.1415;
        cur_pos = (cur_pos + 1) % sound_buffer.size();
	}
}