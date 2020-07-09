#pragma once

#include <cstdint>
#include <cstring>
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
public:
	enum class Mode { H_BLANK, V_BLANK, OAM_SEARCH, PIXEL_DRAWING, DISABLED };

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

	Memory*   mem;

	Mode      mode;

	uint8_t   VRAM[0x2000];
	uint8_t   OAM[0xA0];

	uint64_t  clock_cycle = 0;
	uint8_t   phase       = 0;
	uint8_t   max_phase;

	uint8_t   current_line_pixels[SCREEN_WIDTH];
	uint8_t   screen_buffer[SCREEN_HEIGHT][SCREEN_WIDTH];
	

	// here sprites are sorted in reversed order due to their priority
	std::vector<Sprite> current_line_sorted_sprites;

	void    switch_mode();
	void    _switch_mode_to(Mode mode);

	void    execute_oam_search();
	void    execute_pixel_drawing();


	void    draw_background();
	void    draw_sprites();

	// address into VRAM
	std::array<uint8_t, 8> bytes_to_pixel_line(uint16_t address) const;
	uint8_t get_pixel_from_tile(uint16_t tile_address, uint8_t x, uint8_t y) const;
	uint8_t get_colour(uint8_t palette, uint8_t index) const;
	
public:
	PPU(Memory* mem);

	void    execute_one_cycle();

	uint8_t read_byte_VRAM(uint16_t addr) const;
	uint8_t read_byte_OAM(uint16_t addr) const;
	void    write_byte_VRAM(uint16_t addr, uint8_t value);
	void    write_byte_OAM(uint16_t addr, uint8_t value);

	Mode    get_mode() const;

	uint8_t (*get_screen_buffer())[160];
};


inline std::array<uint8_t, 8> PPU::bytes_to_pixel_line(uint16_t address) const
{
	uint8_t first = VRAM[address];
	uint8_t second = VRAM[address + 1];
	std::array<uint8_t, 8> res;
	for (uint8_t i = 0; i < 8; i++)
		res[7 - i] = ((GET_BIT(first, i) >> i) << 1) | (GET_BIT(second, i) >> i);
	return res;
}

inline uint8_t PPU::get_pixel_from_tile(uint16_t tile_address, uint8_t x, uint8_t y) const
{
	uint8_t first = VRAM[tile_address + y * 2];
	uint8_t second = VRAM[tile_address + y * 2 + 1];
	uint8_t bit = 7 - x;
	return ((GET_BIT(first, bit) >> (bit)) << 1) | (GET_BIT(second, bit) >> (bit));
}


inline uint8_t PPU::get_colour(uint8_t palette, uint8_t index) const
{
	return (palette & (0b00000011 << (index * 2))) >> (index * 2);
}

inline PPU::Mode PPU::get_mode() const
{
	return mode;
}

inline uint8_t (*PPU::get_screen_buffer())[160]
{
	return screen_buffer;
}