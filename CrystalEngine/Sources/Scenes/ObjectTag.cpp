#include "Scenes/ObjectTag.h"
#include "Scenes/GameObject.h"

using namespace Scenes;

void ObjectTag::Tag(GameObject* gameObject)
{
    gameObject->AddTag(this);
}

void ObjectTag::Untag(GameObject* gameObject) const
{
    gameObject->RemoveTag(this);
}

