
#include <XEngine.h> // <> for external includes, "" for internal includes
#include <ImGui/Inc/imgui.h>

#include "Guitar.h"

Guitar guitar;
//--------------------------------------------------

void GameInit()
{
	guitar.Initialize();
}

bool GameLoop(float deltaTime)
{
	
	if (X::IsKeyPressed(X::Keys::SPACE)) {
		guitar.Update(deltaTime);

	}

	ImGui::Begin("FSM", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	{
		
		guitar.DebugUI();
	}

	ImGui::End();


	const bool quit = X::IsKeyPressed(X::Keys::ESCAPE);

	return quit;
}

void GameCleanup()
{
	guitar.Terminate();

}

//--------------------------------------------------

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	X::Start("FSM");
	GameInit();

	X::Run(GameLoop);

	GameCleanup();
	X::Stop();
	return 0;
}