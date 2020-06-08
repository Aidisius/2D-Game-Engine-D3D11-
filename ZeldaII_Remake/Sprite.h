#pragma once
#include "Vec2D.h"
#include "PixelColors.h"
#include <string>

class Sprite
{
public:
	// Constructor and destructor
	Sprite(std::string picName);
	~Sprite();

public:
	// Functions and operator overloads
	Sprite& operator=(const Sprite& rhs);
	void PutPixel(int x, int y, Color c);
	Color GetPixel(int x, int y) const;
	Vec2DInt GetPos();
	void SetPos(int x_in, int y_in);
	int GetWidth() const;
	int GetHeight() const;

private:
	Color* pPixels = nullptr;
	std::string name;
	Vec2DInt pos = {50, 50};
	int width;
	int height;
};