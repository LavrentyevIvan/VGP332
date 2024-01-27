#include "Precompiled.h"

#include "DFS.h"

using namespace AI;

bool DFS::Run(GridBasedGraph& graph, int startX, int startY, int endX, int EndY)
{
	bool found = false;

	graph.ResetSearchParams();
	mOpenList.clear();
	mClosedList.clear();

	GridBasedGraph::Node* node = graph.GetNode(startX, startY);

	node->opened = true;
	mOpenList.push_back(node);

	while (!found && !mOpenList.empty())
	{
		node = mOpenList.back();
		mOpenList.pop_back();

		if (node->column == endX && node->row == EndY)
		{
			found = true;
		}
		else
		{
			for (GridBasedGraph::Node* neighbor : node->neighbors)
			{
				if (neighbor == nullptr || neighbor->opened)
				{
					continue;
				}
				mOpenList.push_back(neighbor);
				neighbor->opened = true;
				neighbor->parent = node;
			}
		}

		mClosedList.push_back(node);
		node->closed = true;

	}

	return found;
}