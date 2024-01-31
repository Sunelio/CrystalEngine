#include "Utils/WindowsConverter.h"

#include <iostream>
#include <Windows.h>
#include <string>
#include <array>

#include <algorithm>
#include <thread>

#include "Core/Engine.h"
#include "Core/Window.h"
#include "Debug/Logger.h"

namespace Utils
{
    std::string StringUtils::Trim(const std::string& str)
    {
        const auto start = std::find_if(str.begin(), str.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        });

        const auto end = std::find_if(str.rbegin(), str.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base();

        return start < end ? std::string(start, end) : "";
    }

    bool StringUtils::Contains(const std::string& str, const std::string& searchStr)
    {
        return str.find(searchStr) != std::string::npos;
    }

    std::string StringUtils::Replace(const std::string& str, const std::string& toReplace, const std::string& replaceWith)
    {
        std::string newStr = str;
        
        const size_t pos = newStr.find(toReplace);
        if (pos != std::string::npos) {
            newStr.replace(pos, toReplace.length(), replaceWith);
        }

        return newStr;
    }

    std::string StringUtils::ToCapitalized(const std::string& str)
    {
        std::string newStr = str;
        newStr[0] = std::toupper(newStr[0]);
        return newStr;
    }

    std::string StringUtils::CropFromEnd(const std::string& str, const int& count)
    {
        return str.substr(0, str.length() - count);
    }

    void WindowsUtils::OpenVisualStudio(const std::filesystem::path& filePath)
    {
        std::thread vsCodeThread([fp = std::move(filePath)] {
            std::this_thread::sleep_for(std::chrono::nanoseconds(5));
            
            const std::string vsCodeCommand = "code " + fp.generic_string();
            if (std::system(vsCodeCommand.c_str()) == -1) {
                DebugLogError("Failed to open Visual Studio Code.");

                const std::string notepadCommand = "notepad.exe " + fp.generic_string();
                if (std::system(notepadCommand.c_str()) == -1) {
                    DebugLogError("Failed to open Notepad.");
                }
            }
        });
        vsCodeThread.detach();
    }

    std::string WindowsUtils::OpenExplorer()
    {
        OPENFILENAMEA ofn;
        CHAR szFile[260] = { 0 };
        ZeroMemory(&ofn, sizeof(OPENFILENAMEA));
        ofn.lStructSize = sizeof(OPENFILENAMEA);
        ofn.hwndOwner = Core::Engine::Get()->GetWindow()->GetWindow32Context();
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = ".json";
        ofn.nFilterIndex = 1;
        ofn.lpstrInitialDir = absolute(std::filesystem::path("Resources/Scenes")).string().c_str();
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

        if (GetOpenFileNameA(&ofn) == TRUE)
        {
            return ofn.lpstrFile;
        }

        return std::string();
    }
}
