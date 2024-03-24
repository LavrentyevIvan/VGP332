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
X::TextureId textureId;
X::Math::Vector2 position;
Path path;
TileMap tileMap;




//Part 2 Agent

int randX;
int randY;
float viewRange = 500.0f;
float viewAngle = 45.0f;
float radius = 50.0f;
bool isValidTile = false;
X::Math::Vector2 destination = X::Math::Vector2::Zero();


AIWorld aiWorld;
std::vector<std::unique_ptr<Raven>> ravenAgents;
std::vector<std::unique_ptr<Mineral>> minerals;


//Part 3 state stuff and decision modules
int homeStorage;



bool showDebug = false;




void SpawnRaven()
{
	auto& agent = ravenAgents.emplace_back(std::make_unique<Raven>(aiWorld));

	agent->ravenTilemap = &tileMap;
	agent->ptrHomeStorage = &homeStorage;


	agent->Load();
	const float screenWidth = X::GetScreenWidth();
	const float screenHeight = X::GetScreenHeight();
	agent->position = X::Math::Vector2{ 120, 120 };
	agent->destination = destination;
	agent->radius = radius;
	agent->ShowDebug(showDebug);
	

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
	ImGui::End();
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