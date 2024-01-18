//====================================================================================================
// Filename:	RenderTarget.h
// Created by:	Peter Chan
//====================================================================================================

#ifndef INCLUDED_XENGINE_RENDERTARGET_H
#define INCLUDED_XENGINE_RENDERTARGET_H

#include "Texture.h"
#include "XColors.h"

namespace X
{
	class RenderTarget
	{
	public:
		enum class Format
		{
			RGBA_U8,
			RGBA_F16,
			RGBA_U32,
			R_F16,
			R_S32
		};

		RenderTarget() = default;
		~RenderTarget();

		RenderTarget(const RenderTarget&) = delete;
		RenderTarget& operator=(const RenderTarget&) = delete;

		void Initialize(uint32_t width, uint32_t height, Format format);
		void Terminate();

		void BeginRender(const Color& clearColor = Colors::Black);
		void EndRender();

		void BindPS(uint32_t slot);
		void UnbindPS(uint32_t slot);

		void* GetShaderResourceView() const { return mShaderResourceView; }
		uint32_t GetWidth() const { return mWidth; }
		uint32_t GetHeight() const { return mHeight; }

	private:
		ID3D11ShaderResourceView* mShaderResourceView = nullptr;
		ID3D11RenderTargetView* mRenderTargetView = nullptr;
		ID3D11DepthStencilView* mDepthStencilView = nullptr;
		D3D11_VIEWPORT mViewport;
		D3D11_MAPPED_SUBRESOURCE mSubresource;
		uint32_t mWidth = 0;
		uint32_t mHeight = 0;
	};
}

#endif // #ifndef INCLUDED_XENGINE_RENDERTARGET_H
