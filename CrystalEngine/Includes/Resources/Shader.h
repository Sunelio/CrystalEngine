#pragma once
#include "Utils/Typedefs.h"
#include "Resource.h"
#include <string>

namespace Resources
{
	// - VertexShader (inherits from Resource) - //
	class VertexShader : public Resource
	{
	private:
		uint id = 0;
		std::string sourceCode;
		
	public:
		// -- Constructor -- //
		
		// Creates a vertex shader with the given name (has to be a filename).
		VertexShader(const std::string& _name);
		~VertexShader() override;
		VertexShader(const VertexShader&)            = delete;
		VertexShader(VertexShader&&)                 = delete;
		VertexShader& operator=(const VertexShader&) = delete;
		VertexShader& operator=(VertexShader&&)      = delete;

		// -- Methods -- //
		void Load()           override; // Loads the vertex shader source code.
		void SendToPipeline() override; // Compiles and sends the vertex shader to GPU memory.

		void SetId(const uint& _id) { if (id == 0) id = _id; }

		// -- Getters -- //
		uint        GetId()         const { return id; }         // Returns the vertex shader's ID.
		std::string GetSourceCode() const { return sourceCode; } // Returns the vertex shader's source code as a string.
		static ResourceType GetResourceType() { return ResourceType::VertexShader; } // Returns the resource type of this class.
	};

	// - FragmentShader - //
	class FragmentShader : public Resource
	{
	private:
		uint id = 0;
		std::string sourceCode;
		
	public:
		// -- Constructor -- //
		
		// Creates a fragment shader with the given name (has to be a filename).
		FragmentShader(const std::string& _name);
		~FragmentShader() override;
		FragmentShader(const FragmentShader&)            = delete;
		FragmentShader(FragmentShader&&)                 = delete;
		FragmentShader& operator=(const FragmentShader&) = delete;
		FragmentShader& operator=(FragmentShader&&)      = delete;

		// -- Methods -- //
		void Load()           override; // Loads the fragment shader source code.
		void SendToPipeline() override; // Compiles and sends the fragment shader to GPU memory.

		void SetId(const uint& _id) { if (id == 0) id = _id; }
		
		// -- Getters -- //
		uint        GetId()         const { return id; }         // Returns the fragment shader's ID.
		std::string GetSourceCode() const { return sourceCode; } // Returns the fragment shader's source code as a string.
		static ResourceType GetResourceType() { return ResourceType::FragmentShader; } // Returns the resource type of this class.
	};
}
