#pragma once

#include <string>
#include <vector>

namespace Scenes
{
    class GameObject;

    // - ObjectTag: tag system for game objects to use - //
    struct ObjectTag
    {
        std::string name;
        std::vector<GameObject*> tagged;

        ObjectTag(std::string _name) : name(std::move(_name)) {}
        
        void Tag(GameObject* gameObject);         // Gives this tag to the given game object.
        void Untag(GameObject* gameObject) const; // Removes this tag from the given game object.
    };
}
