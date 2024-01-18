#pragma once

#include <XEngine.h>

class TileMap
{
public:
	void LoadTiles(const char* fileName);
	void LoadMap(const char* fileName);

	void Render() const;

private:
	std::vector<int> mMap;
	std::vector<X::TextureId> mTiles; // I forgot to add this in class
	int mColumns = 0;
	int mRows = 0;
};