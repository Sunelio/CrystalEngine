# Light

## Attributes

```cpp
Maths::Transform* transform = nullptr
```

Pointer to the parent LightObject's transform.

<br>

```cpp
Maths::RGB ambient, diffuse = 1, specular = 1
```

The light's ambient, diffuse and specular colors.

<br>

```cpp
float constant = 1, linear = 0, quadratic = 0
```

The light's constant, linear and quadratic attenuation.

<br>

```cpp
float outerCutoff = -1, innerCutoff = -1
```

The light's inner and outer cutoffs.

<br>

