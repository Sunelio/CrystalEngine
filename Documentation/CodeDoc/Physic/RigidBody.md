# RigidBody

<br>
## RigidBody type
#define RIGIDBODY_TYPE_BASE		0
#define RIGIDBODY_TYPE_PARTICLE 1
#define RIGIDBODY_TYPE_SPHERE	2
#define RIGIDBODY_TYPE_BOX		3

## Methods

```h
	void Update(float deltaTime)
```

<br>

```h
	void Render(const class Render::Renderer* render, Render::Camera* camera, Render::LightManager* lightManager)
```

<br>

```h
virtual inline void Update(float deltaTime)
```

<br>


```h
virtual void AddForce(const Maths::Vector3& force) 
```

<br>

```h
virtual inline void ApplyForces()
```


<br>

```h
inline bool HasVolume()

```
void ClearContraints()

<br>

```h

```