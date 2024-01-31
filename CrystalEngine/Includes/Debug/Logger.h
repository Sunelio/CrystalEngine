#pragma once
#include <string>
#include <sstream>

#pragma region Macros

// Returns the name of the file in which it is called.
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

// Crash if the condition is false.
#define Assert(condition, message) Debug::Assertion(condition, message, __FILENAME__, __FUNCTION__, __LINE__)

// Log info to console and file cache at the same time.
#define DebugLogInfo(message) Debug::Logger::LogToAll(Debug::LogType::Info, message, __FILENAME__, __FUNCTION__, __LINE__)
// Log warning to console and file cache at the same time.
#define DebugLogWarning(message) Debug::Logger::LogToAll(Debug::LogType::Warning, message, __FILENAME__, __FUNCTION__, __LINE__)
// Log error to console and file cache at the same time.
#define DebugLogError(message) Debug::Logger::LogToAll(Debug::LogType::Error, message, __FILENAME__, __FUNCTION__, __LINE__)
// Log of specified log type in console and file cache at the same time.
#define DebugLogWithType(type, message) Debug::Logger::LogToAll(type, message, __FILENAME__, __FUNCTION__, __LINE__)

// Log only to the console.
#define DebugLogToConsole(type, message) Debug::Logger::LogToConsole(type, message, __FILENAME__, __FUNCTION__, __LINE__)
// Add log only to file cache.
#define DebugLogToFile(type, message) Debug::Logger::LogToFile(type, message, __FILENAME__, __FUNCTION__, __LINE__)

#pragma endregion 

namespace Debug
{
    enum class LogType
    {
        Info,
        Warning,
        Error
    };
    std::string LogTypeToStr(const LogType& type);
    void Assertion(const bool& condition, const std::string& message, const char* sourceFile, const char* sourceFunction, const long& sourceLine, const bool& throwError = true);

    class Logger
    {
    private:
        inline static Logger* instance = nullptr;
        std::string           filename;
        std::stringstream     fileCache;
        
    public:
        Logger();
        Logger(const std::string& _filename);
        ~Logger();
        Logger(const Logger&)            = delete;
        Logger(Logger&&)                 = delete;
        Logger& operator=(const Logger&) = delete;
        Logger& operator=(Logger&&)      = delete;
        
        static void SetLogFilename(const std::string& _filename, const bool& clearFile = false);
        static void SaveLogsToFile();
        static void LogToConsole  (const LogType& type, const std::string& message, const char* sourceFile, const char* sourceFunction, const long& sourceLine);
        static void LogToFile     (const LogType& type, const std::string& message, const char* sourceFile, const char* sourceFunction, const long& sourceLine);
        static void LogToAll      (const LogType& type, const std::string& message, const char* sourceFile, const char* sourceFunction, const long& sourceLine);

        static std::stringstream* GetLogs() {
            return &instance->fileCache;
        }

    private:
        static void CheckInstance();
        static std::string FormatLog(const LogType& type, const std::string& message, const char* sourceFile, const char* sourceFunction, const long& sourceLine);
    };
}
