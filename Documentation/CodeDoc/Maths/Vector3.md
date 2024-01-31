# Vector3: Point in 3D space

## Attributes

```cpp
float x, y, z
```

Vector components.

<br>

## Constructors

```cpp
Vector3()
```

Null vector.

<br>

```cpp
Vector3(const float& all)
```

Vector with equal coordinates.

<br>

```cpp
Vector3(const float& _x, const float& _y, const float& _z)
```

Vector with 3 coordinates.

<br>

```cpp
Vector3(const Vector3& p1, const Vector3& p2)
```

Vector from 2 points.

<br>

```cpp
Vector3(const Vector3& angles, const float& length)
```

Vector from angles and length.

<br>

## Operators

```cpp
float Dot(const Vector3& v) const
```

Computes dot product with another vector.

<br>

```cpp
Vector3 Cross(const Vector3& v) const
```

Computes cross product with another vector.

<br>

## Length

```cpp
float GetLength() const
```

Returns the vector's length.

<br>

```cpp
void SetLength(const float& length)
```

Modifies the vector's length to correspond to the given value.

<br>

## Normalization

```cpp
void Normalize()
```

Normalizes the given vector so that its length is 1.

<br>

```cpp
Vector3 GetNormalized() const
```

Returns a normalized copy of the vector.

<br>

## Negation

```cpp
void Negate()
```

Negates the vector's coordinates.

<br>

```cpp
Vector3 GetNegated() const
```

Returns a negated copy of the vector.

<br>

## Copy signs

```cpp
void CopySign(const Vector3& source)
```

Copies the signs from the source vector to this vector.

<br>

```cpp
Vector3 GetCopiedSign(const Vector3& source) const
```

Returns a copy of this vector with the given vector's signs.

<br>

## Miscellaneous

```cpp
float GetDistanceFromPoint(const Vector3& p) const
```

Interprets the vector as a point and returns the distance to another point.

<br>

```cpp
static Vector3 Lerp(const Vector3& start, const Vector3& dest, const float& val)
```

Linearly interpolates from start to dest.

<br>

## Conversions

```cpp
Vector4 ToVector4() const
```

Creates a Vector4 from this vector.

<br>

```cpp
std::string ToString(const int& precision = 2) const
```

Returns the vector's contents as a string.

<br>

## Non-member functions

```cpp
Vector3 GetSphericalCoords(const float& r, const float& pitch, const float& yaw)
```

Returns the coordinates of a point on a sphere of radius r, using the given angles.

<br>

