#pragma once

#include "../../Scripts/PlayerMovement.h"

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
 static rfk::NamespaceFragment const& getNamespaceFragment_13014071540839220562u_12141551223541155496() noexcept {
static rfk::NamespaceFragment fragment("Scripts", 13014071540839220562u);
static bool initialized = false;
if (!initialized) {
initialized = true;
fragment.setNestedEntitiesCapacity(1u);
fragment.addNestedEntity(*rfk::getArchetype<Scripts::PlayerMovement>());
}
return fragment; }
static rfk::NamespaceFragmentRegisterer const namespaceFragmentRegisterer_13014071540839220562u_12141551223541155496(rfk::generated::getNamespaceFragment_13014071540839220562u_12141551223541155496());
 }
rfk::Struct const& Scripts::PlayerMovement::staticGetArchetype() noexcept {
static bool initialized = false;
static rfk::Struct type("PlayerMovement", 3641450823726275197u, sizeof(PlayerMovement), 0);
if (!initialized) {
initialized = true;
type.setDirectParentsCapacity(1);
type.addDirectParent(rfk::getArchetype<Scripts::ObjectScript>(), static_cast<rfk::EAccessSpecifier>(1));
PlayerMovement::_rfk_registerChildClass<PlayerMovement>(type);
static rfk::StaticMethod defaultSharedInstantiator("", 0u, rfk::getType<rfk::SharedPtr<PlayerMovement>>(),new rfk::NonMemberFunction<rfk::SharedPtr<PlayerMovement>()>(&rfk::internal::CodeGenerationHelpers::defaultSharedInstantiator<PlayerMovement>),rfk::EMethodFlags::Default, nullptr);
type.addSharedInstantiator(defaultSharedInstantiator);
static rfk::StaticMethod defaultUniqueInstantiator("", 0u, rfk::getType<rfk::UniquePtr<PlayerMovement>>(),new rfk::NonMemberFunction<rfk::UniquePtr<PlayerMovement>()>(&rfk::internal::CodeGenerationHelpers::defaultUniqueInstantiator<PlayerMovement>),rfk::EMethodFlags::Default, nullptr);
type.addUniqueInstantiator(defaultUniqueInstantiator);
[[maybe_unused]] rfk::Method* method = nullptr; [[maybe_unused]] rfk::StaticMethod* staticMethod = nullptr;
type.setMethodsCapacity(2u); type.setStaticMethodsCapacity(0u); 
method = type.addMethod("Start", 573445713856718136u, rfk::getType<void>(), new rfk::MemberFunction<PlayerMovement, void ()>(static_cast<void (PlayerMovement::*)()>(& PlayerMovement::Start)), static_cast<rfk::EMethodFlags>(161));
method->inheritBaseMethodProperties();
method = type.addMethod("Update", 6736848392162226545u, rfk::getType<void>(), new rfk::MemberFunction<PlayerMovement, void ()>(static_cast<void (PlayerMovement::*)()>(& PlayerMovement::Update)), static_cast<rfk::EMethodFlags>(161));
method->inheritBaseMethodProperties();
}
return type; }

template <> rfk::Archetype const* rfk::getArchetype<Scripts::PlayerMovement>() noexcept { return &Scripts::PlayerMovement::staticGetArchetype(); }


