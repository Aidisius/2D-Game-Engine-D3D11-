#include "Graphics.h"


// this function initializes and prepares Direct3D for use
Graphics::Graphics(HWND hWnd)
{
    // create a struct to hold information about the swap chain
    DXGI_SWAP_CHAIN_DESC scd;

    // clear out the struct for use
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

    // fill the swap chain description struct
    scd.BufferCount = 1;                                   // one back buffer
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    // use 32-bit color
    scd.BufferDesc.Width = screenWidth;                    // set the back buffer width
    scd.BufferDesc.Height = screenHeight;                  // set the back buffer height
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;     // how swap chain is to be used
    scd.OutputWindow = hWnd;                               // the window to be used
    scd.SampleDesc.Count = 4;                              // how many multisamples
    scd.Windowed = TRUE;                                   // windowed/full-screen mode
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;    // allow full-screen switching

    // create a device, device context and swap chain using the information in the scd struct
    D3D11CreateDeviceAndSwapChain(NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        NULL,
        NULL,
        NULL,
        D3D11_SDK_VERSION,
        &scd,
        &swapChain,
        &device,
        NULL,
        &deviceContext);


    // get the address of the back buffer
    ID3D11Texture2D* pBackBuffer;
    swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

    // use the back buffer address to create the render target
    device->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
    pBackBuffer->Release();

    // set the render target as the back buffer
    deviceContext->OMSetRenderTargets(1, &backbuffer, NULL);


    // Set the viewport
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = screenWidth;
    viewport.Height = screenHeight;

    deviceContext->RSSetViewports(1, &viewport);

    InitPipeline();
    InitGraphics();
}


// this is the function used to render a single frame
void Graphics::RenderFrame(void)
{
    // clear the back buffer to a blue
    const float color[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
    deviceContext->ClearRenderTargetView(backbuffer, color);

    // select which vertex buffer to display
    UINT stride = sizeof(VERTEX);
    UINT offset = 0;
    deviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

    // select which primtive type we are using
    deviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // draw the vertex buffer to the back buffer
    deviceContext->Draw(3, 0);

    // switch the back buffer and the front buffer
    swapChain->Present(0, 0);
}


// this is the function that cleans up Direct3D and COM
Graphics::~Graphics()
{
    swapChain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode

    // close and release all existing COM objects
    pInputLayout->Release();
    pVertexShader->Release();
    pPixelShader->Release();
    pVertexBuffer->Release();
    swapChain->Release();
    backbuffer->Release();
    device->Release();
    deviceContext->Release();
}


// this is the function that creates the shape to render
void Graphics::InitGraphics()
{
    // create a triangle using the VERTEX struct
    VERTEX OurVertices[] =
    {
        {0.0f, 0.5f, 0.0f, {1.0f, 0.0f, 0.0f, 1.0f}},
        {0.45f, -0.5, 0.0f, {0.0f, 1.0f, 0.0f, 1.0f}},
        {-0.45f, -0.5f, 0.0f, {0.0f, 0.0f, 1.0f, 1.0f} }
    };



    // create the vertex buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
    bd.ByteWidth = sizeof(VERTEX) * 3;             // size is the VERTEX struct * 3
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

    device->CreateBuffer(&bd, NULL, &pVertexBuffer);       // create the buffer


    // copy the vertices into the buffer
    D3D11_MAPPED_SUBRESOURCE ms;
    deviceContext->Map(pVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
    memcpy(ms.pData, OurVertices, sizeof(OurVertices));                 // copy the data
    deviceContext->Unmap(pVertexBuffer, NULL);                                      // unmap the buffer
}


// this function loads and prepares the shaders
void Graphics::InitPipeline()
{
    // load and compile the two shaders
    ID3D10Blob* VS, * PS;
    D3DCompileFromFile(L"shaders.shader", 0, 0, "VShader", "vs_4_0", 0, 0, &VS, 0);
    D3DCompileFromFile(L"shaders.shader", 0, 0, "PShader", "ps_4_0", 0, 0, &PS, 0);
    assert(VS->GetBufferSize() > 0 && VS->GetBufferSize() < 0xFFFFFFFFFFFFFFFF);
    assert(PS->GetBufferSize() > 0 && PS->GetBufferSize() < 0xFFFFFFFFFFFFFFFF);

    // encapsulate both shaders into shader objects
    device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVertexShader);
    device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPixelShader);

    // set the shader objects
    deviceContext->VSSetShader(pVertexShader, 0, 0);
    deviceContext->PSSetShader(pPixelShader, 0, 0);

    // create the input layout object
    D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    device->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &pInputLayout);
    deviceContext->IASetInputLayout(pInputLayout);
}