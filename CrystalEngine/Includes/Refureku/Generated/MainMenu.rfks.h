#pragma once

#include "../../Scripts/MainMenu.h"

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
 static rfk::NamespaceFragment const& getNamespaceFragment_13014071540839220562u_11347176390895750094() noexcept {
static rfk::NamespaceFragment fragment("Scripts", 13014071540839220562u);
static bool initialized = false;
if (!initialized) {
initialized = true;
fragment.setNestedEntitiesCapacity(1u);
fragment.addNestedEntity(*rfk::getArchetype<Scripts::MainMenu>());
}
return fragment; }
static rfk::NamespaceFragmentRegisterer const namespaceFragmentRegisterer_13014071540839220562u_11347176390895750094(rfk::generated::getNamespaceFragment_13014071540839220562u_11347176390895750094());
 }
rfk::Struct const& Scripts::MainMenu::staticGetArchetype() noexcept {
static bool initialized = false;
static rfk::Struct type("MainMenu", 14614980728422222855u, sizeof(MainMenu), 0);
if (!initialized) {
initialized = true;
type.setDirectParentsCapacity(1);
type.addDirectParent(rfk::getArchetype<Scripts::ObjectScript>(), static_cast<rfk::EAccessSpecifier>(1));
MainMenu::_rfk_registerChildClass<MainMenu>(type);
static rfk::StaticMethod defaultSharedInstantiator("", 0u, rfk::getType<rfk::SharedPtr<MainMenu>>(),new rfk::NonMemberFunction<rfk::SharedPtr<MainMenu>()>(&rfk::internal::CodeGenerationHelpers::defaultSharedInstantiator<MainMenu>),rfk::EMethodFlags::Default, nullptr);
type.addSharedInstantiator(defaultSharedInstantiator);
static rfk::StaticMethod defaultUniqueInstantiator("", 0u, rfk::getType<rfk::UniquePtr<MainMenu>>(),new rfk::NonMemberFunction<rfk::UniquePtr<MainMenu>()>(&rfk::internal::CodeGenerationHelpers::defaultUniqueInstantiator<MainMenu>),rfk::EMethodFlags::Default, nullptr);
type.addUniqueInstantiator(defaultUniqueInstantiator);
[[maybe_unused]] rfk::Method* method = nullptr; [[maybe_unused]] rfk::StaticMethod* staticMethod = nullptr;
type.setMethodsCapacity(2u); type.setStaticMethodsCapacity(0u); 
method = type.addMethod("Start", 8755570019168211854u, rfk::getType<void>(), new rfk::MemberFunction<MainMenu, void ()>(static_cast<void (MainMenu::*)()>(& MainMenu::Start)), static_cast<rfk::EMethodFlags>(161));
method->inheritBaseMethodProperties();
method = type.addMethod("Update", 5600338811350203355u, rfk::getType<void>(), new rfk::MemberFunction<MainMenu, void ()>(static_cast<void (MainMenu::*)()>(& MainMenu::Update)), static_cast<rfk::EMethodFlags>(161));
method->inheritBaseMethodProperties();
}
return type; }

template <> rfk::Archetype const* rfk::getArchetype<Scripts::MainMenu>() noexcept { return &Scripts::MainMenu::staticGetArchetype(); }


