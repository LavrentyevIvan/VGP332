#pragma once

#include <AI.h>

class SCV : public AI::Agent {
public:
	SCV(AI::AIWorld& world);
	~SCV() override = default;

	void Load();
	void Unload();

	void Update(float deltatime);
	void Render();
	
	void ShowDebug(bool debug);

private:
	std::unique_ptr<AI::SteeringModule> mSteeringModule;
	std::array<X::TextureId, 16> mTextureIds;

};
