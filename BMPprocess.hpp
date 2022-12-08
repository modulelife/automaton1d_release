#pragma once
#ifndef _BMPPROCESS_HPP_
#define _BMPPROCESS_HPP_
#include"windows.h"
#include <fstream>


extern RGBTRIPLE run_clr_table[];
extern RGBTRIPLE xor_clr_table[];






void write_bmp(const char* path, unsigned int** const& lattice_record, unsigned int const& lattice_length, \
	unsigned int const& gen_num, const RGBTRIPLE* const& clr_table);

void write_bmp(const char* path, unsigned int** const& lattice_record, unsigned int const& lattice_length, \
	unsigned int const& gen_num, const RGBTRIPLE* const& clr_table, unsigned int dly);




















#endif// !_BMPPROCESS_HPP_