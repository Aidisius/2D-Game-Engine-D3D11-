#pragma once
#include "AlteredWindows.h"
#include "PixelColors.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <cassert>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

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

	struct VERTEX {FLOAT X, Y, Z; FLOAT U, V;};  // A struct to define a single vertex (cartesian and textureCoords)

	void BeginFrame();                                 // Sets up new frame for changes
	void EndFrame();								   // Finishes frame and shifts frame to GPUvoid InitPipeline();                               // Loads and prepares the shaders
	void AddPixelToBuffer(int x, int y, Color c);      // Changes a pixel in pPixelBuffer
	void AddPixelToBuffer(int x, int y, int r, int g, int b)
	{
		AddPixelToBuffer(x, y, {unsigned char(r), unsigned char(g), unsigned char(b)});
	}

private:
	// Pointers
	IDXGISwapChain*				pSwapChain;
	ID3D11Device*				pDevice;
	ID3D11DeviceContext*			pDeviceContext;
	ID3D11RenderTargetView*		pBackbuffer;
	ID3D11Texture2D*				pSysBufferTexture;
	ID3D11ShaderResourceView*	pSysBufferTextureView;
	ID3D11PixelShader*			pPixelShader;
	ID3D11VertexShader*			pVertexShader;
	ID3D11Buffer*				pVertexBuffer;
	ID3D11InputLayout*			pInputLayout;
	ID3D11SamplerState*			pSamplerState;
	D3D11_MAPPED_SUBRESOURCE							mappedSysBufferTexture;
	Color* pPixelBuffer = nullptr;

public:
	// Global variables
	static constexpr int screenWidth = 800;
	static constexpr int screenHeight = 600;
};