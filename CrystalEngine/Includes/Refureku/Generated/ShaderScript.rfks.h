#pragma once

#include "../../Scripts/ShaderScript.h"

#include <type_traits>
#include <Refureku/TypeInfo/Entity/DefaultEntityRegisterer.h>
#include <Refureku/TypeInfo/Archetypes/ArchetypeRegisterer.h>
#include <Refureku/TypeInfo/Namespace/Namespace.h>
#include <Refureku/TypeInfo/Namespace/NamespaceFragment.h>
#include <Refureku/TypeInfo/Namespace/NamespaceFragmentRegisterer.h>
#include <Refureku/TypeInfo/Archetypes/Template/TypeTemplateParameter.h>
#include <Refureku/TypeInfo/Archetypes/Template/NonTypeTemplateParameter.h>
#include <Refureku/TypeInfo/Archetypes/Template/TemplateTemplateParameter.h>

namespace rfk::generated { 
 static rfk::NamespaceFragment const& getNamespaceFragment_13014071540839220562u_5253629498757110390() noexcept {
static rfk::NamespaceFragment fragment("Scripts", 13014071540839220562u);
static bool initialized = false;
if (!initialized) {
initialized = true;
fragment.setNestedEntitiesCapacity(3u);
fragment.addNestedEntity(*rfk::getArchetype<Scripts::ShaderScript>());
fragment.addNestedEntity(*rfk::getArchetype<Scripts::MeshShaderScript>());
fragment.addNestedEntity(*rfk::getArchetype<Scripts::ScreenShaderScript>());
}
return fragment; }
static rfk::NamespaceFragmentRegisterer const namespaceFragmentRegisterer_13014071540839220562u_5253629498757110390(rfk::generated::getNamespaceFragment_13014071540839220562u_5253629498757110390());
 }
rfk::Struct const& Scripts::ShaderScript::staticGetArchetype() noexcept {
static bool initialized = false;
static rfk::Struct type("ShaderScript", 12033525625960550051u, sizeof(ShaderScript), 0);
if (!initialized) {
initialized = true;
ShaderScript::_rfk_registerChildClass<ShaderScript>(type);
static rfk::StaticMethod defaultSharedInstantiator("", 0u, rfk::getType<rfk::SharedPtr<ShaderScript>>(),new rfk::NonMemberFunction<rfk::SharedPtr<ShaderScript>()>(&rfk::internal::CodeGenerationHelpers::defaultSharedInstantiator<ShaderScript>),rfk::EMethodFlags::Default, nullptr);
type.addSharedInstantiator(defaultSharedInstantiator);
static rfk::StaticMethod defaultUniqueInstantiator("", 0u, rfk::getType<rfk::UniquePtr<ShaderScript>>(),new rfk::NonMemberFunction<rfk::UniquePtr<ShaderScript>()>(&rfk::internal::CodeGenerationHelpers::defaultUniqueInstantiator<ShaderScript>),rfk::EMethodFlags::Default, nullptr);
type.addUniqueInstantiator(defaultUniqueInstantiator);
type.setMethodsCapacity(0u); type.setStaticMethodsCapacity(0u); 
}
return type; }

template <> rfk::Archetype const* rfk::getArchetype<Scripts::ShaderScript>() noexcept { return &Scripts::ShaderScript::staticGetArchetype(); }

rfk::Struct const& Scripts::MeshShaderScript::staticGetArchetype() noexcept {
static bool initialized = false;
static rfk::Struct type("MeshShaderScript", 940664969152402266u, sizeof(MeshShaderScript), 0);
if (!initialized) {
initialized = true;
type.setDirectParentsCapacity(1);
type.addDirectParent(rfk::getArchetype<Scripts::ShaderScript>(), static_cast<rfk::EAccessSpecifier>(1));
MeshShaderScript::_rfk_registerChildClass<MeshShaderScript>(type);
static rfk::StaticMethod defaultSharedInstantiator("", 0u, rfk::getType<rfk::SharedPtr<MeshShaderScript>>(),new rfk::NonMemberFunction<rfk::SharedPtr<MeshShaderScript>()>(&rfk::internal::CodeGenerationHelpers::defaultSharedInstantiator<MeshShaderScript>),rfk::EMethodFlags::Default, nullptr);
type.addSharedInstantiator(defaultSharedInstantiator);
static rfk::StaticMethod defaultUniqueInstantiator("", 0u, rfk::getType<rfk::UniquePtr<MeshShaderScript>>(),new rfk::NonMemberFunction<rfk::UniquePtr<MeshShaderScript>()>(&rfk::internal::CodeGenerationHelpers::defaultUniqueInstantiator<MeshShaderScript>),rfk::EMethodFlags::Default, nullptr);
type.addUniqueInstantiator(defaultUniqueInstantiator);
type.setMethodsCapacity(0u); type.setStaticMethodsCapacity(0u); 
}
return type; }

template <> rfk::Archetype const* rfk::getArchetype<Scripts::MeshShaderScript>() noexcept { return &Scripts::MeshShaderScript::staticGetArchetype(); }

rfk::Struct const& Scripts::ScreenShaderScript::staticGetArchetype() noexcept {
static bool initialized = false;
static rfk::Struct type("ScreenShaderScript", 9159848383377547671u, sizeof(ScreenShaderScript), 0);
if (!initialized) {
initialized = true;
type.setDirectParentsCapacity(1);
type.addDirectParent(rfk::getArchetype<Scripts::ShaderScript>(), static_cast<rfk::EAccessSpecifier>(1));
ScreenShaderScript::_rfk_registerChildClass<ScreenShaderScript>(type);
static rfk::StaticMethod defaultSharedInstantiator("", 0u, rfk::getType<rfk::SharedPtr<ScreenShaderScript>>(),new rfk::NonMemberFunction<rfk::SharedPtr<ScreenShaderScript>()>(&rfk::internal::CodeGenerationHelpers::defaultSharedInstantiator<ScreenShaderScript>),rfk::EMethodFlags::Default, nullptr);
type.addSharedInstantiator(defaultSharedInstantiator);
static rfk::StaticMethod defaultUniqueInstantiator("", 0u, rfk::getType<rfk::UniquePtr<ScreenShaderScript>>(),new rfk::NonMemberFunction<rfk::UniquePtr<ScreenShaderScript>()>(&rfk::internal::CodeGenerationHelpers::defaultUniqueInstantiator<ScreenShaderScript>),rfk::EMethodFlags::Default, nullptr);
type.addUniqueInstantiator(defaultUniqueInstantiator);
type.setMethodsCapacity(0u); type.setStaticMethodsCapacity(0u); 
}
return type; }

template <> rfk::Archetype const* rfk::getArchetype<Scripts::ScreenShaderScript>() noexcept { return &Scripts::ScreenShaderScript::staticGetArchetype(); }


