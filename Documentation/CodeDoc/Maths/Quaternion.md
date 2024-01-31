# Quaternion: used for rotation in 4 dimensions

## Attributes

```cpp
float w, x, y, z
```

Quaternion components.

<br>

## Constructors

```cpp
Quaternion()
```

Null quaternion.

<br>

```cpp
Quaternion(const float& all)
```

Quaternion with equal values.

<br>

```cpp
Quaternion(const float& w, const float& x, const float& y, const float& z)
```

Quaternion with 4 values.

<br>

```cpp
Quaternion(const Vector3& eulerAngles)
```

Quaternion from euler angles (pitch, roll, yaw).

<br>

```cpp
Quaternion(const AngleAxis& angleAxis)
```

Quaternion from angle axis.

<br>

```cpp
Quaternion(const Mat4& matrix)
```

Quaternion from 4x4 matrix.

<br>

## Static constructors

```cpp
static Quaternion FromPitch(const float& angle)
```

Quaternion that rotates around the X axis by the given angle.

<br>

```cpp
static Quaternion FromRoll(const float& angle)
```

Quaternion that rotates around the Y axis by the given angle.

<br>

```cpp
static Quaternion FromYaw(const float& angle)
```

Quaternion that rotates around the Z axis by the given angle.

<br>

```cpp
static Quaternion FromEuler(const Vector3& angles)
```

Quaternion from euler angles (pitch, roll, yaw).

<br>

```cpp
static Quaternion FromAngleAxis(const AngleAxis& angleAxis)
```

Quaternion from angle axis.

<br>

```cpp
static Quaternion FromMatrix(const Mat4& matrix)
```

Quaternion from 4x4 matrix.

<br>

## Operators

```cpp
float Dot(const Quaternion& q) const
```

Computes dot product with another quaternion.

<br>

## Modulus

```cpp
float GetModulus() const
```

Returns the absolute value of the quaternion.

<br>

## Normalization

```cpp
void Normalize()
```

Normalizes this quaternion so that its modulus is 1.

<br>

```cpp
Quaternion GetNormalized() const
```

Returns a normalized copy of the quaternion.

<br>

## Conjugation

```cpp
void Conjugate()
```

Conjugates the quaternion by inverting the sign of its x, y and z values.

<br>

```cpp
Quaternion GetConjugate() const
```

Returns the conjugate of the quaternion.

<br>

## Inversion

```cpp
void Inverse()
```

Inverses the quaternion.

<br>

```cpp
Quaternion GetInverse() const
```

Returns the inverse of the quaternion.

<br>

## Rotation

```cpp
Quaternion RotateQuat(const Quaternion& q) const
```

Applies this quaternion's rotation to the given quaternion and returns it.

<br>

```cpp
Vector3 RotateVec(const Vector3& v) const
```

Applies this quaternion's rotation to the given 3D vector and returns it.

<br>

## Interpolation

```cpp
static Quaternion Lerp(const Quaternion& start, const Quaternion& dest, const float& val)
```

Interpolates between start and dest without normalizing the output.

<br>

```cpp
static Quaternion NLerp(const Quaternion& start, const Quaternion& dest, const float& val)
```

Normalized interpolation of start and dest.

<br>

```cpp
static Quaternion SLerp(const Quaternion& start, const Quaternion& dest, const float& val, const bool& useShortestPath = true)
```

Spherical interpolation of start and dest.

<br>

## Conversions

```cpp
AngleAxis ToAngleAxis() const
```

Returns the angle-axis rotation that corresponds to this quaternion.

<br>

```cpp
Mat4 ToMatrix() const
```

Returns the rotation matrix that corresponds to this quaternion.

<br>

```cpp
Vector3 ToEuler() const
```

Returns the euler angles that correspond to this quaternion.

<br>

```cpp
std::string ToString(const int& precision = 2) const
```

Returns the quaternion's contents as a string.

<br>

