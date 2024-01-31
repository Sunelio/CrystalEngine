#pragma once
#include <string>
#include <unordered_map>

namespace Resources { class AssimpParser; }
namespace Anim
{
	class Animation;
	class Skeleton;

	// - Animator: Holds all of a skeleton's animations, decides which one(s) are currently active and updates them - //
	class Animator
	{
	private:
		Skeleton*      skeleton;
		std::string    curAnimName;
		std::unordered_map<std::string, Animation*> animations;
		friend Resources::AssimpParser;
    
	public:
		Animator(Skeleton* baseSkeleton) : skeleton(baseSkeleton) {}
		~Animator();
		Animator(const Animator&)            = delete;
		Animator(Animator&&)                 = delete;
		Animator& operator=(const Animator&) = delete;
		Animator& operator=(Animator&&)      = delete;
    
		Animation* GetAnimation(const std::string& name); // Returns the animation with the given name, or nullptr if none is found.
		std::unordered_map<std::string, Animation*>& GetAllAnimations() { return animations; } // Returns a reference to the map of animations with their names as keys.

		bool        IsAnimating            () const; // Returns true if one or more animations are active, false if not.
		Animation*  GetCurrentAnimation    (); // Returns the current animation, or nullptr if none is active.
		std::string GetCurrentAnimationName(); // Returns the name of the current animation, or an empty string if none is active.
		void        SetCurrentAnimation(const std::string& name); // Sets the current animation to one with the given name. Does nothing if the name 
    
		void Update(const float& deltaTime);
		
	private:
		Animation* AddAnimation(const std::string& name);
	};
}
