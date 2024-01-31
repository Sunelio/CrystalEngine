# RGBA: red, green, blue and alpha between 0 and 1

## Attributes

```cpp
float r, g, b, a = 1
```

Red, green, blue and alpha components.

<br>

## Constructors

```cpp
RGBA()
```

Default constructor (r = g = b = 0, a = 1).

<br>

```cpp
RGBA(const float& all)
```

RGBA with equal values.

<br>

```cpp
RGBA(const float& _r, const float& _g, const float& _b, const float& _a)
```

RGBA color with 4 values.

<br>

```cpp
RGBA(const RGB& rgb, const float& _a = 1)
```

RGBA color from RGB.

<br>

## Miscellaneous

```cpp
RGB toRGB() const
```

Convert RGBA color to RGB.

<br>

```cpp
float* ptr()
```

Returns a pointer to the r component.

<br>

## Operators

```cpp
RGBA operator+(const RGBA& c) const ; }
```

```cpp
RGBA operator+(const float& v) const ; }
```

```cpp
RGBA operator-(const RGBA& c) const ; }
```

```cpp
RGBA operator-(const float& v) const ; }
```

```cpp
RGBA operator*(const RGBA& c) const ; }
```

```cpp
RGBA operator*(const float& v) const ; }
```

```cpp
RGBA operator/(const RGBA& c) const ; }
```

```cpp
RGBA operator/(const float& v) const ; }
```

```cpp
void operator+=(const RGBA& c)
```

```cpp
void operator+=(const float& v)
```

```cpp
void operator-=(const RGBA& c)
```

```cpp
void operator-=(const float& v)
```

```cpp
void operator*=(const RGBA& c)
```

```cpp
void operator*=(const float& v)
```

```cpp
void operator/=(const RGBA& c)
```

```cpp
void operator/=(const float& v)
```

```cpp
bool operator==(const RGBA& c) const
```

```cpp
bool operator!=(const RGBA& c) const
```

# RGB: red, green and blue between 0 and 1

## Attributes

```cpp
float r, g, b
```

Red, green and blue components.

<br>

## Constructors

```cpp
RGB()
```

Default constructor (r = g = b = 0).

<br>

```cpp
RGB(const float& all)
```

RGB with equal values.

<br>

```cpp
RGB(const float& _r, const float& _g, const float& _b)
```

RGB color with 3 values.

<br>

```cpp
RGB(const RGBA& rgba)
```

RGB color from RGBA.

<br>

## Miscellaneous

```cpp
RGBA toRGBA(const float& a = 1) const
```

Convert RGB color to RGBA.

<br>

```cpp
float* ptr()
```

Returns a pointer to the r component.

<br>

# HSVA: hue, saturtion, value and alpha (h in radians, sva between 0 and 1)

## Attributes

```cpp
float h, s, v, a = 1
```

Hue, saturtion, value and alpha components.

<br>

## Constructors

```cpp
HSVA()
```

Default constructor (h = s = v = 0, a = 1).

<br>

```cpp
HSVA(const float& all)
```

HSVA color with equal values.

<br>

```cpp
HSVA(const float& _h, const float& _s, const float& _v, const float& _a)
```

HSVA color with 4 values.

<br>

```cpp
HSVA(const HSV& hsv, const float& _a = 1)
```

HSVA color from HSV.

<br>

## Miscellaneous

```cpp
HSV toHSV() const
```

Convert HSVA color to HSV.

<br>

```cpp
float* ptr()
```

Returns a pointer to the h component.

<br>

# HSV: hue, saturation and value (h in radians, sv between 0 and 1)

## Attributes

```cpp
float h, s, v
```

Hue, saturtion and value components.

<br>

## Constructors

```cpp
HSV()
```

Default constructor (h = s = v = 0).

<br>

```cpp
HSV(const float& all)
```

HSV color with equal values.

<br>

```cpp
HSV(const float& _h, const float& _s, const float& _v)
```

HSV color with 3 values.

<br>

```cpp
HSV(const HSVA& hsva)
```

HSV color from HSVA.

<br>

## Miscellaneous

```cpp
HSVA toHSVA(const float& a = 1) const
```

Convert HSV color to HSVA.

<br>

```cpp
float* ptr()
```

Returns a pointer to the h component.

<br>

## Color Methods

```cpp
float ColorGetHue(const RGBA& rgb)
```

Returns the given RGB color's hue.

<br>

```cpp
RGBA ColorLerp(const float& val, const RGBA& start, const RGBA& end)
```

Linear interpolation between two given colors.

<br>

```cpp
HSV BlendHSV(const HSV& col0, const HSV& col1)
```

Blend between two HSV colors.

<br>

```cpp
HSVA RGBAtoHSVA(const RGBA& color)
```

Convert an RGBA color (0 <= rgba <= 1) to HSVA.

<br>

```cpp
RGBA HSVAtoRGBA(const HSVA& hsva)
```

Convert an HSVA color to RGBA.

<br>

```cpp
RGBA ColorShift(const RGBA& color, const float& hue)
```

Shifts the hue of the given color.

<br>

