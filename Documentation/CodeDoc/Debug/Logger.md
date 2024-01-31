```cpp
#define __FILENAME__(strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
```

Returns the name of the file in which it is called.

<br>

```cpp
#define Assert(condition, message) Debug::Assertion::DoAssertion(condition, message, __FILENAME__, __FUNCTION__, __LINE__)
```

Crash if the condition is false.

<br>

```cpp
#define DebugLog(message) Debug::Logger::LogToAll(Debug::LogTypes::Info, message, __FILENAME__, __FUNCTION__, __LINE__)
```

Log info to console and file cache at the same time.

<br>

```cpp
#define DebugLogWarning(message) Debug::Logger::LogToAll(Debug::LogTypes::Warning, message, __FILENAME__, __FUNCTION__, __LINE__)
```

Log warning to console and file cache at the same time.

<br>

```cpp
#define DebugLogError(message) Debug::Logger::LogToAll(Debug::LogTypes::Error, message, __FILENAME__, __FUNCTION__, __LINE__)
```

Log error to console and file cache at the same time.

<br>

```cpp
#define DebugLogWithType(type, message) Debug::Logger::LogToAll(type, message, __FILENAME__, __FUNCTION__, __LINE__)
```

Log of specified log type in console and file cache at the same time.

<br>

```cpp
#define DebugLogToConsole(type, message) Debug::Logger::LogToConsole(type, message, __FILENAME__, __FUNCTION__, __LINE__)
```

Log only to the console.

<br>

```cpp
#define DebugLogToFile(type, message) Debug::Logger::LogToFile(type, message, __FILENAME__, __FUNCTION__, __LINE__)
```

Add log only to file cache.

<br>

