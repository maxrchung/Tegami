#include "png.h"
#include "PNGWrapper.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

PNGWrapper::PNGWrapper(const std::string& path) {
	FILE* file = fopen(path.c_str(), "rb");
	png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	info = png_create_info_struct(png);
	setjmp(png_jmpbuf(png));

	png_init_io(png, file);
	png_set_sig_bytes(png, 8);
	png_read_info(png, info);
	width = png_get_image_width(png, info);
	height = png_get_image_height(png, info);
	png_get_color_type(png, info);
	png_get_bit_depth(png, info);
	png_set_interlace_handling(png);
	png_read_update_info(png, info);
	setjmp(png_jmpbuf(png));

	auto rows = (png_bytep*)malloc(sizeof(png_bytep) * height);
	for (auto y = 0; y<height; y++)
		rows[y] = (png_byte*)malloc(png_get_rowbytes(png, info));
	png_read_image(png, rows);

	fclose(file);

	// For simpler access, I'm putting everything into grid
	grid = std::vector<std::vector<Color>>(width, std::vector<Color>(height, Color()));
	for (auto y = 0; y<height; y++) {
		auto row = rows[y];
		for (auto x = 0; x<width; x++) {
			auto ptr = &(row[x * 4]);
			grid[x][y] = Color(ptr[0], ptr[1], ptr[2]);
		}
	}
}

Color PNGWrapper::Get(int x, int y) const {
	return grid[x][y];
}