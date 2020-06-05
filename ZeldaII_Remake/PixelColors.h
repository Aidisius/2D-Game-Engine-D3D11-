#pragma once

class Color
{ 
public:
	// Constructors
	constexpr Color() : rgba() {}
	constexpr Color(const Color& rhs)
		:
		rgba(rhs.rgba)
	{}
	constexpr Color(unsigned int dw)
		:
		rgba(dw)
	{}
	constexpr Color(unsigned char x, unsigned char r, unsigned char g, unsigned char b)
		:
		rgba((x << 24u) | (r << 16u) | (g << 8u) | b)
	{}
	constexpr Color(unsigned char r, unsigned char g, unsigned char b)
		:
		rgba((r << 16u) | (g << 8u) | b)
	{}
	constexpr Color(Color col, unsigned char x)
		:
		Color((x << 24u) | col.rgba)
	{}

public:
	// Functions and operator overloads
	Color& operator =(Color rhs)
	{
		rgba = rhs.rgba;
		return *this;
	}
	bool operator ==(const Color& rhs) const
	{
		return rgba == rhs.rgba;
	}
	bool operator !=(const Color& rhs) const
	{
		return !(rgba == rhs.rgba);
	}
	constexpr unsigned char GetA() const
	{
		return rgba >> 24u;
	}
	constexpr unsigned char GetR() const
	{
		return (rgba >> 16u) & 0xFFu;
	}
	constexpr unsigned char GetG() const
	{
		return (rgba >> 8u) & 0xFFu;
	}
	constexpr unsigned char GetB() const
	{
		return rgba & 0xFFu;
	}
	void SetA(unsigned char a)
	{
		rgba = (rgba & 0xFFFFFFu) | (a << 24u);
	}
	void SetR(unsigned char r)
	{
		rgba = (rgba & 0xFF00FFFFu) | (r << 16u);
	}
	void SetG(unsigned char g)
	{
		rgba = (rgba & 0xFFFF00FFu) | (g << 8u);
	}
	void SetB(unsigned char b)
	{
		rgba = (rgba & 0xFFFFFF00u) | b;
	}

public:
	// Variables
	unsigned int rgba;
};


namespace Colors
{
	// Prebuilt colors
	static constexpr Color MakeRGB(unsigned char r, unsigned char g, unsigned char b)
	{
		return (r << 16) | (g << 8) | b;
	}

	static constexpr Color White = MakeRGB(255u, 255u, 255u);
	static constexpr Color Black = MakeRGB(0u, 0u, 0u);
	static constexpr Color Gray = MakeRGB(0x80u, 0x80u, 0x80u);
	static constexpr Color LightGray = MakeRGB(0xD3u, 0xD3u, 0xD3u);
	static constexpr Color Red = MakeRGB(255u, 0u, 0u);
	static constexpr Color Green = MakeRGB(0u, 255u, 0u);
	static constexpr Color Blue = MakeRGB(0u, 0u, 255u);
	static constexpr Color Yellow = MakeRGB(255u, 255u, 0u);
	static constexpr Color Cyan = MakeRGB(0u, 255u, 255u);
	static constexpr Color Magenta = MakeRGB(255u, 0u, 255u);
}