# Matrix<R, C>: a matrix with R rows and C columns

## Shortcuts for square matrices

```cpp
typedef Matrix<2, 2> Mat2
```

2x2 matrix.

<br>

```cpp
typedef Matrix<3, 3> Mat3
```

3x3 matrix.

<br>

```cpp
typedef Matrix<4, 4> Mat4
```

4x4 matrix.

<br>

## Constructors

```cpp
Matrix(const bool& identity = true)
```

Default constructor.

<br>

```cpp
Matrix(const Matrix& matrix)
```

Copy operator.

<br>

```cpp
Matrix(const float matrix[R][C])
```

Matrix from float 2D array.

<br>

```cpp
Matrix(const Vector3& eulerAngles)
```

Matrix from euler angles (pitch, roll, yaw).

<br>

```cpp
Matrix(const AngleAxis& angleAxis)
```

Matrix from angle-axis.

<br>

```cpp
Matrix(const Quaternion& quaternion)
```

Matrix from quaternion.

<br>

```cpp
Matrix(const float& m00, const float& m01,
       const float& m10, const float& m11)
```

Matrix 2x2 constructor.

<br>

```cpp
Matrix(const float& m00, const float& m01, const float& m02,
       const float& m10, const float& m11, const float& m12,
       const float& m20, const float& m21, const float& m22)
```

Matrix 3x3 constructor.

<br>

```cpp
Matrix(const float& m00, const float& m01, const float& m02, const float& m03,
       const float& m10, const float& m11, const float& m12, const float& m13,
       const float& m20, const float& m21, const float& m22, const float& m23,
       const float& m30, const float& m31, const float& m32, const float& m33)
```

Matrix 4x4 constructor.

<br>

```cpp
Matrix(const Mat2& a, const Mat2& b, const Mat2& c, const Mat2& d)
```

Matrix 4x4 constructor from 2x2 matrices.

<br>

## Static constructors

```cpp
static Matrix FromTranslation(const Vector3& translation)
```

Matrix from translation.

<br>

```cpp
static Matrix FromPitch(const float& angle)
```

Matrix that rotates around the X axis by the given angle.

<br>

```cpp
static Matrix FromRoll(const float& angle)
```

Matrix that rotates around the Y axis by the given angle.

<br>

```cpp
static Matrix FromYaw(const float& angle)
```

Matrix that rotates around the Z axis by the given angle.

<br>

```cpp
static Matrix FromEuler(const Vector3& angles)
```

Matrix from euler angles (pitch, roll, yaw).

<br>

```cpp
static Matrix FromAngleAxis(const AngleAxis& angleAxis)
```

Matrix from angle-axis.

<br>

```cpp
static Matrix FromQuaternion(const Quaternion& quaternion)
```

Matrix from quaternion.

<br>

```cpp
static Matrix FromScale(const Vector3& scale)
```

Matrix from scale.

<br>

```cpp
static Matrix FromTransform(const Vector3& pos, const Quaternion& rot, const Vector3& scale, const bool& reverse = false)
```

Matrix from transform.

<br>

## Operators

**Matrix bracket operators**

```cpp
const float* operator[](int index) const
```

```cpp
float* operator[](int index)
```

**Matrix copy**

```cpp
Matrix& operator=(const Matrix& matrix)
```

```cpp
Matrix& operator=(float** matrix)
```

**Matrix addition**

```cpp
Matrix operator+(const float& val) const
```

```cpp
Matrix operator+(const Matrix& matrix) const
```

**Matrix negation and subtraction**

```cpp
Matrix operator-() const
```

```cpp
Matrix operator-(const float& val) const
```

```cpp
Matrix operator-(const Matrix& matrix) const
```

**Matrix multiplication**

```cpp
Matrix operator*(const float& val) const
```

```cpp
template<int R2, int C2> Matrix<(R > R2 ? R : R2),(C > C2 ? C : C2)> operator*(const Matrix<R2, C2>& matrix) const
```

**Matrix division by a scalar**

```cpp
Matrix operator/(const float& val) const
```

**Matrix addition assignment**

```cpp
void operator+=(const float& val)
```

```cpp
void operator+=(const Matrix& matrix)
```

**Matrix subtraction assignment**

```cpp
void operator-=(const float& val)
```

```cpp
void operator-=(const Matrix& matrix)
```

**Matrix multiplication assignment**

```cpp
void operator*=(const float& val)
```

```cpp
template<int R2, int C2> void operator*=(const Matrix<R2, C2>& matrix)
```

**Matrix power**

```cpp
Matrix operator^(const float& n) const
```

```cpp
Matrix Pow(const float& n) const
```

## Getters

```cpp
const float* AsPtr() const
```

Returns a constant pointer to the matrix's float array.

<br>

```cpp
float* AsPtr()
```

Returns a pointer to the matrix's float array.

<br>

```cpp
int GetRows() const
```

Returns the matrix's number of rows.

<br>

```cpp
int GetColumns() const
```

Returns the matrix's number of columns.

<br>

```cpp
float GetMatrixValue(int i, int j) const
```

Returns the element at the given indices.

<br>

```cpp
bool IsSquare() const
```

Returns true if the matrix is square, false if not.

<br>

```cpp
bool IsIdentity() const
```

Returns true if the matrix is identity.

<br>

```cpp
bool IsSymmetrical() const
```

Returns true if the matrix is identity.

<br>

## Determinants

```cpp
float Det2() const
```

Returns a 2x2 matrix's determinant.

<br>

```cpp
float Det3() const
```

Returns a 3x3 matrix's determinant.

<br>

```cpp
float Det4() const
```

Returns a 4x4 matrix's determinant.

<br>

## Inverses

```cpp
Mat2 Inv2() const
```

Returns the inverse of a 2x2 matrix's.

<br>

```cpp
Mat3 Inv3() const
```

Returns the inverse of a 3x3 matrix's.

<br>

```cpp
Mat4 Inv4() const
```

Returns the inverse of a 4x4 matrix's.

<br>

## Transposition

```cpp
void Transpose()
```

Transposes the matrix.

<br>

```cpp
Matrix GetTransposed() const
```

Returns a transposed copy of this matrix.

<br>

## Conversions

```cpp
AngleAxis ToAngleAxis() const
```

Conversion to angle axis rotation (matrix must be 3x3 or 4x4 rotation only).

<br>

```cpp
Quaternion ToQuaternion() const
```

Conversion to quaternion (matrix must be 3x3 or 4x4 rotation only).

<br>

```cpp
std::string ToString(const int& precision = 2) const
```

Returns matrix contents as string.

<br>

