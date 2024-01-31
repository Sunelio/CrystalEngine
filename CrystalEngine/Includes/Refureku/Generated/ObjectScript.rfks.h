#pragma once

#include "../../Scripts/ObjectScript.h"

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
 static rfk::NamespaceFragment const& getNamespaceFragment_13014071540839220562u_10561717960963077940() noexcept {
static rfk::NamespaceFragment fragment("Scripts", 13014071540839220562u);
static bool initialized = false;
if (!initialized) {
initialized = true;
fragment.setNestedEntitiesCapacity(1u);
fragment.addNestedEntity(*rfk::getArchetype<Scripts::ObjectScript>());
}
return fragment; }
static rfk::NamespaceFragmentRegisterer const namespaceFragmentRegisterer_13014071540839220562u_10561717960963077940(rfk::generated::getNamespaceFragment_13014071540839220562u_10561717960963077940());
 }
rfk::Struct const& Scripts::ObjectScript::staticGetArchetype() noexcept {
static bool initialized = false;
static rfk::Struct type("ObjectScript", 4594340287538102857u, sizeof(ObjectScript), 0);
if (!initialized) {
initialized = true;
ObjectScript::_rfk_registerChildClass<ObjectScript>(type);
static rfk::StaticMethod defaultSharedInstantiator("", 0u, rfk::getType<rfk::SharedPtr<ObjectScript>>(),new rfk::NonMemberFunction<rfk::SharedPtr<ObjectScript>()>(&rfk::internal::CodeGenerationHelpers::defaultSharedInstantiator<ObjectScript>),rfk::EMethodFlags::Default, nullptr);
type.addSharedInstantiator(defaultSharedInstantiator);
static rfk::StaticMethod defaultUniqueInstantiator("", 0u, rfk::getType<rfk::UniquePtr<ObjectScript>>(),new rfk::NonMemberFunction<rfk::UniquePtr<ObjectScript>()>(&rfk::internal::CodeGenerationHelpers::defaultUniqueInstantiator<ObjectScript>),rfk::EMethodFlags::Default, nullptr);
type.addUniqueInstantiator(defaultUniqueInstantiator);
[[maybe_unused]] rfk::Method* method = nullptr; [[maybe_unused]] rfk::StaticMethod* staticMethod = nullptr;
type.setMethodsCapacity(5u); type.setStaticMethodsCapacity(0u); 
method = type.addMethod("Start", 5860818235174737860u, rfk::getType<void>(), new rfk::MemberFunction<ObjectScript, void ()>(static_cast<void (ObjectScript::*)()>(& ObjectScript::Start)), static_cast<rfk::EMethodFlags>(49));
method = type.addMethod("LateStart", 11217714994061018656u, rfk::getType<void>(), new rfk::MemberFunction<ObjectScript, void ()>(static_cast<void (ObjectScript::*)()>(& ObjectScript::LateStart)), static_cast<rfk::EMethodFlags>(49));
method = type.addMethod("Update", 3162879846934400453u, rfk::getType<void>(), new rfk::MemberFunction<ObjectScript, void ()>(static_cast<void (ObjectScript::*)()>(& ObjectScript::Update)), static_cast<rfk::EMethodFlags>(49));
method = type.addMethod("LateUpdate", 4021773355047057097u, rfk::getType<void>(), new rfk::MemberFunction<ObjectScript, void ()>(static_cast<void (ObjectScript::*)()>(& ObjectScript::LateUpdate)), static_cast<rfk::EMethodFlags>(49));
method = type.addMethod("GetArchId", 15730833245051527794u, rfk::getType<size_t>(), new rfk::MemberFunction<ObjectScript, size_t ()>(static_cast<size_t (ObjectScript::*)() const>(& ObjectScript::GetArchId)), static_cast<rfk::EMethodFlags>(529));
}
return type; }

template <> rfk::Archetype const* rfk::getArchetype<Scripts::ObjectScript>() noexcept { return &Scripts::ObjectScript::staticGetArchetype(); }


