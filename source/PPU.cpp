#include "PPU.h"
#include "Memory.h"

#define INC_LY mem->write_byte(Memory::ADDR_IO_LY, mem->read_byte(Memory::ADDR_IO_LY) + 1)

PPU::PPU(Memory* mem) : mem(mem)
{
	memset(VRAM.data(), 0, 0x2000);
	memset(OAM.data(), 0, 0xA0);

	mode = Mode::DISABLED;
}

void PPU::switch_mode()
{
	switch (mode)
	{
	case Mode::DISABLED:
	{
		mem->write_byte(Memory::ADDR_IO_LY, 0);
		_switch_mode_to(Mode::OAM_SEARCH);
		break;
	}
	case Mode::H_BLANK:
	{
		INC_LY;
		if (mem->read_byte(Memory::ADDR_IO_LY) < 144)
			_switch_mode_to(Mode::OAM_SEARCH);
		else
			_switch_mode_to(Mode::V_BLANK);
		break;
	}
	case Mode::V_BLANK:
	{
		if (mem->read_byte(Memory::ADDR_IO_LY) < 153)
			INC_LY;
		else
		{
			mem->write_byte(Memory::ADDR_IO_LY, 0);
			_switch_mode_to(Mode::OAM_SEARCH);
		}
		break;
	}
	case Mode::OAM_SEARCH:
	{
		_switch_mode_to(Mode::PIXEL_DRAWING);
		break;
	}
	case Mode::PIXEL_DRAWING:
		_switch_mode_to(Mode::H_BLANK);
	}
}

void PPU::_switch_mode_to(Mode mode)
{
	this->mode = mode;
	switch (mode)
	{
	case Mode::H_BLANK:
		max_phase = 51 - 1;
		mem->reset_IO_flag(Memory::ADDR_IO_STAT, 0);
		mem->reset_IO_flag(Memory::ADDR_IO_STAT, 1);

		if (mem->get_IO_flag(Memory::ADDR_IO_STAT, 3)) // Bit 3 - Mode 0 H-Blank Interrupt (1 = Enable) (Read/Write)
			mem->set_IO_flag(Memory::ADDR_IO_IF, 1); // Bit 1: LCD STAT Interrupt Request (INT 48h)  (1 = Request)
		break;

	case Mode::V_BLANK:
		max_phase = 114 - 1;
		mem->set_IO_flag(Memory::ADDR_IO_STAT, 0);
		mem->reset_IO_flag(Memory::ADDR_IO_STAT, 1);

		_check_new_line();

		mem->set_IO_flag(Memory::ADDR_IO_IF, 0); // Bit 0: V-Blank  Interrupt Request (INT 40h)  (1 = Request)
		if (mem->get_IO_flag(Memory::ADDR_IO_STAT, 4)) // Bit 4 - Mode 1 V-Blank Interrupt (1 = Enable) (Read/Write)
			mem->set_IO_flag(Memory::ADDR_IO_IF, 1); // Bit 1: LCD STAT Interrupt Request (INT 48h)  (1 = Request)
		break;

	case Mode::OAM_SEARCH:
		max_phase = 20 - 1;
		mem->reset_IO_flag(Memory::ADDR_IO_STAT, 0);
		mem->set_IO_flag(Memory::ADDR_IO_STAT, 1);

		_check_new_line();

		if (mem->get_IO_flag(Memory::ADDR_IO_STAT, 5)) // Bit 5 - Mode 2 OAM Interrupt (1 = Enable) (Read/Write)
			mem->set_IO_flag(Memory::ADDR_IO_IF, 1); // Bit 1: LCD STAT Interrupt Request (INT 48h) (1 = Request)

		execute_oam_search();
		break;

	case Mode::PIXEL_DRAWING:
		max_phase = 43 - 1;
		mem->set_IO_flag(Memory::ADDR_IO_STAT, 0);
		mem->set_IO_flag(Memory::ADDR_IO_STAT, 1);
		execute_pixel_drawing();
	}
}

void PPU::_check_new_line()
{
	if (mem->read_byte(Memory::ADDR_IO_LY) == mem->read_byte(Memory::ADDR_IO_LYC))  
	{
		mem->set_IO_flag(Memory::ADDR_IO_STAT, 2); // Bit 2 - Coincidence Flag (0:LYC <> LY, 1:LYC = LY) (Read Only)

		if (mem->get_IO_flag(Memory::ADDR_IO_STAT, 6)) // Bit 6 - LYC=LY Coincidence Interrupt (1 = Enable) (Read/Write)
			mem->set_IO_flag(Memory::ADDR_IO_IF, 1); // Bit 1: LCD STAT Interrupt Request (INT 48h)  (1 = Request)
	}
	else
		mem->reset_IO_flag(Memory::ADDR_IO_STAT, 2); // Bit 2 - Coincidence Flag (0:LYC <> LY, 1:LYC = LY) (Read Only)
}

void PPU::execute_one_cycle()
{
	if (mode == Mode::DISABLED)
	{
		if (!mem->get_IO_flag(Memory::ADDR_IO_LCDC, 7)) // Bit 7 - LCD Display Enable (0 = Off, 1 = On)
			return;
		else
			phase = 0;
	}
	else if (!mem->get_IO_flag(Memory::ADDR_IO_LCDC, 7))
	{
		mode = Mode::DISABLED;
		return;
	}

	if (phase == 0)
		switch_mode();

	if (phase == max_phase)
		phase = 0;
	else
		phase++;
}

void PPU::execute_oam_search()
{
	Sprite* sprites = reinterpret_cast<Sprite*>(OAM.data());
	auto& buff = current_line_sorted_sprites;
	buff.clear();

	uint8_t LY = mem->read_byte(Memory::ADDR_IO_LY);
	uint8_t h = mem->get_IO_flag(Memory::ADDR_IO_LCDC, 2) ? 16 : 8; // Bit 2 - OBJ (Sprite) Size(0 = 8x8, 1 = 8x16)

	size_t sprites_added = 0;
	for (size_t i = 0; i < SPRITES_QUANTITY; i++)
		if (IN_RANGE(sprites[i].y, LY + 16 - h + 1, LY + 16))
		{
			buff.push_back(sprites[i]);

			if (h == 16)
				buff.back().tile_number &= 0xfe; // the least significant bit of the tile index is ignored

			sprites_added++;
			if (sprites_added == MAX_SPRITES_PER_LINE)
				break;
		}

	int32_t n = buff.size();
	for (int32_t i = 0; i < n - 1; i++)
		for (int32_t j = 0; j < n - i - 1; j++)
		{
			if (buff[j].x > buff[j + 1].x)
				std::swap(buff[j], buff[j + 1]);
		}
}

void PPU::execute_pixel_drawing()
{
	if (mem->get_IO_flag(Memory::ADDR_IO_LCDC, 0))
	{
		draw_background();
		draw_window();
	}
	else
		current_line_pixels.fill(0);
	
	background_line_pixels = current_line_pixels;
	if (!mem->get_dma_transfer())
		draw_sprites();
	
	screen_buffer[mem->read_byte(Memory::ADDR_IO_LY)] = current_line_pixels;
}

void PPU::draw_background()
{
	uint8_t LY = mem->read_IO_byte(Memory::ADDR_IO_LY);
	uint8_t SCX = mem->read_IO_byte(Memory::ADDR_IO_SCX);
	uint8_t SCY = mem->read_IO_byte(Memory::ADDR_IO_SCY);

	// integer division used on purpose
	uint16_t tile_idx = 32 * (((SCY + LY) % 256) / 8) + (SCX % 256) / 8;
	uint8_t x_in_tile = SCX % 8;
	uint8_t y_in_tile = (SCY + LY) % 8;

	uint8_t parsed_palette[4];
	parse_palette(mem->read_IO_byte(Memory::ADDR_IO_BGP), parsed_palette);

	for (uint8_t pixel_x = 0; pixel_x < SCREEN_WIDTH; pixel_x++)
	{
		uint8_t tile;
		if (mem->get_IO_flag(Memory::ADDR_IO_LCDC, 3)) // Bit 3 - BG Tile Map Display Select  (0 = 9800-9BFF, 1 = 9C00-9FFF)
			tile = VRAM[0x9C00 - Memory::ADDR_VRAM_START + tile_idx];
		else
			tile = VRAM[0x9800 - Memory::ADDR_VRAM_START + tile_idx];

		uint16_t tile_address;
		if (mem->get_IO_flag(Memory::ADDR_IO_LCDC, 4)) // Bit 4 - BG & Window Tile Data Select (0 = 8800-97FF, 1 = 8000-8FFF)
			tile_address = 0x8000 + tile * 16 - Memory::ADDR_VRAM_START;
		else
			tile_address = 0x9000 + ((int8_t)tile) * 16 - Memory::ADDR_VRAM_START;

		uint8_t pixel_idx = get_pixel_from_tile(tile_address, x_in_tile, y_in_tile);
		uint8_t pixel_colour = parsed_palette[pixel_idx];
		current_line_pixels[pixel_x] = pixel_colour;

		x_in_tile++;
		if (x_in_tile == 8)
		{
			x_in_tile = 0;
			tile_idx++;
			if (((uint16_t)SCX) + pixel_x == 255)
				tile_idx -= 32;
			parse_palette(mem->read_IO_byte(Memory::ADDR_IO_BGP), parsed_palette);
		}
	}
}

void PPU::get_tile_map(std::array<PPU::line<256>, 256>& screen)
{
	for (uint8_t LY = 0; LY != 255; LY++)
	{
		// integer division used on purpose
		uint16_t tile_idx = 32 * (LY / 8);
		uint8_t x_in_tile = 0;
		uint8_t y_in_tile = LY % 8;

		uint8_t parsed_palette[4];
		parse_palette(mem->read_IO_byte(Memory::ADDR_IO_BGP), parsed_palette);

		for (uint8_t pixel_x = 0; pixel_x != 255; pixel_x++)
		{
			uint8_t tile;
			if (mem->get_IO_flag(Memory::ADDR_IO_LCDC, 3)) // Bit 3 - BG Tile Map Display Select  (0 = 9800-9BFF, 1 = 9C00-9FFF)
				tile = VRAM[0x9C00 - Memory::ADDR_VRAM_START + tile_idx];
			else
				tile = VRAM[0x9800 - Memory::ADDR_VRAM_START + tile_idx];

			uint16_t tile_address;
			if (mem->get_IO_flag(Memory::ADDR_IO_LCDC, 4)) // Bit 4 - BG & Window Tile Data Select (0 = 8800-97FF, 1 = 8000-8FFF)
				tile_address = 0x8000 + tile * 16 - Memory::ADDR_VRAM_START;
			else
				tile_address = 0x9000 + ((int8_t)tile) * 16 - Memory::ADDR_VRAM_START;

			uint8_t pixel_idx = get_pixel_from_tile(tile_address, x_in_tile, y_in_tile);
			uint8_t pixel_colour = parsed_palette[pixel_idx];
			screen[LY][pixel_x] = pixel_colour;

			x_in_tile++;
			if (x_in_tile == 8)
			{
				x_in_tile = 0;
				tile_idx++;
				parse_palette(mem->read_IO_byte(Memory::ADDR_IO_BGP), parsed_palette);
			}
		}
	}
}

void PPU::draw_window()
{
	if (!mem->get_IO_flag(Memory::ADDR_IO_LCDC, 5)) // Bit 5 - Window Display Enable (0 = Off, 1 = On)
		return;

	uint8_t LY = mem->read_IO_byte(Memory::ADDR_IO_LY);
	int16_t WX = mem->read_IO_byte(Memory::ADDR_IO_WX) - 7;
	uint8_t WY = mem->read_IO_byte(Memory::ADDR_IO_WY);
	if (LY < WY)
		return;

	uint8_t start_pixel_x = (WX < 0) ? 0 : WX;
	for (uint8_t pixel_x = start_pixel_x; pixel_x < SCREEN_WIDTH; pixel_x++)
	{
		// integer division used on purpose
		uint16_t tile_idx = 32 * ((LY - WY) / 8) + (pixel_x - WX) / 8;

		uint8_t tile;
		if (mem->get_IO_flag(Memory::ADDR_IO_LCDC, 6)) // Bit 6 - Window Tile Map Display Select (0 = 9800-9BFF, 1 = 9C00-9FFF)
			tile = VRAM[0x9C00 - Memory::ADDR_VRAM_START + tile_idx];
		else
			tile = VRAM[0x9800 - Memory::ADDR_VRAM_START + tile_idx];

		uint16_t tile_address;
		if (mem->get_IO_flag(Memory::ADDR_IO_LCDC, 4)) // Bit 4 - BG & Window Tile Data Select (0 = 8800-97FF, 1 = 8000-8FFF)
			tile_address = 0x8000 + tile * 16 - Memory::ADDR_VRAM_START;
		else
			tile_address = 0x9000 + ((int8_t)tile) * 16 - Memory::ADDR_VRAM_START;

		uint8_t pixel_idx = get_pixel_from_tile(tile_address, (pixel_x - WX) % 8, (LY - WY) % 8);
		uint8_t pixel_colour = get_colour(mem->read_IO_byte(Memory::ADDR_IO_BGP), pixel_idx);
		current_line_pixels[pixel_x] = pixel_colour;
	}
}

void PPU::draw_sprites()
{
	if (!mem->get_IO_flag(Memory::ADDR_IO_LCDC, 1)) // Bit 1 - OBJ (Sprite) Display Enable(0 = Off, 1 = On)
		return;

	uint8_t zero_index_colour = get_colour(mem->read_byte(Memory::ADDR_IO_BGP), 0);

	uint8_t h = mem->get_IO_flag(Memory::ADDR_IO_LCDC, 2) ? 16 : 8; // Bit 2 - OBJ (Sprite) Size(0 = 8x8, 1 = 8x16)
	uint8_t LY = mem->read_byte(Memory::ADDR_IO_LY);

	for (int8_t i = current_line_sorted_sprites.size() - 1; i >= 0; i--)
	{
		Sprite sprite = current_line_sorted_sprites[i];

		uint8_t palette = (GET_BIT(sprite.flags, 4)) ?
			mem->read_byte(Memory::ADDR_IO_OBP1) :
			mem->read_byte(Memory::ADDR_IO_OBP0); // Bit4   Palette number  **Non CGB Mode Only** (0 = OBP0, 1 = OBP1)

		bool flip_x = GET_BIT(sprite.flags, 5); // Bit5   X flip (0 = Normal, 1 = Horizontally mirrored)
		bool flip_y = GET_BIT(sprite.flags, 6); // Bit6   Y flip (0 = Normal, 1 = Vertically mirrored)

		uint8_t line_in_sprite = LY - (sprite.y - 16);
		if (flip_y)
			line_in_sprite = (h - 1) - line_in_sprite;

		uint16_t line_address = ADDR_SPRITES_DATA_START + sprite.tile_number * 16
			+ line_in_sprite * 2;

		std::array<uint8_t, 8> pixel_line = bytes_to_pixel_line(line_address);

		int16_t start_index = (sprite.x - 8 < 0) ? (8 - sprite.x) : 0; // sprites are always 8 pixels wide
		int16_t end_index = (sprite.x >= 160) ? (168 - sprite.x) : 8; // exclusive

		for (int16_t pixel_idx = start_index; pixel_idx < end_index; pixel_idx++)
		{
			if (GET_BIT(sprite.flags, 7)) // Bit7   OBJ-to-BG Priority (0 = OBJ Above BG, 1 = OBJ Behind BG color 1-3)
			{
				uint8_t background_colour = background_line_pixels[sprite.x - 8 + pixel_idx];
				if (background_colour != zero_index_colour)
				{
					current_line_pixels[sprite.x - 8 + pixel_idx] = background_colour;
					continue;
				}
			}
			
			uint8_t colour_idx;
			if (!flip_x)
				colour_idx = pixel_line[pixel_idx];
			else
				colour_idx = pixel_line[7 - pixel_idx];

			if (colour_idx == 0)
				continue;

			current_line_pixels[sprite.x - 8 + pixel_idx] = get_colour(palette, colour_idx);
		}
	}
}

uint8_t PPU::read_byte_VRAM(uint16_t addr) const
{
	if (mode == Mode::PIXEL_DRAWING)
		return 0xFF;
	else
		return VRAM[addr - Memory::ADDR_VRAM_START];
}

void PPU::write_byte_VRAM(uint16_t addr, uint8_t value)
{
	if (mode == Mode::PIXEL_DRAWING)
		return;
	VRAM[addr - Memory::ADDR_VRAM_START] = value;
}

uint8_t PPU::read_byte_OAM(uint16_t addr) const
{
	if (mode == Mode::OAM_SEARCH || mode == Mode::PIXEL_DRAWING)
		return 0xFF;
	else
		return OAM[addr - Memory::ADDR_OAM_START];
}

void PPU::write_byte_OAM(uint16_t addr, uint8_t value)
{
	if (mode == Mode::OAM_SEARCH || mode == Mode::PIXEL_DRAWING)
		return;
	OAM[addr - Memory::ADDR_OAM_START] = value;
}