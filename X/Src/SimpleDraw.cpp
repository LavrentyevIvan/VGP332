//====================================================================================================
// Filename:	SimpleDraw.cpp
// Created by:	Peter Chan
//====================================================================================================

#include "Precompiled.h"
#include "SimpleDraw.h"

#include "ConstantBuffer.h"
#include "GraphicsSystem.h"
#include "PixelShader.h"
#include "Vertex.h"
#include "VertexShader.h"

using namespace X;

namespace
{
	const char* kSimpleShader =
		"cbuffer CBSimpleDraw : register(b0)"
		"{"
		"	matrix transform;"
		"}"
		"struct A2V"
		"{"
		"	float4 position : POSITION;"
		"	float4 color : COLOR;"
		"};"
		"struct V2P"
		"{"
		"	float4 position : SV_POSITION;"
		"	float4 color : COLOR;"
		"};"
		"V2P VS(A2V input)"
		"{"
		"	V2P output = (V2P)0;"
		"	output.position = mul(input.position, transform);"
		"	output.color = input.color;"
		"	return output;"
		"}"
		"float4 PS(V2P input) : SV_Target"
		"{"
		"	return input.color;"
		"}";

	struct CBSimpleDraw
	{
		Math::Matrix4 transform;
	};

	class SimpleDrawImpl
	{
	public:
		// Constructor
		SimpleDrawImpl();

		// Destructor
		~SimpleDrawImpl();

		// Functions to startup/shutdown simple draw
		void Initialize(uint32_t maxVertices);
		void Terminate();

		// Function to set 2D sizes and transform
		void SetTransform(const Math::Matrix4& transform);
		void SetScreenSize(uint32_t width, uint32_t height);
		void SetPixelSize(uint32_t pixelSize);

		// Functions to add 3D lines
		void AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color);
		void AddAABB(const Math::AABB& aabb, const Color& color);
		void AddOBB(const Math::OBB& obb, const Color& color);
		void AddSphere(const Math::Sphere& sphere, const Color& color, uint32_t slices, uint32_t rings);
		void AddTransform(const Math::Matrix4& transform);

		// Function to add pixel
		void AddPixel(int x, int y, const Color& color);

		// Functions to add screen lines
		void AddScreenLine(const Math::Vector2& v0, const Math::Vector2& v1, const Color& color);
		void AddScreenRect(const Math::Rect& rect, const Color& color);
		void AddScreenCircle(const Math::Circle& circle, const Color& color);
		void AddScreenArc(const Math::Vector2& center, float radius, float fromAngle, float toAngle, const Color& color);
		void AddScreenDiamond(const Math::Vector2& center, float size, const Color& color);
		void AddScreenGrid(uint32_t cellSize, const Color& color);

		// Function to render all the lines added
		void Render(const Camera& camera);

	private:
		VertexShader mVertexShader;
		PixelShader mPixelShader;

		TypedConstantBuffer<CBSimpleDraw> mConstantBuffer;

		ID3D11Buffer* mVertexBuffer;
		ID3D11Buffer* mVertexBuffer2D;
		ID3D11Buffer* mPixelBuffer;

		VertexPC* mVertices3D;
		VertexPC* mVertices2D;
		VertexPC* mPixels;

		uint32_t mMaxVertices;
		uint32_t mNumVertices3D;
		uint32_t mNumVertices2D;
		uint32_t mMaxPixels;
		uint32_t mNumPixels;

		Math::Matrix4 mTransform;
		uint32_t mScreenWidth;
		uint32_t mScreenHeight;
		uint32_t mPixelSize;

		X::Color mGridColor;
		uint32_t mGridCellSize;
		bool mShowGrid;

		bool mInitialized;
	};

	SimpleDrawImpl::SimpleDrawImpl()
		: mVertexBuffer(nullptr)
		, mVertexBuffer2D(nullptr)
		, mPixelBuffer(nullptr)
		, mVertices3D(nullptr)
		, mVertices2D(nullptr)
		, mPixels(nullptr)
		, mMaxVertices(0)
		, mNumVertices2D(0)
		, mNumVertices3D(0)
		, mMaxPixels(0)
		, mNumPixels(0)
		, mScreenWidth(0)
		, mScreenHeight(0)
		, mPixelSize(1)
		, mGridColor(X::Colors::DarkGray)
		, mGridCellSize(0)
		, mShowGrid(false)
		, mInitialized(false)
	{
	}

	SimpleDrawImpl::~SimpleDrawImpl()
	{
		XASSERT(!mInitialized, "[SimpleDraw] System not shutdown properly.");
	}

	void SimpleDrawImpl::Initialize(uint32_t maxVertices)
	{
		XASSERT(!mInitialized, "[SimpleDraw] Already initialized.");

		const uint32_t kSimpleShaderSize = (uint32_t)strlen(kSimpleShader) + 1;
		mVertexShader.Initialize(kSimpleShader, kSimpleShaderSize, "VS", "vs_5_0", VertexPC::Format);
		mPixelShader.Initialize(kSimpleShader, kSimpleShaderSize, "PS", "ps_5_0");
		mConstantBuffer.Initialize();

		D3D11_BUFFER_DESC bd{};
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = maxVertices * sizeof(VertexPC);
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;

		// Create vertex buffers for 3D/2D lines
		auto graphicsystem = GraphicsSystem::Get();
		ID3D11Device* device = graphicsystem->GetDevice();
		device->CreateBuffer(&bd, nullptr, &mVertexBuffer);
		device->CreateBuffer(&bd, nullptr, &mVertexBuffer2D);

		// Create line buffers
		mVertices3D = new VertexPC[maxVertices];
		mVertices2D = new VertexPC[maxVertices];

		mMaxVertices = maxVertices;
		mNumVertices3D = 0;
		mNumVertices2D = 0;

		// Create pixel buffer
		const uint32_t pixelCount = graphicsystem->GetBackBufferWidth() * graphicsystem->GetBackBufferHeight();
		bd.ByteWidth = pixelCount * sizeof(VertexPC);
		device->CreateBuffer(&bd, nullptr, &mPixelBuffer);

		mPixels = new VertexPC[pixelCount];
		mMaxPixels = pixelCount;
		mNumPixels = 0;

		// Set flag
		mInitialized = true;
	}

	void SimpleDrawImpl::Terminate()
	{
		XASSERT(mInitialized, "[SimpleDraw] Not initialized.");

		// Release everything
		SafeDeleteArray(mPixels);
		SafeDeleteArray(mVertices2D);
		SafeDeleteArray(mVertices3D);

		SafeRelease(mPixelBuffer);
		SafeRelease(mVertexBuffer2D);
		SafeRelease(mVertexBuffer);

		mConstantBuffer.Terminate();
		mPixelShader.Terminate();
		mVertexShader.Terminate();

		// Clear flag
		mInitialized = false;
	}

	void SimpleDrawImpl::SetTransform(const Math::Matrix4& transform)
	{
		mTransform = transform;
	}

	void SimpleDrawImpl::SetScreenSize(uint32_t width, uint32_t height)
	{
		mScreenWidth = width;
		mScreenHeight = height;
	}

	void SimpleDrawImpl::SetPixelSize(uint32_t pixelSize)
	{
		mPixelSize = pixelSize;
	}

	void SimpleDrawImpl::AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color)
	{
		XASSERT(mInitialized, "[SimpleDraw] Not initialized.");

		// Check if we have enough space
		if (mNumVertices3D + 2 <= mMaxVertices)
		{
			// Add line
			mVertices3D[mNumVertices3D++] = { v0, color };
			mVertices3D[mNumVertices3D++] = { v1, color };
		}
	}

	void SimpleDrawImpl::AddAABB(const Math::AABB& aabb, const Color& color)
	{
		XASSERT(mInitialized, "[SimpleDraw] Not initialized.");

		// Check if we have enough space
		if (mNumVertices3D + 24 <= mMaxVertices)
		{
			float minX = aabb.center.x - aabb.extend.x;
			float minY = aabb.center.y - aabb.extend.y;
			float minZ = aabb.center.z - aabb.extend.z;
			float maxX = aabb.center.x + aabb.extend.x;
			float maxY = aabb.center.y + aabb.extend.y;
			float maxZ = aabb.center.z + aabb.extend.z;

			// Add lines
			mVertices3D[mNumVertices3D++] = { Math::Vector3(minX, minY, minZ), color };
			mVertices3D[mNumVertices3D++] = { Math::Vector3(minX, minY, maxZ), color };

			mVertices3D[mNumVertices3D++] = { Math::Vector3(minX, minY, maxZ), color };
			mVertices3D[mNumVertices3D++] = { Math::Vector3(maxX, minY, maxZ), color };

			mVertices3D[mNumVertices3D++] = { Math::Vector3(maxX, minY, maxZ), color };
			mVertices3D[mNumVertices3D++] = { Math::Vector3(maxX, minY, minZ), color };

			mVertices3D[mNumVertices3D++] = { Math::Vector3(maxX, minY, minZ), color };
			mVertices3D[mNumVertices3D++] = { Math::Vector3(minX, minY, minZ), color };

			mVertices3D[mNumVertices3D++] = { Math::Vector3(minX, minY, minZ), color };
			mVertices3D[mNumVertices3D++] = { Math::Vector3(minX, maxY, minZ), color };

			mVertices3D[mNumVertices3D++] = { Math::Vector3(minX, minY, maxZ), color };
			mVertices3D[mNumVertices3D++] = { Math::Vector3(minX, maxY, maxZ), color };

			mVertices3D[mNumVertices3D++] = { Math::Vector3(maxX, minY, maxZ), color };
			mVertices3D[mNumVertices3D++] = { Math::Vector3(maxX, maxY, maxZ), color };

			mVertices3D[mNumVertices3D++] = { Math::Vector3(maxX, minY, minZ), color };
			mVertices3D[mNumVertices3D++] = { Math::Vector3(maxX, maxY, minZ), color };

			mVertices3D[mNumVertices3D++] = { Math::Vector3(minX, maxY, minZ), color };
			mVertices3D[mNumVertices3D++] = { Math::Vector3(minX, maxY, maxZ), color };

			mVertices3D[mNumVertices3D++] = { Math::Vector3(minX, maxY, maxZ), color };
			mVertices3D[mNumVertices3D++] = { Math::Vector3(maxX, maxY, maxZ), color };

			mVertices3D[mNumVertices3D++] = { Math::Vector3(maxX, maxY, maxZ), color };
			mVertices3D[mNumVertices3D++] = { Math::Vector3(maxX, maxY, minZ), color };

			mVertices3D[mNumVertices3D++] = { Math::Vector3(maxX, maxY, minZ), color };
			mVertices3D[mNumVertices3D++] = { Math::Vector3(minX, maxY, minZ), color };
		}
	}

	void SimpleDrawImpl::AddOBB(const Math::OBB& obb, const Color& color)
	{
		XASSERT(mInitialized, "[SimpleDraw] Not initialized.");

		Math::Matrix4 matTrans = Math::Matrix4::Translation(obb.center);
		Math::Matrix4 matRot = Math::MatrixRotationQuaternion(obb.orientation);
		Math::Matrix4 matScale = Math::Matrix4::Scaling(obb.extend);
		Math::Matrix4 toWorld = matScale * matRot * matTrans;

		Math::Vector3 points[] =
		{
			Math::Vector3(-1.0f, -1.0f, -1.0f),
			Math::Vector3(-1.0f,  1.0f, -1.0f),
			Math::Vector3(1.0f,  1.0f, -1.0f),
			Math::Vector3(1.0f, -1.0f, -1.0f),
			Math::Vector3(-1.0f, -1.0f,  1.0f),
			Math::Vector3(-1.0f,  1.0f,  1.0f),
			Math::Vector3(1.0f,  1.0f,  1.0f),
			Math::Vector3(1.0f, -1.0f,  1.0f)
		};

		for (uint32_t i = 0; i < 8; ++i)
		{
			points[i] = Math::TransformCoord(points[i], toWorld);
		}

		AddLine(points[0], points[1], color);
		AddLine(points[1], points[2], color);
		AddLine(points[2], points[3], color);
		AddLine(points[3], points[0], color);

		AddLine(points[0], points[4], color);
		AddLine(points[1], points[5], color);
		AddLine(points[2], points[6], color);
		AddLine(points[3], points[7], color);

		AddLine(points[4], points[5], color);
		AddLine(points[5], points[6], color);
		AddLine(points[6], points[7], color);
		AddLine(points[7], points[4], color);
	}

	void SimpleDrawImpl::AddSphere(const Math::Sphere& sphere, const Color& color, uint32_t slices, uint32_t rings)
	{
		XASSERT(mInitialized, "[SimpleDraw] Not initialized.");

		const float x = sphere.center.x;
		const float y = sphere.center.y;
		const float z = sphere.center.z;
		const float radius = sphere.radius;

		const uint32_t kSlices = Math::Max(3u, slices);
		const uint32_t kRings = Math::Max(2u, rings);
		const uint32_t kLines = (4 * kSlices * kRings) - (2 * kSlices);

		// Check if we have enough space
		if (mNumVertices3D + kLines <= mMaxVertices)
		{
			// Add lines
			const float kTheta = Math::kPi / (float)kRings;
			const float kPhi = Math::kTwoPi / (float)kSlices;
			for (uint32_t j = 0; j < kSlices; ++j)
			{
				for (uint32_t i = 0; i < kRings; ++i)
				{
					const float a = i * kTheta;
					const float b = a + kTheta;
					const float ay = radius * cos(a);
					const float by = radius * cos(b);

					const float theta = j * kPhi;
					const float phi = theta + kPhi;

					const float ar = sqrt(radius * radius - ay * ay);
					const float br = sqrt(radius * radius - by * by);

					const float x0 = x + (ar * sin(theta));
					const float y0 = y + (ay);
					const float z0 = z + (ar * cos(theta));

					const float x1 = x + (br * sin(theta));
					const float y1 = y + (by);
					const float z1 = z + (br * cos(theta));

					const float x2 = x + (br * sin(phi));
					const float y2 = y + (by);
					const float z2 = z + (br * cos(phi));

					mVertices3D[mNumVertices3D++] = { Math::Vector3(x0, y0, z0), color };
					mVertices3D[mNumVertices3D++] = { Math::Vector3(x1, y1, z1), color };

					if (i < kRings - 1)
					{
						mVertices3D[mNumVertices3D++] = { Math::Vector3(x1, y1, z1), color };
						mVertices3D[mNumVertices3D++] = { Math::Vector3(x2, y2, z2), color };
					}
				}
			}
		}
	}

	void SimpleDrawImpl::AddTransform(const Math::Matrix4& transform)
	{
		Math::Vector3 position = Math::GetTranslation(transform);
		Math::Vector3 right = Math::GetRight(transform);
		Math::Vector3 up = Math::GetUp(transform);
		Math::Vector3 forward = Math::GetForward(transform);
		AddLine(position, position + right, Colors::Red);
		AddLine(position, position + up, Colors::Green);
		AddLine(position, position + forward, Colors::Blue);
	}

	void SimpleDrawImpl::AddPixel(int x, int y, const Color& color)
	{
		XASSERT(mInitialized, "[SimpleDraw] Not initialized.");

		const uint32_t pixelCount = mPixelSize * mPixelSize;

		// Check if we have enough space
		if (mNumPixels + pixelCount < mMaxPixels)
		{
			const int posX = x * mPixelSize;
			const int posY = y * mPixelSize;
			for (uint32_t yy = 0; yy < mPixelSize; ++yy)
				for (uint32_t xx = 0; xx < mPixelSize; ++xx)
					mPixels[mNumPixels++] = { Math::Vector3(static_cast<float>(posX + xx), static_cast<float>(posY + yy), 0.0f), color };
		}
	}

	void SimpleDrawImpl::AddScreenLine(const Math::Vector2& v0, const Math::Vector2& v1, const Color& color)
	{
		XASSERT(mInitialized, "[SimpleDraw] Not initialized.");

		// Check if we have enough space
		if (mNumVertices2D + 2 <= mMaxVertices)
		{
			// Add line
			mVertices2D[mNumVertices2D++] = { Math::Vector3(v0.x, v0.y, 0.0f), color };
			mVertices2D[mNumVertices2D++] = { Math::Vector3(v1.x, v1.y, 0.0f), color };
		}
	}

	void SimpleDrawImpl::AddScreenRect(const Math::Rect& rect, const Color& color)
	{
		XASSERT(mInitialized, "[SimpleDraw] Not initialized.");

		// Check if we have enough space
		if (mNumVertices2D + (8 * mPixelSize) <= mMaxVertices)
		{
			float l = rect.left * mPixelSize;
			float t = rect.top * mPixelSize;
			float r = (rect.right + 1.0f) * mPixelSize - 1.0f;
			float b = (rect.bottom + 1.0f) * mPixelSize - 1.0f;

			// Add lines
			for (uint32_t i = 0; i < mPixelSize; ++i)
			{
				mVertices2D[mNumVertices2D++] = { Math::Vector3(l, t, 0.0f), color };
				mVertices2D[mNumVertices2D++] = { Math::Vector3(r, t, 0.0f), color };

				mVertices2D[mNumVertices2D++] = { Math::Vector3(r, t, 0.0f), color };
				mVertices2D[mNumVertices2D++] = { Math::Vector3(r, b, 0.0f), color };

				mVertices2D[mNumVertices2D++] = { Math::Vector3(r, b, 0.0f), color };
				mVertices2D[mNumVertices2D++] = { Math::Vector3(l, b, 0.0f), color };

				mVertices2D[mNumVertices2D++] = { Math::Vector3(l, b, 0.0f), color };
				mVertices2D[mNumVertices2D++] = { Math::Vector3(l, t, 0.0f), color };

				l += 1.0f;
				t += 1.0f;
				r -= 1.0f;
				b -= 1.0f;
			}
		}
	}

	void SimpleDrawImpl::AddScreenCircle(const Math::Circle& circle, const Color& color)
	{
		XASSERT(mInitialized, "[SimpleDraw] Not initialized.");

		// Check if we have enough space
		if (mNumVertices2D + 32 <= mMaxVertices)
		{
			float x = circle.center.x;
			float y = circle.center.y;
			float r = circle.radius;

			// Add line
			const float kAngle = Math::kPi / 8.0f;
			for (uint32_t i = 0; i < 16; ++i)
			{
				const float alpha = i * kAngle;
				const float beta = alpha + kAngle;
				const float x0 = x + (r * sin(alpha));
				const float y0 = y + (r * cos(alpha));
				const float x1 = x + (r * sin(beta));
				const float y1 = y + (r * cos(beta));
				mVertices2D[mNumVertices2D++] = { Math::Vector3(x0, y0, 0.0f), color };
				mVertices2D[mNumVertices2D++] = { Math::Vector3(x1, y1, 0.0f), color };
			}
		}
	}

	void SimpleDrawImpl::AddScreenArc(const Math::Vector2& center, float radius, float fromAngle, float toAngle, const Color& color)
	{
		XASSERT(mInitialized, "[SimpleDraw] Not initialized.");

		// Check if we have enough space
		if (mNumVertices2D + 32 <= mMaxVertices)
		{
			float x = center.x;
			float y = center.y;
			float r = radius;

			// Add line
			const float kAngle = (toAngle - fromAngle) / 16.0f;
			for (uint32_t i = 0; i < 16; ++i)
			{
				const float alpha = i * kAngle + fromAngle;
				const float beta = alpha + kAngle;
				const float x0 = x + (r * cos(alpha));
				const float y0 = y + (r * sin(alpha));
				const float x1 = x + (r * cos(beta));
				const float y1 = y + (r * sin(beta));
				mVertices2D[mNumVertices2D++] = { Math::Vector3(x0, y0, 0.0f), color };
				mVertices2D[mNumVertices2D++] = { Math::Vector3(x1, y1, 0.0f), color };
			}
		}
	}

	void SimpleDrawImpl::AddScreenDiamond(const Math::Vector2& center, float size, const Color& color)
	{
		XASSERT(mInitialized, "[SimpleDraw] Not initialized.");

		// Check if we have enough space
		if (mNumVertices2D + 8 <= mMaxVertices)
		{
			mVertices2D[mNumVertices2D++] = { Math::Vector3(center.x, center.y - size, 0.0f), color };
			mVertices2D[mNumVertices2D++] = { Math::Vector3(center.x + size, center.y, 0.0f), color };

			mVertices2D[mNumVertices2D++] = { Math::Vector3(center.x + size, center.y, 0.0f), color };
			mVertices2D[mNumVertices2D++] = { Math::Vector3(center.x, center.y + size, 0.0f), color };

			mVertices2D[mNumVertices2D++] = { Math::Vector3(center.x, center.y + size, 0.0f), color };
			mVertices2D[mNumVertices2D++] = { Math::Vector3(center.x - size, center.y, 0.0f), color };

			mVertices2D[mNumVertices2D++] = { Math::Vector3(center.x - size, center.y, 0.0f), color };
			mVertices2D[mNumVertices2D++] = { Math::Vector3(center.x, center.y - size, 0.0f), color };
		}
	}

	void SimpleDrawImpl::AddScreenGrid(uint32_t cellSize, const Color& color)
	{
		mGridColor = color;
		mGridCellSize = cellSize;
		mShowGrid = (cellSize > 0);
	}

	void SimpleDrawImpl::Render(const Camera& camera)
	{
		XASSERT(mInitialized, "[SimpleDraw] Not initialized.");
		
		const Math::Matrix4& matView = camera.GetViewMatrix();
		const Math::Matrix4& matProj = camera.GetProjectionMatrix();

		mVertexShader.Bind();
		mPixelShader.Bind();

		CBSimpleDraw cb;
		cb.transform = Math::Transpose(matView * matProj);
		mConstantBuffer.Set(cb);
		mConstantBuffer.BindVS(0);

		UINT stride = sizeof(VertexPC);
		UINT offset = 0;

		auto gs = GraphicsSystem::Get();
		auto context = gs->GetContext();

		// Draw 3D lines
		if (mNumVertices3D > 0)
		{
			D3D11_MAPPED_SUBRESOURCE resource;
			context->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
			memcpy(resource.pData, mVertices3D, mNumVertices3D * stride);
			context->Unmap(mVertexBuffer, 0);

			context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
			context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
			context->Draw(mNumVertices3D, 0);
		}

		if (mNumVertices2D > 0 || mNumPixels > 0 || mShowGrid)
		{
			const uint32_t w = (mScreenWidth == 0) ? gs->GetBackBufferWidth() : mScreenWidth;
			const uint32_t h = (mScreenHeight == 0) ? gs->GetBackBufferHeight() : mScreenHeight;
			Math::Matrix4 matInvScreen
			(
				2.0f / w, 0.0f, 0.0f, 0.0f,
				0.0f, -2.0f / h, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				-1.0f, 1.0f, 0.0f, 1.0f
			);
			cb.transform = Math::Transpose(mTransform * matInvScreen);
			mConstantBuffer.Set(cb);
			mConstantBuffer.BindVS(0);

			// Draw grid
			if (mShowGrid)
			{
				static std::vector<VertexPC> gridLines;
				gridLines.clear();

				for (uint32_t x = mGridCellSize; x <= w; x += mGridCellSize)
				{
					gridLines.push_back({ { (float)x, 0.0f, 0.0f }, mGridColor });
					gridLines.push_back({ { (float)x, (float)h, 0.0f }, mGridColor });
				}
				for (uint32_t y = mGridCellSize; y <= h; y += mGridCellSize)
				{
					gridLines.push_back({ { 0.0f, (float)y, 0.0f }, mGridColor });
					gridLines.push_back({ { (float)w, (float)y, 0.0f }, mGridColor });
				}

				D3D11_MAPPED_SUBRESOURCE resource;
				context->Map(mPixelBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
				memcpy(resource.pData, gridLines.data(), gridLines.size() * stride);
				context->Unmap(mPixelBuffer, 0);

				context->IASetVertexBuffers(0, 1, &mPixelBuffer, &stride, &offset);
				context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
				context->Draw((UINT)gridLines.size(), 0);
			}

			// Draw pixels
			if (mNumPixels > 0)
			{
				D3D11_MAPPED_SUBRESOURCE resource;
				context->Map(mPixelBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
				memcpy(resource.pData, mPixels, mNumPixels * stride);
				context->Unmap(mPixelBuffer, 0);

				context->IASetVertexBuffers(0, 1, &mPixelBuffer, &stride, &offset);
				context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
				context->Draw(mNumPixels, 0);
			}

			// Draw 2D lines
			if (mNumVertices2D > 0)
			{
				D3D11_MAPPED_SUBRESOURCE resource;
				context->Map(mVertexBuffer2D, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
				memcpy(resource.pData, mVertices2D, mNumVertices2D * stride);
				context->Unmap(mVertexBuffer2D, 0);

				context->IASetVertexBuffers(0, 1, &mVertexBuffer2D, &stride, &offset);
				context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
				context->Draw(mNumVertices2D, 0);
			}
		}

		// Reset index
		mNumVertices3D = 0;
		mNumVertices2D = 0;
		mNumPixels = 0;
		mShowGrid = false;
	}

	SimpleDrawImpl* sSimpleDrawImpl = nullptr;
}

//====================================================================================================
// Function Definitions
//====================================================================================================

void SimpleDraw::Initialize(uint32_t maxVertices)
{
	if (nullptr == sSimpleDrawImpl)
	{
		sSimpleDrawImpl = new SimpleDrawImpl();
		sSimpleDrawImpl->Initialize(maxVertices);
	}
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::Terminate()
{
	if (nullptr != sSimpleDrawImpl)
	{
		sSimpleDrawImpl->Terminate();
		delete sSimpleDrawImpl;
		sSimpleDrawImpl = nullptr;
	}
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::SetTransform(const Math::Matrix4& transform)
{
	sSimpleDrawImpl->SetTransform(transform);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::SetScreenSize(uint32_t width, uint32_t height)
{
	sSimpleDrawImpl->SetScreenSize(width, height);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::SetPixelSize(uint32_t pixelSize)
{
	sSimpleDrawImpl->SetPixelSize(pixelSize);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color)
{
	XASSERT(sSimpleDrawImpl != nullptr, "[SimpleDraw] Not initialized.");
	sSimpleDrawImpl->AddLine(v0, v1, color);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::AddLine(float x0, float y0, float z0, float x1, float y1, float z1, const Color& color)
{
	AddLine(Math::Vector3(x0, y0, z0), Math::Vector3(x1, y1, z1), color);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::AddAABB(const Math::AABB& aabb, const Color& color)
{
	XASSERT(sSimpleDrawImpl != nullptr, "[SimpleDraw] Not initialized.");
	sSimpleDrawImpl->AddAABB(aabb, color);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::AddAABB(const Math::Vector3& min, const Math::Vector3& max, const Color& color)
{
	AddAABB(Math::AABB((min + max) * 0.5f, (max - min) * 0.5f), color);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::AddAABB(const Math::Vector3& center, float radius, const Color& color)
{
	AddAABB(Math::AABB(center, Math::Vector3(radius, radius, radius)), color);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::AddAABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, const Color& color)
{
	AddAABB(Math::Vector3(minX, minY, minZ), Math::Vector3(maxX, maxY, maxZ), color);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::AddOBB(const Math::OBB& obb, const Color& color)
{
	XASSERT(sSimpleDrawImpl != nullptr, "[SimpleDraw] Not initialized.");
	sSimpleDrawImpl->AddOBB(obb, color);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::AddSphere(const Math::Sphere& sphere, const Color& color, uint32_t slices, uint32_t rings)
{
	XASSERT(sSimpleDrawImpl != nullptr, "[SimpleDraw] Not initialized.");
	sSimpleDrawImpl->AddSphere(sphere, color, slices, rings);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::AddSphere(const Math::Vector3& center, float radius, const Color& color, uint32_t slices, uint32_t rings)
{
	AddSphere(Math::Sphere(center, radius), color, slices, rings);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::AddSphere(float x, float y, float z, float radius, const Color& color, uint32_t slices, uint32_t rings)
{
	AddSphere(Math::Sphere(x, y, z, radius), color, slices, rings);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::AddTransform(const Math::Matrix4& transform)
{
	XASSERT(sSimpleDrawImpl != nullptr, "[SimpleDraw] Not initialized.");
	sSimpleDrawImpl->AddTransform(transform);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::AddPixel(int x, int y, const Color& color)
{
	XASSERT(sSimpleDrawImpl != nullptr, "[SimpleDraw] Not initialized.");
	sSimpleDrawImpl->AddPixel(x, y, color);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::AddScreenLine(const Math::Vector2& v0, const Math::Vector2& v1, const Color& color)
{
	XASSERT(sSimpleDrawImpl != nullptr, "[SimpleDraw] Not initialized.");
	sSimpleDrawImpl->AddScreenLine(v0, v1, color);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::AddScreenLine(float x0, float y0, float x1, float y1, const Color& color)
{
	AddScreenLine(Math::Vector2(x0, y0), Math::Vector2(x1, y1), color);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::AddScreenRect(const Math::Rect& rect, const Color& color)
{
	XASSERT(sSimpleDrawImpl != nullptr, "[SimpleDraw] Not initialized.");
	sSimpleDrawImpl->AddScreenRect(rect, color);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::AddScreenRect(const Math::Vector2& min, const Math::Vector2& max, const Color& color)
{
	AddScreenRect(Math::Rect(min.x, min.y, max.x, max.y), color);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::AddScreenRect(float left, float top, float right, float bottom, const Color& color)
{
	AddScreenRect(Math::Rect(left, top, right, bottom), color);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::AddScreenCircle(const Math::Circle& circle, const Color& color)
{
	XASSERT(sSimpleDrawImpl != nullptr, "[SimpleDraw] Not initialized.");
	sSimpleDrawImpl->AddScreenCircle(circle, color);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::AddScreenCircle(const Math::Vector2& center, float r, const Color& color)
{
	AddScreenCircle(Math::Circle(center, r), color);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::AddScreenCircle(float x, float y, float r, const Color& color)
{
	AddScreenCircle(Math::Circle(x, y, r), color);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::AddScreenArc(const Math::Vector2& center, float r, float fromAngle, float toAngle, const Color& color)
{
	XASSERT(sSimpleDrawImpl != nullptr, "[SimpleDraw] Not initialized.");
	sSimpleDrawImpl->AddScreenArc(center, r, fromAngle, toAngle, color);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::AddScreenDiamond(const Math::Vector2& center, float size, const Color& color)
{
	XASSERT(sSimpleDrawImpl != nullptr, "[SimpleDraw] Not initialized.");
	sSimpleDrawImpl->AddScreenDiamond(center, size, color);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::AddScreenDiamond(float x, float y, float size, const Color& color)
{
	AddScreenDiamond(Math::Vector2(x, y), size, color);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::AddScreenGrid(uint32_t cellSize, const Color& color)
{
	XASSERT(sSimpleDrawImpl != nullptr, "[SimpleDraw] Not initialized.");
	sSimpleDrawImpl->AddScreenGrid(cellSize, color);
}

//----------------------------------------------------------------------------------------------------

void SimpleDraw::Render(const Camera& camera)
{
	XASSERT(sSimpleDrawImpl != nullptr, "[SimpleDraw] Not initialized.");
	sSimpleDrawImpl->Render(camera);
}