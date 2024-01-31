#pragma once

#include "../../Scripts/DefaultScripts.h"

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
 static rfk::NamespaceFragment const& getNamespaceFragment_13014071540839220562u_12202216559138300075() noexcept {
static rfk::NamespaceFragment fragment("Scripts", 13014071540839220562u);
static bool initialized = false;
if (!initialized) {
initialized = true;
fragment.setNestedEntitiesCapacity(5u);
fragment.addNestedEntity(*rfk::getArchetype<Scripts::RotateObject>());
fragment.addNestedEntity(*rfk::getArchetype<Scripts::DefaultMeshShaderScript>());
fragment.addNestedEntity(*rfk::getArchetype<Scripts::GridMeshShaderScript>());
fragment.addNestedEntity(*rfk::getArchetype<Scripts::SkyboxMeshShaderScript>());
fragment.addNestedEntity(*rfk::getArchetype<Scripts::DefaultScreenShaderScript>());
}
return fragment; }
static rfk::NamespaceFragmentRegisterer const namespaceFragmentRegisterer_13014071540839220562u_12202216559138300075(rfk::generated::getNamespaceFragment_13014071540839220562u_12202216559138300075());
 }
rfk::Struct const& Scripts::RotateObject::staticGetArchetype() noexcept {
static bool initialized = false;
static rfk::Struct type("RotateObject", 13500217998530388985u, sizeof(RotateObject), 0);
if (!initialized) {
initialized = true;
type.setDirectParentsCapacity(1);
type.addDirectParent(rfk::getArchetype<Scripts::ObjectScript>(), static_cast<rfk::EAccessSpecifier>(1));
RotateObject::_rfk_registerChildClass<RotateObject>(type);
static rfk::StaticMethod defaultSharedInstantiator("", 0u, rfk::getType<rfk::SharedPtr<RotateObject>>(),new rfk::NonMemberFunction<rfk::SharedPtr<RotateObject>()>(&rfk::internal::CodeGenerationHelpers::defaultSharedInstantiator<RotateObject>),rfk::EMethodFlags::Default, nullptr);
type.addSharedInstantiator(defaultSharedInstantiator);
static rfk::StaticMethod defaultUniqueInstantiator("", 0u, rfk::getType<rfk::UniquePtr<RotateObject>>(),new rfk::NonMemberFunction<rfk::UniquePtr<RotateObject>()>(&rfk::internal::CodeGenerationHelpers::defaultUniqueInstantiator<RotateObject>),rfk::EMethodFlags::Default, nullptr);
type.addUniqueInstantiator(defaultUniqueInstantiator);
[[maybe_unused]] rfk::Method* method = nullptr; [[maybe_unused]] rfk::StaticMethod* staticMethod = nullptr;
type.setMethodsCapacity(1u); type.setStaticMethodsCapacity(0u); 
method = type.addMethod("Update", 14938981434523277141u, rfk::getType<void>(), new rfk::MemberFunction<RotateObject, void ()>(static_cast<void (RotateObject::*)()>(& RotateObject::Update)), static_cast<rfk::EMethodFlags>(161));
method->inheritBaseMethodProperties();
}
return type; }

template <> rfk::Archetype const* rfk::getArchetype<Scripts::RotateObject>() noexcept { return &Scripts::RotateObject::staticGetArchetype(); }

rfk::Struct const& Scripts::DefaultMeshShaderScript::staticGetArchetype() noexcept {
static bool initialized = false;
static rfk::Struct type("DefaultMeshShaderScript", 11106507401630149025u, sizeof(DefaultMeshShaderScript), 0);
if (!initialized) {
initialized = true;
type.setDirectParentsCapacity(1);
type.addDirectParent(rfk::getArchetype<Scripts::MeshShaderScript>(), static_cast<rfk::EAccessSpecifier>(1));
DefaultMeshShaderScript::_rfk_registerChildClass<DefaultMeshShaderScript>(type);
static rfk::StaticMethod defaultSharedInstantiator("", 0u, rfk::getType<rfk::SharedPtr<DefaultMeshShaderScript>>(),new rfk::NonMemberFunction<rfk::SharedPtr<DefaultMeshShaderScript>()>(&rfk::internal::CodeGenerationHelpers::defaultSharedInstantiator<DefaultMeshShaderScript>),rfk::EMethodFlags::Default, nullptr);
type.addSharedInstantiator(defaultSharedInstantiator);
static rfk::StaticMethod defaultUniqueInstantiator("", 0u, rfk::getType<rfk::UniquePtr<DefaultMeshShaderScript>>(),new rfk::NonMemberFunction<rfk::UniquePtr<DefaultMeshShaderScript>()>(&rfk::internal::CodeGenerationHelpers::defaultUniqueInstantiator<DefaultMeshShaderScript>),rfk::EMethodFlags::Default, nullptr);
type.addUniqueInstantiator(defaultUniqueInstantiator);
[[maybe_unused]] rfk::Method* method = nullptr; [[maybe_unused]] rfk::StaticMethod* staticMethod = nullptr;
type.setMethodsCapacity(1u); type.setStaticMethodsCapacity(0u); 
method = type.addMethod("SetShaderVariables", 7609267691533302992u, rfk::getType<void>(), new rfk::MemberFunction<DefaultMeshShaderScript, void ()>(static_cast<void (DefaultMeshShaderScript::*)()>(& DefaultMeshShaderScript::SetShaderVariables)), static_cast<rfk::EMethodFlags>(161));
method->inheritBaseMethodProperties();
}
return type; }

template <> rfk::Archetype const* rfk::getArchetype<Scripts::DefaultMeshShaderScript>() noexcept { return &Scripts::DefaultMeshShaderScript::staticGetArchetype(); }

rfk::Struct const& Scripts::GridMeshShaderScript::staticGetArchetype() noexcept {
static bool initialized = false;
static rfk::Struct type("GridMeshShaderScript", 745563347051526254u, sizeof(GridMeshShaderScript), 0);
if (!initialized) {
initialized = true;
type.setDirectParentsCapacity(1);
type.addDirectParent(rfk::getArchetype<Scripts::MeshShaderScript>(), static_cast<rfk::EAccessSpecifier>(1));
GridMeshShaderScript::_rfk_registerChildClass<GridMeshShaderScript>(type);
static rfk::StaticMethod defaultSharedInstantiator("", 0u, rfk::getType<rfk::SharedPtr<GridMeshShaderScript>>(),new rfk::NonMemberFunction<rfk::SharedPtr<GridMeshShaderScript>()>(&rfk::internal::CodeGenerationHelpers::defaultSharedInstantiator<GridMeshShaderScript>),rfk::EMethodFlags::Default, nullptr);
type.addSharedInstantiator(defaultSharedInstantiator);
static rfk::StaticMethod defaultUniqueInstantiator("", 0u, rfk::getType<rfk::UniquePtr<GridMeshShaderScript>>(),new rfk::NonMemberFunction<rfk::UniquePtr<GridMeshShaderScript>()>(&rfk::internal::CodeGenerationHelpers::defaultUniqueInstantiator<GridMeshShaderScript>),rfk::EMethodFlags::Default, nullptr);
type.addUniqueInstantiator(defaultUniqueInstantiator);
[[maybe_unused]] rfk::Method* method = nullptr; [[maybe_unused]] rfk::StaticMethod* staticMethod = nullptr;
type.setMethodsCapacity(1u); type.setStaticMethodsCapacity(0u); 
method = type.addMethod("SetShaderVariables", 13686239516247808131u, rfk::getType<void>(), new rfk::MemberFunction<GridMeshShaderScript, void ()>(static_cast<void (GridMeshShaderScript::*)()>(& GridMeshShaderScript::SetShaderVariables)), static_cast<rfk::EMethodFlags>(161));
method->inheritBaseMethodProperties();
}
return type; }

template <> rfk::Archetype const* rfk::getArchetype<Scripts::GridMeshShaderScript>() noexcept { return &Scripts::GridMeshShaderScript::staticGetArchetype(); }

rfk::Struct const& Scripts::SkyboxMeshShaderScript::staticGetArchetype() noexcept {
static bool initialized = false;
static rfk::Struct type("SkyboxMeshShaderScript", 7137916971729792256u, sizeof(SkyboxMeshShaderScript), 0);
if (!initialized) {
initialized = true;
type.setDirectParentsCapacity(1);
type.addDirectParent(rfk::getArchetype<Scripts::MeshShaderScript>(), static_cast<rfk::EAccessSpecifier>(1));
SkyboxMeshShaderScript::_rfk_registerChildClass<SkyboxMeshShaderScript>(type);
static rfk::StaticMethod defaultSharedInstantiator("", 0u, rfk::getType<rfk::SharedPtr<SkyboxMeshShaderScript>>(),new rfk::NonMemberFunction<rfk::SharedPtr<SkyboxMeshShaderScript>()>(&rfk::internal::CodeGenerationHelpers::defaultSharedInstantiator<SkyboxMeshShaderScript>),rfk::EMethodFlags::Default, nullptr);
type.addSharedInstantiator(defaultSharedInstantiator);
static rfk::StaticMethod defaultUniqueInstantiator("", 0u, rfk::getType<rfk::UniquePtr<SkyboxMeshShaderScript>>(),new rfk::NonMemberFunction<rfk::UniquePtr<SkyboxMeshShaderScript>()>(&rfk::internal::CodeGenerationHelpers::defaultUniqueInstantiator<SkyboxMeshShaderScript>),rfk::EMethodFlags::Default, nullptr);
type.addUniqueInstantiator(defaultUniqueInstantiator);
[[maybe_unused]] rfk::Method* method = nullptr; [[maybe_unused]] rfk::StaticMethod* staticMethod = nullptr;
type.setMethodsCapacity(1u); type.setStaticMethodsCapacity(0u); 
method = type.addMethod("SetShaderVariables", 7602762210731550529u, rfk::getType<void>(), new rfk::MemberFunction<SkyboxMeshShaderScript, void ()>(static_cast<void (SkyboxMeshShaderScript::*)()>(& SkyboxMeshShaderScript::SetShaderVariables)), static_cast<rfk::EMethodFlags>(161));
method->inheritBaseMethodProperties();
}
return type; }

template <> rfk::Archetype const* rfk::getArchetype<Scripts::SkyboxMeshShaderScript>() noexcept { return &Scripts::SkyboxMeshShaderScript::staticGetArchetype(); }

rfk::Struct const& Scripts::DefaultScreenShaderScript::staticGetArchetype() noexcept {
static bool initialized = false;
static rfk::Struct type("DefaultScreenShaderScript", 15755420364495242212u, sizeof(DefaultScreenShaderScript), 0);
if (!initialized) {
initialized = true;
type.setDirectParentsCapacity(1);
type.addDirectParent(rfk::getArchetype<Scripts::ScreenShaderScript>(), static_cast<rfk::EAccessSpecifier>(1));
DefaultScreenShaderScript::_rfk_registerChildClass<DefaultScreenShaderScript>(type);
static rfk::StaticMethod defaultSharedInstantiator("", 0u, rfk::getType<rfk::SharedPtr<DefaultScreenShaderScript>>(),new rfk::NonMemberFunction<rfk::SharedPtr<DefaultScreenShaderScript>()>(&rfk::internal::CodeGenerationHelpers::defaultSharedInstantiator<DefaultScreenShaderScript>),rfk::EMethodFlags::Default, nullptr);
type.addSharedInstantiator(defaultSharedInstantiator);
static rfk::StaticMethod defaultUniqueInstantiator("", 0u, rfk::getType<rfk::UniquePtr<DefaultScreenShaderScript>>(),new rfk::NonMemberFunction<rfk::UniquePtr<DefaultScreenShaderScript>()>(&rfk::internal::CodeGenerationHelpers::defaultUniqueInstantiator<DefaultScreenShaderScript>),rfk::EMethodFlags::Default, nullptr);
type.addUniqueInstantiator(defaultUniqueInstantiator);
[[maybe_unused]] rfk::Method* method = nullptr; [[maybe_unused]] rfk::StaticMethod* staticMethod = nullptr;
type.setMethodsCapacity(1u); type.setStaticMethodsCapacity(0u); 
method = type.addMethod("SetShaderVariables", 9044782638071516997u, rfk::getType<void>(), new rfk::MemberFunction<DefaultScreenShaderScript, void ()>(static_cast<void (DefaultScreenShaderScript::*)()>(& DefaultScreenShaderScript::SetShaderVariables)), static_cast<rfk::EMethodFlags>(161));
method->inheritBaseMethodProperties();
}
return type; }

template <> rfk::Archetype const* rfk::getArchetype<Scripts::DefaultScreenShaderScript>() noexcept { return &Scripts::DefaultScreenShaderScript::staticGetArchetype(); }


