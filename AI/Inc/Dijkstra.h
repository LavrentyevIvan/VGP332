#pragma once
#include "GridBasedGraph.h" //maybe

namespace AI
{

	using GetCost = std::function<float(const GridBasedGraph::Node*, const GridBasedGraph::Node*)>;

	class Dijkstra
	{
	public:
		bool Run(GridBasedGraph& graph, int startX, int startY, int endX, int EndY, GetCost GetCost);

		const NodeList& GetClosedList() const { return mClosedList; }

	private:
		NodeList mOpenList;
		NodeList mClosedList;
	};
}