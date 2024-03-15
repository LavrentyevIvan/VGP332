#pragma once
#include "Entity.h"
namespace AI {
	using EntityPtrs = std::vector<Entity*>;

	class AIWorld
	{
	public:
		using Obstacles = std::vector<X::Math::Circle>;
		using Walls = std::vector<X::Math::LineSegment>;
		void Initialize();
		void Update();

		void Register(Entity* entity);
		void Unregister(Entity* entity);

		void AddObstacle(const X::Math::Circle& obstacle);
		void AddWall(const X::Math::LineSegment& wall);

		bool HasLineOfSight(const X::Math::LineSegment& lineSegment) const;

		const Obstacles& GetObstacles() const { return mObstacles; }
		const Walls& GetWalls() const { return mWalls; }
		
		
		EntityPtrs GetEntities() const { return mEntities; }

		EntityPtrs GetEntitiesInRAnge(const X::Math::Circle& range, uint32_t typeId);
	
		uint32_t GetNextId() const {
			XASSERT(mNextId < UINT32_MAX, "AIWorld: ran out of ids");
			return mNextId++;

		}
		
	private:
		mutable uint32_t mNextId = 0;
		EntityPtrs mEntities;

		Obstacles mObstacles;
		Walls mWalls;
	};


}