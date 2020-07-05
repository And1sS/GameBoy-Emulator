#pragma once

#include <cstdint>
#include <vector>
#include <array>

#ifndef IN_RANGE
#define IN_RANGE(VAL, START, INCLUSIVE_END) (((VAL) >= (START)) && ((VAL) <= (INCLUSIVE_END)))
#endif // !IN_RANGE

#ifndef GET_BIT
#define GET_BIT(VAL, INDX) ((VAL) & (1 << (INDX)))
#endif // !GET_BIT

class Memory;

class PPU
{
private:
	static constexpr size_t SPRITES_QUANTITY     = 40;
	static constexpr size_t MAX_SPRITES_PER_LINE = 10;

	static constexpr size_t SCREEN_WIDTH         = 160;
	static constexpr size_t SCREEN_HEIGHT        = 144;

	static constexpr uint16_t ADDR_SPRITES_DATA_START = 0x0000; // real address is 0x8000

	struct Sprite
	{
		uint8_t y;
		uint8_t x;
		uint8_t tile_number;
		uint8_t flags;
	};

	enum class Mode { H_BLANK, V_BLANK, OAM_SEARCH, PIXEL_DRAWING };
	Mode      mode;

	Memory*   mem;

	uint8_t   VRAM[0x2000];
	uint8_t   OAM[0x100];

	uint64_t  clock_cycle = 0;
	uint8_t   phase       = 0;
	uint8_t   max_phase;

	uint8_t   current_line_pixels[SCREEN_WIDTH];
	uint8_t   screen_buffer[SCREEN_HEIGHT][SCREEN_WIDTH];

	// here sprites are sorted in reversed order due to their priority
	std::vector<Sprite> current_line_sorted_sprites;

	void    switch_mode();

	void    execute_oam_search();
	void    execute_pixel_drawing();
	void    execute_HBLANK();
	void    execute_VBLANK();


	void    draw_background();
	void    draw_sprites();

	// address into VRAM
	std::array<uint8_t, 8> bytes_to_pixel_line(uint16_t address);
	inline std::uint8_t get_colour(uint8_t palette, uint8_t index);
	
public:
	void    execute_one_cycle();

	uint8_t read_byte_VRAM(uint16_t addr);
	uint8_t read_byte_OAM(uint16_t addr);
	void    write_byte_VRAM(uint16_t addr, uint8_t value);
	void    write_byte_OAM(uint16_t addr, uint8_t value);
};