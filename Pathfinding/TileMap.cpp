#include "TileMap.h"

using namespace AI;


namespace
{
	// Converts 2D grid coordinates (x, y) to a single index for a 1D array representation.
   // Useful for accessing elements in a 1D array that represents a 2D grid.

	inline int ToIndex(int x, int y, int columns)
	{
		return x + (y * columns);
	}
}

void TileMap::LoadTiles(const char* fileName)
{
	if (!std::filesystem::exists(fileName))
	{
		return;
	}

	// Open the file to read tile data.
	std::fstream file(fileName);

	int count = 0; // Number of tiles to be read.
	std::string buffer; // Temporary buffer to hold read strings.
	file >> buffer;
	file >> count; 


	mTiles.clear();
	mTiles.reserve(count);

	for (int i = 0; i < count; ++i)
	{
		int isWalkable = 0;
		file >> buffer;
		file >> isWalkable;


		// Add a new texture ID to the list of tile textures.
	 // Load the texture from the file and store its ID.
		Tile& tile = mTiles.emplace_back();
		tile.textureId = X::LoadTexture(buffer.c_str());
		tile.isWalkable = isWalkable == 0;
	}
	file.close();

	mTileWidth = X::GetSpriteWidth(mTiles[0].textureId);
	mTileheight= X::GetSpriteHeight(mTiles[0].textureId);


}

void TileMap::LoadMap(const char* fileName)
{

	if (!std::filesystem::exists(fileName))
	{
		return;
	}

	std::fstream file(fileName);
	int colums = 0;
	int rows = 0;
	std::string buffer;
	char tileType;

	// Read and store the number of columns and rows of the map.
	file >> buffer;
	file >> colums;
	file >> buffer;
	file >> rows;

	// Set the map dimensions based on the read values.
	mColumns = colums;
	mRows = rows;

	mMap.resize(colums * rows);

	// Read the tile types from the file and store them in the map.
	for (int y = 0; y < rows; ++y)
	{
		for (int x = 0; x < colums; ++x)
		{
			file >> tileType; // Read the type of each tile.
			int mapIndex = ToIndex(x, y, colums); // Convert 2D coordinates to 1D index.
			mMap[mapIndex] = tileType - '0'; // Store the tile type (convert char to int).
		}
	}
	file.close();

	// Lambda function to get the neighbor node for the graph.
	auto GetNeighbor = [&](int x, int y) -> AI::GridBasedGraph::Node*
		{
			if (IsBlocked(x, y))
			{
				return nullptr;
			}
			return mGraph.GetNode(x, y);
		};

	// Set up the neighbor relationships in the graph for each node.
	mGraph.Initialize(colums, rows);
	for (int y = 0; y < rows; ++y)
	{
		for (int x = 0; x < colums; ++x)
		{
			if (IsBlocked(x, y))
			{
				continue;
			}

			GridBasedGraph::Node* node = mGraph.GetNode(x, y);
			node->neighbors[GridBasedGraph::North] = GetNeighbor(x,y - 1);
			node->neighbors[GridBasedGraph::South] = GetNeighbor(x, y + 1 );
			node->neighbors[GridBasedGraph::East] = GetNeighbor(x + 1, y);
			node->neighbors[GridBasedGraph::West] = GetNeighbor(x - 1, y);
			node->neighbors[GridBasedGraph::NorthEast] = GetNeighbor(x + 1, y - 1);
			node->neighbors[GridBasedGraph::NorthWest] = GetNeighbor(x - 1, y - 1);
			node->neighbors[GridBasedGraph::SouthEast] = GetNeighbor(x + 1, y + 1);
			node->neighbors[GridBasedGraph::SouthWest] = GetNeighbor(x - 1, y + 1);
		}
	}
	
}

void TileMap::Render() const
{
	// Draw the map using mTiles and mMap
	X::Math::Vector2 position;
	for (int y = 0; y < mRows; ++y)
	{
		for (int x = 0; x < mColumns; ++x)
		{
			int mapIndex = ToIndex(x, y, mColumns); //Convert to 1D index from 2D coordinates.
			int tileType = mMap[mapIndex];
			X::DrawSprite(mTiles[tileType].textureId, position, X::Pivot::TopLeft);
			position.x += mTileWidth; // Move to the next tile position horizontally.
		}
		position.x = 0.0f; // Reset horizontal position after a row is complete.
		position.y += mTileheight; // Move to the next row position vertically.
	}

	// Draw debug drawnode
	for (int y = 0; y < mRows; ++y)
	{
		for (int x = 0; x < mColumns; ++x)
		{
			const GridBasedGraph::Node* node = mGraph.GetNode(x, y);
			for (const GridBasedGraph::Node* neighbor : node->neighbors)
			{
				if (neighbor != nullptr)
				{
					const X::Math::Vector2 a = GetPixelPosition(node->column, node->row);
					const X::Math::Vector2 b = GetPixelPosition(neighbor->column, neighbor->row);

					X::DrawScreenLine(a, b, X::Colors::Blue);
				}
			}
		}
	}
	//draw the search branches
	for (int y = 0; y < mRows; ++y)
	{
		for (int x = 0; x < mColumns; ++x)
		{
			const GridBasedGraph::Node* node = mGraph.GetNode(x, y);
			
			if (node->parent != nullptr)
			{
				const X::Math::Vector2 a = GetPixelPosition(node->column, node->row);
				const X::Math::Vector2 b = GetPixelPosition(node->parent->column, node->parent->row);

				X::DrawScreenLine(a, b, X::Colors::White);
			}
			
		}
	}
}


bool TileMap::IsBlocked(int x, int y) const
{
	if (x >= 0 && x < mColumns &&
		y >= 0 && y < mRows)
	{
		const int index = ToIndex(x, y, mColumns);
		const int tileId = mMap[index];
		if (mTiles[tileId].isWalkable)
		{
			return false;
		}
		
	}

	return true;
}

X::Math::Vector2 TileMap::GetPixelPosition(int x, int y)const
{
	return {
		(x + 0.5f) * mTileWidth,
		(y + 0.5f)* mTileheight
	};

}

Path TileMap::FindPathBFS(int startX, int startY, int endX, int endY)
{
	Path path;
	BFS bfs;

	if (bfs.Run(mGraph, startX, startY, endX, endY))
	{
		const NodeList& closedList = bfs.GetClosedList();

		GridBasedGraph::Node* node = closedList.back();
		while (node != nullptr)
		{
			path.push_back(GetPixelPosition(node->column, node->row));
			node = node->parent;
		}
		std::reverse(path.begin(), path.end());
	}
	return path;
}

Path TileMap::FindPathDFS(int startX, int startY, int endX, int endY)
{
	Path path;
	DFS dfs;

	if (dfs.Run(mGraph, startX, startY, endX, endY))
	{
		const NodeList& closedList = dfs.GetClosedList();

		GridBasedGraph::Node* node = closedList.back();
		while (node != nullptr)
		{
			path.push_back(GetPixelPosition(node->column, node->row));
			node = node->parent;
		}
		std::reverse(path.begin(), path.end());
	}
	return path;
}
Path TileMap::FindPathDijkstra(int startX, int startY, int endX, int endY)
{
	auto getCost = [](const GridBasedGraph::Node* node, const GridBasedGraph::Node* neightbor)
		{
			if (node->column != neightbor->column && node->row != neightbor->row)
			{
				return 5.0f;
			}
			return 1.0f;
		};

	Path path;
	Dijkstra dijkstra;

	if (dijkstra.Run(mGraph, startX, startY, endX, endY,getCost))
	{
		const NodeList& closedList = dijkstra.GetClosedList();

		GridBasedGraph::Node* node = closedList.back();
		while (node != nullptr)
		{
			path.push_back(GetPixelPosition(node->column, node->row));
			node = node->parent;
		}
		std::reverse(path.begin(), path.end());
	}
	return path;
}
//How to convert a 2d array into a 1d array.

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