#pragma once
#include "Utils/Typedefs.h"
#include "Resource.h"
#include <vector>

namespace Scripts { struct ShaderScript; }
namespace Resources
{
	// - ShaderProgram (inherits from Resource) - //
	class ShaderProgram : public Resource
	{
	private:
		uint id = 0;
		std::vector<Resource*> attachedShaders;
		Scripts::ShaderScript* script = nullptr;

	public:
        // -- Constructor -- //
        
		// Creates a shader program with the given name.
		ShaderProgram(const std::string& _name);
		~ShaderProgram() override;
		ShaderProgram(const ShaderProgram&)            = delete;
		ShaderProgram(ShaderProgram&&)                 = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;
		ShaderProgram& operator=(ShaderProgram&&)      = delete;

		// -- Methods -- //
		void AttachShader (Resource* shader);  // Attaches the given shader to be linked with the shader program.
		void AttachShaders(size_t count, ...); // Attaches the given shaders to be linked with the shader program.
		
		void Load()           override; // Loads the shader program if all attached shaders have been loaded.
		void SendToPipeline() override; // Links and sends the shader program to GPU memory is all attached shaders have been sent to GPU memory.

		void SetScript(Scripts::ShaderScript* _script); // Sets the shader's script to the given one.
		void SetId(const uint& _id) { if (id == 0) id = _id; }

		// -- Getters -- //
		uint                   GetId()              const { return id; }              // Returns the shader program's ID.
		std::vector<Resource*> GetAttachedShaders() const { return attachedShaders; } // Returns all of the shaders attached to the shader program.
		Scripts::ShaderScript* GetShaderScript()    const { return script; }          // Returns the shader's script so that it can be populated and ran.
		static ResourceType    GetResourceType() { return ResourceType::ShaderProgram; }
	};
}