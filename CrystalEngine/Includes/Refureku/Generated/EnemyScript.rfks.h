#pragma once

#include "../../Scripts/EnemyScript.h"

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
 static rfk::NamespaceFragment const& getNamespaceFragment_13014071540839220562u_5734254785199574631() noexcept {
static rfk::NamespaceFragment fragment("Scripts", 13014071540839220562u);
static bool initialized = false;
if (!initialized) {
initialized = true;
fragment.setNestedEntitiesCapacity(1u);
fragment.addNestedEntity(*rfk::getArchetype<Scripts::EnemyScript>());
}
return fragment; }
static rfk::NamespaceFragmentRegisterer const namespaceFragmentRegisterer_13014071540839220562u_5734254785199574631(rfk::generated::getNamespaceFragment_13014071540839220562u_5734254785199574631());
 }
rfk::Struct const& Scripts::EnemyScript::staticGetArchetype() noexcept {
static bool initialized = false;
static rfk::Struct type("EnemyScript", 4688438574914726708u, sizeof(EnemyScript), 0);
if (!initialized) {
initialized = true;
type.setDirectParentsCapacity(1);
type.addDirectParent(rfk::getArchetype<Scripts::ObjectScript>(), static_cast<rfk::EAccessSpecifier>(1));
EnemyScript::_rfk_registerChildClass<EnemyScript>(type);
static rfk::StaticMethod defaultSharedInstantiator("", 0u, rfk::getType<rfk::SharedPtr<EnemyScript>>(),new rfk::NonMemberFunction<rfk::SharedPtr<EnemyScript>()>(&rfk::internal::CodeGenerationHelpers::defaultSharedInstantiator<EnemyScript>),rfk::EMethodFlags::Default, nullptr);
type.addSharedInstantiator(defaultSharedInstantiator);
static rfk::StaticMethod defaultUniqueInstantiator("", 0u, rfk::getType<rfk::UniquePtr<EnemyScript>>(),new rfk::NonMemberFunction<rfk::UniquePtr<EnemyScript>()>(&rfk::internal::CodeGenerationHelpers::defaultUniqueInstantiator<EnemyScript>),rfk::EMethodFlags::Default, nullptr);
type.addUniqueInstantiator(defaultUniqueInstantiator);
[[maybe_unused]] rfk::Method* method = nullptr; [[maybe_unused]] rfk::StaticMethod* staticMethod = nullptr;
type.setMethodsCapacity(4u); type.setStaticMethodsCapacity(0u); 
method = type.addMethod("IdleMode", 7019794456971169012u, rfk::getType<void>(), new rfk::MemberFunction<EnemyScript, void ()>(static_cast<void (EnemyScript::*)()>(& EnemyScript::IdleMode)), static_cast<rfk::EMethodFlags>(1));
method = type.addMethod("ChaseMode", 8896419677590533636u, rfk::getType<void>(), new rfk::MemberFunction<EnemyScript, void ()>(static_cast<void (EnemyScript::*)()>(& EnemyScript::ChaseMode)), static_cast<rfk::EMethodFlags>(1));
method = type.addMethod("Start", 1785175826989520739u, rfk::getType<void>(), new rfk::MemberFunction<EnemyScript, void ()>(static_cast<void (EnemyScript::*)()>(& EnemyScript::Start)), static_cast<rfk::EMethodFlags>(161));
method->inheritBaseMethodProperties();
method = type.addMethod("Update", 1893957821149637920u, rfk::getType<void>(), new rfk::MemberFunction<EnemyScript, void ()>(static_cast<void (EnemyScript::*)()>(& EnemyScript::Update)), static_cast<rfk::EMethodFlags>(161));
method->inheritBaseMethodProperties();
}
return type; }

template <> rfk::Archetype const* rfk::getArchetype<Scripts::EnemyScript>() noexcept { return &Scripts::EnemyScript::staticGetArchetype(); }


