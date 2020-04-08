#pragma once

#include <vector>
#include <memory>
#include <map>
#include <tuple>
#include <glm/glm.hpp>

using namespace std;

class GameObject;

struct WorldCellDefinition {
	bool blocked;
	glm::vec3 position;
};

class WorldVertex;

// A world edge is an edge that connects to world vertices. It has a cost associated to it, which
// represents the cost of moving from one vertex to another. If above 999, we can consider that the edge
// is some unpassable terrain.
struct WorldEdge {
	shared_ptr<WorldVertex> node1;
	shared_ptr<WorldVertex> node2;
	int cost;
};

// A world vertex is a vertex that exists in the game world and in which entities can move around.
// Each vertex is given a position, which roughly equals the centrer position of the empty cell in the worls space.
class WorldVertex {
private:
	// The position of that edge in the world space.
	glm::vec3 position;

	// Vector containing all edges the vertex connects to.
	// this can be used to create a graph with any number of connectivity
	vector<WorldEdge> edges;

	// Calculated cost for the current node. Will be changed in the pathfinding algorithm.
	// TODO: Do this without mutating our original world?
	int calculatedCost = 0;

	// Pointer to the previous node when the path was last calculated.
	// TODO: Do this without mutating our original world?
	WorldVertex* prev;

public:
	WorldVertex(glm::vec3 position);

	// Connects two vertices with an edge of 'edgeCost'
	void connect(shared_ptr<WorldVertex> n, int edgeCost);

	// Determines the opposing node on an edge.
	// if the edge does not contain the current node, it will return the current node
	WorldVertex& getConnectingNode(WorldEdge e);

	// Links two nodes together via an edge. Updates both nodes edges vector
	void addEdge(WorldEdge e) { edges.push_back(e); }

	// getters
	inline glm::vec3 getPosition() const { return position; }
	inline vector<WorldEdge> getEdges() const { return edges; }
	inline int getCalculatedCost() const { return calculatedCost; }
	inline WorldVertex* getPrev() const { return prev; }

	// setters
	inline void setPrev(WorldVertex* newPrev) { prev = newPrev; }
	inline void setCalculatedCost(int newCost) { calculatedCost = newCost; }

	// Operators
	bool operator==(const WorldVertex&) const;
};

// Path represent a path inside the game world graph that an entity is given.
class Path {
private:
	// The path that this path represents. It is used to allow an entity to move along that path.
	vector<WorldVertex*> path;

	// The index of the current position of the entity along the path.
	int currentIndex;

	// The objective node at the end of the path for quick reference.
	shared_ptr<WorldVertex> objective;

public:
	Path(vector<WorldVertex*> givenPath, int givenIndex, shared_ptr<WorldVertex> givenObjective) : path(givenPath), currentIndex(givenIndex), objective(givenObjective) {}

	// Method that udpates the position of the entity along the path.
	void update(glm::vec3 position);

	// Get the current node for this path.
	inline WorldVertex* currentNode() const { return path.at(currentIndex); }

	// getters
	inline shared_ptr<WorldVertex> getObjective() { return objective; }
	inline bool complete() const { return currentIndex >= path.size() - 1; }
};

// A Gameworld is a graph that contains multiple vertices and edges representing the various areas of the game world that entities can move to.
// This graph is used to make the movement of autonomous agent a bit more realistic.
class GameWorld
{
private:
	// The vertices of this graph. The vertices are used for reference, but each node should be able to pathfinded without referencing this array.
	vector<shared_ptr<WorldVertex>> vertices;

	// A map of all the vertices in the graph. Thiswill a a vertex based representation of the definition passed to the constructor
	// so any blocked cell will be missing from this map.
	map<tuple<int, int>, shared_ptr<WorldVertex>> cellsMap;
public:
	GameWorld(vector<vector<WorldCellDefinition>>& worldDefinition);

	// Finds the closest node to the given position.
	shared_ptr<WorldVertex> closestNodeToPosition(glm::vec3 position);

	// Finds a randomized end node and then triggers a pathfinding algorithm to that node
	shared_ptr<Path> randomizedPathfind(shared_ptr<WorldVertex> start, float distance);

	// Creates a path from the start node to the end node by making sure that the given path is the shortes path possible.
	shared_ptr<Path> pathfind(shared_ptr<WorldVertex> start, shared_ptr<WorldVertex> end);
};

