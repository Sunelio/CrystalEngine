#pragma once
#include <functional>

#include "Maths/Maths.h"
#include "Utils/Typedefs.h"

#include "../Externals/Includes/LightJson/JsonVirtual.h"

namespace Core      { class Engine; struct WindowInputs; }
namespace Render    { class Renderer; }
namespace Scenes    { class CanvasObject; }
namespace Resources { class RenderTexture; class Font; class ResourceManager; class Texture; }

namespace Interface
{
	enum class WidgetAnchor
	{
		TopLeft,
		TopCenter,
		TopRight,
		LeftCenter,
		Center,
		RightCenter,
		BottomLeft,
		BottomCenter,
		BottomRight,
	};
	
	enum class WidgetType
	{
		Empty,
		Button,
		Text,
	};
	
    std::string WidgetTypeToString(const WidgetType& type );

	class Widget : public LightJson::JsonVirtual
	{
	protected:
		size_t       id     = 0;
		WidgetType   type   = WidgetType::Empty;
		uint         layer  = 0;
		WidgetAnchor anchor = WidgetAnchor::TopLeft;

		Widget*				 parent = nullptr;
		std::vector<Widget*> children = {};

		Scenes::CanvasObject* canvas;

	public:
		std::string    name;
		Maths::Vector2 position = Maths::Vector2(0);
		float          rotation = 0;
		Maths::Vector2 scale    = Maths::Vector2(1);
		Resources::RenderTexture* texture = nullptr;

	public:
		Widget(Scenes::CanvasObject* _canvas, const size_t& _id, std::string _name, const uint& _layer = 0, const WidgetAnchor& _anchor = WidgetAnchor::TopLeft, const WidgetType& _type = WidgetType::Empty);
		~Widget() override;
		Widget(const Widget&)            = delete;
		Widget(Widget&&)                 = delete;
		Widget& operator=(const Widget&) = delete;
		Widget& operator=(Widget&&)      = delete;

		virtual void Update(const Core::WindowInputs& inputs) {}
		virtual void Draw  (const Render::Renderer* renderer) {}

		void AddChild   (Widget*       child);     // Adds the given Widget to this widget children.
		void RemoveChild(const size_t& childId);   // Removes the child at the given id from this widgets children.
		bool SetParent  (Widget*       newParent); // Sets this widgets parent to the given widget.

		WidgetType GetType  () const { return type;   }
		size_t	   GetId    () const { return id;     }
		std::string GetName () const { return name;   }
		Widget*	   GetParent() const { return parent; }
		uint	   GetLayer () const { return layer;  }

		std::vector<Widget*> GetChildren() const { return children; } // Returns an array of all the widget's children.
		Widget* FindInChildren(const std::string& searchName) const;  // Recursively searches for a widget with the given name.
		Widget* FindInChildren(const size_t& searchId) const;         // Recursively searches for a widget with the given ID.

		void SetId   (const size_t& _id)  { id    = _id;    }
		void SetLayer(const uint& _layer) { layer = _layer; }

		LightJson::JsonElement* Serialize() const                   override; 
		void Deserialize(const LightJson::JsonElement* jsonElement) override;
	};

	class TextWidget : public Widget
	{
	public:
		std::string		 text;
		Resources::Font* font = nullptr;
		Maths::RGBA      color;

	public:
		TextWidget(Scenes::CanvasObject* _canvas, const size_t& _id, const std::string& _name, const uint& _layer = 0, const WidgetAnchor& _anchor = WidgetAnchor::TopLeft);
		void Update(const Core::WindowInputs& inputs) override {}
		void Draw  (const Render::Renderer* renderer) override;

		LightJson::JsonElement* Serialize() const                   override; 
		void Deserialize(const LightJson::JsonElement* jsonElement) override;
	};

	class ButtonWidget : public Widget
	{
	private:
		bool isHovered = false;
		bool isPressed = false;

		std::vector<std::function<void()>> callsback;

	public :
		Resources::Texture* currentTexture = nullptr;
		Resources::Texture* buttonTexture  = nullptr;
		Resources::Texture* hoverTexture   = nullptr;
		Resources::Texture* pressTexture   = nullptr;
		Maths::Vector2      textureSize;
		
	public:
		ButtonWidget(Scenes::CanvasObject* _canvas, const size_t& _id, const std::string& _name, const uint& _layer = 0, const WidgetAnchor& _anchor = WidgetAnchor::TopLeft);
		void Update(const Core::WindowInputs& inputs) override;
		void Draw  (const Render::Renderer* renderer) override;
		std::vector<std::function<void()>> GetCallsback() { return callsback; }
		void AddCallback(std::function<void()> function) { callsback.push_back(function); }

		LightJson::JsonElement* Serialize() const                   override; 
		void Deserialize(const LightJson::JsonElement* jsonElement) override;
	};
}
