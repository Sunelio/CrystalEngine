#include "Debug/Logger.h"
#include "Core/Engine.h"
#include <filesystem>
#include <fstream>
#include <iostream>

using namespace Debug;
using namespace Core;

std::string Debug::LogTypeToStr(const LogType& type)
{
    switch (type)
    {
    case LogType::Info:
        return "INFO: ";
    case LogType::Warning:
        return "WARNING: ";
    case LogType::Error:
        return "ERROR: ";
    default:
        return "UNCATEGORIZED: ";
    }
}

void Debug::Assertion(const bool& condition, const std::string& message, const char* sourceFile, const char* sourceFunction, const long& sourceLine, const bool& throwError)
{
    if (condition) return;

    Logger::LogToAll(LogType::Error, std::string("Assertion failed, ") + message, sourceFile, sourceFunction, sourceLine);
    if (throwError)
    {
        Logger::SaveLogsToFile();
        throw std::runtime_error(message);
    }
}

Logger::Logger()
{
    if (!instance)
        instance = this;
}

Logger::Logger(const std::string& _filename)
{
    if (!instance)
        instance = this;
    SetLogFilename(_filename);
}

Logger::~Logger()
{
    SaveLogsToFile();
}

void Logger::SetLogFilename(const std::string& _filename, const bool& clearFile)
{
    CheckInstance();
    instance->filename = _filename;
    if (clearFile)
    {
        std::ofstream file(instance->filename);
        file.close();
    }
}

void Logger::SaveLogsToFile()
{
    CheckInstance();
    if (instance->filename.empty())
    {
        DebugLogWarning("Unable to save logs because the log file name isn't set.");
        return;
    }
    if (std::fstream f(instance->filename, std::fstream::out); f.is_open())
    {
        f << instance->fileCache.str();
        f.close();
        return;
    }
    DebugLogWarning(std::string("Unable to open log file: ") + instance->filename);
}

void Logger::LogToConsole(const LogType& type, const std::string& message, const char* sourceFile, const char* sourceFunction, const long& sourceLine)
{
    std::cout << FormatLog(type, message, sourceFile, sourceFunction, sourceLine) << std::endl;
}

void Logger::LogToFile(const LogType& type, const std::string& message, const char* sourceFile, const char* sourceFunction, const long& sourceLine)
{
    CheckInstance();
    instance->fileCache << FormatLog(type, message, sourceFile, sourceFunction, sourceLine) << std::endl;
}

void Logger::LogToAll(const LogType& type, const std::string& message, const char* sourceFile, const char* sourceFunction, const long& sourceLine)
{
    CheckInstance();
    std::cout           << FormatLog(type, message, sourceFile, sourceFunction, sourceLine) << std::endl;
    instance->fileCache << FormatLog(type, message, sourceFile, sourceFunction, sourceLine) << std::endl;
}

void Logger::CheckInstance()
{
    if (!instance)
        if (const Engine* engine = Engine::Get())
            instance = engine->GetLogger();
}

std::string Logger::FormatLog(const LogType& type, const std::string& message, const char* sourceFile, const char* sourceFunction, const long& sourceLine)
{
    std::string output = "\n";
    output += LogTypeToStr(type);
    output += std::filesystem::path(sourceFile).filename().string();
    if (sourceLine >= 0) {
        output += "(";
        output += std::to_string(sourceLine);
        output += ")";
    }
    if (sourceFunction[0] != '\0') {
        output += " in ";
        output += sourceFunction;
    }
    output += "\n";
    output += message;
    return output;
}
