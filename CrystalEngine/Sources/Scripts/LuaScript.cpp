#include <fstream>
#include <filesystem>

#include "Scripts/LuaScript.h"

#include "../../CrystalLua/Externals/Includes/sol/sol.hpp"

#include "Core/Engine.h"
#include "Core/TimeManager.h"

#include "Debug/Logger.h"

#include "Scenes/GameObject.h"
#include "Scenes/Scene.h"

#include "Maths/Transform.h"

#include "../../CrystalLua/Includes/LuaWrapper.h"
#include <Refureku/Generated/LuaScript.rfks.h>

#include "Utils/WindowsConverter.h"

using namespace Wrapper;
using namespace Scripts;

static void RunLuaFunc(sol::state& state, const char* funcName)
{
    //std::cout << "Calling state at: " << &state << std::endl;

    try {
        const sol::protected_function_result result = state[funcName]();
        if (!result.valid()) {
            const sol::error err = result;
            DebugLogWarning("Failed to call lua function \'" + std::string(funcName) + "\', error: " + err.what());
        }
    }
    catch (const sol::error& e) {
        DebugLogWarning("Unable to run lua func. \'" + std::string(funcName) + "\' with \'" + std::string(e.what()) + "\'");
    }
}

std::string LuaScript::CreateLuaFile(const std::string& fileName)
{
    const std::string fn = Utils::StringUtils::Trim(fileName);
    const std::string path = "Resources\\Scripts\\" + fn + ".lua";

    if(std::filesystem::exists(path)) return "";
    
    std::ofstream file(path);
    file << luaScriptTemplate;
    file.close();
    return path;
}

LuaScript::LuaScript(std::string _name) : ObjectScript(), name(std::move(_name))
{
    archId = staticGetArchetype().getId();
    state = new sol::state();
    
    //std::cout << "Created state at: " << state << std::endl;

    try {
        state->open_libraries(sol::lib::base);
        LuaWrapper().Bind(*state);

        const std::filesystem::path path = name;
        const sol::protected_function_result result = state->script_file(absolute(path).string());
        if (!result.valid()) {
            const sol::error err = result;
            DebugLogWarning("Failed to load lua script. \'" + name + "\', error: " + err.what());
        }
    }
    catch (const sol::error& e) {
        DebugLogWarning("Unable to load lua script. \'" + std::string(name) + "\' with \'" + std::string(e.what()) + "\'");
    }
}

void LuaScript::Start()	     { SetValues(); RunLuaFunc(*state, "Start");      }
void LuaScript::LateStart()  {              RunLuaFunc(*state, "LateStart");  }
void LuaScript::Update()	 {              RunLuaFunc(*state, "Update");     }
void LuaScript::LateUpdate() {              RunLuaFunc(*state, "LateUpdate"); }

void LuaScript::SetValues()
{
    (*state)["object"] = object;
    (*state)["transform"] = transform;
    (*state)["time"] = time;
    (*state)["scene"] = scene;
    (*state)["engine"] = engine;
}