# Geometry3D

## Methods

```cpp
void ResetRaycastResult(RaycastResult* outResult)
```

<br>

```cpp
Point Intersection(Plane p1, Plane p2, Plane p3)
```

<br>

```cpp
bool PointInSphere(const Point& point, const Sphere& sphere)	
```

<br>

```cpp
bool PointInAABB(const Point& point, const AABB& aabb)
```

<br>

```cpp
bool PointInOBB(const Point& point, const OBB& obb)
```

<br>

```cpp
bool PointOnPlane(const Point& point, const Plane& plane)
```


<br>

```cpp
bool PointOnLine(const Point& point, const Line& line)
```

<br>

```cpp
bool PointOnRay(const Point& point, const Ray& ray)
```

<br>

```cpp
bool OverlapOnAxis(const AABB& aabb, const OBB& obb, const Maths::Vector3& axis)
```

<br>

```cpp
bool OverlapOnAxis(const OBB& obb1, const OBB& obb2, const Maths::Vector3& axis)
```

<br>

```cpp
bool OverlapOnAxis(const AABB& aabb, const Triangle& triangle, const Maths::Vector3& axis)
```

<br>

```cpp
bool OverlapOnAxis(const OBB& obb, const Triangle& triangle, const Maths::Vector3& axis)
```

<br>

```cpp
bool OverlapOnAxis(const Triangle& t1, const Triangle& t2, const Maths::Vector3& axis)
```

<br>

```cpp
bool SphereSphere(const Sphere& s1, const Sphere& s2)
```


<br>

```cpp
bool SphereAABB(const Sphere& sphere, const AABB& aabb)
```


<br>

```cpp
bool SphereOBB(const Sphere& sphere, const OBB& obb)
```


<br>

```cpp
bool SpherePlane(const Sphere& sphere, const Plane& plane)
```


<br>

```cpp
bool AABBAABB(const AABB& aabb1, const AABB& aabb2)
```

<br>

```cpp
bool AABBOBB(const AABB& aabb, const OBB& obb)
```


<br>

```cpp
bool AABBPlane(const AABB& aabb, const Plane& plane)
```


<br>

```cpp
bool OBBOBB(const OBB& obb1, const OBB& obb2)
```

<br>

```cpp
bool OBBPlane(const OBB& obb, const Plane& plane)
```


<br>

```cpp
bool PlanePlane(const Plane& plane1, const Plane& plane2)
```

<br>

```cpp
bool Raycast(const Sphere& sphere, const Ray& ray, RaycastResult* outResult)
```

<br>

```cpp
bool Raycast(const AABB& aabb, const Ray& ray, RaycastResult* outResult)
```

<br>

```cpp
bool Raycast(const OBB& obb, const Ray& ray, RaycastResult* outResult)
```

<br>

```cpp
bool Raycast(const Plane& plane, const Ray& ray, RaycastResult* outResult)
```

<br>

```cpp
bool Raycast(const Triangle& triangle, const Ray& ray, RaycastResult* outResult)
```

<br>

```cpp
bool Linetest(const Sphere& sphere, const Line& line)
```

<br>

```cpp
bool Linetest(const AABB& aabb, const Line& line)
```

<br>

```cpp
bool Linetest(const OBB& obb, const Line& line)
```

<br>

```cpp
bool Linetest(const Plane& plane, const Line& line)
```

<br>

```cpp

```

<br>

```cpp
bool Linetest(const Triangle& triangle, const Line& line)
```

<br>

```cpp
bool Raycast(const Ray& ray, const Sphere& sphere, RaycastResult* outResult)
```

<br>

```cpp
bool Raycast(const Ray& ray, const AABB& aabb, RaycastResult* outResult)
```

<br>

```cpp
bool Raycast(const Ray& ray, const OBB& obb, RaycastResult* outResult)
```

<br>

```cpp
bool Raycast(const Ray& ray, const Plane& plane, RaycastResult* outResult)
```

<br>

```cpp
bool Linetest(const Line& line, const Sphere& sphere)
```

<br>

```cpp
bool Linetest(const Line& line, const AABB& aabb)
```

<br>

```cpp
bool Linetest(const Line& line, const OBB& obb)
```

<br>

```cpp
bool Linetest(const Line& line, const Plane& plane)
```

<br>

```cpp
bool PointInTriangle(const Point& p, const Triangle& t)
```

<br>

```cpp
Plane FromTriangle(const Triangle& t)
```

<br>

```cpp
Point ClosestPoint(const Triangle& t, const Point& p)
```

<br>

```cpp
bool TriangleSphere(const Triangle& t, const Sphere& s)
```

<br>

```cpp
bool TriangleAABB(const Triangle& t, const AABB& a)
```

<br>

```cpp
bool TriangleOBB(const Triangle& t, const OBB& o)
```

<br>

```cpp
bool TriangleTriangle(const Triangle& t1, const Triangle& t2)
```

<br>

```cpp
bool TriangleTriangleRobust(const Triangle& t1, const Triangle& t2)
```

<br>

```cpp
bool TrianglePlane(const Triangle& t, const Plane& p)
```

<br>

```cpp
void ResetCollisionManifold(CollisionManifold* result)
```

<br>

```cpp
CollisionManifold FindCollisionFeatures(const Sphere& A, const Sphere& B)
```

<br>

```cpp
CollisionManifold FindCollisionFeatures(const OBB& A, const Sphere& B)
```

<br>

```cpp
CollisionManifold FindCollisionFeatures(const OBB& A, const OBB& B)
```


