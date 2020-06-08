#include "Sprite.h"
#include "AlteredWindows.h"
#include <cassert>
#include <fstream>


Sprite::Sprite(std::string picName)
{
	std::ifstream file(picName, std::ios::binary);
	assert(file);

	BITMAPFILEHEADER bmFileHeader;
	file.read(reinterpret_cast<char*>(&bmFileHeader), sizeof(bmFileHeader));

	BITMAPINFOHEADER bmInfoHeader;
	file.read(reinterpret_cast<char*>(&bmInfoHeader), sizeof(bmInfoHeader));

	assert(bmInfoHeader.biBitCount == 24 || bmInfoHeader.biBitCount == 32);
	assert(bmInfoHeader.biCompression == BI_RGB);

	const bool is32b = bmInfoHeader.biBitCount == 32;

	width = bmInfoHeader.biWidth;

	// Test for reverse row order and control y loop accordingly
	int yStart;
	int yEnd;
	int dy;
	if (bmInfoHeader.biHeight < 0) {
		height = -bmInfoHeader.biHeight;
		yStart = 0;
		yEnd = height;
		dy = 1;
	}
	else {
		height = bmInfoHeader.biHeight;
		yStart = height - 1;
		yEnd = -1;
		dy = -1;
	}

	pPixels = new Color[width * height];

	file.seekg(bmFileHeader.bfOffBits);
	// Padding is for the case of 24 bit depth only
	const int padding = (4 - (width * 3) % 4) % 4;

	for (int y = yStart; y != yEnd; y += dy) {
		for (int x = 0; x < width; x++) {
			PutPixel(x, y, Color(file.get(), file.get(), file.get()));
			if (is32b) {
				file.seekg(1, std::ios::cur);
			}
		}
		if (!is32b) {
			file.seekg(padding, std::ios::cur);
		}
	}
}

Sprite::~Sprite()
{
	delete[] pPixels;
	pPixels = nullptr;
}

Sprite& Sprite::operator=(const Sprite& rhs)
{
	width = rhs.width;
	height = rhs.height;

	delete[] pPixels;
	pPixels = new Color[width * height];

	const int nPixels = width * height;
	for (int i = 0; i < nPixels; i++) {
		pPixels[i] = rhs.pPixels[i];
	}

	return *this;
}

void Sprite::PutPixel(int x, int y, Color c)
{
	assert(x >= 0);
	assert(x < width);
	assert(y >= 0);
	assert(y < height);
	pPixels[y * width + x] = c;
}

Color Sprite::GetPixel(int x, int y) const
{
	assert(x >= 0);
	assert(x < width);
	assert(y >= 0);
	assert(y < height);
	return pPixels[y * width + x];
}

Vec2DInt Sprite::GetPos()
{
	return pos;
}

void Sprite::SetPos(int x_in, int y_in)
{
	pos.x = x_in;
	pos.y = y_in;
}

int Sprite::GetWidth() const
{
	return width;
}

int Sprite::GetHeight() const
{
	return height;;
}
