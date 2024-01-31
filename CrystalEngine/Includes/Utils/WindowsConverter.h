#pragma once

#include <filesystem>
#include <string>

namespace Utils
{
    class StringUtils
    {
    public:
        static std::string Trim(const std::string& str);
        static bool Contains(const std::string& str, const std::string& searchStr);
        static std::string Replace(const std::string& str, const std::string& toReplace, const std::string& replaceWith);
        static std::string ToCapitalized(const std::string& str);
        static std::string CropFromEnd(const std::string& str, const int& count);
    };

    class WindowsUtils
    {
    public:
        static void OpenVisualStudio(const std::filesystem::path& filePath);
        static std::string OpenExplorer();
    };
}
