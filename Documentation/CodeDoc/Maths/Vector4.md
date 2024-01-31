# Vector4: Point in 3D space with homogeneous coordinates

## Attributes

```cpp
float x, y, z, w
```

Vector components.

<br>

## Constructors

```cpp
Vector4()
```

Null vector.

<br>

```cpp
Vector4(const float& _x, const float& _y, const float& _z, const float& _w)
```

Vector with 4 coordinates.

<br>

```cpp
Vector4(const Vector3& vec, const float& _w)
```

Vector4 from Vector3.

<br>

## Operators

```cpp
Vector4 operator*(const Mat4& val) const
```

Multiplication with 4x4 matrix.

<br>

## Homogenization

```cpp
void Homogenize()
```

Homogenizes the vector by dividing its elements by w.

<br>

```cpp
Vector4 GetHomogenized() const
```

Returns a homogenized copy of the vector.

<br>

## Conversions

```cpp
Vector3 ToVector3(const bool& homogenizeVec = false) const
```

Creates a Vector3 from this vector.

<br>

```cpp
std::string ToString(const int& precision = 2) const
```

Returns the vector's contents as a string.

<br>

