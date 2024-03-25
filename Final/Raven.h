#pragma once

#include <AI.h>
#include "TileMap.h"
class VisualSensor;
enum class ravenStates
{
	GoHome,
	SearchForMushroom,
	MoveToMushroom,
	HarvestMushroom
};

class Raven : public AI::Agent
{
public:
	Raven(AI::AIWorld& world);

	void Load();
	void Unload();

	void Update(float deltaTime);
	void Render();

	void ShowDebug(bool debug);

	void SetSeek(bool active);
	void SetArrive(bool active);
	void SetWander(bool active);
	void SetTargetDestination(const X::Math::Vector2& targetDestination);
	X::Math::Vector2 GetTargetDestination();
	void SetTarget(Entity* target);
	Entity* GetTarget() { return mTarget; }
	
	void CollectMushroom();
	int GetMushroomsCollected() { return collectedMushrooms; }

	const AI::PerceptionModule* GetPerception() const { return mPerceptionModule.get(); }

	TileMap* ravenTilemap;
	int* ptrHomeStorage;

	void changeState(ravenStates newState);
	ravenStates getState() { return currentState; }


	int DepositMushrooms(int ravenMushrooms);
	void resetMushrooms();

	void setHarvested(bool onHarvest);
	
	bool getHarvested() { return hasHarvested; }

	void setTimer(float count);
	float getTimer() { return timer; }
private:
	std::unique_ptr<AI::PerceptionModule> mPerceptionModule;
	std::unique_ptr<AI::SteeringModule> mSteeringModule;
	std::unique_ptr<AI::DecisionModule<Raven>> mDecisionModule;

	VisualSensor* mVisualSensor = nullptr;
	AI::SeekBehaviour* mSeekBehaviour = nullptr;
	AI::ArriveBehaviour* mArriveBehaviour = nullptr;

	std::array<X::TextureId, 32> mTextureIds;
	X::Math::Vector2 mDestination;
	AI::StateMachine<Raven> rStateMachine;

	Entity* mTarget;
	int collectedMushrooms;
	ravenStates currentState;
	float timer;
	bool hasHarvested;
};