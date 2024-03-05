#pragma once
#include "MemoryRecord.h"
#include "Sensor.h"

namespace AI 
{
	class Agent;
	using ImportanceCalculator = std::function<float(const Agent&, const MemoryRecord&)>;

	class PerceptionModule
	{
	public:
		PerceptionModule(Agent& agent, ImportanceCalculator calculator);

		template<class SensorType>
		SensorType* AddSensor()
		{
			static_assert(std::is_base_of_v<Sensor, SensorType>, "PerceptionModule: SensorType is not of type Sensor");
			auto& newSensor = mSensors.emplace_back(std::make_unique<SensorType>());
			return static_cast<SensorType*>(newSensor.get());
		}

		void Update(float deltaTime);
		void SetMemorySpan(float memorySpan) { mMemorySpan = memorySpan;  }
		const MemoryRecords& GetMemoryRecords() const { return mMemoryRecords; }
	private:
		using Sensors = std::vector<std::unique_ptr<Sensor>>;

		ImportanceCalculator mComputeImportance;
		Agent& mAgent;
		Sensors mSensors;
		MemoryRecords mMemoryRecords;
		float mMemorySpan;

	};

}