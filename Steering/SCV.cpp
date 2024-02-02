#include "SCV.h"
#include "TypeIds.h"
SCV::SCV(AI::AIWorld& world)
	: Agent(world, static_cast<uint32_t>(AgentType::SCV))
{

}

void SCV::Load()
{
	for (int i = 0; i<mTextureIds.size(); ++i){
		char name[128];
		sprintf_s(name, "scv_%02i.png", i + 1);
		mTextureIds[i] = X::LoadTexture(name);
	}
	
}

void SCV::Unload()
{
}

void SCV::Update(float deltatime)
{
	
}

void SCV::Render()
{
	const float angle = atan2(-heading.x, heading.y) + X::Math::kPi;
	const float percent = angle / X::Math::kTwoPi;
	const int frame = static_cast<int>(percent * mTextureIds.size()) % mTextureIds.size();
	X::DrawSprite(mTextureIds[frame], position);
}


void SCV::ShowDebug(bool debug)
{
}
