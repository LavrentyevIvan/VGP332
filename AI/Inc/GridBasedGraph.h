#pragma once

namespace AI
{
	class GridBasedGraph
	{

	public:
		enum Direction
		{
			North,
			South,
			East,
			West,
			NorthEast,
			NorthWest,
			SouthEast,
			SouthWest
		};


		struct Node
		{
			std::array<Node*, 8> neighbors = {};
			int column = 0;
			int row = 0;

			Node* parent = nullptr;

			bool opened = false;
			bool closed = false;
			float cost = 0.0f;
			float heuristic = 0.0f;
			void Reset()
			{
				parent = nullptr;
				opened = false;
				closed = false;
				cost = 0.0f;
				heuristic = 0.0f;
			}

			//std::array<Node
		};

		void Initialize(int colums, int rows);

		void ResetSearchParams();


		int GetColumns() const { return mColumns; }
		int GetRows() const { return mRows; }

		Node* GetNode(int x, int y);
		const Node* GetNode(int x, int y) const;


	private:
		int GetIndex(int x, int y) const;

		int mColumns = 0;
		int mRows = 0;

		std::vector<Node> mNodes;
	};

	using NodeList = std::list<GridBasedGraph::Node*>;
}