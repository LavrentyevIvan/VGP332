#pragma once

namespace AI {
	class AIWorld;

	class Entity {
	public:
		Entity(AIWorld& world, uint32_t typeId);
		virtual ~Entity();

		Entity(const Entity&) = delete;
		Entity(const Entity&&) = delete;
		Entity& operator=(const Entity&) = delete;
		Entity& operator=(const Entity&&) = delete;

		AIWorld& world;
		X::Math::Vector2 position = X::Math::Vector2::Zero();
		X::Math::Vector2 heading = X::Math::Vector2::YAxis();
		float radius = 1.0f;

		X::Math::Matrix3 GetWorldTransform() const;
		uint32_t GetTypeId() const { return static_cast<uint32_t>(mUniqueId >> 32); }
		uint32_t GetUniqueId() const {return mUniqueId;}
	private :
		const uint64_t mUniqueId = 0;
	};
}