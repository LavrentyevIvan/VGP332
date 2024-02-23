#pragma once
#include "Entity.h"
namespace AI {
	using EntityPtrs = std::vector<Entity*>;
	class AIWorld {

	public:
		void Initialize();
		void Update();

		void Register(Entity* entity);
		void Unregister(Entity* entity);

		
		
		EntityPtrs GetEntities() const { return mEntities; }

		EntityPtrs GetEntitiesInRAnge(const X::Math::Circle& range, uint32_t typeId);
	
		uint32_t GetNextId() const {
			XASSERT(mNextId < UINT32_MAX, "AIWorld: ran out of ids");
			return mNextId++;

		}
		
	private:
		mutable uint32_t mNextId = 0;
		EntityPtrs mEntities;

	};


}