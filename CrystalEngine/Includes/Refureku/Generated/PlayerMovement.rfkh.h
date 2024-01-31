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


#define Scripts_PlayerMovement_GENERATED	\
RFK_UNPACK_IF_NOT_PARSING(friend rfk::internal::CodeGenerationHelpers;\
friend rfk::internal::implements_template1__rfk_registerChildClass<PlayerMovement, void, void(rfk::Struct&)>; \
\
private: template <typename ChildClass> static void _rfk_registerChildClass(rfk::Struct& childClass) noexcept {\
rfk::Struct const& thisClass = staticGetArchetype();\
if constexpr (!std::is_same_v<ChildClass, PlayerMovement>)const_cast<rfk::Struct&>(thisClass).addSubclass(childClass, rfk::internal::CodeGenerationHelpers::computeClassPointerOffset<ChildClass, PlayerMovement>());\
else\
{\
childClass.setFieldsCapacity(4u + rfk::internal::CodeGenerationHelpers::getReflectedFieldsCount< Scripts::ObjectScript>()); childClass.setStaticFieldsCapacity(0u + rfk::internal::CodeGenerationHelpers::getReflectedStaticFieldsCount< Scripts::ObjectScript>()); \
}\
[[maybe_unused]] rfk::Field* field = nullptr; [[maybe_unused]] rfk::StaticField* staticField = nullptr;\
)\
__RFK_DISABLE_WARNING_PUSH \
__RFK_DISABLE_WARNING_OFFSETOF \
RFK_UNPACK_IF_NOT_PARSING(field = childClass.addField("speed", std::hash<std::string>()(std::string("c:@N@Scripts@S@PlayerMovement@FI@speed") + rfk::internal::getTypename<ChildClass>()), rfk::getType<float>(), static_cast<rfk::EFieldFlags>(1), offsetof(ChildClass, speed), &thisClass);\
field = childClass.addField("sensibility", std::hash<std::string>()(std::string("c:@N@Scripts@S@PlayerMovement@FI@sensibility") + rfk::internal::getTypename<ChildClass>()), rfk::getType<float>(), static_cast<rfk::EFieldFlags>(1), offsetof(ChildClass, sensibility), &thisClass);\
field = childClass.addField("light", std::hash<std::string>()(std::string("c:@N@Scripts@S@PlayerMovement@FI@light") + rfk::internal::getTypename<ChildClass>()), rfk::getType<Scenes::GameObject *>(), static_cast<rfk::EFieldFlags>(1), offsetof(ChildClass, light), &thisClass);\
field = childClass.addField("state", std::hash<std::string>()(std::string("c:@N@Scripts@S@PlayerMovement@FI@state") + rfk::internal::getTypename<ChildClass>()), rfk::getType<Scripts::PlayerState>(), static_cast<rfk::EFieldFlags>(1), offsetof(ChildClass, state), &thisClass);\
)\
__RFK_DISABLE_WARNING_POP \
RFK_UNPACK_IF_NOT_PARSING(rfk::internal::CodeGenerationHelpers::registerChildClass<Scripts::ObjectScript, ChildClass>(childClass);\
}\
\
public:  static rfk::Struct const& staticGetArchetype() noexcept;\
\
)\


#define File_PlayerMovement_GENERATED	\
template <>  rfk::Archetype const* rfk::getArchetype<Scripts::PlayerMovement>() noexcept;\


