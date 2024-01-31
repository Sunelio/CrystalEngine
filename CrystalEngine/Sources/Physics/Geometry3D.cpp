#include "Physics/Geometry3D.h"
#include <cfloat>
#include "glad/glad.h"
#include "Maths/Matrix.h"
#include "Maths/Transform.h"

using namespace Geometry3D;
using namespace Maths;

#pragma warning(disable : 4267)
#pragma warning(disable : 4172)

#ifndef  M_PI
	#define M_PI 3.14159265358979323846f
#endif


#define CMP(x, y) \
	(fabsf(x - y) <= FLT_EPSILON * fmaxf(1.0f, fmaxf(fabsf(x), fabsf(y))))


namespace Geometry3D
{

	float Length(const Line& line) {
		return (line.start - line.end).GetLength();
	}

	float LengthSq(const Line& line) {
		return (line.start - line.end).GetLength();
	}

	Ray FromPoints(const Point& from, const Point& to) {
		return Ray(
			from,
			(to - from).GetNormalized()
		);
	}

	Vector3 GetMin(const AABB& aabb) {
		Vector3 p1 = aabb.position + aabb.size;
		Vector3 p2 = aabb.position - aabb.size;

		return Vector3(fminf(p1.x, p2.x), fminf(p1.y, p2.y), fminf(p1.z, p2.z));
	}
	Vector3 GetMax(const AABB& aabb) {
		Vector3 p1 = aabb.position + aabb.size;
		Vector3 p2 = aabb.position - aabb.size;

		return Vector3(fmaxf(p1.x, p2.x), fmaxf(p1.y, p2.y), fmaxf(p1.z, p2.z));
	}

	AABB FromMinMax(const Vector3& min, const Vector3& max) {
		return AABB((min + max) * 0.5f, (max - min) * 0.5f);
	}

	float PlaneEquation(const Point& point, const Plane& plane) {
		return point.Dot(plane.normal) - plane.distance;
	}

	float PlaneEquation(const Plane& plane, const Point& point) {
		return point.Dot(plane.normal) - plane.distance;
	}

	std::ostream& operator<<(std::ostream& os, const Line& shape) {
		os << "start: (" << shape.start.x << ", " << shape.start.y << ", " << shape.start.z << "), end: (";
		os << shape.end.x << ", " << shape.end.y << ", " << shape.end.z << ")";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const Ray& shape) {
		os << "origin: (" << shape.origin.x << ", " << shape.origin.y << ", " << shape.origin.z << "), ";
		os << "direction: (" << shape.direction.x << ", " << shape.direction.y << ", " << shape.direction.z << ")";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const Sphere& shape) {
		os << "position:" << shape.position.x << ", " << shape.position.y << ", " << shape.position.z << "), ";
		os << "radius: " << shape.radius;
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const AABB& shape) {
		Vector3 min = GetMin(shape);
		Vector3 max = GetMax(shape);
		os << "min: (" << min.x << ", " << min.y << ", " << min.z << "), ";
		os << "max: (" << max.x << ", " << max.y << ", " << max.z << ")";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const Plane& shape) {
		os << "normal: (" << shape.normal.x << ", " << shape.normal.y << ", " << shape.normal.z << "), ";
		os << "distance: " << shape.distance;
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const Triangle& shape) {
		os << "a: (" << shape.a.x << ", " << shape.a.y << ", " << shape.a.z << "), ";
		os << "b: (" << shape.b.x << ", " << shape.b.y << ", " << shape.b.z << "), ";
		os << "c: (" << shape.c.x << ", " << shape.c.y << ", " << shape.c.z << ")";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const OBB& shape) {
		os << "position:" << shape.position.x << ", " << shape.position.y << ", " << shape.position.z << "), ";
		os << "size:" << shape.size.x << ", " << shape.size.y << ", " << shape.size.z << "), ";
		os << "x basis:" << shape.orientation.m[0][0] << ", " << shape.orientation.m[1][0] << ", " << shape.orientation.m[2][0] << "), ";
		os << "y basis:" << shape.orientation.m[0][1] << ", " << shape.orientation.m[1][1] << ", " << shape.orientation.m[2][1] << "), ";
		os << "z basis:" << shape.orientation.m[0][2] << ", " << shape.orientation.m[1][2] << ", " << shape.orientation.m[2][2] << ")";
		return os;
	}

	bool PointInSphere(const Point& point, const Sphere& sphere) {
		return (point - sphere.position).GetLengthSq() < sphere.radius * sphere.radius;
	}

	bool PointOnPlane(const Point& point, const Plane& plane) {
		// This should probably use an epsilon!
		//return Dot(point, plane.normal) - plane.distance == 0.0f;

		

		return CMP(point.Dot(plane.normal) - plane.distance, 0.0f);
	}

	bool PointInAABB(const Point& point, const AABB& aabb) {
		Point min = GetMin(aabb);
		Point max = GetMax(aabb);

		if (point.x < min.x || point.y < min.y || point.z < min.z) {
			return false;
		}
		if (point.x > max.x || point.y > max.y || point.z > max.z) {
			return false;
		}

		return true;
	}

	bool PointInOBB(const Point& point, const OBB& obb) {
		Vector3 dir = point - obb.position;

		for (int i = 0; i < 3; ++i) {
			const float* orientation = &obb.orientation.array[i * 3];
			Vector3 axis(orientation[0], orientation[1], orientation[2]);

			float distance = dir.Dot(axis);

			if (distance > obb.size[i]) {
				return false;
			}
			if (distance < -obb.size[i]) {
				return false;
			}
		}

		return true;
	}

	Point ClosestPoint(const Sphere& sphere, const Point& point) {
		Vector3 sphereToPoint = point - sphere.position;
		(sphereToPoint).Normalize();
		sphereToPoint = sphereToPoint * sphere.radius;
		return sphereToPoint + sphere.position;
	}

	Point ClosestPoint(const AABB& aabb, const Point& point) {
		Point result = point;
		Point min = GetMin(aabb);
		Point max = GetMax(aabb);

		result.x = (result.x < min.x) ? min.x : result.x;
		result.y = (result.y < min.x) ? min.y : result.y;
		result.z = (result.z < min.x) ? min.z : result.z;

		result.x = (result.x > max.x) ? max.x : result.x;
		result.y = (result.y > max.x) ? max.y : result.y;
		result.z = (result.z > max.x) ? max.z : result.z;

		return result;
	}

	Point ClosestPoint(const OBB& obb, const Point& point) {
		Point result = obb.position;
		Vector3 dir = point - obb.position;

		for (int i = 0; i < 3; ++i) {
			const float* orientation = &obb.orientation.array[i * 3];
			Vector3 axis(orientation[0], orientation[1], orientation[2]);

			float distance = dir.Dot(axis);

			if (distance > obb.size[i]) {
				distance = obb.size[i];
			}
			if (distance < -obb.size[i]) {
				distance = -obb.size[i];
			}

			result = result + (axis * distance);
		}

		return result;
	}

	Point ClosestPoint(const Plane& plane, const Point& point) {
		float distance = plane.normal.Dot(point) - plane.distance;


		return point - plane.normal * distance;
	}

	bool PointOnLine(const Point& point, const Line& line) {
		Point closest = ClosestPoint(line, point);
		float distanceSq = (closest - point).GetLengthSq();
		return CMP(distanceSq, 0.0f);
	}

	Point ClosestPoint(const Line& line, const Point& point) {
		Vector3 lVec = line.end - line.start; // Line Vector
		// Project "point" onto the "Line Vector", computing:
		// closest(t) = start + t * (end - start)
		// T is how far along the line the projected point is
		float t = (point - line.start).Dot(lVec) / lVec.Dot(lVec);
		// Clamp t to the 0 to 1 range
		t = fmaxf(t, 0.0f);
		t = fminf(t, 1.0f);
		// Return projected position of t
		return line.start + lVec * t;
	}

	bool PointOnRay(const Point& point, const Ray& ray) {
		if (point == ray.origin) {
			return true;
		}

		Vector3 norm = point - ray.origin;
		norm.Normalize();
		float diff = norm.Dot(ray.direction); // Direction is normalized
		// If BOTH vectors point in the same direction, diff should be 1
		return CMP(diff, 1.0f);
	}

	Point ClosestPoint(const Ray& ray, const Point& point) {
 
		float t = (point - ray.origin).Dot(ray.direction);


		t = fmaxf(t, 0.0f);


		return Point(ray.origin + ray.direction * t);
	}

	bool PointInPlane(const Point& point, const Plane& plane) {
		return PointOnPlane(point, plane);
	}
	bool PointInLine(const Point& point, const Line& line) {
		return PointOnLine(point, line);
	}
	bool PointInRay(const Point& point, const Ray& ray) {
		return PointOnRay(point, ray);
	}
	bool ContainsPoint(const Sphere& sphere, const Point& point) {
		return PointInSphere(point, sphere);
	}
	bool ContainsPoint(const Point& point, const Sphere& sphere) {
		return PointInSphere(point, sphere);
	}
	bool ContainsPoint(const AABB& aabb, const Point& point) {
		return PointInAABB(point, aabb);
	}
	bool ContainsPoint(const Point& point, const AABB& aabb) {
		return PointInAABB(point, aabb);
	}
	bool ContainsPoint(const Point& point, const OBB& obb) {
		return PointInOBB(point, obb);
	}
	bool ContainsPoint(const OBB& obb, const Point& point) {
		return PointInOBB(point, obb);
	}
	bool ContainsPoint(const Point& point, const Plane& plane) {
		return PointOnPlane(point, plane);
	}
	bool ContainsPoint(const Plane& plane, const Point& point) {
		return PointOnPlane(point, plane);
	}
	bool ContainsPoint(const Point& point, const Line& line) {
		return PointOnLine(point, line);
	}
	bool ContainsPoint(const Line& line, const Point& point) {
		return PointOnLine(point, line);
	}
	bool ContainsPoint(const Point& point, const Ray& ray) {
		return PointOnRay(point, ray);
	}
	bool ContainsPoint(const Ray& ray, const Point& point) {
		return PointOnRay(point, ray);
	}
	Point ClosestPoint(const Point& point, const Sphere& sphere) {
		return ClosestPoint(sphere, point);
	}
	Point ClosestPoint(const Point& point, const AABB& aabb) {
		return ClosestPoint(aabb, point);
	}
	Point ClosestPoint(const Point& point, const OBB& obb) {
		return ClosestPoint(obb, point);
	}
	Point ClosestPoint(const Point& point, const Plane& plane) {
		return ClosestPoint(plane, point);
	}
	Point ClosestPoint(const Point& point, const Line& line) {
		return ClosestPoint(line, point);
	}
	Point ClosestPoint(const Point& point, const Ray& ray) {
		return ClosestPoint(ray, point);
	}
	Point ClosestPoint(const Point& p, const Triangle& t) {
		return ClosestPoint(t, p);
	}

	bool SphereSphere(const Sphere& s1, const Sphere& s2) {
		float radiiSum = s1.radius + s2.radius;
		float sqDistance = (s1.position - s2.position).GetLengthSq();
		return sqDistance < radiiSum* radiiSum;
	}

	bool SphereAABB(const Sphere& sphere, const AABB& aabb) {
		Point closestPoint = ClosestPoint(aabb, sphere.position);
		float distSq = (sphere.position - closestPoint).GetLengthSq();
		float radiusSq = sphere.radius * sphere.radius;
		return distSq < radiusSq;
	}

	bool SphereOBB(const Sphere& sphere, const OBB& obb) {
		Point closestPoint = ClosestPoint(obb, sphere.position);
		float distSq = (sphere.position - closestPoint).GetLengthSq();
		float radiusSq = sphere.radius * sphere.radius;
		return distSq < radiusSq;
	}

	bool SpherePlane(const Sphere& sphere, const Plane& plane) {
		Point closestPoint = ClosestPoint(plane, sphere.position);
		float distSq = (sphere.position - closestPoint).GetLengthSq();
		float radiusSq = sphere.radius * sphere.radius;
		return distSq < radiusSq;
	}

	bool AABBAABB(const AABB& aabb1, const AABB& aabb2) {
		Point aMin = GetMin(aabb1);
		Point aMax = GetMax(aabb1);
		Point bMin = GetMin(aabb2);
		Point bMax = GetMax(aabb2);

		return	(aMin.x <= bMax.x && aMax.x >= bMin.x) &&
			(aMin.y <= bMax.y && aMax.y >= bMin.y) &&
			(aMin.z <= bMax.z && aMax.z >= bMin.z);
	}

	bool AABBOBB(const AABB& aabb, const OBB& obb) {
		const float* o = obb.orientation.array;

		Vector3 test[15] = {
			Vector3(1, 0, 0), // AABB axis 1
			Vector3(0, 1, 0), // AABB axis 2
			Vector3(0, 0, 1), // AABB axis 3
			Vector3(o[0], o[1], o[2]),
			Vector3(o[3], o[4], o[5]),
			Vector3(o[6], o[7], o[8])
		};

		for (int i = 0; i < 3; ++i) { // Fill out rest of axis
			test[6 + i * 3 + 0] = test[i].Cross(test[0]);
			test[6 + i * 3 + 1] = test[i].Cross(test[1]);
			test[6 + i * 3 + 2] = test[i].Cross(test[2]);
		}

		for (int i = 0; i < 15; ++i) {
			if (!OverlapOnAxis(aabb, obb, test[i])) {
				return false;
			}
		}

		return true;
	}

	bool OverlapOnAxis(const AABB& aabb, const OBB& obb, const Vector3& axis) 
	{
		Interval a = GetInterval(aabb, axis);
		Interval b = GetInterval(obb, axis);
		return ((b.min <= a.max) && (a.min <= b.max));
	}

	bool OverlapOnAxis(const OBB& obb1, const OBB& obb2, const Vector3& axis) 
	{
		Interval a = GetInterval(obb1, axis);
		Interval b = GetInterval(obb2, axis);
		return ((b.min <= a.max) && (a.min <= b.max));
	}

	bool OverlapOnAxis(const AABB& aabb, const Triangle& triangle, const Vector3& axis) {
		Interval a = GetInterval(aabb, axis);
		Interval b = GetInterval(triangle, axis);
		return ((b.min <= a.max) && (a.min <= b.max));
	}

	bool OverlapOnAxis(const OBB& obb, const Triangle& triangle, const Vector3& axis) {
		Interval a = GetInterval(obb, axis);
		Interval b = GetInterval(triangle, axis);
		return ((b.min <= a.max) && (a.min <= b.max));
	}

	bool OverlapOnAxis(const Triangle& t1, const Triangle& t2, const Vector3& axis) {
		Interval a = GetInterval(t1, axis);
		Interval b = GetInterval(t2, axis);
		return ((b.min <= a.max) && (a.min <= b.max));
	}

	Interval GetInterval(const Triangle& triangle, const Vector3& axis) {
		Interval result;

		result.min = axis.Dot(triangle.points[0]);
		result.max = result.min;
		for (int i = 1; i < 3; ++i) {
			float value = axis.Dot(triangle.points[i]);
			result.min = fminf(result.min, value);
			result.max = fmaxf(result.max, value);
		}

		return result;
	}

	Interval GetInterval(const OBB& obb, const Vector3& axis) {
		Vector3 vertex[8];

		Vector3 C = obb.position;	// OBB Center
		Vector3 E = obb.size;		// OBB Extents
		const float* o = obb.orientation.array;
		Vector3 A[] = {			// OBB Axis
			Vector3(o[0], o[1], o[2]),
			Vector3(o[3], o[4], o[5]),
			Vector3(o[6], o[7], o[8]),
		};

		vertex[0] = C + A[0] * E[0] + A[1] * E[1] + A[2] * E[2];
		vertex[1] = C - A[0] * E[0] + A[1] * E[1] + A[2] * E[2];
		vertex[2] = C + A[0] * E[0] - A[1] * E[1] + A[2] * E[2];
		vertex[3] = C + A[0] * E[0] + A[1] * E[1] - A[2] * E[2];
		vertex[4] = C - A[0] * E[0] - A[1] * E[1] - A[2] * E[2];
		vertex[5] = C + A[0] * E[0] - A[1] * E[1] - A[2] * E[2];
		vertex[6] = C - A[0] * E[0] + A[1] * E[1] - A[2] * E[2];
		vertex[7] = C - A[0] * E[0] - A[1] * E[1] + A[2] * E[2];

		Interval result;
		result.min = result.max = axis.Dot(vertex[0]);

		for (int i = 1; i < 8; ++i) {
			float projection = axis.Dot(vertex[i]);
			result.min = (projection < result.min) ? projection : result.min;
			result.max = (projection > result.max) ? projection : result.max;
		}

		return result;
	}

	Interval GetInterval(const AABB& aabb, const Vector3& axis) {
		Vector3 i = GetMin(aabb);
		Vector3 a = GetMax(aabb);

		Vector3 vertex[8] = {
			Vector3(i.x, a.y, a.z),
			Vector3(i.x, a.y, i.z),
			Vector3(i.x, i.y, a.z),
			Vector3(i.x, i.y, i.z),
			Vector3(a.x, a.y, a.z),
			Vector3(a.x, a.y, i.z),
			Vector3(a.x, i.y, a.z),
			Vector3(a.x, i.y, i.z)
		};

		Interval result;
		result.min = result.max = axis.Dot(vertex[0]);

		for (int i = 1; i < 8; ++i) {
			float projection = axis.Dot(vertex[i]);
			result.min = (projection < result.min) ? projection : result.min;
			result.max = (projection > result.max) ? projection : result.max;
		}

		return result;
	}

	bool AABBPlane(const AABB& aabb, const Plane& plane) {
	
		float pLen = aabb.size.x * fabsf(plane.normal.x) +
			aabb.size.y * fabsf(plane.normal.y) +
			aabb.size.z * fabsf(plane.normal.z);
	
		float dist = plane.normal.Dot(aabb.position) - plane.distance;

		return fabsf(dist) <= pLen;
	}

	bool OBBOBB(const OBB& obb1, const OBB& obb2) {
		const float* o1 = obb1.orientation.array;
		const float* o2 = obb2.orientation.array;

		Vector3 test[15] = {
			Vector3(o1[0], o1[1], o1[2]),
			Vector3(o1[3], o1[4], o1[5]),
			Vector3(o1[6], o1[7], o1[8]),
			Vector3(o2[0], o2[1], o2[2]),
			Vector3(o2[3], o2[4], o2[5]),
			Vector3(o2[6], o2[7], o2[8])
		};

		for (int i = 0; i < 3; ++i) { // Fill out rest of axis
			test[6 + i * 3 + 0] = test[i].Cross(test[0]);
			test[6 + i * 3 + 1] = test[i].Cross(test[1]);
			test[6 + i * 3 + 2] = test[i].Cross(test[2]);
		}

		for (int i = 0; i < 15; ++i) {
			if (!OverlapOnAxis(obb1, obb2, test[i])) {
				return false; // Seperating axis found
			}
		}

		return true; // Seperating axis not found
	}

	bool OBBPlane(const OBB& obb, const Plane& plane) {
		
		const float* o = obb.orientation.array;
		Vector3 rot[] = { // rotation / orientation
			Vector3(o[0], o[1], o[2]),
			Vector3(o[3], o[4], o[5]),
			Vector3(o[6], o[7], o[8]),
		};
		Vector3 normal = plane.normal;


		float pLen = obb.size.x * fabsf(normal.Dot(rot[0])) +
			obb.size.y * fabsf(normal.Dot(rot[1])) +
			obb.size.z * fabsf(normal.Dot(rot[2]));

		float dist = plane.normal.Dot(obb.position) - plane.distance;
		return fabsf(dist) <= pLen;
	}

	bool PlanePlane(const Plane& plane1, const Plane& plane2) {

		Vector3 d = plane1.normal.Cross(plane2.normal);


		return !(CMP(d.Dot(d), 0));

	}

	bool Raycast(const Sphere& sphere, const Ray& ray, RaycastResult* outResult) {
		ResetRaycastResult(outResult);

		Vector3 e = sphere.position - ray.origin;
		float rSq = sphere.radius * sphere.radius;

		float eSq = e.GetLengthSq();
		float a = e.Dot(ray.direction); // ray.direction is assumed to be normalized
		float bSq = /*sqrtf(*/eSq - (a * a)/*)*/;
		float f = sqrt(fabsf((rSq)- /*(b * b)*/bSq));

		// Assume normal intersection!
		float t = a - f;

		// No collision has happened
		if (rSq - (eSq - a * a) < 0.0f) {
			return false;
		}
		// Ray starts inside the sphere
		else if (eSq < rSq) {
			// Just reverse direction
			t = a + f;
		}
		if (outResult != 0) {
			outResult->t = t;
			outResult->hit = true;
			outResult->point = ray.origin + ray.direction * t;
			outResult->normal = (outResult->point - sphere.position).GetNormalized();
		}
		return true;
	}

	bool Raycast(const OBB& obb, const Ray& ray, RaycastResult* outResult) {
		ResetRaycastResult(outResult);

		const float* o = obb.orientation.array;
		const float* size = &obb.size.x;

		Vector3 p = obb.position - ray.origin;

		Vector3 X(o[0], o[1], o[2]);
		Vector3 Y(o[3], o[4], o[5]);
		Vector3 Z(o[6], o[7], o[8]);

		Vector3 f(
			X.Dot(ray.direction),
			Y.Dot(ray.direction),
			Z.Dot(ray.direction)
		);

		Vector3 e(
			X.Dot(p),
			Y.Dot(p),
			Z.Dot(p)
		);

#if 1
		float t[6] = { 0, 0, 0, 0, 0, 0 };
		for (int i = 0; i < 3; ++i) {
			if (CMP(f[i], 0)) {
				if (-e[i] - size[i] > 0 || -e[i] + size[i] < 0) {
					return false;
				}
				f[i] = 0.00001f; // Avoid div by 0!
			}

			t[i * 2 + 0] = (e[i] + size[i]) / f[i]; // tmin[x, y, z]
			t[i * 2 + 1] = (e[i] - size[i]) / f[i]; // tmax[x, y, z]
		}

		float tmin = fmaxf(fmaxf(fminf(t[0], t[1]), fminf(t[2], t[3])), fminf(t[4], t[5]));
		float tmax = fminf(fminf(fmaxf(t[0], t[1]), fmaxf(t[2], t[3])), fmaxf(t[4], t[5]));
#else 
		// The above loop simplifies the below if statements
		// this is done to make sure the sample fits into the book
		if (CMP(f.x, 0)) {
			if (-e.x - obb.size.x > 0 || -e.x + obb.size.x < 0) {
				return -1;
			}
			f.x = 0.00001f; // Avoid div by 0!
		}
		else if (CMP(f.y, 0)) {
			if (-e.y - obb.size.y > 0 || -e.y + obb.size.y < 0) {
				return -1;
			}
			f.y = 0.00001f; // Avoid div by 0!
		}
		else if (CMP(f.z, 0)) {
			if (-e.z - obb.size.z > 0 || -e.z + obb.size.z < 0) {
				return -1;
			}
			f.z = 0.00001f; // Avoid div by 0!
		}
		float t1 = (e.x + obb.size.x) / f.x;
		float t2 = (e.x - obb.size.x) / f.x;
		float t3 = (e.y + obb.size.y) / f.y;
		float t4 = (e.y - obb.size.y) / f.y;
		float t5 = (e.z + obb.size.z) / f.z;
		float t6 = (e.z - obb.size.z) / f.z;
		float tmin = fmaxf(fmaxf(fminf(t1, t2), fminf(t3, t4)), fminf(t5, t6));
		float tmax = fminf(fminf(fmaxf(t1, t2), fmaxf(t3, t4)), fmaxf(t5, t6));
#endif

		// if tmax < 0, ray is intersecting AABB
		// but entire AABB is behing it's origin
		if (tmax < 0) {
			return false;
		}

		// if tmin > tmax, ray doesn't intersect AABB
		if (tmin > tmax) {
			return false;
		}

		// If tmin is < 0, tmax is closer
		float t_result = tmin;

		if (tmin < 0.0f) {
			t_result = tmax;
		}

		if (outResult != 0) {
			outResult->hit = true;
			outResult->t = t_result;
			outResult->point = ray.origin + ray.direction * t_result;

			Vector3 normals[] = {
				X,			// +x
				X * -1.0f,	// -x
				Y,			// +y
				Y * -1.0f,	// -y
				Z,			// +z
				Z * -1.0f	// -z
			};

			for (int i = 0; i < 6; ++i) {
				if (CMP(t_result, t[i])) {
					outResult->normal = (normals[i]).GetNormalized();
				}
			}
		}
		return true;
	}

	void ResetRaycastResult(RaycastResult* outResult) {
		if (outResult != 0) {
			outResult->t = -1;
			outResult->hit = false;
			outResult->normal = Vector3(0, 0, 1);
			outResult->point = Vector3(0, 0, 0);
		}
	}

	bool Raycast(const AABB& aabb, const Ray& ray, RaycastResult* outResult) {
		ResetRaycastResult(outResult);

		Vector3 min = GetMin(aabb);
		Vector3 max = GetMax(aabb);


		float t1 = (min.x - ray.origin.x) / (CMP(ray.direction.x, 0.0f) ? 0.00001f : ray.direction.x);
		float t2 = (max.x - ray.origin.x) / (CMP(ray.direction.x, 0.0f) ? 0.00001f : ray.direction.x);
		float t3 = (min.y - ray.origin.y) / (CMP(ray.direction.y, 0.0f) ? 0.00001f : ray.direction.y);
		float t4 = (max.y - ray.origin.y) / (CMP(ray.direction.y, 0.0f) ? 0.00001f : ray.direction.y);
		float t5 = (min.z - ray.origin.z) / (CMP(ray.direction.z, 0.0f) ? 0.00001f : ray.direction.z);
		float t6 = (max.z - ray.origin.z) / (CMP(ray.direction.z, 0.0f) ? 0.00001f : ray.direction.z);

		float tmin = fmaxf(fmaxf(fminf(t1, t2), fminf(t3, t4)), fminf(t5, t6));
		float tmax = fminf(fminf(fmaxf(t1, t2), fmaxf(t3, t4)), fmaxf(t5, t6));

		// if tmax < 0, ray is intersecting AABB
		// but entire AABB is behing it's origin
		if (tmax < 0) {
			return false;
		}

		// if tmin > tmax, ray doesn't intersect AABB
		if (tmin > tmax) {
			return false;
		}

		float t_result = tmin;

		// If tmin is < 0, tmax is closer
		if (tmin < 0.0f) {
			t_result = tmax;
		}

		if (outResult != 0) {
			outResult->t = t_result;
			outResult->hit = true;
			outResult->point = ray.origin + ray.direction * t_result;

			Vector3 normals[] = {
				Vector3(-1, 0, 0),
				Vector3(1, 0, 0),
				Vector3(0, -1, 0),
				Vector3(0, 1, 0),
				Vector3(0, 0, -1),
				Vector3(0, 0, 1)
			};
			float t[] = { t1, t2, t3, t4, t5, t6 };

			for (int i = 0; i < 6; ++i) {
				if (CMP(t_result, t[i])) {
					outResult->normal = normals[i];
				}
			}
		}

		return true;
	}

	bool Raycast(const Plane& plane, const Ray& ray, RaycastResult* outResult) {
		ResetRaycastResult(outResult);

		float nd = ray.direction.Dot(plane.normal);
		float pn = ray.origin.Dot(plane.normal);

		if (nd >= 0.0f) {
			return false;
		}

		float t = (plane.distance - pn) / nd;

		
		if (t >= 0.0f) {
			if (outResult != 0) {
				outResult->t = t;
				outResult->hit = true;
				outResult->point = ray.origin + ray.direction * t;
				outResult->normal = (plane.normal).GetNormalized();
			}
			return true;
		}

		return false;
	}

	bool Linetest(const Sphere& sphere, const Line& line) {
		Point closest = ClosestPoint(line, sphere.position);
		float distSq = (sphere.position - closest).GetLengthSq();
		return distSq <= (sphere.radius * sphere.radius);
	}

	bool Linetest(const Plane& plane, const Line& line) {
		Vector3 ab = line.end - line.start;

		float nA = plane.normal.Dot(line.start);
		float nAB = plane.normal.Dot(ab);

		if (CMP(nAB, 0)) {
			return false;
		}

		float t = (plane.distance - nA) / nAB;
		return t >= 0.0f && t <= 1.0f;
	}

	bool Linetest(const AABB& aabb, const Line& line) {
		Ray ray;
		ray.origin = line.start;
		ray.direction = (line.end - line.start).GetNormalized();
		RaycastResult raycast;
		if (!Raycast(aabb, ray, &raycast)) {
			return false;
		}
		float t = raycast.t;

		return t >= 0 && t * t <= LengthSq(line);
	}

	bool Linetest(const OBB& obb, const Line& line) {
		if ((line.end - line.start).GetLengthSq() < 0.0000001f) {
			return PointInOBB(line.start, obb);
		}
		Ray ray;
		ray.origin = line.start;
		ray.direction = (line.end - line.start).GetNormalized();
		RaycastResult result;
		if (!Raycast(obb, ray, &result)) {
			return false;
		}
		float t = result.t;

		return t >= 0 && t * t <= LengthSq(line);
	}

	bool Raycast(const Ray& ray, const Sphere& sphere, RaycastResult* outResult) {
		return Raycast(sphere, ray, outResult);
	}

	bool Raycast(const Ray& ray, const AABB& aabb, RaycastResult* outResult) {
		return Raycast(aabb, ray, outResult);
	}

	bool Raycast(const Ray& ray, const OBB& obb, RaycastResult* outResult) {
		return Raycast(obb, ray, outResult);
	}

	bool Raycast(const Ray& ray, const Plane& plane, RaycastResult* outResult) {
		return Raycast(plane, ray, outResult);
	}

	bool Linetest(const Line& line, const Sphere& sphere) {
		return Linetest(sphere, line);
	}

	bool Linetest(const Line& line, const AABB& aabb) {
		return Linetest(aabb, line);
	}

	bool Linetest(const Line& line, const OBB& obb) {
		return Linetest(obb, line);
	}

	bool Linetest(const Line& line, const Plane& plane) {
		return Linetest(plane, line);
	}

	Vector3 Centroid(const Triangle& t) {
		Vector3 result;
		result.x = t.a.x + t.b.x + t.c.x;
		result.y = t.a.y + t.b.y + t.c.y;
		result.z = t.a.z + t.b.z + t.c.z;
		result = result * (1.0f / 3.0f);
		return result;
	}

	bool PointInTriangle(const Point& p, const Triangle& t) {

		Vector3 a = t.a - p;
		Vector3 b = t.b - p;
		Vector3 c = t.c - p;


		Vector3 normPBC = b.Cross(c);
		Vector3 normPCA = c.Cross(a);
		Vector3 normPAB = a.Cross(b);

		
		if (normPBC.Dot(normPCA) < 0.0f) {
			return false;
		}
		else if (normPBC.Dot(normPAB) < 0.0f) {
			return false;
		}

		
		return true;
	}


	Vector3 Barycentric(const Point& p, const Triangle& t) {
		Vector3 ap = p - t.a;
		Vector3 bp = p - t.b;
		Vector3 cp = p - t.c;

		Vector3 ab = t.b - t.a;
		Vector3 ac = t.c - t.a;
		Vector3 bc = t.c - t.b;
		Vector3 cb = t.b - t.c;
		Vector3 ca = t.a - t.c;

		Vector3 v = ab - Project(ab, cb);
		float a = 1.0f - v.Dot(ap) / v.Dot(ab);

		v = bc - Project(bc, ac);
		float b = 1.0f - v.Dot(bp) / v.Dot(bc);

		v = ca - Project(ca, ab);
		float c = 1.0f - v.Dot(cp) / v.Dot(ca);

		return Vector3(a, b, c);
	}

	Plane FromTriangle(const Triangle& t) {
		Plane result;
		result.normal = (t.b - t.a).Cross(t.c - t.a).GetNormalized();
		result.distance = result.normal.Dot(t.a);
		return result;
	}

	Point ClosestPoint(const Triangle& t, const Point& p) {
		Plane plane = FromTriangle(t);
		Point closest = ClosestPoint(plane, p);

		// Closest point was inside triangle
		if (PointInTriangle(closest, t)) {
			return closest;
		}

		Point c1 = ClosestPoint(Line(t.a, t.b), closest); 
		Point c2 = ClosestPoint(Line(t.b, t.c), closest); 
		Point c3 = ClosestPoint(Line(t.c, t.a), closest);

		float magSq1 = (closest - c1).GetLengthSq();
		float magSq2 = (closest - c2).GetLengthSq();
		float magSq3 = (closest - c3).GetLengthSq();

		if (magSq1 < magSq2 && magSq1 < magSq3) {
			return c1;
		}
		else if (magSq2 < magSq1 && magSq2 < magSq3) {
			return c2;
		}

		return c3;
	}

	bool TriangleSphere(const Triangle& t, const Sphere& s) {
		Point closest = ClosestPoint(t, s.position);
		float magSq = (closest - s.position).GetLengthSq();
		return magSq <= s.radius * s.radius;
	}

	bool TriangleAABB(const Triangle& t, const AABB& a) {
		// Compute the edge vectors of the triangle  (ABC)
		Vector3 f0 = t.b - t.a;
		Vector3 f1 = t.c - t.b;
		Vector3 f2 = t.a - t.c;

		// Compute the face normals of the AABB
		Vector3 u0(1.0f, 0.0f, 0.0f);
		Vector3 u1(0.0f, 1.0f, 0.0f);
		Vector3 u2(0.0f, 0.0f, 1.0f);

		Vector3 test[13] = {
			// 3 Normals of AABB
			u0, // AABB Axis 1
			u1, // AABB Axis 2
			u2, // AABB Axis 3
			// 1 Normal of the Triangle
			f0.Cross(f1),
			// 9 Axis, cross products of all edges
			u0.Cross(f0),
			u0.Cross(f1),
			u0.Cross(f2),
			u1.Cross(f0),
			u1.Cross(f1),
			u1.Cross(f2),
			u2.Cross(f0),
			u2.Cross(f1),
			u2.Cross(f2)
		};

		for (int i = 0; i < 13; ++i) {
			if (!OverlapOnAxis(a, t, test[i])) {
				return false; // Seperating axis found
			}
		}

		return true; // Seperating axis not found
	}

	bool TriangleOBB(const Triangle& t, const OBB& o) {
		// Compute the edge vectors of the triangle  (ABC)
		Vector3 f0 = t.b - t.a;
		Vector3 f1 = t.c - t.b;
		Vector3 f2 = t.a - t.c;

		// Compute the face normals of the AABB
		const float* orientation = o.orientation.array;
		Vector3 u0(orientation[0], orientation[1], orientation[2]);
		Vector3 u1(orientation[3], orientation[4], orientation[5]);
		Vector3 u2(orientation[6], orientation[7], orientation[8]);

		Vector3 test[13] = {
			// 3 Normals of AABB
			u0, // AABB Axis 1
			u1, // AABB Axis 2
			u2, // AABB Axis 3
			// 1 Normal of the Triangle
			f0.Cross(f1),
			// 9 Axis, cross products of all edges
			u0.Cross(f0),
			u0.Cross(f1),
			u0.Cross(f2),
			u1.Cross(f0),
			u1.Cross(f1),
			u1.Cross(f2),
			u2.Cross(f0),
			u2.Cross(f1),
			u2.Cross(f2)
		};

		for (int i = 0; i < 13; ++i) {
			if (!OverlapOnAxis(o, t, test[i])) {
				return false; // Seperating axis found
			}
		}

		return true; // Seperating axis not found
	}

	bool TriangleTriangle(const Triangle& t1, const Triangle& t2) {
#if 0
		Vector3 axisToTest[] = {
			// Triangle 1, Normal
			SatCrossEdge(t1.a, t1.b, t1.b, t1.c),
			// Triangle 2, Normal
			SatCrossEdge(t2.a, t2.b, t2.b, t2.c),

			// Cross Product of edges
			SatCrossEdge(t2.a, t2.b, t1.a, t1.b),
			SatCrossEdge(t2.a, t2.b, t1.b, t1.c),
			SatCrossEdge(t2.a, t2.b, t1.c, t1.a),

			SatCrossEdge(t2.b, t2.c, t1.a, t1.b),
			SatCrossEdge(t2.b, t2.c, t1.b, t1.c),
			SatCrossEdge(t2.b, t2.c, t1.c, t1.a),

			SatCrossEdge(t2.c, t2.a, t1.a, t1.b),
			SatCrossEdge(t2.c, t2.a, t1.b, t1.c),
			SatCrossEdge(t2.c, t2.a, t1.c, t1.a),
		};
#else 
		Vector3 t1_f0 = t1.b - t1.a; // Edge 0
		Vector3 t1_f1 = t1.c - t1.b; // Edge 1
		Vector3 t1_f2 = t1.a - t1.c; // Edge 2

		Vector3 t2_f0 = t2.b - t2.a; // Edge 0
		Vector3 t2_f1 = t2.c - t2.b; // Edge 1
		Vector3 t2_f2 = t2.a - t2.c; // Edge 2

		Vector3 axisToTest[] = {
			// Triangle 1, Normal
			t1_f0.Cross(t1_f1),
			// Triangle 2, Normal
			t2_f0.Cross(t2_f1),

			// Cross Product of edges
			t2_f0.Cross(t1_f0),
			t2_f0.Cross(t1_f1),
			t2_f0.Cross(t1_f2),

			t2_f1.Cross(t1_f0),
			t2_f1.Cross(t1_f1),
			t2_f1.Cross(t1_f2),

			t2_f2.Cross(t1_f0),
			t2_f2.Cross(t1_f1),
			t2_f2.Cross(t1_f2),
		};
#endif

		for (int i = 0; i < 11; ++i) {
			if (!OverlapOnAxis(t1, t2, axisToTest[i])) {
				return false; // Seperating axis found
			}
		}

		return true; // Seperating axis not found
	}

	bool TriangleTriangleRobust(const Triangle& t1, const Triangle& t2) {
		Vector3 axisToTest[] = {
			// Triangle 1, Normal
			SatCrossEdge(t1.a, t1.b, t1.b, t1.c),
			// Triangle 2, Normal
			SatCrossEdge(t2.a, t2.b, t2.b, t2.c),

			// Cross Product of edges
			SatCrossEdge(t2.a, t2.b, t1.a, t1.b),
			SatCrossEdge(t2.a, t2.b, t1.b, t1.c),
			SatCrossEdge(t2.a, t2.b, t1.c, t1.a),

			SatCrossEdge(t2.b, t2.c, t1.a, t1.b),
			SatCrossEdge(t2.b, t2.c, t1.b, t1.c),
			SatCrossEdge(t2.b, t2.c, t1.c, t1.a),

			SatCrossEdge(t2.c, t2.a, t1.a, t1.b),
			SatCrossEdge(t2.c, t2.a, t1.b, t1.c),
			SatCrossEdge(t2.c, t2.a, t1.c, t1.a),
		};

		for (int i = 0; i < 11; ++i) {
			if (!OverlapOnAxis(t1, t2, axisToTest[i])) {
				if (!CMP((axisToTest[i].GetLengthSq()), 0)) {
					return false; // Seperating axis found
				}
			}
		}

		return true; // Seperating axis not found
	}

	Vector3 SatCrossEdge(const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& d) {
		Vector3 ab = b - a;
		Vector3 cd = d - c;

		Vector3 result = ab.Cross(cd);
		if (!CMP(result.GetLengthSq(), 0)) { // Is ab and cd parallel?
			return result; // Not parallel!
		}
		else { // ab and cd are parallel
			// Get an axis perpendicular to AB
			Vector3 axis = ab.Cross(c - a);
			result = ab.Cross(axis);
			if (!CMP(result.GetLengthSq(), 0)) { // Still parallel?
				return result; // Not parallel
			}
		}
		// New axis being tested is parallel too.
		// This means that a, b, c and d are on a line
		// Nothing we can do!
		return Vector3();
	}

	Point debugRaycastResult;

	bool Raycast(const Triangle& triangle, const Ray& ray, RaycastResult* outResult) {
		ResetRaycastResult(outResult);
		Plane plane = FromTriangle(triangle);

		RaycastResult planeResult;
		if (!Raycast(plane, ray, &planeResult)) {
			return false;
		}
		float t = planeResult.t;

		Point result = ray.origin + ray.direction * t;

		Vector3 barycentric = Barycentric(result, triangle);
		if (barycentric.x >= 0.0f && barycentric.x <= 1.0f &&
			barycentric.y >= 0.0f && barycentric.y <= 1.0f &&
			barycentric.z >= 0.0f && barycentric.z <= 1.0f) {

			if (outResult != 0) {
				outResult->t = t;
				outResult->hit = true;
				outResult->point = ray.origin + ray.direction * t;
				outResult->normal = plane.normal;
			}

			return true;
		}

		return false;
	}

	bool Linetest(const Triangle& triangle, const Line& line) {
		Ray ray;
		ray.origin = line.start;
		ray.direction = (line.end - line.start).GetNormalized();
		RaycastResult raycast;
		if (!Raycast(triangle, ray, &raycast)) {
			return false;
		}
		float t = raycast.t;

		return t >= 0 && t * t <= LengthSq(line);
	}




	bool TrianglePlane(const Triangle& t, const Plane& p) {
		float side1 = PlaneEquation(t.a, p);
		float side2 = PlaneEquation(t.b, p);
		float side3 = PlaneEquation(t.c, p);

		
		if (CMP(side1, 0) && CMP(side2, 0) && CMP(side3, 0)) {
			return true;
		}

		
		if (side1 > 0 && side2 > 0 && side3 > 0) {
			return false;
		}

		
		if (side1 < 0 && side2 < 0 && side3 < 0) {
			return false;
		}

		return true;
	}


	Point Intersection(Plane p1, Plane p2, Plane p3) {

#if 1
		Mat3 D(
			p1.normal.x, p2.normal.x, p3.normal.x,
			p1.normal.y, p2.normal.y, p3.normal.y,
			p1.normal.z, p2.normal.z, p3.normal.z
		);
		Vector3 A(-p1.distance, -p2.distance, -p3.distance);

		Mat3 Dx = D, Dy = D, Dz = D;
		Dx.m[0][0] = A.x; Dx.m[1][0] = A.y; Dx.m[2][0] = A.z;
		Dy.m[0][1] = A.x; Dy.m[1][1] = A.y; Dy.m[2][1] = A.z;
		Dz.m[0][2] = A.x; Dz.m[1][2] = A.y; Dz.m[2][2] = A.z;

		float detD = D.Det3();

		if (CMP(detD, 0)) {
			return Point();
		}

		float detDx = Dx.Det3();
		float detDy = Dy.Det3();
		float detDz = Dz.Det3();

		return Point(detDx / detD, detDy / detD, detDz / detD);
#else 
		Vector3 m1(p1.normal.x, p2.normal.x, p3.normal.x);
		Vector3 m2(p1.normal.y, p2.normal.y, p3.normal.y);
		Vector3 m3(p1.normal.z, p2.normal.z, p3.normal.z);
		Vector3 d(-p1.distance, -p2.distance, -p3.distance);

		Vector3 u = Cross(m2, m3);
		Vector3 v = Cross(m1, d);
		float denom = Dot(m1, u);
		if (CMP(denom, 0.0f)) {
			return Point();
		}
		Point result;
		result.x = Dot(d, u) / denom;
		result.y = Dot(m3, v) / denom;
		result.z = -Dot(m2, v) / denom;
		return result;
#endif
	}

	std::vector<Point> GetVertices(const OBB& obb) {
		std::vector<Vector3> v;
		v.resize(8);

		Vector3 C = obb.position;	// OBB Center
		Vector3 E = obb.size;		// OBB Extents
		const float* o = obb.orientation.array;
		Vector3 A[] = {			// OBB Axis
			Vector3(o[0], o[1], o[2]),
			Vector3(o[3], o[4], o[5]),
			Vector3(o[6], o[7], o[8]),
		};

		v[0] = C + A[0] * E[0] + A[1] * E[1] + A[2] * E[2];
		v[1] = C - A[0] * E[0] + A[1] * E[1] + A[2] * E[2];
		v[2] = C + A[0] * E[0] - A[1] * E[1] + A[2] * E[2];
		v[3] = C + A[0] * E[0] + A[1] * E[1] - A[2] * E[2];
		v[4] = C - A[0] * E[0] - A[1] * E[1] - A[2] * E[2];
		v[5] = C + A[0] * E[0] - A[1] * E[1] - A[2] * E[2];
		v[6] = C - A[0] * E[0] + A[1] * E[1] - A[2] * E[2];
		v[7] = C - A[0] * E[0] - A[1] * E[1] + A[2] * E[2];

		return v;
	}

	std::vector<Line> GetEdges(const OBB& obb) {
		std::vector<Line> result;
		result.reserve(12);
		std::vector<Point> v = GetVertices(obb);

		int index[][2] = { // Indices of edges
			{ 6, 1 },{ 6, 3 },{ 6, 4 },{ 2, 7 },{ 2, 5 },{ 2, 0 },
			{ 0, 1 },{ 0, 3 },{ 7, 1 },{ 7, 4 },{ 4, 5 },{ 5, 3 }
		};

		for (int j = 0; j < 12; ++j) {
			result.push_back(Line(
				v[index[j][0]], v[index[j][1]]
			));
		}

		return result;
	}

	std::vector<Plane>& GetPlanes(const OBB& obb) 
	{
		Vector3 c = obb.position;	// OBB Center
		Vector3 e = obb.size;		// OBB Extents
		const float* o = obb.orientation.array;
#if 0 //NOTE(o.luanda) : Row vectos 
		Vector3 a[] = {			// OBB Axis
			Vector3(o[0], o[1], o[2]),
			Vector3(o[3], o[4], o[5]),
			Vector3(o[6], o[7], o[8]),
		};
#endif
		//NOTE(o.luanda): Column vectos
		Vector3 a[] = {			// OBB Axis
			Vector3(o[0], o[3], o[6]),
			Vector3(o[1], o[4], o[7]),
			Vector3(o[2], o[5], o[8]),
		};
		std::vector<Plane> result;
		result.resize(6);

		result[0] = Plane(a[0],          a[0].Dot(c + a[0] * e.x));
		result[1] = Plane(a[0] * -1.0f, -a[0].Dot(c - a[0] * e.x));
		result[2] = Plane(a[1],          a[1].Dot(c + a[1] * e.y));
		result[3] = Plane(a[1] * -1.0f, -a[1].Dot(c - a[1] * e.y));
		result[4] = Plane(a[2],          a[2].Dot(c + a[2] * e.z));
		result[5] = Plane(a[2] * -1.0f, -a[2].Dot(c - a[2] * e.z));

		return result; // TODO(Warning): This returns a reference to a local variable.
	}


	bool ClipToPlane(const Plane& plane, const Line& line, Point* outPoint) {
		Vector3 ab = line.end - line.start;

		float nA = plane.normal.Dot(line.start);
		float nAB = plane.normal.Dot(ab);

		if (CMP(nAB, 0)) {
			return false;
		}

		float t = (plane.distance - nA) / nAB;
		if (t >= 0.0f && t <= 1.0f) {
			if (outPoint != 0) {
				*outPoint = line.start + ab * t;
			}
			return true;
		}

		return false;
	}

	std::vector<Point> ClipEdgesToOBB(const std::vector<Line>& edges, const OBB& obb) {
		std::vector<Point> result;
		result.reserve(edges.size() * 3);
		Point intersection;

		std::vector<Plane>& planes = GetPlanes(obb);

		for (int i = 0; i < planes.size(); ++i) {
			for (int j = 0; j < edges.size(); ++j) {
				if (ClipToPlane(planes[i], edges[j], &intersection)) {
					if (PointInOBB(intersection, obb)) {
						result.push_back(intersection);
					}
				}
			}
		}

		return result;
	}

	float PenetrationDepth(const OBB& o1, const OBB& o2, const Vector3& axis, bool* outShouldFlip) 
	{
		Interval i1 = GetInterval(o1, axis.GetNormalized());
		Interval i2 = GetInterval(o2, axis.GetNormalized());

		if (!((i2.min <= i1.max) && (i1.min <= i2.max))) {
			return 0.0f; // No penerattion
		}

		float len1 = i1.max - i1.min;
		float len2 = i2.max - i2.min;
		float min = fminf(i1.min, i2.min);
		float max = fmaxf(i1.max, i2.max);
		float length = max - min;

		if (outShouldFlip != 0) {
			*outShouldFlip = (i2.min < i1.min);
		}

		return (len1 + len2) - length;
	}




	CollisionManifold FindCollisionFeatures(const OBB& A, const OBB& B) 
	{
		CollisionManifold result; // Will return result of intersection!
		ResetCollisionManifold(&result);

		Sphere s1(A.position, A.size.GetLength());
		Sphere s2(B.position, B.size.GetLength());

		if (!SphereSphere(s1, s2)) {
			return result;
		}

		const float* o1 = A.orientation.array;
		const float* o2 = B.orientation.array;

		Vector3 test[15] = {
			Vector3(o1[0], o1[1], o1[2]),
			Vector3(o1[3], o1[4], o1[5]),
			Vector3(o1[6], o1[7], o1[8]),
			Vector3(o2[0], o2[1], o2[2]),
			Vector3(o2[3], o2[4], o2[5]),
			Vector3(o2[6], o2[7], o2[8])
		};

		for (int i = 0; i < 3; ++i) { // Fill out rest of axis
			test[6 + i * 3 + 0] = test[i].Cross(test[0]);
			test[6 + i * 3 + 1] = test[i].Cross(test[1]);
			test[6 + i * 3 + 2] = test[i].Cross(test[2]);
		}

		Vector3* hitNormal = 0;
		bool shouldFlip;

		for (int i = 0; i < 15; ++i) {
			if (test[i].x < 0.000001f) test[i].x = 0.0f;
			if (test[i].y < 0.000001f) test[i].y = 0.0f;
			if (test[i].z < 0.000001f) test[i].z = 0.0f;
			if (test[i].GetLengthSq() < 0.001f) {
				continue;
			}

			float depth = PenetrationDepth(A, B, test[i], &shouldFlip);
			if (depth <= 0.0f) {
				return result;
			}
			else if (depth < result.depth) {
				if (shouldFlip) {
					test[i] = test[i] * -1.0f;
				}
				result.depth = depth;
				hitNormal = &test[i];
			}
		}

		if (hitNormal == 0) 
		{
			return result;
		}
		Vector3 axis = (*hitNormal).GetNormalized();

		std::vector<Point> c1 = ClipEdgesToOBB(GetEdges(B), A);
		std::vector<Point> c2 = ClipEdgesToOBB(GetEdges(A), B);
		result.contacts.reserve(c1.size() + c2.size());
		result.contacts.insert(result.contacts.end(), c1.begin(), c1.end());
		result.contacts.insert(result.contacts.end(), c2.begin(), c2.end());

		Interval i = GetInterval(A, axis);
		float distance = (i.max - i.min) * 0.5f - result.depth * 0.5f;
		Vector3 pointOnPlane = A.position + axis * distance;

		for (int i = result.contacts.size() - 1; i >= 0; --i) {
			Vector3 contact = result.contacts[i];
			result.contacts[i] = contact + (axis * axis.Dot(pointOnPlane - contact));

			// This bit is in the "There is more" section of the book
			for (int j = result.contacts.size() - 1; j > i; --j) {
				if ((result.contacts[j] - result.contacts[i]).GetLengthSq() < 0.0001f) {
					result.contacts.erase(result.contacts.begin() + j);
					break;
				}
			}
		}

		result.colliding = true;
		result.normal = axis;

		return result;
	}

	CollisionManifold FindCollisionFeatures(const Sphere& A, const Sphere& B) {
		CollisionManifold result; // Will return result of intersection!
		ResetCollisionManifold(&result);

		float r = A.radius + B.radius;
		Vector3 d = B.position - A.position;

		if (d.GetLengthSq() - r * r > 0 || d.GetLengthSq() == 0.0f) {
			return result;
		}
		d.Normalize();

		result.colliding = true;
		result.normal = d;
		result.depth = fabsf(d.GetLength() - r) * 0.5f;

		// dtp - Distance to intersection point
		float dtp = A.radius - result.depth;
		Point contact = A.position + d * dtp;

		result.contacts.push_back(contact);

		return result;
	}

	CollisionManifold FindCollisionFeatures(const OBB& A, const Sphere& B) {
		CollisionManifold result; // Will return result of intersection!
		ResetCollisionManifold(&result);

		Point closestPoint = ClosestPoint(A, B.position);

		float distanceSq = (closestPoint - B.position).GetLengthSq();
		if (distanceSq > B.radius * B.radius) {
			return result;
		}

		Vector3 normal;
		if (CMP(distanceSq, 0.0f)) {
			if (CMP((closestPoint - A.position).GetLengthSq(), 0.0f)) {
				return result;

			}
			// Closest point is at the center of the sphere
			normal = (closestPoint - A.position).GetNormalized();
		}
		else {
			normal = (B.position - closestPoint).GetNormalized();
		}

		Point outsidePoint = B.position - normal * B.radius;

		float distance = (closestPoint - outsidePoint).GetLength();

		result.colliding = true;
		result.contacts.push_back(closestPoint + (outsidePoint - closestPoint) * 0.5f);
		result.normal = normal;
		result.depth = distance * 0.5f;

		return result;
	}

	void ResetCollisionManifold(CollisionManifold* result) 
	{
		if (result != 0) 
		{
			result->colliding = false;
			result->normal = Vector3(0, 0, 1);
			result->depth = FLT_MAX;
			if (result->contacts.size() > 0) 
			{
				result->contacts.clear();
			}
		}
	}
}
