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


#define Scripts_LuaScript_GENERATED	\
static_assert(std::is_invocable_r_v<rfk::SharedPtr<Scripts::LuaScript>, decltype(&Scripts::LuaScript::customInstantiator), const std::string &>, "[Refureku] Instantiator requires &Scripts::LuaScript::customInstantiator to be a static method.");\
static_assert(std::is_same_v<rfk::SharedPtr<Scripts::LuaScript>, std::invoke_result_t<decltype(&Scripts::LuaScript::customInstantiator), const std::string &>> || std::is_same_v<rfk::UniquePtr<Scripts::LuaScript>, std::invoke_result_t<decltype(&Scripts::LuaScript::customInstantiator), const std::string &>>, "[Refureku] Instantiator requires the return type to be rfk::SharedPtr<Scripts::LuaScript> or rfk::UniquePtr<Scripts::LuaScript>.");\
RFK_UNPACK_IF_NOT_PARSING(friend rfk::internal::CodeGenerationHelpers;\
friend rfk::internal::implements_template1__rfk_registerChildClass<LuaScript, void, void(rfk::Struct&)>; \
\
private: template <typename ChildClass> static void _rfk_registerChildClass(rfk::Struct& childClass) noexcept {\
rfk::Struct const& thisClass = staticGetArchetype();\
if constexpr (!std::is_same_v<ChildClass, LuaScript>)const_cast<rfk::Struct&>(thisClass).addSubclass(childClass, rfk::internal::CodeGenerationHelpers::computeClassPointerOffset<ChildClass, LuaScript>());\
else\
{\
childClass.setFieldsCapacity(0u + rfk::internal::CodeGenerationHelpers::getReflectedFieldsCount< Scripts::ObjectScript>()); childClass.setStaticFieldsCapacity(0u + rfk::internal::CodeGenerationHelpers::getReflectedStaticFieldsCount< Scripts::ObjectScript>()); \
}\
rfk::internal::CodeGenerationHelpers::registerChildClass<Scripts::ObjectScript, ChildClass>(childClass);\
}\
\
public:  static rfk::Struct const& staticGetArchetype() noexcept;\
\
)\


#define File_LuaScript_GENERATED	\
template <>  rfk::Archetype const* rfk::getArchetype<Scripts::LuaScript>() noexcept;\


