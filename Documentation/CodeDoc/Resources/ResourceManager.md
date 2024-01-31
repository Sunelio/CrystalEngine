# ResourceManager: Stores all engine resources

## Handling resources

```cpp
template<typename T> T* Create(const std::string& name)
```

Creates and loads a new resource with the given name and type.

<br>

```cpp
template<typename T> T* Get(const std::string& name)
```

Returns the resource with the given name and type if it exists.

<br>

```cpp
void Delete(const std::string& name)
```

Deletes the resource with the given name.

<br>

## Methods

```cpp
template<typename T> bool IsSupported() const
```

Returns true if the specified type is supported by the resource manager.

<br>

```cpp
bool Exists(const std::string& name) const
```

Returns true if a resource with the given name has been created.

<br>

```cpp
bool InPipeline(const std::string& name) const
```

Returns true if a resource with the given name has been sent to GPU memory.

<br>

```cpp
void SendLoadedResources()
```

Sends newly loaded resources to GPU memory (useful with asynchronous resource loading).

<br>

