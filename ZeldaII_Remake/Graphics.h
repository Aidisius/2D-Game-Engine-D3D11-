#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include "PixelColors.h"
#include <cassert>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

class Graphics
{
public:
	// Constructors and destructor
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics operator=(const Graphics&) = delete;
	~Graphics();

public:
	// Functions and structs

	/*void BeginFrame();
	void EndFrame();
	void AddPixelToBuffer();*/

	
	struct VERTEX {FLOAT X, Y, Z; const float color[4];};  // A struct to define a single vertex

	void RenderFrame(void);                                // Renders a single frame
	void InitGraphics(void);                               // Creates the shape to render
	void InitPipeline(void);                               // Loads and prepares the shaders

private:
	// Pointers
	IDXGISwapChain*               swapChain;               // Points to the swap chain interface
	ID3D11Device*                 device;                  // Points to our Direct3D device interface
	ID3D11DeviceContext*          deviceContext;           // Points to our Direct3D device context
	ID3D11RenderTargetView*       backbuffer;              // Points to our back buffer
	ID3D11InputLayout*            pInputLayout;            // Points to the input layout
	ID3D11VertexShader*           pVertexShader;           // Points to the vertex shader
	ID3D11PixelShader*            pPixelShader;            // Points to the pixel shader
	ID3D11Buffer*                 pVertexBuffer;           // Points to the vertex buffer

	//ID3D11Texture2D*              pSysBufferTexture;
	//ID3D11ShaderResourceView*     pSysBufferTextureView;
	//ID3D11SamplerState*           pSamplerState;
	//D3D11_MAPPED_SUBRESOURCE      mappedsysBufferTexture;
	//Color*                        pPixelBuffer = nullptr;  // Points to array of Color objects, later taken to display to the screen in one big texture for the whole window

public:
	// Global variables
	static constexpr int screenWidth = 800;
	static constexpr int screenHeight = 600;
};