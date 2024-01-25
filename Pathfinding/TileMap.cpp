#include "TileMap.h"

namespace
{
	inline int ToIndex(int x, int y, int columns)
	{
		return x + (y * columns);
	}
}

void TileMap::LoadTiles(const char* fileName)
{
	// TODO - Read the provided file and populate mTiles here
	if (!std::filesystem::exists(fileName)) {
		return;

	}
	std::fstream file(fileName);

	int count = 0;
	int isWalkable = 0;
	std::string buffer;
	file >> buffer;
	file >> count;



	mTiles.clear();
	mTiles.reserve(count);

	for (int i = 0; i < count; ++i) { 
		file >> buffer;
		file >> isWalkable;
		
		X::TextureId& tileId = mTiles.emplace_back();
		tileId = X::LoadTexture(buffer.c_str());
	}
	file.close();

	mTileWidth = X::GetSpriteWidth(mTiles[0]);
	mTileHeight = X::GetSpriteHeight(mTiles[0]);

}

void TileMap::LoadMap(const char* fileName)
{
	// TODO - Read the provided file and populate mMap here
	if (!std::filesystem::exists(fileName)) {
		return;
	}

	std::fstream file(fileName);
	int columns = 0;
	int rows = 0;
	std::string buffer;
	char tileType;

	file >> buffer;
	file >> columns;
	file >> buffer;
	file >> rows;

	mColumns = columns;
	mRows = rows;
	mMap.resize(columns * rows);
	for (int y = 0; y < rows; ++y) {
		for (int x = 0; x < columns; ++x) {
			file >> tileType;
			int mapIndex = ToIndex(x, y, columns);
			mMap[mapIndex] = tileType - '0';
		}
	}
	file.close();

}

void TileMap::Render() const
{
	X::Math::Vector2 position;
	for (int y = 0; y < mRows; ++y) {
		for (int x = 0; x < mColumns; ++x) {
			int mapIndex = ToIndex(x, y, mColumns);
			int tileType = mMap[mapIndex];
			X::DrawSprite(mTiles[tileType], position, X::Pivot::TopLeft);
			position.x += mTileWidth;

		}
		position.x = 0.0f;
		position.y += mTileHeight;
	}
}

// 2D map - 5 columns x 4 rows
// [0][0][0][0][0]
// [0][0][0][0][0]
// [0][0][0][X][0]   X is at (3, 2)
// [0][0][0][0][0]

// Stored as 1D - 5x4 = 20 slots
// [0][0][0][0][0] [0][0][0][0][0] [0][0][0][X][0] [0][0][0][0][0]
//
// index = column + (row * columnCount)
//       = 3 + (2 * 5)
//       = 13