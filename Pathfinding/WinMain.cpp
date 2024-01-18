#include "TileMap.h"
#include <XEngine.h> // <> for external includes, "" for internal includes

TileMap tileMap;
X::TextureId textureId;
X::Math::Vector2 position;

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
	if (X::IsKeyDown(X::Keys::DOWN))
		position.y += moveSpeed * deltaTime;

	tileMap.Render();

	X::DrawSprite(textureId, position, X::Pivot::TopLeft, X::Flip::Horizontal);
	X::DrawScreenDiamond(position, 5.0f, X::Colors::Cyan);

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