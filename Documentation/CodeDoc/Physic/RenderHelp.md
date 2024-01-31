# PhysicSystem

## Attributes

<br>
```h
static class Resources::SubMesh* cubeMesh;
```
<br>
```h
static class Resources::SubMesh* sphereMesh;
```
<br>
## Methods

```cpp
	static void InitRenderHelpData()
```

<br>

```cpp
	static void CleanUpRenderHelpData()
```

<br>

```cpp
	void AddRigidBody(RigidBody* body)
```

<br>


```cpp
static void DrawCube(const Maths::Mat4& modelMat, const class Render::Renderer* render, class Render::Camera* camera,
			class Render::LightManager* lightManager)
```

<br>

```cpp
static void DrawSphere(const Maths::Mat4& modelMat, const class Render::Renderer* render, class Render::Camera* camera,
			class Render::LightManager* lightManager, int type = RIGIDBODY_TYPE_PARTICLE)
```


<br>

```cpp


```


<br>

```cpp

```