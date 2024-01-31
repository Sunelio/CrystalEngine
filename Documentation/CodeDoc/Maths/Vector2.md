# Vector2: Point in 2D space

## Attributes

```cpp
float x, y
```

Vector components.

<br>

## Constructors

```cpp
Vector2()
```

Null vector.

<br>

```cpp
Vector2(const float& _x, const float& _y)
```

Vector with 2 coordinates.

<br>

```cpp
Vector2(const Vector2& p1, const Vector2& p2)
```

Vector from 2 points.

<br>

```cpp
Vector2(const float& rad, const float& length, const bool& isAngle)
```

Vector from angle (bool is only used to differenciate with the (_x, _y) constructor).

<br>

## Operators

```cpp
float Dot(const Vector2& v) const
```

Computes dot product with another vector.

<br>

```cpp
float Cross(const Vector2& v) const
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

Normalizes the vector so that its length is 1.

<br>

```cpp
Vector2 GetNormalized() const
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
Vector2 GetNegated() const
```

Returns a negated copy of the vector.

<br>

## Copy signs

```cpp
void CopySign(const Vector2& source)
```

Copies the signs from the given vector to this vector.

<br>

```cpp
Vector2 GetCopiedSign(const Vector2& source) const
```

Returns a copy of this vector with the given vector's signs.

<br>

## Angles

```cpp
float GetAngle() const
```

Returns the angle (in radians) of the vector with the horizontal axis.

<br>

```cpp
float GetAngleWithVector2(const Vector2& v) const
```

Returns the angle (in radians) between this vector and the given one.

<br>

## Rotation

```cpp
void Rotate(const float& angle)
```

Rotates the vector by the given angle (in rad).

<br>

```cpp
void RotateAsPoint(const Vector2& pivot, const float& angle)
```

Rotates the point around the given pivot point by the given angle (in rad).

<br>

## Miscellaneous

```cpp
Vector2 GetNormal() const
```

Returns the normal the vector.

<br>

```cpp
float GetDistanceFromPoint(const Vector2& p) const
```

Interprets the vector as a point and returns the distance to another point.

<br>

```cpp
static Vector2 Lerp(const Vector2& start, const Vector2& dest, const float& val)
```

Calculates linear interpolation for a value from a start point to an end point.

<br>

## Conversion

```cpp
std::string ToString(const int& precision = 2) const
```

Returns the vector's contents as a string.

<br>

