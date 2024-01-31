#include <sstream>
#include <fstream>
#include <algorithm>
#include <filesystem>

#include "Scripts/PythonScript.h"
#include "PythonOpaque.h"
#include "Debug/Logger.h"

#include "../../CrystalPython/Externals/Includes/pybind/pybind11.h"
#include "../../CrystalPython/Externals/Includes/pybind/embed.h"
#include <Refureku/Generated/PythonScript.rfks.h>

#include "Utils/WindowsConverter.h"

using namespace Scripts;

static void RunPythonFunc(pybind11::object& object, const char* funcName)
{
    //std::cout << "Calling object at: " << &object << std::endl;

    try {
        object.attr(funcName)();
    }
    catch (pybind11::error_already_set e) {
        DebugLogWarning("Unable to run python func. \'" + std::string(funcName) + "\' with \'" + std::string(e.what()) + "\'");
    }
}

std::string PythonScript::CreatePyFile(const std::string& fileName)
{
    const std::string fn = Utils::StringUtils::Trim(fileName);
    const std::string path = "Resources\\Scripts\\" + fn + ".py";

    if(std::filesystem::exists(path)) return "";
    
    const std::string patchedTemplate = Utils::StringUtils::Replace(pythonScriptTemplate, "__NAME__", fn);
    std::ofstream file(path);
    file << patchedTemplate;
    file.close();
    return path;
}

PythonScript::PythonScript(std::string _name) : ObjectScript(), name(std::move(_name))
{
    archId = staticGetArchetype().getId();
    
    // Initialize pybind once.
    if (!pybindInitialized)
    {
        pybind11::initialize_interpreter();
        pybind11::module_ sys = pybind11::module::import("sys");
        sys.attr("path").attr("insert")(1,  std::filesystem::current_path().string().c_str());
        sys.attr("path").attr("insert")(1, (std::filesystem::current_path().string() + "\\Resources\\Scripts").c_str());
        pybind11::print(sys.attr("path"));
        pybindInitialized = true;
    }

    // Translate the name from filename format to python import format.
    std::string pyName = name;
    if (pyName.substr(pyName.size() - 3, 3) == ".py")
        pyName = pyName.substr(0, pyName.size() - 3);
    
    std::replace(pyName.begin(), pyName.end(), '/', '.');
    std::replace(pyName.begin(), pyName.end(), '\\', '.');
    const size_t lastDotIndex = pyName.find_last_of('.') + 1;

    // Import the file as a module with pybind.
    try {
        const pybind11::module_ pyModule = pybind11::module::import(pyName.c_str());
        const pybind11::object  pyClass  = pyModule.attr(pyName.substr(lastDotIndex, pyName.size() - lastDotIndex).c_str());
        pyObject = new pybind11::object();
       *pyObject = pyClass();

       //std::cout << "Created object at: " << pyObject << std::endl;
    }
    catch (const pybind11::error_already_set& e) {
        DebugLogError("Unable to load python. \'" + std::string(pyName) + "\' with \'" + std::string(e.what()) + "\'");
    }
}

void PythonScript::Start()      { SetValues(); RunPythonFunc(*pyObject, "Start"     ); }
void PythonScript::LateStart()  {              RunPythonFunc(*pyObject, "LateStart" ); }
void PythonScript::Update()     {              RunPythonFunc(*pyObject, "Update"    ); }
void PythonScript::LateUpdate() {              RunPythonFunc(*pyObject, "LateUpdate"); }

void PythonScript::SetValues()
{
    pyObject->attr("object"   ) = object;
    pyObject->attr("transform") = transform;
    pyObject->attr("time"     ) = time;
    pyObject->attr("scene"    ) = scene;
    pyObject->attr("engine"   ) = engine;
}
