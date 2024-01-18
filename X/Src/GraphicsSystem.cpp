//====================================================================================================
// Filename:	GraphicsSystem.cpp
// Created by:	Peter Chan
//====================================================================================================

#include "Precompiled.h"
#include "GraphicsSystem.h"

using namespace X;

namespace
{
	HWND sWindow = nullptr;
	WindowMessageHandler sPreviousWndProc = 0;

	const D3D_DRIVER_TYPE kDriverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	const UINT kNumDriverTypes = ARRAYSIZE(kDriverTypes);

	const D3D_FEATURE_LEVEL kFeatureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	const UINT kNumFeatureLevels = ARRAYSIZE(kFeatureLevels);

	GraphicsSystem* sGraphicsSystem = nullptr;
}

LRESULT CALLBACK X::GraphicsSystemMessageHandler(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (sGraphicsSystem)
	{
		switch (message)
		{
			case WM_SIZE:
			{
				const uint32_t width = static_cast<uint32_t>(LOWORD(lParam));
				const uint32_t height = static_cast<uint32_t>(HIWORD(lParam));
				sGraphicsSystem->Resize(width, height);
				break;
			}
		}
	}

	return CallWindowProcA((WNDPROC)sPreviousWndProc, window, message, wParam, lParam);
}

void GraphicsSystem::StaticInitialize(HWND window, bool fullscreen)
{
	XASSERT(sGraphicsSystem == nullptr, "[GraphicsSystem] System already initialized!");
	sGraphicsSystem = new GraphicsSystem();
	sGraphicsSystem->Initialize(window, fullscreen);
}

void GraphicsSystem::StaticTerminate()
{
	if (sGraphicsSystem != nullptr)
	{
		sGraphicsSystem->Terminate();
		SafeDelete(sGraphicsSystem);
	}
}

GraphicsSystem* GraphicsSystem::Get()
{
	XASSERT(sGraphicsSystem != nullptr, "[GraphicsSystem] No system registered.");
	return sGraphicsSystem;
}

GraphicsSystem::GraphicsSystem()
	: mD3DDevice(nullptr)
	, mImmediateContext(nullptr)
	, mSwapChain(nullptr)
	, mRenderTargetView(nullptr)
	, mDepthStencilBuffer(nullptr)
	, mDepthStencilView(nullptr)
	, mDisableDepthStencil(nullptr)
	, mDriverType(D3D_DRIVER_TYPE_NULL)
	, mFeatureLevel(D3D_FEATURE_LEVEL_11_0)
{
}

GraphicsSystem::~GraphicsSystem()
{
	XASSERT(mD3DDevice == nullptr, "[GraphicsSystem] Terminate() must be called to clean up!");
}

void GraphicsSystem::Initialize(HWND window, bool fullscreen)
{
	// Now that we have a window that is displaying, we can continue to set up a Direct3D 11 device.
	// Setup is necessary if we are going to render any 3D scene. The first thing to do is to create
	// three objects: a device, an immediate context, and a swap chain. The immediate context is a
	// new object in Direct3D 11.

	// In Direct3D 11, the immediate context is used by the application to perform rendering onto a
	// buffer, and the device contains methods to create resources.

	// The swap chain is responsible for taking the buffer to which the device renders, and displaying
	// the content on the actual monitor screen. The swap chain contains two or more buffers, mainly
	// the front and the back. These are textures to which the device renders in order to display on
	// the monitor. The front buffer is what is being presented currently to the user. This buffer is
	// read-only and cannot be modified. The back buffer is the render target to which the device will
	// draw. Once it finishes the drawing operation, the swap chain will present the backbuffer by
	// swapping the two buffers. The back buffer becomes the front buffer, and vice versa.

	RECT rc = { 0 };
	GetClientRect(window, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;

	// To create the swap chain, we fill out a DXGI_SWAPCHAIN_DESC structure that describes the swap
	// chain we are about to create. A few fields are worth mentioning. BackBufferUsage is a flag that
	// tells the application how the back buffer will be used. In this case, we want to render to the
	// back buffer, so we'll set BackBufferUsage to DXGI_USAGE_RENDER_TARGET_OUTPUT. The OutputWindow
	// field represents the window that the swap chain will use to present images on the screen.
	// SampleDesc is used to enable multi-sampling. Since this tutorial does not use multi-sampling,
	// SampleDesc's Count is set to 1 and Quality to 0 to have multi-sampling disabled.

	DXGI_SWAP_CHAIN_DESC descSwapChain;
	ZeroMemory(&descSwapChain, sizeof(descSwapChain));
	descSwapChain.BufferCount = 1;
	descSwapChain.BufferDesc.Width = width;
	descSwapChain.BufferDesc.Height = height;
	descSwapChain.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	descSwapChain.BufferDesc.RefreshRate.Numerator = 60;
	descSwapChain.BufferDesc.RefreshRate.Denominator = 1;
	descSwapChain.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	descSwapChain.OutputWindow = window;
	descSwapChain.SampleDesc.Count = 1;
	descSwapChain.SampleDesc.Quality = 0;
	descSwapChain.Windowed = !fullscreen;

	HRESULT hr = S_OK;
	for (UINT driverTypeIndex = 0; driverTypeIndex < kNumDriverTypes; ++driverTypeIndex)
	{
		mDriverType = kDriverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain
		(
			nullptr,
			mDriverType,
			nullptr,
			createDeviceFlags,
			kFeatureLevels,
			kNumFeatureLevels,
			D3D11_SDK_VERSION,
			&descSwapChain,
			&mSwapChain,
			&mD3DDevice,
			&mFeatureLevel,
			&mImmediateContext
		);
		if (SUCCEEDED(hr))
		{
			break;
		}
	}
	XASSERT(SUCCEEDED(hr), "[GraphicsSystem] Failed to create device or swap chain.");

	// Cache swap chain description
	mSwapChain->GetDesc(&mSwapChainDesc);

	// Initialize render target and depth stencil views
	Resize(GetBackBufferWidth(), GetBackBufferHeight());

	// Hook application to window's procedure
	sWindow = window;
	sPreviousWndProc = (WindowMessageHandler)GetWindowLongPtrA(window, GWLP_WNDPROC);
	SetWindowLongPtrA(window, GWLP_WNDPROC, (LONG_PTR)GraphicsSystemMessageHandler);
}

void GraphicsSystem::Terminate()
{
	// Restore original window's procedure
	SetWindowLongPtrA(sWindow, GWLP_WNDPROC, (LONG_PTR)sPreviousWndProc);
	sWindow = nullptr;

	SafeRelease(mDisableDepthStencil);
	SafeRelease(mDepthStencilView);
	SafeRelease(mDepthStencilBuffer);
	SafeRelease(mRenderTargetView);
	SafeRelease(mSwapChain);
	SafeRelease(mImmediateContext);
	SafeRelease(mD3DDevice);
}

void GraphicsSystem::BeginRender(const Color& clearColor)
{
	mImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	mImmediateContext->ClearRenderTargetView(mRenderTargetView, (const FLOAT*)&clearColor);
	mImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void GraphicsSystem::EndRender()
{
	mSwapChain->Present(1, 0);
}

void GraphicsSystem::ToggleFullscreen()
{
	BOOL fullscreen;
	mSwapChain->GetFullscreenState(&fullscreen, nullptr);
	mSwapChain->SetFullscreenState(!fullscreen, nullptr);
}

void GraphicsSystem::Resize(uint32_t width, uint32_t height)
{
	mImmediateContext->OMSetRenderTargets(0, nullptr, nullptr);

	SafeRelease(mRenderTargetView);
	SafeRelease(mDepthStencilView);

	HRESULT hr;
	if (width != GetBackBufferWidth() || height != GetBackBufferHeight())
	{
		hr = mSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
		XASSERT(SUCCEEDED(hr), "[GraphicsSystem] Failed to resize swap chain buffer.");

		// Cache swap chain description
		mSwapChain->GetDesc(&mSwapChainDesc);
	}

	// Create a render target view
	ID3D11Texture2D* backBuffer = nullptr;
	hr = mSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	//hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	XASSERT(SUCCEEDED(hr), "[GraphicsSystem] Failed to access swap chain buffer.");

	hr = mD3DDevice->CreateRenderTargetView(backBuffer, nullptr, &mRenderTargetView);
	SafeRelease(backBuffer);
	XASSERT(SUCCEEDED(hr), "[GraphicsSystem] Failed to create render target view.");

	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = GetBackBufferWidth();
	descDepth.Height = GetBackBufferHeight();
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = mD3DDevice->CreateTexture2D(&descDepth, nullptr, &mDepthStencilBuffer);
	XASSERT(SUCCEEDED(hr), "[GraphicsSystem] Failed to create depth stencil buffer.");

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = mD3DDevice->CreateDepthStencilView(mDepthStencilBuffer, &descDSV, &mDepthStencilView);
	XASSERT(SUCCEEDED(hr), "[GraphicsSystem] Failed to create depth stencil view.");

	// Set the render target view and depth stencil view
	mImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	// Setup the viewport
	mViewport.Width = static_cast<float>(GetBackBufferWidth());
	mViewport.Height = static_cast<float>(GetBackBufferHeight());
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;
	mViewport.TopLeftX = 0;
	mViewport.TopLeftY = 0;
	mImmediateContext->RSSetViewports(1, &mViewport);
}

void GraphicsSystem::ResetRenderTarget()
{
	XASSERT(mImmediateContext != nullptr, "[GraphicsSystem] Failed to reset render target.");
	mImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
}

void GraphicsSystem::ResetViewport()
{
	XASSERT(mImmediateContext != nullptr, "[GraphicsSystem] Failed to reset viewport.");
	mImmediateContext->RSSetViewports(1, &mViewport);
}

void GraphicsSystem::EnableDepthTesting(bool enable)
{
	XASSERT(mImmediateContext != nullptr, "[GraphicsSystem] Failed to set depth stencil state.");
	mImmediateContext->OMSetDepthStencilState(enable ? nullptr : mDisableDepthStencil, 0);
}

uint32_t GraphicsSystem::GetBackBufferWidth() const
{
	XASSERT(mSwapChain != nullptr, "[GraphicsSystem] Failed to get swap chain buffer width.");
	return mSwapChainDesc.BufferDesc.Width;
}

uint32_t GraphicsSystem::GetBackBufferHeight() const
{
	XASSERT(mSwapChain != nullptr, "[GraphicsSystem] Failed to get swap chain buffer width.");
	return mSwapChainDesc.BufferDesc.Height;
}

float GraphicsSystem::GetBackBufferAspectRatio() const
{
	return static_cast<float>(GetBackBufferWidth()) / static_cast<float>(GetBackBufferHeight());
}