# Transform: position, rotation and scale in 3D space

**By default, this is used to transform objects. To transform cameras, set isCamera to true.**

## Constructors

```cpp
Transform(const bool& _isCamera = false)
```

Transform with default values and optional parameter to set transform type (camera or object).

<br>

```cpp
Transform(const Vector3& position, const Quaternion& rotation, const Vector3& _scale, const bool& _isCamera = false)
```

Transform from position, rotation and scale and optional parameter to set transform type (camera or object).

<br>

## Position

```cpp
Vector3 GetPosition() const
```

Returns the transform's position.

<br>

```cpp
void SetPosition(const Vector3& position)
```

Modifies the transform's position.

<br>

```cpp
void Move(const Vector3& movement)
```

Adds the given vector to the transform's position.

<br>

## Direction vectors

```cpp
Vector3 Forward() const
```

Returns the transform's forward vector.

<br>

```cpp
Vector3 Up() const
```

Returns the transform's up vector.

<br>

```cpp
Vector3 Right() const
```

Returns the transform's right vector.

<br>

```cpp
void SetForward(const Vector3& forward)
```

Modifies the transform's rotation to point in the given direction.

<br>

## Rotation

```cpp
Quaternion GetRotation() const
```

Returns the transform's rotation.

<br>

```cpp
void SetRotation(const Quaternion& rotation)
```

Modified the transform's rotation.

<br>

```cpp
void Rotate(const Quaternion& rotation)
```

Rotates the transform by the given quaternion.

<br>

## Scale

```cpp
Vector3 GetScale() const
```

Returns the transform's scale.

<br>

```cpp
Vector3 GetUniformScale() const
```

Returns the maximum value between the x, y and z elements of the transform's scale.

<br>

```cpp
void SetScale(const Vector3& _scale)
```

Modifies the transform's scale.

<br>

## Is Camera

```cpp
bool IsCamera() const
```

Returns true if the transform is a camera transform, false if it is an object transform.

<br>

```cpp
void SetIsCamera(const bool& _isCamera)
```

Modifies the transform's type (object transform or camera transform).

<br>

## Setting multiple values at a time

```cpp
void SetPosRot(const Vector3& position, const Quaternion& rotation)
```

Modify the transform's position and rotation.

<br>

```cpp
void SetValues(const Vector3& position, const Quaternion& rotation, const Vector3& _scale)
```

Modify all of the transform's values.

<br>

## Matrices

```cpp
Mat4 GetLocalMat() const
```

Returns the transform's local matrix.

<br>

```cpp
Mat4 GetParentMat() const
```

Returns the transform's parent matrix.

<br>

```cpp
Mat4 GetWorldMat() const
```

Returns the transform's world matrix (local * parent).

<br>

```cpp
void SetParentMat(const Mat4& mat)
```

Modifies the transform's parent matrix.

<br>

## Interpolation

```cpp
static Transform Lerp(const Transform& start, const Transform& dest, const float& val, const bool& useSlerp = true)
```

Linearly interpolates between start and dest.

<br>

