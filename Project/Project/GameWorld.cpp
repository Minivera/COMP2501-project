#include "GameWorld.h"

#include <queue>

void WorldVertex::connect(shared_ptr<WorldVertex> vertex, int edgeCost) {
	if (this == vertex.get()) {
		return;
	}

	// Make sure this edge doesn't already exist in the other vertex
	for (auto it = vertex->edges.begin(); it != vertex->edges.end(); it++) {
		auto connected = getConnectingNode(*it);
		if (connected == *this) {
			return;
		}
	}

	// Creates an edge corresponding to vertex
	shared_ptr<WorldVertex> thisValue(this);
	WorldEdge edge = { thisValue, vertex, edgeCost };

	// Adds the edge to the vector in both vertex.
	this->addEdge(edge);
	vertex->addEdge(edge);
}

WorldVertex& WorldVertex::getConnectingNode(WorldEdge edge) {
	if (edge.node1.get() == this) {
		return *edge.node2;
	}
	else if (edge.node2.get() == this) {
		return *edge.node1;
	}
	else {
		return *this;
	}
}

bool WorldVertex::operator==(const WorldVertex& other) const {
	return this->position == other.position;
}

void Path::update(glm::vec3 position) {
	// Check if we need to update, which only happens if we are not at our objective
	if (currentIndex < path.size() - 1) {
		// Check if we are closer to the current element of the path or the next one.
		if (glm::distance(position, path.at(currentIndex)->position) > glm::distance(position, path.at(currentIndex + 1)->position)) {
			currentIndex++;
		}
	}
}

GameWorld::GameWorld(vector<vector<WorldCellDefinition>>& worldDefinition) {
	// loop into the definition of the 2D world
	for (int row = 0; row < worldDefinition.size(); row++) {
		for (int column = 0; column < worldDefinition.size(); column++) {
			auto cell = worldDefinition.at(row).at(column);

			// Create the vertex if it's not blocked
			if (cell.blocked) {
				continue;
			}

			auto vertex = make_shared<WorldVertex>();
			vertex->position = worldDefinition.at(row).at(column).position;
			vertices.push_back(vertex);
			cellsMap.insert(make_pair(make_tuple(row, column), vertex));
		}
	}

	// loop again to create the edges
	for (int row = 0; row < worldDefinition.size(); row++) {
		for (int column = 0; column < worldDefinition.at(row).size(); column++) {
			// Check if we have a vertex at the current position
			auto cell = cellsMap.find(make_tuple(row, column));
			if (cell == cellsMap.end()) {
				continue;
			}

			// Loop again to go through all cells around the current cell
			for (int vertical = -1; vertical < 2; vertical++) {
				for (int horizontal = -1; horizontal < 2; horizontal++) {
					// Make sure we don't get out of bounds
					if (row + vertical < 0 || row + vertical > worldDefinition.size() - 1 ||
						column + horizontal < 0 || column + horizontal > worldDefinition.at(row).size() - 1) {
						continue;
					}

					// Check if we have a vertex at the current position
					auto foundCell = cellsMap.find(make_tuple(row + vertical, column + horizontal));
					if (foundCell != cellsMap.end()) {
						cell->second->connect(foundCell->second, 1);
					}
				}
			}
		}
	}
}

// Utility struct for the purposes of pathfinding
struct QNode {
	shared_ptr<WorldVertex> node;
	int cost;
};

// Class used to compare 2 QNode structs
class compareNode
{
public:
	int operator() (const QNode& n1, const QNode& n2)
	{
		return n1.cost > n2.cost;
	}
};

Path& GameWorld::pathfind(shared_ptr<WorldVertex> start, shared_ptr<WorldVertex> end) {

	// Priority queue used in pathfinding.
	// it is created using the NodeTuple struct with a min compare function called compareNode
	priority_queue <QNode, vector<QNode>, compareNode> pq;

	std::vector<shared_ptr<WorldVertex>> pathNodes;
	// Reset all the nodes costs
	for (auto it = vertices.begin(); it != vertices.end(); it++) {
		(*it)->calculatedCost = 0;
	}

	// The startnode is added to the pq with cost 0
	QNode startNode = { start, 0 };
	pq.push(startNode);

	// Now that the pq is setup, we can start the algorithm
	// keep in mind that the as the QNode struct has a pointer to the corresponding node
	// some function calls will use pointer syntax (->) 
	while (!pq.empty()) {
		// Get the current lowest-cost node in pq
		QNode lowest = pq.top();

		// If the current node is the end node, done!
		if (lowest.node == end) {
			break;
		}

		// Open node
		vector<WorldEdge> neighbours = lowest.node->edges;
		for (int i = 0; i < neighbours.size(); i++) {
			// Compute cost to get to neighbouring node
			// Cost = the cost to get the corrent node + cost to traverse the edge

			WorldEdge currentEdge = neighbours.at(i);
			WorldVertex& node = lowest.node->getConnectingNode(currentEdge);

			// Calculate distance as part of the algorithm to obtain the A* algorithm
			int nodeCost = lowest.cost + glm::distance(currentEdge.node1->position, currentEdge.node2->position) * neighbours.at(i).cost;

			//if current node cost is higher than calculated, update node, and add QNode to queue			
			if (node.calculatedCost > nodeCost) {
				node.calculatedCost = nodeCost;
				node.prev = lowest.node;

				shared_ptr<WorldVertex> nodePtr(&node);
				QNode updatedNode = { nodePtr, nodeCost };
				pq.push(updatedNode);
			}
		}

		// Remove node form queue
		pq.pop();
	}

	// Queue is done, go in reverse from END to START to determine path
	shared_ptr<WorldVertex> currentNode = end->prev;

	// While the current node isn't null, or the end, mark the current node as on the path
	pathNodes.push_back(end);
	while (currentNode != NULL && currentNode != start) {
		pathNodes.push_back(currentNode);

		// This can lead to an infinite loop between two node, for some reason
		currentNode = currentNode->prev;
	}
	pathNodes.push_back(currentNode);

	std::reverse(pathNodes.begin(), pathNodes.end());

	return Path{
		pathNodes,
		0,
		end
	};
}