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


#define Scripts_PythonScript_GENERATED	\
static_assert(std::is_invocable_r_v<rfk::SharedPtr<Scripts::PythonScript>, decltype(&Scripts::PythonScript::customInstantiator), const std::string &>, "[Refureku] Instantiator requires &Scripts::PythonScript::customInstantiator to be a static method.");\
static_assert(std::is_same_v<rfk::SharedPtr<Scripts::PythonScript>, std::invoke_result_t<decltype(&Scripts::PythonScript::customInstantiator), const std::string &>> || std::is_same_v<rfk::UniquePtr<Scripts::PythonScript>, std::invoke_result_t<decltype(&Scripts::PythonScript::customInstantiator), const std::string &>>, "[Refureku] Instantiator requires the return type to be rfk::SharedPtr<Scripts::PythonScript> or rfk::UniquePtr<Scripts::PythonScript>.");\
RFK_UNPACK_IF_NOT_PARSING(friend rfk::internal::CodeGenerationHelpers;\
friend rfk::internal::implements_template1__rfk_registerChildClass<PythonScript, void, void(rfk::Struct&)>; \
\
private: template <typename ChildClass> static void _rfk_registerChildClass(rfk::Struct& childClass) noexcept {\
rfk::Struct const& thisClass = staticGetArchetype();\
if constexpr (!std::is_same_v<ChildClass, PythonScript>)const_cast<rfk::Struct&>(thisClass).addSubclass(childClass, rfk::internal::CodeGenerationHelpers::computeClassPointerOffset<ChildClass, PythonScript>());\
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


#define File_PythonScript_GENERATED	\
template <>  rfk::Archetype const* rfk::getArchetype<Scripts::PythonScript>() noexcept;\


