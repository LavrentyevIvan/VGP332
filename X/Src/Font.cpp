//====================================================================================================
// Filename:	Font.cpp
// Created by:	Peter Chan
//====================================================================================================

#include "Precompiled.h"
#include "Font.h"

#include "GraphicsSystem.h"
#include <FW1FontWrapper/Inc/FW1FontWrapper.h>

using namespace X;

namespace
{
	const wchar_t* sFontFamily = L"Consolas";
}

//----------------------------------------------------------------------------------------------------

Font::Font()
	: mFW1Factory(nullptr)
	, mFontWrapper(nullptr)
{
}

//----------------------------------------------------------------------------------------------------

Font::~Font()
{
}

//----------------------------------------------------------------------------------------------------

void Font::Initialize()
{
	ID3D11Device* device = GraphicsSystem::Get()->GetDevice();
	FW1CreateFactory(FW1_VERSION, &mFW1Factory);
	auto hr = mFW1Factory->CreateFontWrapper(device, sFontFamily, &mFontWrapper);
	XASSERT(SUCCEEDED(hr), "[Font] Failed to initialize FW1FontWrapper. Error = %x", hr);
}

//----------------------------------------------------------------------------------------------------

void Font::Terminate()
{
	SafeRelease(mFontWrapper);
	SafeRelease(mFW1Factory);
}

//----------------------------------------------------------------------------------------------------

float Font::GetStringWidth(const wchar_t* str, float size) const
{
	FW1_RECTF layoutRect;
	layoutRect.Left = 0.0f;
	layoutRect.Top = 0.0f;
	layoutRect.Right = (float)GraphicsSystem::Get()->GetBackBufferWidth();
	layoutRect.Bottom = (float)GraphicsSystem::Get()->GetBackBufferHeight();
	auto rect = mFontWrapper->MeasureString(str, sFontFamily, size, &layoutRect, FW1_RESTORESTATE);
	return rect.Right - rect.Left;
}

//----------------------------------------------------------------------------------------------------

void Font::Draw(const wchar_t* str, float x, float y, float size, uint32_t color)
{
	ID3D11DeviceContext* context = GraphicsSystem::Get()->GetContext();
	mFontWrapper->DrawString(context, str, x, y, size, color, FW1_RESTORESTATE);
}