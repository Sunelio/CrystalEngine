#include "Interface/Widget.h"

#include "Render/Renderer.h"

#include "Core/Engine.h"
#include "Core/Window.h"

#include "Resources/ResourceManager.h"
#include "Resources/Texture.h"
#include "Resources/Font.h"

#include "Scenes/GameObject.h"

#include <LightJson/JsonElement.h>
#include <Utils/JsonHelper.h>

#include "Scenes/Scene.h"
#include "Scenes/SceneManager.h"
using namespace Interface;
using namespace Resources;
using namespace Maths;
using namespace LightJson;

namespace Interface
{
	std::string WidgetTypeToString(const WidgetType& type)
	{
		switch (type)
		{
		case WidgetType::Button:
			return "Button";
		case WidgetType::Text:
			return "Text";
		case WidgetType::Empty:
		default:
			return "Empty";
		}
	}

	Widget::Widget(Scenes::CanvasObject* _canvas, const size_t& _id, std::string _name, const uint& _layer, const WidgetAnchor& _anchor, const WidgetType& _type) 
	: id(_id), type(_type), layer(_layer), anchor(_anchor), canvas(_canvas), name(std::move(_name)) { }

	Widget::~Widget()
	{
		for (const Widget* widget : children) delete widget;
		children.clear();
		
		const auto iterator = std::find(canvas->widgets.begin(), canvas->widgets.end(), this);
		if (iterator == canvas->widgets.end()) return;
		canvas->widgets.erase(iterator);
		canvas->idGenerator.Free(id);
	}

	void Widget::AddChild(Widget* child)
	{
		children.emplace_back(child);
	}

	void Widget::RemoveChild(const size_t& childId)
	{
		const auto iterator = std::find_if(children.begin(), children.end(), [childId](const Widget* child){ return child->GetId() == childId; });
		if (iterator == children.end()) return;
		children.erase(iterator);
	}

	bool Widget::SetParent(Widget* newParent)
	{
		if (!FindInChildren(newParent->GetId()))
		{
			if (parent) parent->RemoveChild(id);
			parent = newParent;
			newParent->children.push_back(this);
			return true;
		}
		return false;
	}

	Widget* Widget::FindInChildren(const std::string& searchName) const
	{
		// Find in direct children.
		const auto iterator = std::find_if(children.begin(), children.end(), [searchName](const Widget* child){ return child->name == searchName; });
		if (iterator != children.end()) return *iterator;

		// Find in children's children.
		for (const Widget* child : children) {
			Widget* foundChild = child->FindInChildren(searchName);
			if (foundChild != nullptr)
				return foundChild;
		}
		return nullptr;
	}

	Widget* Widget::FindInChildren(const size_t& searchId) const
	{
		// Find in direct children.
		const auto iterator = std::find_if(children.begin(), children.end(), [searchId](const Widget* child){ return child->GetId() == searchId; });
		if (iterator != children.end()) return *iterator;

		// Find in children's children.
		for (const Widget* child : children) {
			Widget* foundChild = child->FindInChildren(searchId);
			if (foundChild != nullptr)
				return foundChild;
		}
		return nullptr;
	}

	JsonElement* Widget::Serialize() const
	{
		JsonUtils::JsonMap map = {};
		map["id"      ] = new NumberElement(id);
		map["name"    ] = new StringElement(name);
		map["type"    ] = new NumberElement((int)type);
		map["position"] = Helper::JsonHelper::Serialize(position);
		map["scale"   ] = Helper::JsonHelper::Serialize(scale);
		map["rotation"] = new NumberElement(rotation);
		map["parent"  ] = new NumberElement(parent->GetId());
		return new ObjectElement(map);
	}

	void Widget::Deserialize(const JsonElement* jsonElement)
	{
		const JsonUtils::JsonMap map = jsonElement->getAsObject();
		position = Helper::JsonHelper::Deserialize<Vector2>(map.at("position"));
		scale    = Helper::JsonHelper::Deserialize<Vector2>(map.at("scale"));
		rotation = map.at("rotation")->getAsFloat();
	}
	
	TextWidget::TextWidget(Scenes::CanvasObject* _canvas, const size_t& _id, const std::string& _name, const uint& _layer, const WidgetAnchor& _anchor)
		: Widget(_canvas, _id, _name, _layer, _anchor, WidgetType::Text) {}

	void TextWidget::Draw(const Render::Renderer* renderer)
	{
		if (font == nullptr || !font->WasSentToPipeline()) return;
		renderer->DrawText(text, font, position, scale, color, 256-id);
	}

	JsonElement* TextWidget::Serialize() const
	{
		ObjectElement* element = (ObjectElement*)Widget::Serialize();
		element->addElement("text", new StringElement(text));
		element->addElement("font", new StringElement(font ? font->GetName() : ""));
		element->addElement("color", Helper::JsonHelper::Serialize(color));
		return element;
	}

	void TextWidget::Deserialize(const JsonElement* jsonElement)
	{
		Widget::Deserialize(jsonElement);
	
		const JsonUtils::JsonMap map = jsonElement->getAsObject();
		text  = map.at("text")->getAsString();
		color = Helper::JsonHelper::Deserialize<RGBA>(map.at("color"));
		const std::string fontName = map.at("font")->getAsString();
		if (!fontName.empty()) font = Core::Engine::Get()->GetResourceManager()->Get<Font>(fontName, true);
	}

	ButtonWidget::ButtonWidget(Scenes::CanvasObject* _canvas, const size_t& _id, const std::string& _name, const uint& _layer, const WidgetAnchor& _anchor)
		: Widget(_canvas,_id, _name, _layer, _anchor, WidgetType::Button)
	{
		if (buttonTexture && hoverTexture && pressTexture) return;

		ResourceManager* resource = Core::Engine::Get()->GetResourceManager();
		buttonTexture = resource->Get<Texture>("Resources/Textures/UI/Button/DefaultButton.png");
		hoverTexture = resource->Get<Texture>("Resources/Textures/UI/Button/ButtonHovered.png");
		pressTexture = resource->Get<Texture>("Resources/Textures/UI/Button/ButtonPress.png");
	}

	void ButtonWidget::Update(const Core::WindowInputs& inputs)
	{
		static Core::Engine*         engine       = Core::Engine::Get();
		static Core::Window*         window       = engine->GetWindow();
		static Render::Renderer*     renderer     = engine->GetRenderer();
		static Scenes::SceneManager* sceneManager = engine->GetSceneManager();

		Vector2 viewportSize = window->GetViewportSize();
		if (viewportSize == Vector2())
			viewportSize = window->GetSize();

		const Vector2 mouseRelative = inputs.mousePos - window->GetViewportPos();
        const size_t  objectId      = 256-renderer->PickObjectFromViewport(mouseRelative, sceneManager->GetActiveScene()->GetRenderTexture(), viewportSize); // Good enough for now but needs to be reworked in the future.
		isHovered = objectId == id;
		isPressed = isHovered && inputs.mouseLeftClick;

		if (isPressed)
		{
			for (int i = 0; i < callsback.size(); i++)
			{
				callsback[i]();
			}
		}
	}

	void ButtonWidget::Draw(const Render::Renderer* renderer)
	{
		Texture* curTexture;
		if      (isPressed) curTexture = pressTexture;
		else if (isHovered) curTexture = hoverTexture;
		else                curTexture = buttonTexture;
	
		if (!curTexture) return;
		const float aspect = (float)curTexture->GetWidth() / (float)curTexture->GetHeight();
		textureSize.x = scale.x * aspect;
		textureSize.y = scale.y;

		renderer->DrawTexture2D(curTexture, scale, position, textureSize, 256-id);
	}

	JsonElement* ButtonWidget::Serialize() const
	{
		ObjectElement* element = (ObjectElement*)Widget::Serialize();
		element->addElement("buttonTexture", new StringElement(buttonTexture ? buttonTexture->GetName() : ""));
		element->addElement("hoverTexture",  new StringElement(hoverTexture  ? hoverTexture ->GetName() : ""));
		element->addElement("pressTexture",  new StringElement(pressTexture  ? pressTexture ->GetName() : ""));
		return element;
	}

	void ButtonWidget::Deserialize(const JsonElement* jsonElement)
	{
		Widget::Deserialize(jsonElement);
	
		const JsonUtils::JsonMap map        = jsonElement->getAsObject();
		const std::string buttonTextureName = map.at("buttonTexture")->getAsString();
		const std::string hoverTextureName  = map.at("hoverTexture" )->getAsString();
		const std::string pressTextureName  = map.at("pressTexture" )->getAsString();
		
		ResourceManager* resourceManager = Core::Engine::Get()->GetResourceManager();
		if (!buttonTextureName.empty()) buttonTexture = resourceManager->Get<Texture>(buttonTextureName);
		if (!hoverTextureName .empty()) hoverTexture  = resourceManager->Get<Texture>(hoverTextureName);
		if (!pressTextureName .empty()) pressTexture  = resourceManager->Get<Texture>(pressTextureName);
	}
}