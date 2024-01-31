#pragma once

#include "../../Scripts/PythonScript.h"

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
 static rfk::NamespaceFragment const& getNamespaceFragment_13014071540839220562u_8890590396464356325() noexcept {
static rfk::NamespaceFragment fragment("Scripts", 13014071540839220562u);
static bool initialized = false;
if (!initialized) {
initialized = true;
fragment.setNestedEntitiesCapacity(1u);
fragment.addNestedEntity(*rfk::getArchetype<Scripts::PythonScript>());
}
return fragment; }
static rfk::NamespaceFragmentRegisterer const namespaceFragmentRegisterer_13014071540839220562u_8890590396464356325(rfk::generated::getNamespaceFragment_13014071540839220562u_8890590396464356325());
 }
rfk::Struct const& Scripts::PythonScript::staticGetArchetype() noexcept {
static bool initialized = false;
static rfk::Struct type("PythonScript", 7766745968041312168u, sizeof(PythonScript), 0);
if (!initialized) {
initialized = true;
type.setDirectParentsCapacity(1);
type.addDirectParent(rfk::getArchetype<Scripts::ObjectScript>(), static_cast<rfk::EAccessSpecifier>(1));
PythonScript::_rfk_registerChildClass<PythonScript>(type);
static rfk::StaticMethod defaultSharedInstantiator("", 0u, rfk::getType<rfk::SharedPtr<PythonScript>>(),new rfk::NonMemberFunction<rfk::SharedPtr<PythonScript>()>(&rfk::internal::CodeGenerationHelpers::defaultSharedInstantiator<PythonScript>),rfk::EMethodFlags::Default, nullptr);
type.addSharedInstantiator(defaultSharedInstantiator);
static rfk::StaticMethod defaultUniqueInstantiator("", 0u, rfk::getType<rfk::UniquePtr<PythonScript>>(),new rfk::NonMemberFunction<rfk::UniquePtr<PythonScript>()>(&rfk::internal::CodeGenerationHelpers::defaultUniqueInstantiator<PythonScript>),rfk::EMethodFlags::Default, nullptr);
type.addUniqueInstantiator(defaultUniqueInstantiator);
[[maybe_unused]] rfk::Method* method = nullptr; [[maybe_unused]] rfk::StaticMethod* staticMethod = nullptr;
type.setMethodsCapacity(6u); type.setStaticMethodsCapacity(2u); 
staticMethod = type.addStaticMethod("customInstantiator", 17879303328412823366u, rfk::getType<rfk::UniquePtr<PythonScript>>(), new rfk::NonMemberFunction<rfk::UniquePtr<PythonScript> (const std::string &)>(& PythonScript::customInstantiator), static_cast<rfk::EMethodFlags>(25));
staticMethod->setParametersCapacity(1); 
staticMethod->addParameter("filename", 0u, rfk::getType<const std::string &>());

staticMethod->setPropertiesCapacity(1);
static_assert((rfk::Instantiator::targetEntityKind & rfk::EEntityKind::Method) != rfk::EEntityKind::Undefined, "[Refureku] rfk::Instantiator can't be applied to a rfk::EEntityKind::Method");static rfk::Instantiator property_17879303328412823366u_0;staticMethod->addProperty(property_17879303328412823366u_0);
type.addUniqueInstantiator(*staticMethod); staticMethod = type.addStaticMethod("CreatePyFile", 17285296582233871052u, rfk::getType<std::string>(), new rfk::NonMemberFunction<std::string (const std::string &)>(& PythonScript::CreatePyFile), static_cast<rfk::EMethodFlags>(9));
staticMethod->setParametersCapacity(1); 
staticMethod->addParameter("fileName", 0u, rfk::getType<const std::string &>());

method = type.addMethod("Start", 11293978269781643719u, rfk::getType<void>(), new rfk::MemberFunction<PythonScript, void ()>(static_cast<void (PythonScript::*)()>(& PythonScript::Start)), static_cast<rfk::EMethodFlags>(161));
method->inheritBaseMethodProperties();
method = type.addMethod("LateStart", 10428589515421157579u, rfk::getType<void>(), new rfk::MemberFunction<PythonScript, void ()>(static_cast<void (PythonScript::*)()>(& PythonScript::LateStart)), static_cast<rfk::EMethodFlags>(161));
method->inheritBaseMethodProperties();
method = type.addMethod("Update", 9716083177762073852u, rfk::getType<void>(), new rfk::MemberFunction<PythonScript, void ()>(static_cast<void (PythonScript::*)()>(& PythonScript::Update)), static_cast<rfk::EMethodFlags>(161));
method->inheritBaseMethodProperties();
method = type.addMethod("LateUpdate", 15019063445476632216u, rfk::getType<void>(), new rfk::MemberFunction<PythonScript, void ()>(static_cast<void (PythonScript::*)()>(& PythonScript::LateUpdate)), static_cast<rfk::EMethodFlags>(161));
method->inheritBaseMethodProperties();
method = type.addMethod("GetObject", 5445895346226065093u, rfk::getType<pybind11::object *>(), new rfk::MemberFunction<PythonScript, pybind11::object *()>(static_cast<pybind11::object *(PythonScript::*)() const>(& PythonScript::GetObject)), static_cast<rfk::EMethodFlags>(529));
method = type.addMethod("GetName", 17899008243929388725u, rfk::getType<std::string>(), new rfk::MemberFunction<PythonScript, std::string ()>(static_cast<std::string (PythonScript::*)() const>(& PythonScript::GetName)), static_cast<rfk::EMethodFlags>(529));
}
return type; }

template <> rfk::Archetype const* rfk::getArchetype<Scripts::PythonScript>() noexcept { return &Scripts::PythonScript::staticGetArchetype(); }


