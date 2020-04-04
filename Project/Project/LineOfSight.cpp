#include "LineOfSight.h"

#include <glm/gtx/vector_angle.hpp>

namespace LineOfSight {
    float Line::angle() const {
        return glm::angle(glm::normalize(glm::vec3(start, 0.0f)), glm::normalize(glm::vec3(dir, 0.0f)));
    }

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
			glm::vec2(x2, y1),
			glm::vec2(x1, y2),
			glm::vec2(x2, y2),
		};
		return rectangle;
	}

    /* Check whether P and Q lie on the same side of line AB */
    inline float side(glm::vec2 p, glm::vec2 q, glm::vec2 a, glm::vec2 b)
    {
        float z1 = (b.x - a.x) * (p.y - a.y) - (p.x - a.x) * (b.y - a.y);
        float z2 = (b.x - a.x) * (q.y - a.y) - (q.x - a.x) * (b.y - a.y);
        return z1 * z2;
    }

    /* Check whether segment line intersects with triangle t0t1t2 */
    bool intersectsWith(const Line& line, glm::vec2 t0, glm::vec2 t1, glm::vec2 t2)
    {
        /* Check whether segment is outside one of the three half-planes
         * delimited by the triangle. */
        float f1 = side(line.start, t2, t0, t1), f2 = side(line.dir, t2, t0, t1);
        float f3 = side(line.start, t0, t1, t2), f4 = side(line.dir, t0, t1, t2);
        float f5 = side(line.start, t1, t2, t0), f6 = side(line.dir, t1, t2, t0);
        /* Check whether triangle is totally inside one of the two half-planes

         * delimited by the segment. */
        float f7 = side(t0, t1, line.start, line.dir);
        float f8 = side(t1, t2, line.start, line.dir);

        /* If segment is strictly outside triangle, or triangle is strictly
         * apart from the line, we're not intersecting */
        if ((f1 < 0 && f2 < 0) || (f3 < 0 && f4 < 0) || (f5 < 0 && f6 < 0)
            || (f7 > 0 && f8 > 0))
        {
            return false;
        }

        /* If segment is aligned with one of the edges, we're overlapping */
        if ((f1 == 0 && f2 == 0) || (f3 == 0 && f4 == 0) || (f5 == 0 && f6 == 0))
        {
            return true;
        }

        /* If segment is outside but not strictly, or triangle is apart but
         * not strictly, we're touching */
        if ((f1 <= 0 && f2 <= 0) || (f3 <= 0 && f4 <= 0) || (f5 <= 0 && f6 <= 0)
            || (f7 >= 0 && f8 >= 0))
        {
            return true;
        }

        /* If both segment points are strictly inside the triangle, we
         * are not intersecting either */
        if (f1 > 0 && f2 > 0 && f3 > 0 && f4 > 0 && f5 > 0 && f6 > 0)
        {
            return false;
        }

        /* Otherwise we're intersecting with at least one edge */
        return true;
    }

    bool intersectsWith(const Line& line, const Rectangle& rect)
    {
		// Check if the given line intersects with one of the two vertices making up the given rectangle
		return intersectsWith(line, rect.p1, rect.p2, rect.p4) || intersectsWith(line, rect.p1, rect.p3, rect.p4);
    }
}