# LightManager

```cpp
inline static constexpr uint MAX_LIGHTS = 30
```

Maximum number of lights that can exist in a scene.

<br>

## Handling Lights

```cpp
Light* Create()
```

Creates a new light and returns it.

<br>

```cpp
Light* Get(const uint& i) const
```

Returns the light at the given index (might be nullptr).

<br>

```cpp
Light*const* GetLights() const
```

Returns the entire array of lights (length is MAX_LIGHTS).

<br>

```cpp
void Remove(const Light* light)
```

Removes the given light.

<br>

```cpp
void Remove(const uint& i)
```

Removes the light at the given index.

<br>

