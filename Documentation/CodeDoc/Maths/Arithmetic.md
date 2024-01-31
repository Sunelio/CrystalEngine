# Arithmetic fuctions

```cpp
int roundInt(const float& val)
```

Rounds the given value to the nearest int.

<br>

```cpp
int floorInt(const float& val)
```

Rounds down the given value.

<br>

```cpp
int ceilInt(const float& val)
```

Rounds up the given value.

<br>

```cpp
float sqpow(const float& val)
```

Returns the sqare power of the given value.

<br>

```cpp
int signof(const float& val)
```

Returns 1 if the given value is positive or null, and -1 if it is negative.

<br>

```cpp
float degToRad(const float& deg)
```

Converts the given angle from degrees to radians.

<br>

```cpp
float radToDeg(const float& rad)
```

Converts the given angle from radians to degrees.

<br>

```cpp
float clamp(float val, const float& min, const float& max)
```

Clamps the given value to be superior or equal to the minimum value and inferior or equal to the maximum value.

<br>

```cpp
float clampUnder(float val, const float& max)
```

Clamps the given value to be inferior or equal to the maximum value.

<br>

```cpp
float clampAbove(float val, const float& min)
```

Clamps the given value to be superior or equal to the minimum value.

<br>

```cpp
float lerp(const float& start, const float& dest, const float& val)
```

Compute linear interpolation between start and dest for the parameter val (if 0 <= val <= 1: start <= return <= end).

<br>

```cpp
float getLerp(const float& start, const float& dest, const float& val)
```

Compute the linear interpolation factor that returns val when lerping between start and end.

<br>

```cpp
float remap(const float& val, const float& inputStart, const float& inputEnd, const float& outputStart, const float& outputEnd)
```

Remaps the given value from one range to another.

<br>

```cpp
bool isPowerOf2(const int& val)
```

Returns true if the given number is a power of 2.

<br>

```cpp
int getPowerOf2Under(const int& val)
```

Returns the closest power of 2 that is inferior or equal to val.

<br>

```cpp
int getPowerOf2Above(const int& val)
```

Returns the closest power of 2 that is superior or equal to val.

<br>

