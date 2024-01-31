#pragma once

#include "../../Scripts/LuaScript.h"

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
 static rfk::NamespaceFragment const& getNamespaceFragment_13014071540839220562u_17768475009625190501() noexcept {
static rfk::NamespaceFragment fragment("Scripts", 13014071540839220562u);
static bool initialized = false;
if (!initialized) {
initialized = true;
fragment.setNestedEntitiesCapacity(1u);
fragment.addNestedEntity(*rfk::getArchetype<Scripts::LuaScript>());
}
return fragment; }
static rfk::NamespaceFragmentRegisterer const namespaceFragmentRegisterer_13014071540839220562u_17768475009625190501(rfk::generated::getNamespaceFragment_13014071540839220562u_17768475009625190501());
 }
rfk::Struct const& Scripts::LuaScript::staticGetArchetype() noexcept {
static bool initialized = false;
static rfk::Struct type("LuaScript", 12553192921614991206u, sizeof(LuaScript), 0);
if (!initialized) {
initialized = true;
type.setDirectParentsCapacity(1);
type.addDirectParent(rfk::getArchetype<Scripts::ObjectScript>(), static_cast<rfk::EAccessSpecifier>(1));
LuaScript::_rfk_registerChildClass<LuaScript>(type);
static rfk::StaticMethod defaultSharedInstantiator("", 0u, rfk::getType<rfk::SharedPtr<LuaScript>>(),new rfk::NonMemberFunction<rfk::SharedPtr<LuaScript>()>(&rfk::internal::CodeGenerationHelpers::defaultSharedInstantiator<LuaScript>),rfk::EMethodFlags::Default, nullptr);
type.addSharedInstantiator(defaultSharedInstantiator);
static rfk::StaticMethod defaultUniqueInstantiator("", 0u, rfk::getType<rfk::UniquePtr<LuaScript>>(),new rfk::NonMemberFunction<rfk::UniquePtr<LuaScript>()>(&rfk::internal::CodeGenerationHelpers::defaultUniqueInstantiator<LuaScript>),rfk::EMethodFlags::Default, nullptr);
type.addUniqueInstantiator(defaultUniqueInstantiator);
[[maybe_unused]] rfk::Method* method = nullptr; [[maybe_unused]] rfk::StaticMethod* staticMethod = nullptr;
type.setMethodsCapacity(7u); type.setStaticMethodsCapacity(2u); 
staticMethod = type.addStaticMethod("customInstantiator", 821485626117699432u, rfk::getType<rfk::UniquePtr<LuaScript>>(), new rfk::NonMemberFunction<rfk::UniquePtr<LuaScript> (const std::string &)>(& LuaScript::customInstantiator), static_cast<rfk::EMethodFlags>(25));
staticMethod->setParametersCapacity(1); 
staticMethod->addParameter("filename", 0u, rfk::getType<const std::string &>());

staticMethod->setPropertiesCapacity(1);
static_assert((rfk::Instantiator::targetEntityKind & rfk::EEntityKind::Method) != rfk::EEntityKind::Undefined, "[Refureku] rfk::Instantiator can't be applied to a rfk::EEntityKind::Method");static rfk::Instantiator property_821485626117699432u_0;staticMethod->addProperty(property_821485626117699432u_0);
type.addUniqueInstantiator(*staticMethod); staticMethod = type.addStaticMethod("CreateLuaFile", 88584167399226191u, rfk::getType<std::string>(), new rfk::NonMemberFunction<std::string (const std::string &)>(& LuaScript::CreateLuaFile), static_cast<rfk::EMethodFlags>(9));
staticMethod->setParametersCapacity(1); 
staticMethod->addParameter("fileName", 0u, rfk::getType<const std::string &>());

method = type.addMethod("Start", 17432500637479275617u, rfk::getType<void>(), new rfk::MemberFunction<LuaScript, void ()>(static_cast<void (LuaScript::*)()>(& LuaScript::Start)), static_cast<rfk::EMethodFlags>(161));
method->inheritBaseMethodProperties();
method = type.addMethod("LateStart", 17064532034052429989u, rfk::getType<void>(), new rfk::MemberFunction<LuaScript, void ()>(static_cast<void (LuaScript::*)()>(& LuaScript::LateStart)), static_cast<rfk::EMethodFlags>(161));
method->inheritBaseMethodProperties();
method = type.addMethod("Update", 7436067838498097314u, rfk::getType<void>(), new rfk::MemberFunction<LuaScript, void ()>(static_cast<void (LuaScript::*)()>(& LuaScript::Update)), static_cast<rfk::EMethodFlags>(161));
method->inheritBaseMethodProperties();
method = type.addMethod("LateUpdate", 9214916208708668974u, rfk::getType<void>(), new rfk::MemberFunction<LuaScript, void ()>(static_cast<void (LuaScript::*)()>(& LuaScript::LateUpdate)), static_cast<rfk::EMethodFlags>(161));
method->inheritBaseMethodProperties();
method = type.addMethod("SetValues", 6636635812847768675u, rfk::getType<void>(), new rfk::MemberFunction<LuaScript, void ()>(static_cast<void (LuaScript::*)()>(& LuaScript::SetValues)), static_cast<rfk::EMethodFlags>(1));
method = type.addMethod("GetState", 14625520410569073721u, rfk::getType<sol::state *>(), new rfk::MemberFunction<LuaScript, sol::state *()>(static_cast<sol::state *(LuaScript::*)() const>(& LuaScript::GetState)), static_cast<rfk::EMethodFlags>(529));
method = type.addMethod("GetName", 9594978139342300499u, rfk::getType<std::string>(), new rfk::MemberFunction<LuaScript, std::string ()>(static_cast<std::string (LuaScript::*)() const>(& LuaScript::GetName)), static_cast<rfk::EMethodFlags>(529));
}
return type; }

template <> rfk::Archetype const* rfk::getArchetype<Scripts::LuaScript>() noexcept { return &Scripts::LuaScript::staticGetArchetype(); }


