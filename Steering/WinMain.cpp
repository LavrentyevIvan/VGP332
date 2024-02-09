
#include <XEngine.h> // <> for external includes, "" for internal includes
#include <AI.h>
#include "SCV.h";
#include <ImGui/Inc/imgui.h>
using namespace AI;



//--------------------------------------------------
AIWorld aiWorld;
std::vector<std::unique_ptr<SCV>> scvAgents;
void SpawnAgent() {
	auto& agent = scvAgents.emplace_back(std::make_unique<SCV>(aiWorld));
	agent->Load();

	const float screenWidth = X::GetScreenWidth();
	const float screenHeight = X::GetScreenHeight();
	agent->position = X::RandomVector2({ 100.0f, 100.0f },
		{ screenWidth - 100.0f, screenHeight - 100.0f });
}
void KillAgent() {
	if (!scvAgents.empty()) {
	auto& agent = scvAgents.back();
		agent->Unload();
		agent.reset();
		scvAgents.pop_back();
	}
	
}

void GameInit()
{
	aiWorld.Initialize();
}

bool GameLoop(float deltaTime)
{
	

	ImGui::Begin("Steering", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	{
		if (ImGui::Button("SpawnAgent")) {
			SpawnAgent();
		}
		if (ImGui::Button("KillAgent")) {
			KillAgent();
		}
	}

	ImGui::End();

	aiWorld.Update();

	for (auto& agent : scvAgents) {
		agent->Update(deltaTime);
	}


	for (auto& agent : scvAgents) {
		agent->Render();
		const bool quit = X::IsKeyPressed(X::Keys::ESCAPE);

		return quit;
	}
	
	
}

void GameCleanup()
{
	for (auto& agent : scvAgents)
	{
		agent->Unload();
		agent.reset();
	}
	scvAgents.clear();

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