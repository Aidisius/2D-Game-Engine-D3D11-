#pragma once
#include "AlteredWindows.h"
#include <d3d11.h>
#include <wrl.h>
#include "PixelColors.h"
#include "Sprite.h"
#include <cassert>

#pragma comment (lib, "d3d11.lib")


class Graphics
{
public:
	// Constructors and destructor
	Graphics(HWND& hWnd);
	Graphics(const Graphics&) = delete;
	Graphics operator=(const Graphics&) = delete;
	~Graphics();

public:
	// Functions and structs

	struct VERTEX { FLOAT X, Y, Z; FLOAT U, V; };					 // A struct to define a single vertex (cartesian and textureCoords)

	void BeginFrame();												 // Sets up new frame for Game Logic changes
	void EndFrame();												 // Copies data from pPixelBuffer to GPU for next frame shown
	void AddPixelToBuffer(int x, int y, Color c);					 // Changes a pixel in pPixelBuffer, used to update screen visually from Game Logic
	void AddPixelToBuffer(int x, int y, int r, int g, int b)
	{
		AddPixelToBuffer(x, y, {unsigned char(r), unsigned char(g), unsigned char(b)});
	}

	void DrawSprite(Sprite& spr, Color chroma);

private:
	// Pointers
	Microsoft::WRL::ComPtr<IDXGISwapChain>				pSwapChain;					// Used to swap backbuffer with frontuffer to update display
	Microsoft::WRL::ComPtr<ID3D11Device>				pDevice;					// Used to initialized resources and enumerate capabilities
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>			pDeviceContext;				// Sets pipeline state and generate rendering commands based off of device resources
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		pBackbuffer;				// Where we draw the next frame to be display, which is switched with the frontbuffer
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				pSysBufferTexture;			// Stores our desired frame update with data for texels (pixels but not squares, dots to account for real life LED size and area of effect)
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	pSysBufferTextureView;		// Specifies that the PShader and VShader can use the texture above
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pPixelShader;				// Used to manage the PShader for the pixel-shader stage of the graphics pipeline
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			pVertexShader;				// Used to manage the VShader for the vertex-shader stage of the graphics pipeline
	Microsoft::WRL::ComPtr<ID3D11Buffer>				pVertexBuffer;				// Used to store our vertex data, initializes vertex memory storage
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			pInputLayout;				// Defines how we feed our vertex data into the input-assembler stage of the graphics pipeline.
	Microsoft::WRL::ComPtr<ID3D11SamplerState>			pSamplerState;				// Used to make sampler state for full screen, not entirely sure what this does so far
	D3D11_MAPPED_SUBRESOURCE							mappedSysBufferTexture;     // Provides access to our texture made to be sent to the graphics pipeline
	Color* pPixelBuffer = nullptr;													// A buffer of pixels the size of the screen to be passed to our texture as the next frame

public:
	// Global variables
	static constexpr int screenWidth = 800;
	static constexpr int screenHeight = 600;
};