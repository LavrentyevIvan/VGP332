#include <XEngine.h> // <> for external includes, "" for internal includes
#include <ImGui/Inc/imgui.h>

#include "TileMap.h"


#include <AI.h>

#include "SCV.h";
#include "Raven.h";
#include "Mineral.h"

using namespace AI;



//--------------------------------------------------


//Part 1 Tilemap
TileMap tileMap;
X::TextureId textureId;
X::Math::Vector2 position;
Path path;





//Part 2 Agent
int startX = 5;
int startY = 9;
int endX = 15;
int endY = 12;
int randX;
int randY;
float viewRange = 300.0f;
float viewAngle = 45.0f;
float radius = 50.0f;
bool isValidTile = false;
X::Math::Vector2 destination = X::Math::Vector2::Zero();


AIWorld aiWorld;
std::vector<std::unique_ptr<Raven>> ravenAgents;
std::vector<std::unique_ptr<Mineral>> minerals;


//wander stuff
bool useWander = false;

float wanderJitter = 5.0f;
float wanderRadius = 20.0f;
float wanderDistance = 50.0f;
float weightWander = 0.1f;



bool showDebug = false;




void SpawnRaven()
{
	auto& agent = ravenAgents.emplace_back(std::make_unique<Raven>(aiWorld));

	agent->Load();

	const float screenWidth = X::GetScreenWidth();
	const float screenHeight = X::GetScreenHeight();

	agent->position = tileMap.GetPixelPosition(100, 50);
	agent->destination = destination;
	agent->radius = radius;
	agent->ShowDebug(showDebug);
	agent->SetWander(useWander);
	

}
void KillRaven()
{
	if (!ravenAgents.empty())
	{
		auto& agent = ravenAgents.back();
		agent->Unload();

		ravenAgents.pop_back();
	}

}

void GameInit()
{
	tileMap.LoadTiles("tiles.txt");
	tileMap.LoadMap("map.txt");
	aiWorld.Initialize();

	srand(time(nullptr));

	//Spawn mushrooms
	for (uint32_t i = 0; i < 10; ++i) 
	{
		auto& mineral = minerals.emplace_back(std::make_unique<Mineral>(aiWorld));
		mineral->Initialize();

		isValidTile = false;
		while(!isValidTile)
		{
			randX = rand() % tileMap.getRows();
			randY = rand() % tileMap.getColumns();

			if (!tileMap.IsBlocked(randX, randY))
			{
				isValidTile = true;
			}
		}
		mineral->position = tileMap.GetPixelPosition(randX, randY);
		
	}

}

bool GameLoop(float deltaTime)
{
	ImGui::Begin("Final", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	{
		const int columns = tileMap.getColumns();
		const int rows = tileMap.getRows();

		ImGui::DragInt("Start X", &startX, 1, 0, columns - 1);
		ImGui::DragInt("Start Y", &startY, 1, 0, rows - 1);

		ImGui::DragInt("End X", &endX, 1, 0, columns - 1);
		ImGui::DragInt("End Y", &endY, 1, 0, rows - 1);
	}

	tileMap.Render();

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
		for (auto& agent : ravenAgents)
		{
			agent->ShowDebug(showDebug);
		}
	}



	if (ImGui::Checkbox("Wander", &useWander))
	{
		for (auto& agent : ravenAgents)
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
	
	ImGui::End();


	if (X::IsMousePressed(X::Mouse::LBUTTON)) {
		const float mouseX = static_cast<float>(X::GetMouseScreenX());
		const float mouseY = static_cast<float>(X::GetMouseScreenY());
		destination = { mouseX, mouseY };
		for (auto& agent : ravenAgents)
		{
			agent->destination = destination;

		}
		for (auto& agent : ravenAgents)
		{
			agent->SetTargetDestination(destination);

		}
	}
	aiWorld.Update();
	for (auto& agent : ravenAgents)
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
	const bool quit = X::IsKeyPressed(X::Keys::ESCAPE);

	return quit;
	
	
}

void GameCleanup()
{
	
	
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
	minerals.clear();
	
}

//--------------------------------------------------

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	X::Start("Final");
	GameInit();

	X::Run(GameLoop);

	GameCleanup();
	X::Stop();
	return 0;
}