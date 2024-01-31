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


#define Scripts_RotateObject_GENERATED	\
RFK_UNPACK_IF_NOT_PARSING(friend rfk::internal::CodeGenerationHelpers;\
friend rfk::internal::implements_template1__rfk_registerChildClass<RotateObject, void, void(rfk::Struct&)>; \
\
private: template <typename ChildClass> static void _rfk_registerChildClass(rfk::Struct& childClass) noexcept {\
rfk::Struct const& thisClass = staticGetArchetype();\
if constexpr (!std::is_same_v<ChildClass, RotateObject>)const_cast<rfk::Struct&>(thisClass).addSubclass(childClass, rfk::internal::CodeGenerationHelpers::computeClassPointerOffset<ChildClass, RotateObject>());\
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


#define Scripts_DefaultMeshShaderScript_GENERATED	\
RFK_UNPACK_IF_NOT_PARSING(friend rfk::internal::CodeGenerationHelpers;\
friend rfk::internal::implements_template1__rfk_registerChildClass<DefaultMeshShaderScript, void, void(rfk::Struct&)>; \
\
private: template <typename ChildClass> static void _rfk_registerChildClass(rfk::Struct& childClass) noexcept {\
rfk::Struct const& thisClass = staticGetArchetype();\
if constexpr (!std::is_same_v<ChildClass, DefaultMeshShaderScript>)const_cast<rfk::Struct&>(thisClass).addSubclass(childClass, rfk::internal::CodeGenerationHelpers::computeClassPointerOffset<ChildClass, DefaultMeshShaderScript>());\
else\
{\
childClass.setFieldsCapacity(0u + rfk::internal::CodeGenerationHelpers::getReflectedFieldsCount< Scripts::MeshShaderScript>()); childClass.setStaticFieldsCapacity(0u + rfk::internal::CodeGenerationHelpers::getReflectedStaticFieldsCount< Scripts::MeshShaderScript>()); \
}\
rfk::internal::CodeGenerationHelpers::registerChildClass<Scripts::MeshShaderScript, ChildClass>(childClass);\
}\
\
public:  static rfk::Struct const& staticGetArchetype() noexcept;\
\
)\


#define Scripts_GridMeshShaderScript_GENERATED	\
RFK_UNPACK_IF_NOT_PARSING(friend rfk::internal::CodeGenerationHelpers;\
friend rfk::internal::implements_template1__rfk_registerChildClass<GridMeshShaderScript, void, void(rfk::Struct&)>; \
\
private: template <typename ChildClass> static void _rfk_registerChildClass(rfk::Struct& childClass) noexcept {\
rfk::Struct const& thisClass = staticGetArchetype();\
if constexpr (!std::is_same_v<ChildClass, GridMeshShaderScript>)const_cast<rfk::Struct&>(thisClass).addSubclass(childClass, rfk::internal::CodeGenerationHelpers::computeClassPointerOffset<ChildClass, GridMeshShaderScript>());\
else\
{\
childClass.setFieldsCapacity(0u + rfk::internal::CodeGenerationHelpers::getReflectedFieldsCount< Scripts::MeshShaderScript>()); childClass.setStaticFieldsCapacity(0u + rfk::internal::CodeGenerationHelpers::getReflectedStaticFieldsCount< Scripts::MeshShaderScript>()); \
}\
rfk::internal::CodeGenerationHelpers::registerChildClass<Scripts::MeshShaderScript, ChildClass>(childClass);\
}\
\
public:  static rfk::Struct const& staticGetArchetype() noexcept;\
\
)\


#define Scripts_SkyboxMeshShaderScript_GENERATED	\
RFK_UNPACK_IF_NOT_PARSING(friend rfk::internal::CodeGenerationHelpers;\
friend rfk::internal::implements_template1__rfk_registerChildClass<SkyboxMeshShaderScript, void, void(rfk::Struct&)>; \
\
private: template <typename ChildClass> static void _rfk_registerChildClass(rfk::Struct& childClass) noexcept {\
rfk::Struct const& thisClass = staticGetArchetype();\
if constexpr (!std::is_same_v<ChildClass, SkyboxMeshShaderScript>)const_cast<rfk::Struct&>(thisClass).addSubclass(childClass, rfk::internal::CodeGenerationHelpers::computeClassPointerOffset<ChildClass, SkyboxMeshShaderScript>());\
else\
{\
childClass.setFieldsCapacity(0u + rfk::internal::CodeGenerationHelpers::getReflectedFieldsCount< Scripts::MeshShaderScript>()); childClass.setStaticFieldsCapacity(0u + rfk::internal::CodeGenerationHelpers::getReflectedStaticFieldsCount< Scripts::MeshShaderScript>()); \
}\
rfk::internal::CodeGenerationHelpers::registerChildClass<Scripts::MeshShaderScript, ChildClass>(childClass);\
}\
\
public:  static rfk::Struct const& staticGetArchetype() noexcept;\
\
)\


#define Scripts_DefaultScreenShaderScript_GENERATED	\
RFK_UNPACK_IF_NOT_PARSING(friend rfk::internal::CodeGenerationHelpers;\
friend rfk::internal::implements_template1__rfk_registerChildClass<DefaultScreenShaderScript, void, void(rfk::Struct&)>; \
\
private: template <typename ChildClass> static void _rfk_registerChildClass(rfk::Struct& childClass) noexcept {\
rfk::Struct const& thisClass = staticGetArchetype();\
if constexpr (!std::is_same_v<ChildClass, DefaultScreenShaderScript>)const_cast<rfk::Struct&>(thisClass).addSubclass(childClass, rfk::internal::CodeGenerationHelpers::computeClassPointerOffset<ChildClass, DefaultScreenShaderScript>());\
else\
{\
childClass.setFieldsCapacity(0u + rfk::internal::CodeGenerationHelpers::getReflectedFieldsCount< Scripts::ScreenShaderScript>()); childClass.setStaticFieldsCapacity(0u + rfk::internal::CodeGenerationHelpers::getReflectedStaticFieldsCount< Scripts::ScreenShaderScript>()); \
}\
rfk::internal::CodeGenerationHelpers::registerChildClass<Scripts::ScreenShaderScript, ChildClass>(childClass);\
}\
\
public:  static rfk::Struct const& staticGetArchetype() noexcept;\
\
)\


#define File_DefaultScripts_GENERATED	\
template <>  rfk::Archetype const* rfk::getArchetype<Scripts::RotateObject>() noexcept;\
template <>  rfk::Archetype const* rfk::getArchetype<Scripts::DefaultMeshShaderScript>() noexcept;\
template <>  rfk::Archetype const* rfk::getArchetype<Scripts::GridMeshShaderScript>() noexcept;\
template <>  rfk::Archetype const* rfk::getArchetype<Scripts::SkyboxMeshShaderScript>() noexcept;\
template <>  rfk::Archetype const* rfk::getArchetype<Scripts::DefaultScreenShaderScript>() noexcept;\


