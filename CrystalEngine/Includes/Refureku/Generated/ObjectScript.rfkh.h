#pragma once

#include "EntityMacros.h"

#include <Refureku/TypeInfo/Entity/EEntityKind.h>
#include <string>
#include <Refureku/Misc/CodeGenerationHelpers.h>
#include <Refureku/Misc/DisableWarningMacros.h>
#include <Refureku/TypeInfo/Functions/Method.h>
#include <Refureku/TypeInfo/Functions/StaticMethod.h>
#include <Refureku/TypeInfo/Variables/Field.h>
#include <Refureku/TypeInfo/Variables/StaticField.h>
#include <Refureku/TypeInfo/Archetypes/Enum.h>
#include <Refureku/TypeInfo/Archetypes/EnumValue.h>
#include <Refureku/TypeInfo/Variables/Variable.h>
#include <Refureku/TypeInfo/Functions/Function.h>
#include <Refureku/TypeInfo/Archetypes/Template/ClassTemplate.h>
#include <Refureku/TypeInfo/Archetypes/Template/ClassTemplateInstantiation.h>
#include <Refureku/TypeInfo/Archetypes/Template/ClassTemplateInstantiationRegisterer.h>
#include <Refureku/TypeInfo/Archetypes/Template/TypeTemplateArgument.h>
#include <Refureku/TypeInfo/Archetypes/Template/NonTypeTemplateArgument.h>
#include <Refureku/TypeInfo/Archetypes/Template/TemplateTemplateArgument.h>


#define Scripts_ObjectScript_GENERATED	\
RFK_UNPACK_IF_NOT_PARSING(friend rfk::internal::CodeGenerationHelpers;\
friend rfk::internal::implements_template1__rfk_registerChildClass<ObjectScript, void, void(rfk::Struct&)>; \
\
private: template <typename ChildClass> static void _rfk_registerChildClass(rfk::Struct& childClass) noexcept {\
rfk::Struct const& thisClass = staticGetArchetype();\
if constexpr (!std::is_same_v<ChildClass, ObjectScript>)const_cast<rfk::Struct&>(thisClass).addSubclass(childClass, rfk::internal::CodeGenerationHelpers::computeClassPointerOffset<ChildClass, ObjectScript>());\
else\
{\
childClass.setFieldsCapacity(6u + 0); childClass.setStaticFieldsCapacity(0u + 0); \
}\
[[maybe_unused]] rfk::Field* field = nullptr; [[maybe_unused]] rfk::StaticField* staticField = nullptr;\
)\
__RFK_DISABLE_WARNING_PUSH \
__RFK_DISABLE_WARNING_OFFSETOF \
RFK_UNPACK_IF_NOT_PARSING(field = childClass.addField("object", std::hash<std::string>()(std::string("c:@N@Scripts@S@ObjectScript@FI@object") + rfk::internal::getTypename<ChildClass>()), rfk::getType<Scenes::GameObject *>(), static_cast<rfk::EFieldFlags>(1), offsetof(ChildClass, object), &thisClass);\
field = childClass.addField("transform", std::hash<std::string>()(std::string("c:@N@Scripts@S@ObjectScript@FI@transform") + rfk::internal::getTypename<ChildClass>()), rfk::getType<Maths::Transform *>(), static_cast<rfk::EFieldFlags>(1), offsetof(ChildClass, transform), &thisClass);\
field = childClass.addField("inputs", std::hash<std::string>()(std::string("c:@N@Scripts@S@ObjectScript@FI@inputs") + rfk::internal::getTypename<ChildClass>()), rfk::getType<Core::WindowInputs>(), static_cast<rfk::EFieldFlags>(1), offsetof(ChildClass, inputs), &thisClass);\
field = childClass.addField("time", std::hash<std::string>()(std::string("c:@N@Scripts@S@ObjectScript@FI@time") + rfk::internal::getTypename<ChildClass>()), rfk::getType<Core::TimeManager *>(), static_cast<rfk::EFieldFlags>(1), offsetof(ChildClass, time), &thisClass);\
field = childClass.addField("scene", std::hash<std::string>()(std::string("c:@N@Scripts@S@ObjectScript@FI@scene") + rfk::internal::getTypename<ChildClass>()), rfk::getType<Scenes::Scene *>(), static_cast<rfk::EFieldFlags>(1), offsetof(ChildClass, scene), &thisClass);\
field = childClass.addField("engine", std::hash<std::string>()(std::string("c:@N@Scripts@S@ObjectScript@FI@engine") + rfk::internal::getTypename<ChildClass>()), rfk::getType<Core::Engine *>(), static_cast<rfk::EFieldFlags>(1), offsetof(ChildClass, engine), &thisClass);\
)\
__RFK_DISABLE_WARNING_POP \
RFK_UNPACK_IF_NOT_PARSING(}\
\
public:  static rfk::Struct const& staticGetArchetype() noexcept;\
\
)\


#define File_ObjectScript_GENERATED	\
template <>  rfk::Archetype const* rfk::getArchetype<Scripts::ObjectScript>() noexcept;\


