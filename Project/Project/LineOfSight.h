#pragma once

#include <glm/glm.hpp>

namespace LineOfSight
{
	struct Line {
		glm::vec2 start;
		glm::vec2 end;
	};

	struct Rectangle {
		glm::vec2 start;
		glm::vec2 end;
	};

	// Methods that allow a user to define lines and rectangles for intersection
	Line drawLine(float x1, float y1, float x2, float y2);
	Rectangle drawRectangle(float x1, float y1, float x2, float y2);

	// Method to get the intersection of two distances with a line
	bool getIntersection(float distance1, float distance2, const Line& line, glm::vec2& result);

	bool isInBoundingBox(const glm::vec2& result, const Rectangle& rect, const int axis);

	// Check if a line intersects with a rectangle
	bool intersectsWith(const Line& line, const Rectangle& rect);
};

