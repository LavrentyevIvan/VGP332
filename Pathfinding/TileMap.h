#pragma once

#include <XEngine.h>
#include <AI.h>
#include "GridBasedGraph.h"


using Path = std::vector<X::Math::Vector2>;

struct Tile
{
	X::TextureId textureId;
	bool isWalkable = false;
};


class TileMap
{
public:
	void LoadTiles(const char* fileName);
	void LoadMap(const char* fileName);

	void Render() const;

	bool IsBlocked(int x, int y) const;
	int getColumns() const { return mColumns; }
	int getRows() const { return mRows; }

	X::Math::Vector2 GetPixelPosition(int x, int y)const;

	Path FindPathBFS(int startX, int startY, int endX, int endY);
	Path FindPathDFS(int startX, int startY, int endX, int endY);
	Path FindPathDijkstra(int startX, int startY, int endX, int endY);
	Path FindPathAStar(int startX, int startY, int endX, int endY);

private:

	AI::GridBasedGraph mGraph;
	std::vector<int> mMap;
	std::vector<Tile> mTiles; 
	int mColumns = 0;
	int mRows = 0;
	int mTileWidth = 0;
	int mTileheight = 0;
};