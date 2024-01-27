#include "TileMap.h"
#include <XEngine.h> // <> for external includes, "" for internal includes
#include <ImGui/Inc/imgui.h>

TileMap tileMap;
X::TextureId textureId;
X::Math::Vector2 position;
Path path;


int startX = 5;
int startY = 9;

int endX = 15;
int endY = 12;


//--------------------------------------------------

void GameInit()
{
	tileMap.LoadTiles("tiles.txt");
	tileMap.LoadMap("map.txt");

	textureId = X::LoadTexture("bird1.png");
	position = { 100.0f, 100.0f };
}

bool GameLoop(float deltaTime)
{
	const float moveSpeed = 200.0f; // pixel per second
	if (X::IsKeyDown(X::Keys::RIGHT))
		position.x += moveSpeed * deltaTime;
	else if (X::IsKeyDown(X::Keys::LEFT))
		position.x -= moveSpeed * deltaTime;
	if (X::IsKeyDown(X::Keys::DOWN))
		position.y += moveSpeed * deltaTime;
	else if (X::IsKeyDown(X::Keys::UP))
		position.y -= moveSpeed * deltaTime;

	
	ImGui::Begin("PathFinding", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	{
		const int columns = tileMap.getColumns();
		const int rows = tileMap.getRows();

		ImGui::DragInt("Start X", &startX, 1, 0 , columns -1);
		ImGui::DragInt("Start Y", &startY, 1, 0, rows - 1);

		ImGui::DragInt("End X", &endX, 1, 0, columns - 1);
		ImGui::DragInt("End Y", &endY, 1, 0, rows - 1);
		
		if (ImGui::Button("RunBFS"))
		{
			path = tileMap.FindPathBFS(startX, startY, endX, endY);
		}
		if (ImGui::Button("RunDFS"))
		{
			path = tileMap.FindPathDFS(startX, startY, endX, endY);
		}
		if (ImGui::Button("RunDijkstra"))
		{
			path = tileMap.FindPathDijkstra(startX, startY, endX, endY);
		}

	}

	ImGui::End();

	tileMap.Render();

	X::DrawSprite(textureId, position, X::Pivot::TopLeft, X::Flip::Horizontal);
	X::DrawScreenDiamond(position, 5.0f, X::Colors::Cyan);



	for (int i = 1; i < path.size(); ++i)
	{
		X::DrawScreenLine(path[i - 1], path[i], X::Colors::Red);
	}

	X::DrawScreenCircle(tileMap.GetPixelPosition(startX,startY), 10.0f, X::Colors::Pink);
	X::DrawScreenCircle(tileMap.GetPixelPosition(endX, endY), 10.0f, X::Colors::Yellow);

	const bool quit = X::IsKeyPressed(X::Keys::ESCAPE);

	return quit;
}

void GameCleanup()
{

}

//--------------------------------------------------

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	X::Start("Pathfinding");
	GameInit();

	X::Run(GameLoop);

	GameCleanup();
	X::Stop();
	return 0;
}