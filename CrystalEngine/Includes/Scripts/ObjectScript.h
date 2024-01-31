#pragma once
#include "Core/Window.h"
#include <Refureku/Refureku.h>
#include "Refureku/Generated/ObjectScript.rfkh.h"


namespace Maths
{
    class Transform;
}
namespace Core
{
    class  Engine;
    class  TimeManager;
}
namespace Scenes
{
    class Scene;
    class GameObject;
}
namespace Scripts NAMESPACE()
{
    enum ScriptType
    {
        Undefined,
        Lua,
        Python
    };

    static constexpr ScriptType AllScriptTypes[] = { Lua, Python };

    inline std::string ScriptTypeToString(const ScriptType& type) {
        switch (type) {
        case ScriptType::Lua:
            return "Lua";
        case ScriptType::Python:
            return "Python";
        case ScriptType::Undefined:
        default:
            return "Undefined";
        }
    }

    inline ScriptType ScriptTypeFromString(const std::string& type) {
        if (type == "Lua") return ScriptType::Lua;
        else if (type == "Python") return ScriptType::Python;
        else return ScriptType::Undefined;
    }

    // - ObjectScript: Class from which all object scripts inherit - //
    struct STRUCT() ObjectScript
    {
    protected:
        size_t archId = 0;

    public:
        // -- Attributes -- //
        // * All attributes are set upon script creation. * //
        FIELD() Scenes::GameObject* object    = nullptr;
        FIELD() Maths::Transform*   transform = nullptr;
        FIELD() Core::WindowInputs  inputs    = {};
        FIELD() Core::TimeManager*  time      = nullptr;
        FIELD() Scenes::Scene*      scene     = nullptr;
        FIELD() Core::Engine*       engine    = nullptr;

        // -- Methods -- //
        ObjectScript();
        virtual ~ObjectScript()                      = default;
        ObjectScript(const ObjectScript&)            = delete;
        ObjectScript(ObjectScript&&)                 = delete;
        ObjectScript& operator=(const ObjectScript&) = delete;
        ObjectScript& operator=(ObjectScript&&)      = delete;
        
        METHOD() virtual void Start     () {} // Overriden by children to be called on scene start.
        METHOD() virtual void LateStart () {} // Overriden by children to be called after scene start.
        METHOD() virtual void Update    () {} // Overriden by children to update their game object.
        METHOD() virtual void LateUpdate() {} // Overriden by children to update their game object after the Update method.
        METHOD() size_t GetArchId() const { return archId; }
        
        Scripts_ObjectScript_GENERATED
    };
}

File_ObjectScript_GENERATED
