#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <stdio.h>
#include <Windows.h>

#include "Memory.h"
#include "CPU.h"
#include "PPU.h"
#include "Timer.h"

HANDLE hConsole;
wchar_t* screen;
const int height = 144;
const int width  = 160;
long long elapsedTime;

void draw_screen(PPU& ppu, CPU& cpu);

int main()
{
	hConsole = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,
		0, 
		NULL,
		CONSOLE_TEXTMODE_BUFFER,
		NULL
	);
	SetConsoleActiveScreenBuffer(hConsole);

	screen = new wchar_t[144 * 162];

	std::ifstream in("cpu_instrs.gb", std::ios::binary);

	Memory* mem = new Memory(in);
	Timer timer(mem);
	CPU cpu(mem, &timer);
	PPU ppu(mem);
	mem->set_timer(&timer);
	mem->set_PPU(&ppu);

	auto prev_mode = ppu.get_mode();
	auto tp1 = std::chrono::system_clock::now();
	auto tp2 = std::chrono::system_clock::now();
	int k = 0;
	while (true)
	{
		if (ppu.get_mode() == PPU::Mode::V_BLANK && prev_mode != ppu.get_mode())
		{
			k++;
			if (k == 10)
			{
				k = 0;
				tp2 = std::chrono::system_clock::now();
				elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(tp2 - tp1).count();

				tp1 = tp2;
			}
			draw_screen(ppu, cpu);
		}

		prev_mode = ppu.get_mode();

		cpu.execute_one_cycle();
		ppu.execute_one_cycle();
		timer.execute_one_cycle();
	}

	delete mem;
}

void draw_screen(PPU& ppu, CPU& cpu)
{
	DWORD dwBytesWritten = 0;
	auto screen_buffer = ppu.get_screen_buffer();
	static wchar_t symbols[4] = { ' ', 0x2592, 0x2593, 0x2588 };

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			screen[i * width + j] = symbols[screen_buffer[i][j]];

	swprintf(screen, 40, L"clocks=%lld, FPS=%f", cpu.get_clock_cycle(), 10000.0 / elapsedTime);
	WriteConsoleOutputCharacter(hConsole, screen, 144 * 162, { 0,0 }, &dwBytesWritten);
}