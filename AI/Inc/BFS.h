#pragma once

#include "GridBasedGraph.h"

namespace AI
{
	
	using NodeList = std::list<GridBasedGraph::Node*>;

	class BFS
	{
	public:
		bool Run(GridBasedGraph& graph, int startX, int startY, int endX, int EndY);

		const NodeList& GetClosedList() const { return mClosedList; }

	private:
		NodeList mOpenList;									//cant access for some reason
		NodeList mClosedList;
	};
}
