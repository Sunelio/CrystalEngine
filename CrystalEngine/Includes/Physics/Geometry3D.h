#pragma once
#include "Maths/Vector3.h"
#include "Maths/Matrix.h"
#include <vector>

#pragma warning(disable : 26495)

namespace Maths
{
	class Vector3;
	class Quaternion;
}

typedef Maths::Vector3 Point;


namespace Geometry3D
{
	struct Line
	{
		Point start;
		Point end;

		inline Line() {}
		inline Line(const Point& s, const Point e): start(s) , end(e) {}

		
	};

	struct Ray
	{
		Point origin;
		Maths::Vector3 direction;

		inline Ray() : direction(0.0f, 0.0f, 1.0f) {}
		inline Ray(const Point& o, const Maths::Vector3 dir) :origin(o) ,direction(dir) 
		{
			direction.Normalize();
		}
	};

	struct Sphere
	{
		Point position;
		float radius;

		inline Sphere() : radius(1.0f) {}
		inline Sphere(const Point& c, float r) : position(c),radius(r) {}
	};

	struct AABB
	{
		Point position;
		Maths::Vector3 size; //Half size;
		inline AABB() : size(1.0f, 1.0f, 1.0f) {}
		inline AABB(const Point& p, const Maths::Vector3 s): position(p),size(s) {}
	};

	struct OBB
	{
		Point position;
		Maths::Vector3 size; //Half size;
		//Maths::Vector3 offset; 
		Maths::Mat3 orientation;

		inline OBB() : size(1.0f, 1.0f, 1.0f) {}
		inline OBB(const Point& p, const Maths::Vector3 s) : position(p), size(s) {}
		inline OBB(const Point& p, const Maths::Vector3 s, const Maths::Mat3& o) 
			: position(p), size(s), orientation(o)
		{}
	};



	struct Plane
	{
		Maths::Vector3 normal;
		float distance;

		inline Plane() : normal(1, 0, 0) {}
		inline Plane(const Maths::Vector3& n, float d)
			: normal(n), distance(d) {}

	};



#if 0//TODO (o.luanda)
	struct Capsule
	{
		Line segment;
		float radius;

	};
#endif
	struct Triangle
	{
		union
		{
			struct
			{
				Point a;
				Point b;
				Point c;
			};

			struct
			{
				Point p1;
				Point p2;
				Point p3;
			};

			Point points[3];
			float values[9];
		};
	};

	struct RaycastResult
	{
		Maths::Vector3 point;
		Maths::Vector3 normal;
		float t;
		bool hit;
	};

	struct Interval
	{
		float min;
		float max;
	};

	void ResetRaycastResult(RaycastResult* outResult);
	Point Intersection(Plane p1, Plane p2, Plane p3);

	std::ostream& operator<<(std::ostream& os, const Line& shape);
	std::ostream& operator<<(std::ostream& os, const Ray& shape);
	std::ostream& operator<<(std::ostream& os, const Sphere& shape);
	std::ostream& operator<<(std::ostream& os, const AABB& shape);
	std::ostream& operator<<(std::ostream& os, const OBB& shape);
	std::ostream& operator<<(std::ostream& os, const Plane& shape);
	std::ostream& operator<<(std::ostream& os, const Triangle& shape);

	float Length(const Line& line);
	float LengthSq(const Line& line);
	Ray FromPoints(const Point& from, const Point& to);
	Maths::Vector3 GetMin(const AABB& aabb);
	Maths::Vector3 GetMax(const AABB& aabb);
	AABB FromMinMax(const Maths::Vector3& min, const Maths::Vector3& max);
	float PlaneEquation(const Point& point, const Plane& plane);
	float PlaneEquation(const Plane& plane, const Point& point);

	bool PointInSphere(const Point& point, const Sphere& sphere);
	bool PointInAABB(const Point& point, const AABB& aabb);
	bool PointInOBB(const Point& point, const OBB& obb);
	bool PointOnPlane(const Point& point, const Plane& plane);
	bool PointOnLine(const Point& point, const Line& line);
	bool PointOnRay(const Point& point, const Ray& ray);

	bool PointInPlane(const Point& point, const Plane& plane);
	bool PointInLine(const Point& point, const Line& line);
	bool PointInRay(const Point& point, const Ray& ray);
	bool ContainsPoint(const Sphere& sphere, const Point& point);
	bool ContainsPoint(const Point& point, const Sphere& sphere);
	bool ContainsPoint(const AABB& aabb, const Point& point);
	bool ContainsPoint(const Point& point, const AABB& aabb);
	bool ContainsPoint(const Point& point, const OBB& obb);
	bool ContainsPoint(const OBB& obb, const Point& point);
	bool ContainsPoint(const Point& point, const Plane& plane);
	bool ContainsPoint(const Plane& plane, const Point& point);
	bool ContainsPoint(const Point& point, const Line& line);
	bool ContainsPoint(const Line& line, const Point& point);
	bool ContainsPoint(const Point& point, const Ray& ray);
	bool ContainsPoint(const Ray& ray, const Point& point);

	Point ClosestPoint(const Sphere& sphere, const Point& point);
	Point ClosestPoint(const AABB& aabb, const Point& point);
	Point ClosestPoint(const OBB& obb, const Point& point);
	Point ClosestPoint(const Plane& plane, const Point& point);
	Point ClosestPoint(const Line& line, const Point& point);
	Point ClosestPoint(const Ray& ray, const Point& point);

	Point ClosestPoint(const Point& point, const Sphere& sphere);
	Point ClosestPoint(const Point& point, const AABB& aabb);
	Point ClosestPoint(const Point& point, const OBB& obb);
	Point ClosestPoint(const Point& point, const Plane& plane);
	Point ClosestPoint(const Point& point, const Line& line);
	Point ClosestPoint(const Point& point, const Ray& ray);
	Point ClosestPoint(const Point& p, const Triangle& t);

	Interval GetInterval(const AABB& aabb, const Maths::Vector3& axis);
	Interval GetInterval(const OBB& obb, const Maths::Vector3& axis);
	Interval GetInterval(const Triangle& triangle, const Maths::Vector3& axis);

	bool OverlapOnAxis(const AABB& aabb, const OBB& obb, const Maths::Vector3& axis);
	bool OverlapOnAxis(const OBB& obb1, const OBB& obb2, const Maths::Vector3& axis);
	bool OverlapOnAxis(const AABB& aabb, const Triangle& triangle, const Maths::Vector3& axis);
	bool OverlapOnAxis(const OBB& obb, const Triangle& triangle, const Maths::Vector3& axis);
	bool OverlapOnAxis(const Triangle& t1, const Triangle& t2, const Maths::Vector3& axis);

	bool SphereSphere(const Sphere& s1, const Sphere& s2);
	bool SphereAABB(const Sphere& sphere, const AABB& aabb);
	bool SphereOBB(const Sphere& sphere, const OBB& obb);
	bool SpherePlane(const Sphere& sphere, const Plane& plane);
	bool AABBAABB(const AABB& aabb1, const AABB& aabb2);
	bool AABBOBB(const AABB& aabb, const OBB& obb);
	bool AABBPlane(const AABB& aabb, const Plane& plane);
	bool OBBOBB(const OBB& obb1, const OBB& obb2);
	bool OBBPlane(const OBB& obb, const Plane& plane);
	bool PlanePlane(const Plane& plane1, const Plane& plane2);

#define AABBSphere(aabb, sphere) \
	SphereAABB(Sphere, AABB)
#define OBBSphere(obb, sphere) \
	SphereOBB(sphere, obb)
#define PlaneSphere(plane, sphere) \
	SpherePlane(sphere, plane)
#define OBBAABB(obb, aabb) \
	AABBOBB(aabb, obb)
#define PlaneAABB(plane, aabb) \
	AABBPlane(aabb, plane)
#define PlaneOBB(plane, obb) \
	OBBPlane(obb, plane)



	bool Raycast(const Sphere& sphere, const Ray& ray, RaycastResult* outResult);
	bool Raycast(const AABB& aabb, const Ray& ray, RaycastResult* outResult);
	bool Raycast(const OBB& obb, const Ray& ray, RaycastResult* outResult);
	bool Raycast(const Plane& plane, const Ray& ray, RaycastResult* outResult);
	bool Raycast(const Triangle& triangle, const Ray& ray, RaycastResult* outResult);

	bool Linetest(const Sphere& sphere, const Line& line);
	bool Linetest(const AABB& aabb, const Line& line);
	bool Linetest(const OBB& obb, const Line& line);
	bool Linetest(const Plane& plane, const Line& line);
	bool Linetest(const Triangle& triangle, const Line& line);

	bool Raycast(const Ray& ray, const Sphere& sphere, RaycastResult* outResult);
	bool Raycast(const Ray& ray, const AABB& aabb, RaycastResult* outResult);
	bool Raycast(const Ray& ray, const OBB& obb, RaycastResult* outResult);
	bool Raycast(const Ray& ray, const Plane& plane, RaycastResult* outResult);
	bool Linetest(const Line& line, const Sphere& sphere);
	bool Linetest(const Line& line, const AABB& aabb);
	bool Linetest(const Line& line, const OBB& obb);
	bool Linetest(const Line& line, const Plane& plane);

	Maths::Vector3 Centroid(const Triangle& t);

	bool PointInTriangle(const Point& p, const Triangle& t);
	Plane FromTriangle(const Triangle& t);
	Point ClosestPoint(const Triangle& t, const Point& p);
	bool TriangleSphere(const Triangle& t, const Sphere& s);
	bool TriangleAABB(const Triangle& t, const AABB& a);
	bool TriangleOBB(const Triangle& t, const OBB& o);
	bool TriangleTriangle(const Triangle& t1, const Triangle& t2);
	bool TriangleTriangleRobust(const Triangle& t1, const Triangle& t2);
	bool TrianglePlane(const Triangle& t, const Plane& p);

#define SphereTriangle(s, t) \
	TriangleSphere(t, s)
#define AABBTriangle(a, t) \
	TriangleAABB(t, a)
#define OBBTriangle(o, t) \
	TriangleOBB(t, o)
#define PlaneTriangle(p, t) \
	TrianglePlane(t, p)

	Maths::Vector3 SatCrossEdge(const Maths::Vector3& a, const Maths::Vector3& b, const Maths::Vector3& c, const Maths::Vector3& d);
	Maths::Vector3 Barycentric(const Point& p, const Triangle& t);


	typedef struct CollisionManifold
	{
		bool colliding;
		Maths::Vector3 normal;
		float depth;
		std::vector<Maths::Vector3> contacts;
	} CollisionManifold;

	void ResetCollisionManifold(CollisionManifold* result);

	CollisionManifold FindCollisionFeatures(const Sphere& A, const Sphere& B);
	CollisionManifold FindCollisionFeatures(const OBB& A, const Sphere& B);
	CollisionManifold FindCollisionFeatures(const OBB& A, const OBB& B);


	std::vector<Point> GetVertices(const OBB& obb);
	std::vector<Line> GetEdges(const OBB& obb);
	std::vector<Plane>& GetPlanes(const OBB& obb);
	bool ClipToPlane(const Plane& plane, const Line& line, Point* outPoint);
	std::vector<Point> ClipEdgesToOBB(const std::vector<Line>& edges, const OBB& obb);
	float PenetrationDepth(const OBB& o1, const OBB& o2, const Maths::Vector3& axis, bool* outShouldFlip);




}

typedef Maths::Vector3 Point3D;
typedef Geometry3D::Ray Ray3D;
typedef Geometry3D::AABB Rectangle3D;
typedef Geometry3D::Interval Interval3D;



