# Particle

## Attributes

<br>
``h
Vector3 position
Vector3 oldPosition
Vector3 forces
Vector3 gravity
float friction
float bounce
float mass

#ifdef EULER_INTEGRATION
		Maths::Vector3 velocity;
#endif // EULER_INTEGRATION

```

## Methods

```cpp
void SetPosition(const Maths::Vector3 pos)
```

<br>

```cpp
	void Render(const class Render::Renderer* render, Render::Camera* camera, Render::LightManager* lightManager)
```

<br>

```cpp
Vector3 GetVelocity()
```

<br>

```cpp
void SolveContraints(const std::vector<Geometry3D::OBB>& obb)

```

<br>


```cpp
void SetMass(float m)
```

<br>

```cpp
float GetBounce()
```

<br>

```cpp
Vector3 GetPosition()

```

<br>

```cpp
void SetBounce(float b)
```

<br>

```cpp
void SetFriction(float f)
```

<br>

```cpp
float invMass()
```