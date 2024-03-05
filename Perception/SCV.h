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
	void SetSeek(bool active);
	void SetFlee(bool active);
	void SetArrive(bool active);
	void SetWander(bool active);
	void SetPursuit(bool active);
	void SetSeperation(bool active);
	void SetAlignment(bool active);
	void SetCohesion(bool active);

	void SetSeekWeight(float weight);
	void SetFleeWeight(float weight);
	void SetArriveWeight(float weight);
	void SetWanderWeight(float weight);
	void SetPursuitWeight(float weight);
	void SetSeparationWeight(float weight);
	void SetAlignmentWeight(float weight);
	void SetCohesionWeight(float weight);


private:
	std::unique_ptr<AI::PerceptionModule> mPerceptionModule;

	std::unique_ptr<AI::SteeringModule> mSteeringModule;
	AI::SeekBehaviour* mSeekBehaviour = nullptr;
	AI::FleeBehaviour* mFleeBehaviour = nullptr;
	AI::ArriveBehaviour* mArriveBehaviour = nullptr;
	AI::WanderBehaviour* mWanderBehaviour = nullptr;
	AI::PursuitBehaviour* mPursuitBehaviour = nullptr;
	AI::SeparationBehaviour* mSeperationBehaviour = nullptr;
	AI::AlignmentBehaviour* mAlignmentBehaviour = nullptr;
	AI::CohesionBehaviour* mCohesionBehaviour = nullptr;
	

	std::array<X::TextureId, 16> mTextureIds;

};
