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


#define Scripts_EnemyScript_GENERATED	\
RFK_UNPACK_IF_NOT_PARSING(friend rfk::internal::CodeGenerationHelpers;\
friend rfk::internal::implements_template1__rfk_registerChildClass<EnemyScript, void, void(rfk::Struct&)>; \
\
private: template <typename ChildClass> static void _rfk_registerChildClass(rfk::Struct& childClass) noexcept {\
rfk::Struct const& thisClass = staticGetArchetype();\
if constexpr (!std::is_same_v<ChildClass, EnemyScript>)const_cast<rfk::Struct&>(thisClass).addSubclass(childClass, rfk::internal::CodeGenerationHelpers::computeClassPointerOffset<ChildClass, EnemyScript>());\
else\
{\
childClass.setFieldsCapacity(17u + rfk::internal::CodeGenerationHelpers::getReflectedFieldsCount< Scripts::ObjectScript>()); childClass.setStaticFieldsCapacity(0u + rfk::internal::CodeGenerationHelpers::getReflectedStaticFieldsCount< Scripts::ObjectScript>()); \
}\
[[maybe_unused]] rfk::Field* field = nullptr; [[maybe_unused]] rfk::StaticField* staticField = nullptr;\
)\
__RFK_DISABLE_WARNING_PUSH \
__RFK_DISABLE_WARNING_OFFSETOF \
RFK_UNPACK_IF_NOT_PARSING(field = childClass.addField("playerDetected", std::hash<std::string>()(std::string("c:@N@Scripts@S@EnemyScript@FI@playerDetected") + rfk::internal::getTypename<ChildClass>()), rfk::getType<bool>(), static_cast<rfk::EFieldFlags>(1), offsetof(ChildClass, playerDetected), &thisClass);\
field = childClass.addField("chasing", std::hash<std::string>()(std::string("c:@N@Scripts@S@EnemyScript@FI@chasing") + rfk::internal::getTypename<ChildClass>()), rfk::getType<bool>(), static_cast<rfk::EFieldFlags>(1), offsetof(ChildClass, chasing), &thisClass);\
field = childClass.addField("speed", std::hash<std::string>()(std::string("c:@N@Scripts@S@EnemyScript@FI@speed") + rfk::internal::getTypename<ChildClass>()), rfk::getType<float>(), static_cast<rfk::EFieldFlags>(1), offsetof(ChildClass, speed), &thisClass);\
field = childClass.addField("killRadius", std::hash<std::string>()(std::string("c:@N@Scripts@S@EnemyScript@FI@killRadius") + rfk::internal::getTypename<ChildClass>()), rfk::getType<float>(), static_cast<rfk::EFieldFlags>(1), offsetof(ChildClass, killRadius), &thisClass);\
field = childClass.addField("detectRadius", std::hash<std::string>()(std::string("c:@N@Scripts@S@EnemyScript@FI@detectRadius") + rfk::internal::getTypename<ChildClass>()), rfk::getType<float>(), static_cast<rfk::EFieldFlags>(1), offsetof(ChildClass, detectRadius), &thisClass);\
field = childClass.addField("teleportRadius", std::hash<std::string>()(std::string("c:@N@Scripts@S@EnemyScript@FI@teleportRadius") + rfk::internal::getTypename<ChildClass>()), rfk::getType<float>(), static_cast<rfk::EFieldFlags>(1), offsetof(ChildClass, teleportRadius), &thisClass);\
field = childClass.addField("animator", std::hash<std::string>()(std::string("c:@N@Scripts@S@EnemyScript@FI@animator") + rfk::internal::getTypename<ChildClass>()), rfk::getType<Anim::Animator *>(), static_cast<rfk::EFieldFlags>(1), offsetof(ChildClass, animator), &thisClass);\
field = childClass.addField("player", std::hash<std::string>()(std::string("c:@N@Scripts@S@EnemyScript@FI@player") + rfk::internal::getTypename<ChildClass>()), rfk::getType<Scenes::GameObject *>(), static_cast<rfk::EFieldFlags>(1), offsetof(ChildClass, player), &thisClass);\
field = childClass.addField("mainSong", std::hash<std::string>()(std::string("c:@N@Scripts@S@EnemyScript@FI@mainSong") + rfk::internal::getTypename<ChildClass>()), rfk::getType<Resources::Sound *>(), static_cast<rfk::EFieldFlags>(1), offsetof(ChildClass, mainSong), &thisClass);\
field = childClass.addField("chaseSong", std::hash<std::string>()(std::string("c:@N@Scripts@S@EnemyScript@FI@chaseSong") + rfk::internal::getTypename<ChildClass>()), rfk::getType<Resources::Sound *>(), static_cast<rfk::EFieldFlags>(1), offsetof(ChildClass, chaseSong), &thisClass);\
field = childClass.addField("deadSong", std::hash<std::string>()(std::string("c:@N@Scripts@S@EnemyScript@FI@deadSong") + rfk::internal::getTypename<ChildClass>()), rfk::getType<Resources::Sound *>(), static_cast<rfk::EFieldFlags>(1), offsetof(ChildClass, deadSong), &thisClass);\
field = childClass.addField("soundObject", std::hash<std::string>()(std::string("c:@N@Scripts@S@EnemyScript@FI@soundObject") + rfk::internal::getTypename<ChildClass>()), rfk::getType<Scenes::SoundObject *>(), static_cast<rfk::EFieldFlags>(1), offsetof(ChildClass, soundObject), &thisClass);\
field = childClass.addField("mainSongName", std::hash<std::string>()(std::string("c:@N@Scripts@S@EnemyScript@FI@mainSongName") + rfk::internal::getTypename<ChildClass>()), rfk::getType<std::string>(), static_cast<rfk::EFieldFlags>(1), offsetof(ChildClass, mainSongName), &thisClass);\
field = childClass.addField("chaseSongName", std::hash<std::string>()(std::string("c:@N@Scripts@S@EnemyScript@FI@chaseSongName") + rfk::internal::getTypename<ChildClass>()), rfk::getType<std::string>(), static_cast<rfk::EFieldFlags>(1), offsetof(ChildClass, chaseSongName), &thisClass);\
field = childClass.addField("deadSongName", std::hash<std::string>()(std::string("c:@N@Scripts@S@EnemyScript@FI@deadSongName") + rfk::internal::getTypename<ChildClass>()), rfk::getType<std::string>(), static_cast<rfk::EFieldFlags>(1), offsetof(ChildClass, deadSongName), &thisClass);\
field = childClass.addField("runAnimation", std::hash<std::string>()(std::string("c:@N@Scripts@S@EnemyScript@FI@runAnimation") + rfk::internal::getTypename<ChildClass>()), rfk::getType<std::string>(), static_cast<rfk::EFieldFlags>(1), offsetof(ChildClass, runAnimation), &thisClass);\
field = childClass.addField("idleAnimation", std::hash<std::string>()(std::string("c:@N@Scripts@S@EnemyScript@FI@idleAnimation") + rfk::internal::getTypename<ChildClass>()), rfk::getType<std::string>(), static_cast<rfk::EFieldFlags>(1), offsetof(ChildClass, idleAnimation), &thisClass);\
)\
__RFK_DISABLE_WARNING_POP \
RFK_UNPACK_IF_NOT_PARSING(rfk::internal::CodeGenerationHelpers::registerChildClass<Scripts::ObjectScript, ChildClass>(childClass);\
}\
\
public:  static rfk::Struct const& staticGetArchetype() noexcept;\
\
)\


#define File_EnemyScript_GENERATED	\
template <>  rfk::Archetype const* rfk::getArchetype<Scripts::EnemyScript>() noexcept;\


