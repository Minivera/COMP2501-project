#pragma once

#include <glm/glm.hpp>

// Taken from https://github.com/iauns/cpm-glm-aabb, then lightly modified
namespace AABB {

    /// Type returned from call to intersect.
    enum class INTERSECTION_TYPE { INSIDE, INTERSECT, OUTSIDE };

    /// Standalone axis aligned bounding box implemented built on top of GLM.
    class AABB
    {
    public:
        /// Builds a null AABB.
        AABB();

        /// Builds an AABB that encompasses a sphere.
        /// \param[in]  center Center of the sphere.
        /// \param[in]  radius Radius of the sphere.
        AABB(const glm::vec3& center, glm::float_t radius);

        /// Builds an AABB that contains the two points.
        AABB(const glm::vec3& p1, const glm::vec3& p2);

        AABB(const AABB& aabb);
        ~AABB();

        /// Set the AABB as NULL (not set).
        inline void setNull() { mMin = glm::vec3(1.0); mMax = glm::vec3(-1.0); }

        /// Returns true if AABB is NULL (not set).
        inline bool isNull() const { return mMin.x > mMax.x || mMin.y > mMax.y || mMin.z > mMax.z; }

        /// Extend the bounding box on all sides by \p val.
        void extend(glm::float_t val);

        /// Expand the AABB to include point \p p.
        void extend(const glm::vec3& p);

        /// Expand the AABB to include a sphere centered at \p center and of radius \p
        /// radius.
        /// \param[in]  center Center of sphere.
        /// \param[in]  radius Radius of sphere.
        void extend(const glm::vec3& center, glm::float_t radius);

        /// Expand the AABB to encompass the given \p aabb.
        void extend(const AABB& aabb);

        /// Translates AABB by vector \p v.
        void translate(const glm::vec3& v);

        /// Scale the AABB by \p scale, centered around \p origin.
        /// \param[in]  scale  3D vector specifying scale along each axis.
        /// \param[in]  origin Origin of scaling operation. Most useful origin would
        ///                    be the center of the AABB.
        void scale(const glm::vec3& scale, const glm::vec3& origin);

        /// Retrieves the center of the AABB.
        glm::vec3 getCenter() const;

        /// Retrieves the diagonal vector (computed as mMax - mMin).
        /// If the AABB is NULL, then a vector of all zeros is returned.
        glm::vec3 getDiagonal() const;

        /// Retrieves the longest edge.
        /// If the AABB is NULL, then 0 is returned.
        glm::float_t getLongestEdge() const;

        /// Retrieves the shortest edge.
        /// If the AABB is NULL, then 0 is returned.
        glm::float_t getShortestEdge() const;

        /// Retrieves the AABB's minimum point.
        inline glm::vec3 getMin() const { return mMin; }

        /// Retrieves the AABB's maximum point.
        inline glm::vec3 getMax() const { return mMax; }

        /// Returns one of the intersection types. If either of the aabbs are invalid,
        /// then OUTSIDE is returned.
        INTERSECTION_TYPE intersect(const AABB& bb) const;

        inline bool collides(const AABB& bb) const { return intersect(bb) != INTERSECTION_TYPE::OUTSIDE; };
    private:
        glm::vec3 mMin;   ///< Minimum point.
        glm::vec3 mMax;   ///< Maximum point.
    };

}