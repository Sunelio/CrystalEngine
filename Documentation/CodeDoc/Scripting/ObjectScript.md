# ObjectScript: Class from which all object scripts inherit

## Attributes

**All attributes are set upon script creation.**

```cpp
Scenes::GameObject* object = nullptr
```

```cpp
Maths::Transform* transform = nullptr
```

```cpp
Core::WindowInputs inputs = {}
```

```cpp
Core::TimeManager* time = nullptr
```

```cpp
Core::Engine* engine = nullptr
```

## Methods

```cpp
virtual void Start()
```

Overriden by children to be called on scene start.

<br>

```cpp
virtual void LateStart()
```

Overriden by children to be called after scene start.

<br>

```cpp
virtual void Update()
```

Overriden by children to update their game object.

<br>

```cpp
virtual void LateUpdate()
```

Overriden by children to update their game object after the Update method.

<br>

