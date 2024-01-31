#pragma once
#include <string>
#include <vector>
#include "Utils/Typedefs.h"
#include "Maths/Transform.h"

namespace Resources { class AssimpParser; }
namespace Anim
{
	class Animator;

	// - Bone: Used to animate meshes by moving vertices according to its animated transform - //
	class Bone
	{
	private:
		uint id = 0;
		std::string name;
		
		Maths::Mat4      offsetMatrix = Maths::Mat4(true);
		Maths::Transform animatedTransform;
		
		Animator& animator;
		friend Resources::AssimpParser;
		
	public:
		Bone* parent = nullptr;
		std::vector<Bone*> children;
	
	public:
		Bone(const uint& boneID, std::string boneName, Animator& skeletonAnimator);
		
		void RecursiveTransformUpdate(); // Recursively updates this bone and its children's animated transforms.

		uint             GetId()                const { return id;                } // Returns this bone's ID.
		std::string      GetName()              const { return name;              } // Returns this bone's name.
		Maths::Mat4      GetOffsetMatrix()      const { return offsetMatrix;      } // Returns the matrix that transforms from the root bone to this one.
		Maths::Transform GetAnimatedTransform() const { return animatedTransform; } // Returns this bone's animated transform.
	};
}
