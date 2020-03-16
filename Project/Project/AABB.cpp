#include "AABB.h"

#include <glm/gtx/component_wise.hpp>

namespace AABB {

    AABB::AABB()
    {
        setNull();
    }

    AABB::AABB(const glm::vec3& center, glm::float_t radius)
    {
        setNull();
        extend(center, radius);
    }

    AABB::AABB(const glm::vec3& p1, const glm::vec3& p2)
    {
        setNull();
        extend(p1);
        extend(p2);
    }

    AABB::AABB(const AABB& aabb)
    {
        setNull();
        extend(aabb);
    }

    AABB::~AABB()
    {
    }

    void AABB::extend(glm::float_t val)
    {
        if (!isNull())
        {
            mMin -= glm::vec3(val);
            mMax += glm::vec3(val);
        }
    }

    void AABB::extend(const glm::vec3& p)
    {
        if (!isNull())
        {
            mMin = glm::min(p, mMin);
            mMax = glm::max(p, mMax);
        }
        else
        {
            mMin = p;
            mMax = p;
        }
    }

    void AABB::extend(const glm::vec3& p, glm::float_t radius)
    {
        glm::vec3 r(radius);
        if (!isNull())
        {
            mMin = glm::min(p - r, mMin);
            mMax = glm::max(p + r, mMax);
        }
        else
        {
            mMin = p - r;
            mMax = p + r;
        }
    }

    void AABB::extend(const AABB& aabb)
    {
        if (!aabb.isNull())
        {
            extend(aabb.mMin);
            extend(aabb.mMax);
        }
    }

    glm::vec3 AABB::getDiagonal() const
    {
        if (!isNull())
            return mMax - mMin;
        else
            return glm::vec3(0);
    }

    glm::float_t AABB::getLongestEdge() const
    {
        return glm::compMax(getDiagonal());
    }

    glm::float_t AABB::getShortestEdge() const
    {
        return glm::compMin(getDiagonal());
    }

    glm::vec3 AABB::getCenter() const
    {
        if (!isNull())
        {
            glm::vec3 d = getDiagonal();
            return mMin + (d * glm::float_t(0.5));
        }
        else
        {
            return glm::vec3(0.0);
        }
    }

    void AABB::translate(const glm::vec3& v)
    {
        if (!isNull())
        {
            mMin += v;
            mMax += v;
        }
    }

    void AABB::scale(const glm::vec3& s, const glm::vec3& o)
    {
        if (!isNull())
        {
            mMin -= o;
            mMax -= o;

            mMin *= s;
            mMax *= s;

            mMin += o;
            mMax += o;
        }
    }

    INTERSECTION_TYPE AABB::intersect(const AABB& b) const
    {
        if (isNull() || b.isNull())
            return INTERSECTION_TYPE::OUTSIDE;

        if ((mMax.x < b.mMin.x) || (mMin.x > b.mMax.x) ||
            (mMax.y < b.mMin.y) || (mMin.y > b.mMax.y) ||
            (mMax.z < b.mMin.z) || (mMin.z > b.mMax.z))
        {
            return INTERSECTION_TYPE::OUTSIDE;
        }

        if ((mMin.x <= b.mMin.x) && (mMax.x >= b.mMax.x) &&
            (mMin.y <= b.mMin.y) && (mMax.y >= b.mMax.y) &&
            (mMin.z <= b.mMin.z) && (mMax.z >= b.mMax.z))
        {
            return INTERSECTION_TYPE::INSIDE;
        }

        return INTERSECTION_TYPE::INTERSECT;
    }
}