
#include <XEngine.h> // <> for external includes, "" for internal includes
#include <ImGui/Inc/imgui.h>
#include <AI.h>

#include "SCV.h";
#include "Raven.h";

#include "Mineral.h"

using namespace AI;



//--------------------------------------------------
AIWorld aiWorld;
std::vector<std::unique_ptr<SCV>> scvAgents;
std::vector<std::unique_ptr<Raven>> ravenAgents;
std::vector<std::unique_ptr<Mineral>> minerals;


X::Math::Vector2 destination = X::Math::Vector2::Zero();

bool useSeek = false;
bool useFlee = false;
bool useArrive = false;
bool useWander = false;
bool usePursuit = false;
bool useSeperation = false;
bool useAlignment = false;
bool useCohesion = false;
float wanderJitter = 5.0f;
float wanderRadius = 20.0f;
float wanderDistance = 50.0f;
float radius = 50.0f;

float viewRange = 300.0f;
float viewAngle = 45.0f;

float weightSeek = 0.1f;
float weightFlee = 0.1f;
float weightArrive = 0.1f;
float weightWander = 0.1f;
float weightPursuit = 0.1f;
float weightSeparation = 0.1f;
float weightAlignment = 0.1f;
float weightCohesion = 0.1f;
bool showDebug = false;

void SpawnAgent() 
{
	auto& agent = scvAgents.emplace_back(std::make_unique<SCV>(aiWorld));
	agent->Load();

	const float screenWidth = X::GetScreenWidth();
	const float screenHeight = X::GetScreenHeight();
	agent->position = X::RandomVector2({ 100.0f, 100.0f },
		{ screenWidth - 100.0f, screenHeight - 100.0f });
	agent->destination = destination;
	agent->radius = radius;
	agent->ShowDebug(showDebug);
	agent->SetSeek(useSeek);
	agent->SetFlee(useFlee);
	agent->SetArrive(useArrive);
	agent->SetWander(useWander);
	agent->SetPursuit(usePursuit);
	agent->SetSeperation(useSeperation);
	agent->SetAlignment(useAlignment);
	agent->SetCohesion(useCohesion);

}
void KillAgent() 
{
	if (!scvAgents.empty())
	{
	auto& agent = scvAgents.back();
	agent->Unload();
	agent.reset();
	scvAgents.pop_back();
	}
	
}
void SpawnRaven()
{
	auto& agent = ravenAgents.emplace_back(std::make_unique<Raven>(aiWorld));
	agent->Load();

	const float screenWidth = X::GetScreenWidth();
	const float screenHeight = X::GetScreenHeight();
	agent->position = X::RandomVector2({ 100.0f, 100.0f },
		{ screenWidth - 100.0f, screenHeight - 100.0f });
	agent->destination = destination;
	agent->radius = radius;
	agent->ShowDebug(showDebug);
	agent->SetWander(useWander);
	

}
void KillRaven()
{
	if (!ravenAgents.empty())
	{
		auto& agent = scvAgents.back();
		agent->Unload();

		ravenAgents.pop_back();
	}

}
void GameInit()
{
	aiWorld.Initialize();

	for (uint32_t i = 0; i < 10; ++i) 
	{
		auto& mineral = minerals.emplace_back(std::make_unique<Mineral>(aiWorld));
		mineral->Initialize();
		
		aiWorld.AddObstacle({ 230.0f, 300.0f, 50.0f });
		
		X::Math::Vector2 topLeft(500.0f, 100.0f);
		X::Math::Vector2 topRight(600.0f, 100.0f);
		X::Math::Vector2 bottomLeft(500.0f, 600.0f);
		X::Math::Vector2 bottomRight(600.0f, 600.0f);
		aiWorld.AddWall({ topLeft, topRight });
		aiWorld.AddWall({ topRight, bottomRight });
		aiWorld.AddWall({ bottomLeft, bottomRight });
		aiWorld.AddWall({ bottomLeft, topLeft });
	}

	aiWorld.AddObstacle({ 230.0f, 300.0f, 50.0f });
}

bool GameLoop(float deltaTime)
{
	ImGui::Begin("Steering", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	{
		if (ImGui::Button("SpawnAgent"))
		{
			SpawnAgent();
		}
		if (ImGui::Button("KillAgent"))
		{
			KillAgent();
		}
		if (ImGui::Button("SpawnRaven"))
		{
			SpawnRaven();
		}
		if (ImGui::Button("KillRaven"))
		{
			KillRaven();
		}
		if (ImGui::Checkbox("ShowDebug", &showDebug))
		{
			for (auto& agent : scvAgents)
			{
				agent->ShowDebug(showDebug);
			}
		}
		if (ImGui::Checkbox("Seek", &useSeek))
		{
			for (auto& agent : scvAgents)
			{
				agent->SetSeek(useSeek);
			}
		}
		ImGui::SameLine();
		if (ImGui::DragFloat("SeekWeight", &weightSeek, 0.1f, 0.1f, 5.0f))
		{
			for (auto& agent : scvAgents)
			{
				agent->SetSeekWeight(weightSeek);
			}
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("Wander", &useWander)) 
		{
			for (auto& agent : scvAgents) 
			{
				agent->SetWander(useWander);
			}
		}
		ImGui::SameLine();
		if (useWander) {
			if (ImGui::CollapsingHeader("WanderSettings", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::DragFloat("Jitter", &wanderJitter, 0.1f, 0.1f, 10.0f);
				ImGui::DragFloat("Radius", &wanderRadius, 0.1f, 0.1f, 100.0f);
				ImGui::DragFloat("Distance", &wanderDistance, 0.1f, 0.1f, 500.0f);


			}
		}
		
		if (ImGui::CollapsingHeader("visualSensor", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::DragFloat("ViewRange", &viewRange, 1.0f, 100.0f, 1000.0f);
			ImGui::DragFloat("ViewAngle", &viewAngle, 1.0f, 1.0f, 360.0f);
		}
	}

	ImGui::End();
	if (X::IsMousePressed(X::Mouse::LBUTTON)) {
		const float mouseX = static_cast<float>(X::GetMouseScreenX());
		const float mouseY = static_cast<float>(X::GetMouseScreenY());
		destination = { mouseX, mouseY };
		for (auto& agent : scvAgents)
		{
			agent->destination = destination;

		}
		for (auto& agent : ravenAgents)
		{
			agent->SetTargetDestination(destination);

		}
	}
	aiWorld.Update();
	for (auto& agent : scvAgents)
	{
		EntityPtrs neighbors = aiWorld.GetEntitiesInRAnge({ agent->position, 500.f }, 0);
		agent->neighbors.clear();
		for (auto& n : neighbors)
		{
			if (n != agent.get())
			{
				agent->neighbors.push_back(static_cast<Agent*>(n));
			}
		}
	}

	
	for (auto& agent : scvAgents) 
	{
		agent->Update(deltaTime);
	}
	for (auto& agent : scvAgents) 
	{
		agent->Render();
	}
	for (auto& agent : ravenAgents) 
	{
		agent->Update(deltaTime);
	}
	for (auto& agent : ravenAgents) 
	{
		agent->Render();
	}
	for (auto& mineral : minerals) 
	{
		mineral->Render();
	}


	auto iter = minerals.begin();
	while (iter != minerals.end()) 
	{
		if (iter->get()->GetHealth() == 0) 
		{
			iter->reset();
			iter = minerals.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	for (auto& mineral : minerals)
	{
		mineral->Render();
	}

	const AIWorld::Obstacles& obstacles = aiWorld.GetObstacles();
	for (const X::Math::Circle& obstacle : obstacles)
	{
		X::DrawScreenCircle(obstacle.center, obstacle.radius, X::Colors::Gray);
	}

	const AIWorld::Walls& walls = aiWorld.GetWalls();
	for (const X::Math::LineSegment& wall : walls)
	{
		X::DrawScreenLine(wall.from, wall.to, X::Colors::Gray);
	}

	const bool quit = X::IsKeyPressed(X::Keys::ESCAPE);

	return quit;
	
	
}

void GameCleanup()
{
	for (auto& agent : scvAgents)
	{
		agent->Unload();
		agent.reset();
	}
	for (auto& agent : ravenAgents)
	{
		agent->Unload();
		agent.reset();
	}
	for (auto& mineral : minerals)
	{
		mineral.reset();
	}
	ravenAgents.clear();
	scvAgents.clear();
	minerals.clear();

}

//--------------------------------------------------

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	X::Start("Steering");
	GameInit();

	X::Run(GameLoop);

	GameCleanup();
	X::Stop();
	return 0;
}