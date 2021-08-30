#include "AABB.h"

#include "Plane3.h"
#include "Matrix4.h"

// Expand this AABB to include the given point.
void AABB::includePoint(const Vector3& point) {

	// If not yet initialised, simply set the AABB to equal the point
	if (!isValid())
	{
		origin = point;
		extents = Vector3(0, 0, 0);
	}
	else
	{
		// Extend each axis separately
		for (int i = 0; i < 3; ++i)
		{
			// Axis displacement from origin to point
			double axisDisp = point[i] - origin[i];

			// Half of extent increase needed (maybe negative if point inside)
			double halfDif = 0.5f * (std::abs(axisDisp) - extents[i]);

			if (halfDif > 0)
			{
				origin[i] += (axisDisp > 0) ? halfDif : -halfDif;
				extents[i] += halfDif;
			}
		}
	}
}

// Expand this AABB to include another AABB
void AABB::includeAABB(const AABB& other)
{
	// Validity check. If both this and other are valid, use the extension
	// algorithm. If only the other AABB is valid, set this AABB equal to it.
	// If neither are valid we do nothing.

	if (isValid() && other.isValid())
	{
		// Extend each axis separately
		for (int i = 0; i < 3; ++i)
		{
		    double displacement = other.origin[i] - origin[i];
		    double difference = other.extents[i] - extents[i];

		    if (fabs(displacement) > fabs(difference))
		    {
				double half_difference = 0.5f * (fabs(displacement) + difference);

				if (half_difference > 0.0f)
				{
					origin[i] += (displacement >= 0.0f) ? half_difference : -half_difference;
					extents[i] += half_difference;
				}
		    }
		    else if (difference > 0.0f)
		    {
				origin[i] = other.origin[i];
				extents[i] = other.extents[i];
		    }
		}
	}
	else if (other.isValid())
	{
		origin = other.origin;
		extents = other.extents;
	}
}

void AABB::getCorners(Vector3 corners[8], const Matrix4& rotation) const
{
	Vector3 x = rotation.xCol().getVector3() * extents.x();
	Vector3 y = rotation.yCol().getVector3() * extents.y();
	Vector3 z = rotation.zCol().getVector3() * extents.z();

	corners[0] = origin - x + y + z;
	corners[1] = origin + x + y + z;
	corners[2] = origin + x - y + z;
	corners[3] = origin - x - y + z;
	corners[4] = origin - x + y - z;
	corners[5] = origin + x + y - z;
	corners[6] = origin + x - y - z;
	corners[7] = origin - x - y - z;
}

void AABB::getPlanes(Plane3 planes[6], const Matrix4& rotation) const
{
	double x = rotation.xCol().getVector3().dot(origin);
	double y = rotation.yCol().getVector3().dot(origin);
	double z = rotation.zCol().getVector3().dot(origin);

	planes[0] = Plane3( rotation.xCol().getVector3(), x + extents[0]);
	planes[1] = Plane3(-rotation.xCol().getVector3(), -(x - extents[0]));
	planes[2] = Plane3( rotation.yCol().getVector3(), y + extents[1]);
	planes[3] = Plane3(-rotation.yCol().getVector3(), -(y - extents[1]));
	planes[4] = Plane3( rotation.zCol().getVector3(), z + extents[2]);
	planes[5] = Plane3(-rotation.zCol().getVector3(), -(z - extents[2]));
}

AABB AABB::createFromOrientedAABB(const AABB& aabb, const Matrix4& transform)
{
	return AABB(
		transform.transformPoint(aabb.origin),
		Vector3(
			fabs(transform[0]  * aabb.extents[0]) +
			fabs(transform[4]  * aabb.extents[1]) +
			fabs(transform[8]  * aabb.extents[2]),
			fabs(transform[1]  * aabb.extents[0]) +
			fabs(transform[5]  * aabb.extents[1]) +
			fabs(transform[9]  * aabb.extents[2]),
			fabs(transform[2]  * aabb.extents[0]) +
			fabs(transform[6]  * aabb.extents[1]) +
			fabs(transform[10] * aabb.extents[2])
		)
	);
}
