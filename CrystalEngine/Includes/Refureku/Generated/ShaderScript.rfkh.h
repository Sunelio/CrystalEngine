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


#define Scripts_ShaderScript_GENERATED	\
RFK_UNPACK_IF_NOT_PARSING(friend rfk::internal::CodeGenerationHelpers;\
friend rfk::internal::implements_template1__rfk_registerChildClass<ShaderScript, void, void(rfk::Struct&)>; \
\
private: template <typename ChildClass> static void _rfk_registerChildClass(rfk::Struct& childClass) noexcept {\
rfk::Struct const& thisClass = staticGetArchetype();\
if constexpr (!std::is_same_v<ChildClass, ShaderScript>)const_cast<rfk::Struct&>(thisClass).addSubclass(childClass, rfk::internal::CodeGenerationHelpers::computeClassPointerOffset<ChildClass, ShaderScript>());\
else\
{\
childClass.setFieldsCapacity(3u + 0); childClass.setStaticFieldsCapacity(0u + 0); \
}\
[[maybe_unused]] rfk::Field* field = nullptr; [[maybe_unused]] rfk::StaticField* staticField = nullptr;\
)\
__RFK_DISABLE_WARNING_PUSH \
__RFK_DISABLE_WARNING_OFFSETOF \
RFK_UNPACK_IF_NOT_PARSING(field = childClass.addField("shader", std::hash<std::string>()(std::string("c:@N@Scripts@S@ShaderScript@FI@shader") + rfk::internal::getTypename<ChildClass>()), rfk::getType<const Resources::ShaderProgram *>(), static_cast<rfk::EFieldFlags>(1), offsetof(ChildClass, shader), &thisClass);\
field = childClass.addField("renderer", std::hash<std::string>()(std::string("c:@N@Scripts@S@ShaderScript@FI@renderer") + rfk::internal::getTypename<ChildClass>()), rfk::getType<const Render::Renderer *>(), static_cast<rfk::EFieldFlags>(1), offsetof(ChildClass, renderer), &thisClass);\
field = childClass.addField("engine", std::hash<std::string>()(std::string("c:@N@Scripts@S@ShaderScript@FI@engine") + rfk::internal::getTypename<ChildClass>()), rfk::getType<const Core::Engine *>(), static_cast<rfk::EFieldFlags>(1), offsetof(ChildClass, engine), &thisClass);\
)\
__RFK_DISABLE_WARNING_POP \
RFK_UNPACK_IF_NOT_PARSING(}\
\
public:  static rfk::Struct const& staticGetArchetype() noexcept;\
\
)\


#define Scripts_MeshShaderScript_GENERATED	\
RFK_UNPACK_IF_NOT_PARSING(friend rfk::internal::CodeGenerationHelpers;\
friend rfk::internal::implements_template1__rfk_registerChildClass<MeshShaderScript, void, void(rfk::Struct&)>; \
\
private: template <typename ChildClass> static void _rfk_registerChildClass(rfk::Struct& childClass) noexcept {\
rfk::Struct const& thisClass = staticGetArchetype();\
if constexpr (!std::is_same_v<ChildClass, MeshShaderScript>)const_cast<rfk::Struct&>(thisClass).addSubclass(childClass, rfk::internal::CodeGenerationHelpers::computeClassPointerOffset<ChildClass, MeshShaderScript>());\
else\
{\
childClass.setFieldsCapacity(5u + rfk::internal::CodeGenerationHelpers::getReflectedFieldsCount< Scripts::ShaderScript>()); childClass.setStaticFieldsCapacity(0u + rfk::internal::CodeGenerationHelpers::getReflectedStaticFieldsCount< Scripts::ShaderScript>()); \
}\
[[maybe_unused]] rfk::Field* field = nullptr; [[maybe_unused]] rfk::StaticField* staticField = nullptr;\
)\
__RFK_DISABLE_WARNING_PUSH \
__RFK_DISABLE_WARNING_OFFSETOF \
RFK_UNPACK_IF_NOT_PARSING(field = childClass.addField("subMesh", std::hash<std::string>()(std::string("c:@N@Scripts@S@MeshShaderScript@FI@subMesh") + rfk::internal::getTypename<ChildClass>()), rfk::getType<const Resources::SubMesh *>(), static_cast<rfk::EFieldFlags>(1), offsetof(ChildClass, subMesh), &thisClass);\
field = childClass.addField("material", std::hash<std::string>()(std::string("c:@N@Scripts@S@MeshShaderScript@FI@material") + rfk::internal::getTypename<ChildClass>()), rfk::getType<const Resources::Material *>(), static_cast<rfk::EFieldFlags>(1), offsetof(ChildClass, material), &thisClass);\
field = childClass.addField("modelMat", std::hash<std::string>()(std::string("c:@N@Scripts@S@MeshShaderScript@FI@modelMat") + rfk::internal::getTypename<ChildClass>()), rfk::getType<const Maths::Mat4 *>(), static_cast<rfk::EFieldFlags>(1), offsetof(ChildClass, modelMat), &thisClass);\
field = childClass.addField("camera", std::hash<std::string>()(std::string("c:@N@Scripts@S@MeshShaderScript@FI@camera") + rfk::internal::getTypename<ChildClass>()), rfk::getType<const Render::Camera *>(), static_cast<rfk::EFieldFlags>(1), offsetof(ChildClass, camera), &thisClass);\
field = childClass.addField("lightManager", std::hash<std::string>()(std::string("c:@N@Scripts@S@MeshShaderScript@FI@lightManager") + rfk::internal::getTypename<ChildClass>()), rfk::getType<const Render::LightManager *>(), static_cast<rfk::EFieldFlags>(1), offsetof(ChildClass, lightManager), &thisClass);\
)\
__RFK_DISABLE_WARNING_POP \
RFK_UNPACK_IF_NOT_PARSING(rfk::internal::CodeGenerationHelpers::registerChildClass<Scripts::ShaderScript, ChildClass>(childClass);\
}\
\
public:  static rfk::Struct const& staticGetArchetype() noexcept;\
\
)\


#define Scripts_ScreenShaderScript_GENERATED	\
RFK_UNPACK_IF_NOT_PARSING(friend rfk::internal::CodeGenerationHelpers;\
friend rfk::internal::implements_template1__rfk_registerChildClass<ScreenShaderScript, void, void(rfk::Struct&)>; \
\
private: template <typename ChildClass> static void _rfk_registerChildClass(rfk::Struct& childClass) noexcept {\
rfk::Struct const& thisClass = staticGetArchetype();\
if constexpr (!std::is_same_v<ChildClass, ScreenShaderScript>)const_cast<rfk::Struct&>(thisClass).addSubclass(childClass, rfk::internal::CodeGenerationHelpers::computeClassPointerOffset<ChildClass, ScreenShaderScript>());\
else\
{\
childClass.setFieldsCapacity(5u + rfk::internal::CodeGenerationHelpers::getReflectedFieldsCount< Scripts::ShaderScript>()); childClass.setStaticFieldsCapacity(0u + rfk::internal::CodeGenerationHelpers::getReflectedStaticFieldsCount< Scripts::ShaderScript>()); \
}\
[[maybe_unused]] rfk::Field* field = nullptr; [[maybe_unused]] rfk::StaticField* staticField = nullptr;\
)\
__RFK_DISABLE_WARNING_PUSH \
__RFK_DISABLE_WARNING_OFFSETOF \
RFK_UNPACK_IF_NOT_PARSING(field = childClass.addField("textureID", std::hash<std::string>()(std::string("c:@N@Scripts@S@ScreenShaderScript@FI@textureID") + rfk::internal::getTypename<ChildClass>()), rfk::getType<uint>(), static_cast<rfk::EFieldFlags>(1), offsetof(ChildClass, textureID), &thisClass);\
field = childClass.addField("rectPos", std::hash<std::string>()(std::string("c:@N@Scripts@S@ScreenShaderScript@FI@rectPos") + rfk::internal::getTypename<ChildClass>()), rfk::getType<Maths::Vector2>(), static_cast<rfk::EFieldFlags>(1), offsetof(ChildClass, rectPos), &thisClass);\
field = childClass.addField("rectSize", std::hash<std::string>()(std::string("c:@N@Scripts@S@ScreenShaderScript@FI@rectSize") + rfk::internal::getTypename<ChildClass>()), rfk::getType<Maths::Vector2>(), static_cast<rfk::EFieldFlags>(1), offsetof(ChildClass, rectSize), &thisClass);\
field = childClass.addField("screenSize", std::hash<std::string>()(std::string("c:@N@Scripts@S@ScreenShaderScript@FI@screenSize") + rfk::internal::getTypename<ChildClass>()), rfk::getType<Maths::Vector2>(), static_cast<rfk::EFieldFlags>(1), offsetof(ChildClass, screenSize), &thisClass);\
field = childClass.addField("postProcessor", std::hash<std::string>()(std::string("c:@N@Scripts@S@ScreenShaderScript@FI@postProcessor") + rfk::internal::getTypename<ChildClass>()), rfk::getType<const Render::PostProcessor *>(), static_cast<rfk::EFieldFlags>(1), offsetof(ChildClass, postProcessor), &thisClass);\
)\
__RFK_DISABLE_WARNING_POP \
RFK_UNPACK_IF_NOT_PARSING(rfk::internal::CodeGenerationHelpers::registerChildClass<Scripts::ShaderScript, ChildClass>(childClass);\
}\
\
public:  static rfk::Struct const& staticGetArchetype() noexcept;\
\
)\


#define File_ShaderScript_GENERATED	\
template <>  rfk::Archetype const* rfk::getArchetype<Scripts::ShaderScript>() noexcept;\
template <>  rfk::Archetype const* rfk::getArchetype<Scripts::MeshShaderScript>() noexcept;\
template <>  rfk::Archetype const* rfk::getArchetype<Scripts::ScreenShaderScript>() noexcept;\


