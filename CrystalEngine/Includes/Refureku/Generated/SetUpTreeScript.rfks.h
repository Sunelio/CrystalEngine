#pragma once

#include "../../Scripts/SetUpTreeScript.h"

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
 static rfk::NamespaceFragment const& getNamespaceFragment_13014071540839220562u_10152472944776147254() noexcept {
static rfk::NamespaceFragment fragment("Scripts", 13014071540839220562u);
static bool initialized = false;
if (!initialized) {
initialized = true;
fragment.setNestedEntitiesCapacity(1u);
fragment.addNestedEntity(*rfk::getArchetype<Scripts::SetUpTreeScript>());
}
return fragment; }
static rfk::NamespaceFragmentRegisterer const namespaceFragmentRegisterer_13014071540839220562u_10152472944776147254(rfk::generated::getNamespaceFragment_13014071540839220562u_10152472944776147254());
 }
rfk::Struct const& Scripts::SetUpTreeScript::staticGetArchetype() noexcept {
static bool initialized = false;
static rfk::Struct type("SetUpTreeScript", 13702942761220615257u, sizeof(SetUpTreeScript), 0);
if (!initialized) {
initialized = true;
type.setDirectParentsCapacity(1);
type.addDirectParent(rfk::getArchetype<Scripts::ObjectScript>(), static_cast<rfk::EAccessSpecifier>(1));
SetUpTreeScript::_rfk_registerChildClass<SetUpTreeScript>(type);
static rfk::StaticMethod defaultSharedInstantiator("", 0u, rfk::getType<rfk::SharedPtr<SetUpTreeScript>>(),new rfk::NonMemberFunction<rfk::SharedPtr<SetUpTreeScript>()>(&rfk::internal::CodeGenerationHelpers::defaultSharedInstantiator<SetUpTreeScript>),rfk::EMethodFlags::Default, nullptr);
type.addSharedInstantiator(defaultSharedInstantiator);
static rfk::StaticMethod defaultUniqueInstantiator("", 0u, rfk::getType<rfk::UniquePtr<SetUpTreeScript>>(),new rfk::NonMemberFunction<rfk::UniquePtr<SetUpTreeScript>()>(&rfk::internal::CodeGenerationHelpers::defaultUniqueInstantiator<SetUpTreeScript>),rfk::EMethodFlags::Default, nullptr);
type.addUniqueInstantiator(defaultUniqueInstantiator);
[[maybe_unused]] rfk::Method* method = nullptr; [[maybe_unused]] rfk::StaticMethod* staticMethod = nullptr;
type.setMethodsCapacity(1u); type.setStaticMethodsCapacity(0u); 
method = type.addMethod("Start", 10415294720769599860u, rfk::getType<void>(), new rfk::MemberFunction<SetUpTreeScript, void ()>(static_cast<void (SetUpTreeScript::*)()>(& SetUpTreeScript::Start)), static_cast<rfk::EMethodFlags>(161));
method->inheritBaseMethodProperties();
}
return type; }

template <> rfk::Archetype const* rfk::getArchetype<Scripts::SetUpTreeScript>() noexcept { return &Scripts::SetUpTreeScript::staticGetArchetype(); }


