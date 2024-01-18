//====================================================================================================
// Filename:	RenderTarget.cpp
// Created by:	Peter Chan
//====================================================================================================

#include "Precompiled.h"
#include "RenderTarget.h"

#include "GraphicsSystem.h"

using namespace X;

namespace
{
	DXGI_FORMAT GetFormats(RenderTarget::Format format)
	{
		switch (format)
		{
		case RenderTarget::Format::RGBA_U8:		return DXGI_FORMAT_R8G8B8A8_UNORM;
		case RenderTarget::Format::RGBA_F16:	return DXGI_FORMAT_R16G16B16A16_FLOAT;
		case RenderTarget::Format::RGBA_U32:	return DXGI_FORMAT_R32G32B32A32_UINT;
		case RenderTarget::Format::R_F16:		return DXGI_FORMAT_R16_FLOAT;
		case RenderTarget::Format::R_S32:		return DXGI_FORMAT_R32_SINT;
		default:
			XASSERT(false, "[RenderTarget] Unsupported format %d.", static_cast<uint32_t>(format));
			break;
		}
		return DXGI_FORMAT_R8G8B8A8_UNORM;
	};
}

RenderTarget::~RenderTarget()
{
	XASSERT(mShaderResourceView == nullptr, "[RenderTarget] Render target not released!");
	XASSERT(mRenderTargetView == nullptr, "[RenderTarget] Render target not released!");
	XASSERT(mDepthStencilView == nullptr, "[RenderTarget] Render target not released!");
}

void RenderTarget::Initialize(uint32_t width, uint32_t height, Format format)
{
	mWidth = width;
	mHeight = height;

	D3D11_TEXTURE2D_DESC desc{};
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = GetFormats(format);
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	auto device = GraphicsSystem::Get()->GetDevice();

	ID3D11Texture2D* texture = nullptr;
	HRESULT hr = device->CreateTexture2D(&desc, nullptr, &texture);
	XASSERT(SUCCEEDED(hr), "[RenderTarget] Failed to create render texture.");

	hr = device->CreateShaderResourceView(texture, nullptr, &mShaderResourceView);
	XASSERT(SUCCEEDED(hr), "[RenderTarget] Failed to create shader resource view.");

	hr = device->CreateRenderTargetView(texture, nullptr, &mRenderTargetView);
	XASSERT(SUCCEEDED(hr), "[RenderTarget] Failed to create render target view.");
	SafeRelease(texture);

	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	hr = device->CreateTexture2D(&desc, nullptr, &texture);
	XASSERT(SUCCEEDED(hr), "[RenderTarget] Failed to create depth stencil texture.");

	hr = device->CreateDepthStencilView(texture, nullptr, &mDepthStencilView);
	XASSERT(SUCCEEDED(hr), "[RenderTarget] Failed to create depth stencil view.");
	SafeRelease(texture);

	mViewport.TopLeftX = 0.0f;
	mViewport.TopLeftY = 0.0f;
	mViewport.Width = static_cast<float>(width);
	mViewport.Height = static_cast<float>(height);
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;
}

void RenderTarget::Terminate()
{
	SafeRelease(mShaderResourceView);
	SafeRelease(mRenderTargetView);
	SafeRelease(mDepthStencilView);
}

void RenderTarget::BeginRender(const Color& clearColor)
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->ClearRenderTargetView(mRenderTargetView, (const FLOAT*)&clearColor);
	context->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	context->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	context->RSSetViewports(1, &mViewport);
}

void RenderTarget::EndRender()
{
	GraphicsSystem::Get()->ResetRenderTarget();
	GraphicsSystem::Get()->ResetViewport();
}

void RenderTarget::BindPS(uint32_t slot)
{
	GraphicsSystem::Get()->GetContext()->PSSetShaderResources(slot, 1, &mShaderResourceView);
}

void RenderTarget::UnbindPS(uint32_t slot)
{
	static ID3D11ShaderResourceView* dummy = nullptr;
	GraphicsSystem::Get()->GetContext()->PSSetShaderResources(slot, 1, &dummy);
}