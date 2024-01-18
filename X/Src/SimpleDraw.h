//====================================================================================================
// Filename:	SimpleDraw.h
// Created by:	Peter Chan
//====================================================================================================

#ifndef INCLUDED_XENGINE_SIMPLEDRAW_H
#define INCLUDED_XENGINE_SIMPLEDRAW_H

#include "Camera.h"
#include "XColors.h"

namespace X {
namespace SimpleDraw {

// Functions to startup/shutdown simple draw
void Initialize(uint32_t maxVertices = 10000);
void Terminate();

void SetTransform(const Math::Matrix4& transform);
void SetScreenSize(uint32_t width, uint32_t height);
void SetPixelSize(uint32_t pixelSize);

// Functions for world space rendering
void AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color);
void AddLine(float x0, float y0, float z0, float x1, float y1, float z1, const Color& color);
void AddAABB(const Math::AABB& aabb, const Color& color);
void AddAABB(const Math::Vector3& min, const Math::Vector3& max, const Color& color);
void AddAABB(const Math::Vector3& center, float radius, const Color& color);
void AddAABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, const Color& color);
void AddOBB(const Math::OBB& obb, const Color& color);
void AddSphere(const Math::Sphere& sphere, const Color& color, uint32_t slices = 8, uint32_t rings = 4);
void AddSphere(const Math::Vector3& center, float radius, const Color& color, uint32_t slices = 8, uint32_t rings = 4);
void AddSphere(float x, float y, float z, float radius, const Color& color, uint32_t slices = 8, uint32_t rings = 4);
void AddTransform(const Math::Matrix4& transform);

// Functions for screen space rendering
void AddPixel(int x, int y, const Color& color);
void AddScreenLine(const Math::Vector2& v0, const Math::Vector2& v1, const Color& color);
void AddScreenLine(float x0, float y0, float x1, float y1, const Color& color);
void AddScreenRect(const Math::Rect& rect, const Color& color);
void AddScreenRect(const Math::Vector2& min, const Math::Vector2& max, const Color& color);
void AddScreenRect(float left, float top, float right, float bottom, const Color& color);
void AddScreenCircle(const Math::Circle& circle, const Color& color);
void AddScreenCircle(const Math::Vector2& center, float r, const Color& color);
void AddScreenCircle(float x, float y, float r, const Color& color);
void AddScreenArc(const Math::Vector2& center, float r, float fromAngle, float toAngle, const Color& color);
void AddScreenDiamond(const Math::Vector2& center, float size, const Color& color);
void AddScreenDiamond(float x, float y, float size, const Color& color);
void AddScreenGrid(uint32_t cellSize, const Color& color);

// Function to actually render all the geometry.
void Render(const Camera& camera);

} // namespace SimpleDraw
} // namespace X

#endif // #ifndef INCLUDED_XENGINE_SIMPLEDRAW_H
