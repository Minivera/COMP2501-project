#pragma once

#include <glm/glm.hpp>

namespace LineOfSight
{
	struct Line {
		glm::vec2 start;
		glm::vec2 dir;

		float angle() const;
	};

	struct Rectangle {
		glm::vec2 p1;
		glm::vec2 p2;
		glm::vec2 p3;
		glm::vec2 p4;
	};

	// Methods that allow a user to define lines and rectangles for intersection
	Line drawLine(float x1, float y1, float x2, float y2);
	Rectangle drawRectangle(float x1, float y1, float x2, float y2);

	// Check if a line intersects with a rectangle
	bool intersectsWith(const Line& line, const Rectangle& rect);

	// Check if a line intersects with a triangle v1, v2, v3
	bool intersectsWith(const Line& line, glm::vec2 v1, glm::vec2 v2, glm::vec2 v3);
};

