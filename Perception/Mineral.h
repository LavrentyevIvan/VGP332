#pragma once

#include <XEngine.h>
#include <AI.h>

class Mineral : public AI::Entity
{
public:
	Mineral(AI::AIWorld& world);

	void Initialize();
	void Render();
private:
	X::TextureId mTextureId;

};