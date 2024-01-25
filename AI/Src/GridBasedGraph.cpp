#include "Precompiled.h"

#include "GridBasedGraph.h"

using namespace AI;

void GridBasedGraph::Initialize(int columns, int rows)
{
	mNodes.clear();
	mNodes.reserve(columns * rows);
	mColumns = columns;
	mRows = rows;

	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < columns; x++)
		{
			Node& node = mNodes.emplace_back();
			node.column = x;
			node.row = y;
		}
	}
}

void  GridBasedGraph::ResetSearchParams()
{
	for (Node& node : mNodes)
	{
		node.Reset();
	}


}


GridBasedGraph::Node* GridBasedGraph::GetNode(int x, int y)
{

	const int index = GetIndex(x, y);
	if (index >= 0 && index < mNodes.size())
	{
		return &mNodes[index];
	}
	return nullptr;

}

const  GridBasedGraph::Node* GridBasedGraph::GetNode(int x, int y) const
{
	const int index = GetIndex(x, y);
	if (index >= 0 && index < mNodes.size())
	{
		return &mNodes[index];
	}
	return nullptr;
}


int  GridBasedGraph::GetIndex(int x, int y) const
{
	return x + (y * mColumns);
}
