# AngleAxis: angle and axis of rotation

## Attributes

```cpp
float angle
```

```cpp
Vector3 axis
```

## Constructors

```cpp
AngleAxis()
```

Default constructor.

<br>

```cpp
AngleAxis(const float& _angle, const Vector3& _axis)
```

Angle-axis from angle and vector.

<br>

```cpp
AngleAxis(const Quaternion& quaternion)
```

Angle-axis from quaternion.

<br>

```cpp
AngleAxis(const Mat4& matrix)
```

Angle-axis from rotation matrix.

<br>

## Static constructors

```cpp
static AngleAxis FromQuaternion(const Quaternion& quaternion)
```

Angle-axis from quaternion.

<br>

```cpp
static AngleAxis FromMatrix(const Mat4& matrix)
```

Angle-axis from rotation matrix.

<br>

## Conversions

```cpp
Quaternion ToQuaternion() const
```

Returns the quaternion that corresponds to this angle-axis rotation.

<br>

```cpp
Mat4 ToMatrix() const
```

Returns the rotation matrix that corresponds to this angle-axis rotation.

<br>

```cpp
std::string ToString(const int& precision = 2) const
```

Returns the contents of the angle-axis rotation as a string.

<br>

