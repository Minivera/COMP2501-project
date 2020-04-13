#include "GameWorld.h"

#include <queue>

#include "Random.h"

WorldVertex::WorldVertex(glm::vec3 position) {
	this->position = position;
}

void WorldVertex::connect(shared_ptr<WorldVertex> vertex, int edgeCost) {
	if (this == vertex.get()) {
		return;
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
		if (glm::distance(position, path.at(currentIndex)->getPosition()) < glm::distance(position, path.at(currentIndex + 1)->getPosition())) {
			currentIndex++;
		}
	}
}

GameWorld::GameWorld(vector<vector<WorldCellDefinition>>& worldDefinition) {
	// loop into the definition of the 2D world
	for (int row = 0; row < worldDefinition.size(); row++) {
		for (int column = 0; column < worldDefinition.at(row).size(); column++) {
			auto cell = worldDefinition.at(row).at(column);

			// Create the vertex if it's not blocked
			if (cell.blocked) {
				continue;
			}

			auto vertex = make_shared<WorldVertex>(worldDefinition.at(row).at(column).position);
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
					if (row + vertical < 0 || row + vertical > worldDefinition.size() ||
						column + horizontal < 0 || column + horizontal > worldDefinition.at(row).size()) {
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

shared_ptr<WorldVertex> GameWorld::closestNodeToPosition(glm::vec3 position) {
	// loop through all the vertices and compare their positions
	for (auto it = vertices.begin(); it != vertices.end(); it++) {
		if (glm::distance(position, (*it)->getPosition()) < 1.0f) {
			return *it;
		}
	}
	return nullptr;
}

shared_ptr<Path> GameWorld::randomizedPathfind(shared_ptr<WorldVertex> start, float distance) {
	for (int i = 0; i < vertices.size(); i++) {
		auto element = vertices.at(i);

		// If we found our start vertex, select a random vetex within the selected distance
		if (element == start) {
			vector<shared_ptr<WorldVertex>> closeVertices;

			// Start looping in the vertices again to find all those that are closer than the distance
			for (auto it = vertices.begin(); it != vertices.end(); it++) {
				if (element != (*it) && glm::distance(element->getPosition(), (*it)->getPosition()) <= distance) {
					closeVertices.push_back(*it);
				}
			}

			// Pathfind from the given start node to the random node
			return pathfind(start, vertices.at(random::randomInt(0, closeVertices.size() - 1)));
		}
	}

	return pathfind(start, *(vertices.end() - 1));
}

// Utility struct for the purposes of pathfinding
struct QNode {
	WorldVertex* node;
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

shared_ptr<Path> GameWorld::pathfind(shared_ptr<WorldVertex> start, shared_ptr<WorldVertex> end) {
	// Priority queue used in pathfinding.
	// it is created using the NodeTuple struct with a min compare function called compareNode
	priority_queue <QNode, vector<QNode>, compareNode> pq;

	std::vector<WorldVertex*> pathNodes;
	// Reset all the nodes costs
	for (auto it = vertices.begin(); it != vertices.end(); it++) {
		(*it)->setCalculatedCost(INT_MAX);
	}

	// The startnode is added to the pq with cost 0
	QNode startNode = { start.get(), 0 };
	pq.push(startNode);

	// Now that the pq is setup, we can start the algorithm
	// keep in mind that the as the QNode struct has a pointer to the corresponding node
	// some function calls will use pointer syntax (->) 
	while (!pq.empty()) {
		// Get the current lowest-cost node in pq
		QNode lowest = pq.top();

		// If the current node is the end node, done!
		if (lowest.node == end.get()) {
			break;
		}

		// Open node
		vector<WorldEdge> neighbours = lowest.node->getEdges();
		for (int i = 0; i < neighbours.size(); i++) {
			// Compute cost to get to neighbouring node

			WorldEdge currentEdge = neighbours.at(i);
			WorldVertex& node = lowest.node->getConnectingNode(currentEdge);

			// Calculate the cost to get the corrent node + cost to traverse the edge
			int nodeCost = lowest.cost + neighbours.at(i).cost;

			//if current node cost is higher than calculated, update node, and add QNode to queue			
			if (node.getCalculatedCost() > nodeCost) {
				node.setCalculatedCost(nodeCost);
				node.setPrev(lowest.node);

				QNode updatedNode = { &node, nodeCost };
				pq.push(updatedNode);
			}
		}

		// Remove node from queue
		pq.pop();
	}

	// Queue is done, go in reverse from END to START to determine path
	WorldVertex* currentNode = end->getPrev();

	// While the current node isn't null, or the end, mark the current node as on the path
	pathNodes.push_back(end.get());
	while (currentNode != NULL && currentNode != start.get()) {
		// This can lead to an infinite loop between two node, for some reason
		// So make sure we prevent that by stopping if we're looping on nodes
		if (std::find(pathNodes.begin(), pathNodes.end(), currentNode) != pathNodes.end()) {
			break;
		}

		pathNodes.push_back(currentNode);
		currentNode = currentNode->getPrev();
	}
	pathNodes.push_back(currentNode);

	std::reverse(pathNodes.begin(), pathNodes.end());

	return make_shared<Path>(
		pathNodes,
		1, // 1 because 0 is the current node
		end
	);
}