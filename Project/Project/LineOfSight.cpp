#include "LineOfSight.h"

namespace LineOfSight {
	Line drawLine(float x1, float y1, float x2, float y2) {
		Line line = {
			glm::vec2(x1, y1),
			glm::vec2(x2, y2),
		};
		return line;
	}

	Rectangle drawRectangle(float x1, float y1, float x2, float y2) {
		Rectangle rectangle = {
			glm::vec2(x1, y1),
			glm::vec2(x2, y2),
		};
		return rectangle;
	}

    bool getIntersection(float distance1, float distance2, const Line& line, glm::vec2& result) {
        if ((distance1 * distance2) >= 0.0f) {
            return false;
        }
        if (distance1 == distance2) {
            return false;
        }
        result = line.start + (line.end - line.start) * (-distance1 / (distance2 - distance1));
        return true;
    }

    bool isInBoundingBox(const glm::vec2& result, const Rectangle& rect, const int axis) {
        if (axis == 1 && result.y > rect.start.y && result.y < rect.end.y) {
            return true;
        }
        if (axis == 2 && result.x > rect.start.x && result.x < rect.end.x) {
            return true;
        }
        return false;
    }

    bool intersectsWith(const Line& line, const Rectangle& rect)
    {
        glm::vec2 result;
        if (line.end.x < rect.start.x && line.start.x < rect.start.x) {
            return false;
        }
        if (line.end.x > rect.end.x && line.start.x > rect.end.x) {
            return false;
        }
        if (line.end.y < rect.start.y && line.start.y < rect.start.y) {
            return false;
        }
        if (line.end.y > rect.end.y && line.start.y > rect.end.y) {
            return false;
        }
        if (line.start.x > rect.start.x && line.start.x < rect.end.x &&
            line.start.y > rect.start.y && line.start.y < rect.end.y)
        {
            return true;
        }
        if ((getIntersection(line.start.x - rect.start.x, line.end.x - rect.start.x, line, result) && isInBoundingBox(result, rect, 1))
            || (getIntersection(line.start.y - rect.start.y, line.end.y - rect.start.y, line, result) && isInBoundingBox(result, rect, 2))
            || (getIntersection(line.start.x - rect.end.x, line.end.x - rect.end.x, line, result) && isInBoundingBox(result, rect, 1))
            || (getIntersection(line.start.y - rect.end.y, line.end.y - rect.end.y, line, result) && isInBoundingBox(result, rect, 2)))
        {
            return true;
        }

        return false;
    }
}