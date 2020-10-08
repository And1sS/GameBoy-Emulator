#pragma once

#include <cstdint>

class Generator 
{
private:
	static constexpr double MAX_TIME = 1;
protected:
	double accumulated_time = 0;

public:
	virtual int16_t get_sample(double elapsed_time) 
	{
		accumulated_time += elapsed_time;
		if (accumulated_time > MAX_TIME)
			accumulated_time -= MAX_TIME;
		return 0;
	};

	virtual void process_sound_IO_write(uint16_t addr, uint8_t value) = 0;

	virtual ~Generator() {};
};

class MockedGenerator : public Generator
{
public:
	void process_sound_IO_write(uint16_t addr, uint8_t value) override {}
};