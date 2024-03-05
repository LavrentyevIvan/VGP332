#pragma once

namespace AI 
{
	using Property = std::variant<int, float, X::Math::Vector2>;

	struct MemoryRecord {
		std::unordered_map<std::string, Property> properties;
		uint64_t uniqueId = 0;
		float lastRecordedTime = 0.0f;
		float importance = 0.0f;
		template<class T>
		T GetProperty(const std::string& key, T defaultValue = T()) const 
		{
			const auto iter = properties.find(key);
			if (iter == properties.end()) 
			{
				return defaultValue;
			}

			const auto& value = iter->second;
			return std::get<T>(value);
		}
	};

	using MemoryRecords = std::list<MemoryRecord>;
}