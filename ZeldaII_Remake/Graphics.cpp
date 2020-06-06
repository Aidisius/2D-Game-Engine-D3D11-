#include "Graphics.h"


namespace shaders
{
#include "PShader.shh"
#include "VShader.shh"
}

// This function initializes and prepares Direct3D for use
Graphics::Graphics(HWND& hWnd)
{
    assert(hWnd != nullptr);

    // Create a struct to hold information about the swap chain
    DXGI_SWAP_CHAIN_DESC pSwapChainDesc = {};
    
    // Fill the swap chain description struct
    pSwapChainDesc.BufferDesc.Width = screenWidth;                    // Set the back buffer width
    pSwapChainDesc.BufferDesc.Height = screenHeight;                  // Set the back buffer height
    pSwapChainDesc.BufferDesc.RefreshRate.Numerator = 1;
    pSwapChainDesc.BufferDesc.RefreshRate.Denominator = 60;           // 60 Hz refresh rate on app window
    pSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;    // Use 8-bit color
    pSwapChainDesc.SampleDesc.Count = 1;                              // How many multisamples for Anti-aliasing
    pSwapChainDesc.SampleDesc.Quality = 0;                            // Lowest image quality for maximum efficiency
    pSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;     // how swap chain is to be used: To write to the screen
    pSwapChainDesc.BufferCount = 1;                                   // One back buffer
    pSwapChainDesc.OutputWindow = hWnd;                               // The window to be used
    pSwapChainDesc.Windowed = TRUE;                                   // Windowed/full-screen mode
    pSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;    // Allow full-screen switching


    // Create a pDevice, pDevice context and swap chain using the information in the scd struct
    D3D11CreateDeviceAndSwapChain(
        nullptr,                                                      // Use the default video adapter
        D3D_DRIVER_TYPE_HARDWARE,                                     // Use Direct3D and the graphics card
        nullptr,                                                      // Not using D3D_DRIVER_TYPE_SOFTWARE, so NULL
        0u,                                                           // No runtime layers enabled/no flags enabled
        nullptr,                                                      // No feature levels. so no pointer to them
        0,                                                            // No feature levels
        D3D11_SDK_VERSION,                                            // SDK version
        &pSwapChainDesc,                                              // Insert initialization parameters given above
        &pSwapChain,                                                  // Get address to pSwapChain made
        &pDevice,                                                     // Get address to pDevice made
        nullptr,                                                      // Pointer to feature levels not needed since there are none
        &pDeviceContext);                                             // Get address to pDevice context made


    // Get the address of the back buffer
    ID3D11Texture2D* ppBackbuffer;
    pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&ppBackbuffer);

    // Use the back buffer address to create the render target
    pDevice->CreateRenderTargetView(ppBackbuffer, nullptr, &pBackbuffer);
    ppBackbuffer->Release();

    // Set the back buffer as the render target 
    pDeviceContext->OMSetRenderTargets(1, &pBackbuffer, nullptr);


    // Set the viewport
    D3D11_VIEWPORT viewport;
    viewport.TopLeftX = 0.0f;                                        // X pos of the left hand size of the viewport
    viewport.TopLeftY = 0.0f;                                        // y pos of the top of the viewport
    viewport.Width = float(Graphics::screenWidth);
    viewport.Height = float(Graphics::screenHeight);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    pDeviceContext->RSSetViewports(1, &viewport);

    // Create texture for cpu render target
    D3D11_TEXTURE2D_DESC sysTexDesc;
    sysTexDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;                  // 3 8-bit colors and 8-bit alpha
    sysTexDesc.Width = Graphics::screenWidth;
    sysTexDesc.Height = Graphics::screenHeight;                      // Make texture the size of the window client screen
    sysTexDesc.ArraySize = 1;                                        // Texture count
    sysTexDesc.MipLevels = 1;                                        // A multisampled texture
    sysTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;               // Binds the texture to a shader stage
    sysTexDesc.Usage = D3D11_USAGE_DYNAMIC;                          // Texture is accessable by both GPU and CPU, good for updates by CPU at least once per frame
    sysTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;              // Texture is mappable to be edited by CPU
    sysTexDesc.SampleDesc.Count = 1;                                 // How many multisamples for Anti-aliasing
    sysTexDesc.SampleDesc.Quality = 0;                               // Lowest image quality for maximum efficiency
    sysTexDesc.MiscFlags = 0;                                        // None

    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
    shaderResourceViewDesc.Format = sysTexDesc.Format;               // 3 8-bit colors and 8-bit alpha
    shaderResourceViewDesc.ViewDimension = 
                         D3D11_SRV_DIMENSION_TEXTURE2D;              // Texture is 2D
    shaderResourceViewDesc.Texture2D.MipLevels = 1;                  // Maximum number of mipmap levels for the view
    pDevice->CreateShaderResourceView(pSysBufferTexture,
                                     &shaderResourceViewDesc,
                                     &pSysBufferTextureView);        // Create the resource view on the texture


    // Create pixel shader and vertex shader
    pDevice->CreatePixelShader(
        shaders::PShaderBytecode,                                // Pointer to compiled shader
        sizeof(shaders::PShaderBytecode),                        // Size of the compiled pixel shader
        nullptr,                                                     // No class linkage interface
        &pPixelShader);                                              // Get address to PixelShader

    pDevice->CreateVertexShader(
        shaders::VShaderBytecode,                               // Pointer to compiled shader
        sizeof(shaders::VShaderBytecode),                       // Size of the compiled vertex shader
        nullptr,                                                     // No class linkage interface
        &pVertexShader);                                             // Get address to VertexShader


    // Make two triangles / a rectangle for the shaders to fill the screen
    const VERTEX screenVertices[] =
    {
        { -1.0f,1.0f,0.5f,0.0f,0.0f },
        { 1.0f,1.0f,0.5f,1.0f,0.0f },
        { 1.0f,-1.0f,0.5f,1.0f,1.0f },
        { -1.0f,1.0f,0.5f,0.0f,0.0f },
        { 1.0f,-1.0f,0.5f,1.0f,1.0f },
        { -1.0f,-1.0f,0.5f,0.0f,1.0f },
    };
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.ByteWidth = sizeof(VERTEX) * 6;                        // 8 Vertices for the two triangles / big rectangle
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;                  // Set flag to Bind screenVertices buffer as a vertex buffer
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;                           // Read and write acces for GPU
    bufferDesc.CPUAccessFlags = 0u;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = screenVertices;                                 // Get data from screenVertices for bufferDesc
    pDevice->CreateBuffer(&bufferDesc, &initData, &pVertexBuffer);      // Take screenVertices data and bufferDesc data and create actual vertex buffer


    // Input Layout for fullscreen rectangle, connecting to shaders in GPU
    const D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
    {
        { "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
        { "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 }
    };
    pDevice->CreateInputLayout(inputElementDesc, 2,                     // Pass array of input-assembler stage input data types, and number of elements in array
        shaders::VShaderBytecode,                                 // Pointer to compiled vertex shader
        sizeof(shaders::VShaderBytecode),                         // Size of compiled vertex shader
        &pInputLayout);                                                // Get address to InputLayout


    // Sampler state for fullscreen texture rectangle
    D3D11_SAMPLER_DESC sampDesc = {};
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;                  // Use point sampling for minification, magnification, mid-level sampling
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;                   // Texture coords outside [0.0, 1.0] are set to texture color at 0.0 or 1.0 respectively
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;                   // Same as above, but for the texture coord "y" instead of texture coord "x"
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;                   // Same as above, although 3rd axis not utilized
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;                  // Never compare sampled data against exisiting sampled data
    sampDesc.MinLOD = 0;                                               // 0 - most detailed mipmap level
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;                               // No upper limit on LOD, least detailed level
    pDevice->CreateSamplerState(&sampDesc, &pSamplerState);


    // allocate memory for sysbuffer (16-byte aligned for faster access)
    pPixelBuffer = reinterpret_cast<Color*>(
        _aligned_malloc(sizeof(Color) * Graphics::screenWidth * Graphics::screenHeight, 16u));
}


// this is the function that cleans up Direct3D and COM
Graphics::~Graphics()
{
    pSwapChain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode

    // close and release all existing COM objects
    pInputLayout->Release();
    pVertexShader->Release();
    pPixelShader->Release();
    pVertexBuffer->Release();
    pSwapChain->Release();
    pBackbuffer->Release();
    pDevice->Release();
    pSysBufferTexture->Release();
    pSysBufferTextureView->Release();
    pSamplerState->Release();
    
    if (pPixelBuffer) {

        _aligned_free(pPixelBuffer);
        pPixelBuffer = nullptr;
    }
    // clear the state of the pDevice context before destruction
    if (pDeviceContext) pDeviceContext->ClearState();
    pDeviceContext->Release();
}


void Graphics::EndFrame()
{
    // Lock and map the adapter memory for copying over the pPixelBuffer
    //    to be a texture made for the rectangle to cover the whole screen
    pDeviceContext->Map(pSysBufferTexture, 0u,
        D3D11_MAP_WRITE_DISCARD, 0u, &mappedSysBufferTexture);
    

    // Setup parameters for copy operation
    Color* pDataSBT = reinterpret_cast<Color*>(mappedSysBufferTexture.pData);
    const size_t dataWidth = mappedSysBufferTexture.RowPitch / sizeof(Color); // Row size of mapped resource / sizeof(Color) = number of Color objects in 2D texture
    const size_t scrWidth = Graphics::screenWidth;
    const size_t rowBytes = scrWidth * sizeof(Color);


    // Copy the pixel buffer a row at a time
    for (size_t y = 0u; y < Graphics::screenHeight; y++)
    {
        memcpy(&pDataSBT[y * dataWidth], &pPixelBuffer[y * scrWidth], rowBytes);
    }
    // Release the adapter memory
    pDeviceContext->Unmap(pSysBufferTexture, 0u);

    // Render scene texture to back buffer (offscreen buffer)
    pDeviceContext->IASetInputLayout(pInputLayout);                                              // Connect InputLayout to the pDevice Context
    pDeviceContext->VSSetShader(pVertexShader, nullptr, 0u);                                     // Connect VertexShader for GPU to vertex shader made
    pDeviceContext->PSSetShader(pPixelShader, nullptr, 0u);                                      // Connect PixelShader for GPU to pixel shader made
    pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);               // When drawing vertices, connect the lines so two triangles
                                                                                                //     are drawn, forming a rectangle
    const UINT stride = sizeof(VERTEX);
    const UINT offset = 0u;
    pDeviceContext->IASetVertexBuffers(0u, 1u, &pVertexBuffer, &stride, &offset);                // Connect VertxBuffer for GPU to vertex buffer made
    pDeviceContext->PSSetShaderResources(0u, 1u, &pSysBufferTextureView);                        // Connect ShaderResource for GPU to texture buffer made
    pDeviceContext->PSSetSamplers(0u, 1u, &pSamplerState);                                       // Connect Sampler for GPU to sampler state made
    pDeviceContext->Draw(6u, 0u);                                                                // Draw the 6 vertices (3 triangles / rectangle)
                                                                                                //    with pixels from pPixelBuffer

    // flip back/front buffers to see results
    pSwapChain->Present(1u, 0u);
}


void Graphics::BeginFrame()
{
    // free sysbuffer memory (aligned free)
    if (pPixelBuffer)
    {
        _aligned_free(pPixelBuffer);
        pPixelBuffer = nullptr;
    }
    // clear the state of the device context before destruction
    if (pDeviceContext) pDeviceContext->ClearState();
}


void Graphics::AddPixelToBuffer(int x, int y, Color c)
{
    assert(x >= 0);
    assert(x < int(Graphics::screenWidth));
    assert(y >= 0);
    assert(y < int(Graphics::screenHeight));
    pPixelBuffer[Graphics::screenWidth * y + x] = c;
}

