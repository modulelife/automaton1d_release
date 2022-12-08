//使用24bit真彩色bmp

#include"windows.h"
#include <fstream>
#include <iostream>

using namespace std;



RGBTRIPLE run_clr_table[24]{};
RGBTRIPLE xor_clr_table[2]{ {0xff,0xff,0xff}, {0x00,0x00,0x00} };




void write_bmp(const char* path, unsigned int** const& lattice_record, unsigned int const& lattice_length, \
	unsigned int const& gen_num, const RGBTRIPLE* const& clr_table)
{
	BITMAPFILEHEADER bmp_header
	{
		bmp_header.bfType = 0x4d42,
		bmp_header.bfSize = 0,
		bmp_header.bfReserved1 = 0,
		bmp_header.bfReserved2 = 0,
		bmp_header.bfOffBits = 54
	};

	BITMAPINFOHEADER info_header
	{
		info_header.biSize = 40,
		info_header.biWidth = 0,
		info_header.biHeight = 0,
		info_header.biPlanes = 1,
		info_header.biBitCount = 24,
		info_header.biCompression = 0,
		info_header.biSizeImage = 0,
		info_header.biXPelsPerMeter = 2834,
		info_header.biYPelsPerMeter = 2834,
		info_header.biClrUsed = 0,
		info_header.biClrImportant = 0
	};

	unsigned int line_comp(0);
	unsigned int R = (lattice_length * 3) % 4;
	line_comp = R == 0 ? 0 : 4 - R;
	info_header.biWidth = lattice_length;
	info_header.biHeight = gen_num;
	DWORD widthBytes = info_header.biWidth * 3 + line_comp;
	DWORD imdata_size = widthBytes * info_header.biHeight;
    info_header.biSizeImage = imdata_size + 2;
	bmp_header.bfSize = info_header.biSizeImage + 54;
	RGBTRIPLE* bmp_line_buf = new RGBTRIPLE[lattice_length];
	size_t bmp_line_buf_size = sizeof(RGBTRIPLE) * lattice_length;
	BYTE comp_data = 0x00;

	const unsigned int* r_lattice_record(nullptr);

	ofstream bmp_out(path, ios::binary);
	bmp_out.write((char*)(&bmp_header), sizeof(bmp_header));
	bmp_out.write((char*)(&info_header), sizeof(info_header));
	RGBTRIPLE* w_bmp_line_buf(nullptr);
	unsigned int line_num(0);
	unsigned int r_index(gen_num);


	while (line_num < gen_num) 
	{
        --r_index;
		r_lattice_record = lattice_record[r_index];
		w_bmp_line_buf = bmp_line_buf;

		for (unsigned int i(0); i < lattice_length; i++) {
			*w_bmp_line_buf = clr_table[*r_lattice_record];
			++r_lattice_record;
			++w_bmp_line_buf;
		}

		bmp_out.write((char*)(bmp_line_buf), bmp_line_buf_size);
		
		for (unsigned int i(0); i < line_comp; i++)
			bmp_out.write((char*)(&comp_data), sizeof(comp_data));
		++line_num;
	}
	
	
	for (unsigned int i(0); i < 2; i++)
		bmp_out.write((char*)(&comp_data), sizeof(comp_data));

	bmp_out.close();

	delete[] bmp_line_buf;
}


void write_bmp(const char* path, unsigned int** const& lattice_record, unsigned int const& lattice_length, \
	unsigned int const& gen_num, const RGBTRIPLE* const& clr_table, unsigned int dly)
{
	BITMAPFILEHEADER bmp_header
	{
		bmp_header.bfType = 0x4d42,
		bmp_header.bfSize = 0,
		bmp_header.bfReserved1 = 0,
		bmp_header.bfReserved2 = 0,
		bmp_header.bfOffBits = 54
	};

	BITMAPINFOHEADER info_header
	{
		info_header.biSize = 40,
		info_header.biWidth = 0,
		info_header.biHeight = 0,
		info_header.biPlanes = 1,
		info_header.biBitCount = 24,
		info_header.biCompression = 0,
		info_header.biSizeImage = 0,
		info_header.biXPelsPerMeter = 2834,
		info_header.biYPelsPerMeter = 2834,
		info_header.biClrUsed = 0,
		info_header.biClrImportant = 0
	};

	unsigned int line_comp(0);
	unsigned int R = (lattice_length * 3) % 4;
	line_comp = R == 0 ? 0 : 4 - R;
	info_header.biWidth = lattice_length;
	info_header.biHeight = gen_num * (dly + 2);
	DWORD widthBytes = info_header.biWidth * 3 + line_comp;
	DWORD imdata_size = widthBytes * info_header.biHeight;
	info_header.biSizeImage = imdata_size + 2;
	bmp_header.bfSize = info_header.biSizeImage + 54;
	RGBTRIPLE* bmp_line_buf = new RGBTRIPLE[lattice_length];
	RGBTRIPLE* bmp_line_blank = new RGBTRIPLE[lattice_length];
	for (unsigned int i(0); i < lattice_length; i++) {
		bmp_line_blank[i] = { 0xff,0xff,0xff };
	}
	size_t bmp_line_buf_size = sizeof(RGBTRIPLE) * lattice_length;
	BYTE comp_data = 0x00;

	const unsigned int* r_lattice_record(nullptr);

	ofstream bmp_out(path, ios::binary);
	bmp_out.write((char*)(&bmp_header), sizeof(bmp_header));
	bmp_out.write((char*)(&info_header), sizeof(info_header));
	RGBTRIPLE* w_bmp_line_buf(nullptr);
	unsigned int line_num(0);
	unsigned int r_index(gen_num);


	while (line_num < gen_num)
	{
		--r_index;
		r_lattice_record = lattice_record[r_index];
		w_bmp_line_buf = bmp_line_buf;

		for (unsigned int i(0); i < lattice_length; i++) {
			*w_bmp_line_buf = clr_table[*r_lattice_record];
			++r_lattice_record;
			++w_bmp_line_buf;
		}
		for (unsigned int i(0); i < dly; i++) {
			bmp_out.write((char*)(bmp_line_buf), bmp_line_buf_size);

			for (unsigned int i(0); i < line_comp; i++)
				bmp_out.write((char*)(&comp_data), sizeof(comp_data));
		}
		
		bmp_out.write((char*)(bmp_line_blank), bmp_line_buf_size);
		for (unsigned int i(0); i < line_comp; i++)
				bmp_out.write((char*)(&comp_data), sizeof(comp_data));
		bmp_out.write((char*)(bmp_line_blank), bmp_line_buf_size);
		for (unsigned int i(0); i < line_comp; i++)
			bmp_out.write((char*)(&comp_data), sizeof(comp_data));

		++line_num;
	}


	for (unsigned int i(0); i < 2; i++)
		bmp_out.write((char*)(&comp_data), sizeof(comp_data));

	bmp_out.close();

	delete[] bmp_line_buf;
	delete[] bmp_line_blank;
}













