#pragma once
#include "Maths/Vector2.h"
#include "Maths/Vector3.h"
#include "Maths/Color.h"
#include <filesystem>
#include <functional>
#include <unordered_map>

struct ImGuiIO;
struct ImGuiStyle;

namespace Resources { class Cubemap; class ResourceManager; class Material; class StaticSubMesh; class Mesh; class Texture; class RenderTexture; class ShaderProgram; class Font; class Sound; }
namespace Render    { class Renderer; class PostProcessor; class Light; class Camera; }
namespace Scenes    { class SoundObject; class InstancedObject; class SceneManager; class GameObject; class ModelObject; class CanvasObject; }
namespace Interface { class Widget; class ButtonWidget; class TextWidget; }
namespace Core      { class Engine; class Window; }
namespace Anim      { class Animator; class Bone; }
namespace Maths     { class Transform; }

namespace UI
{
	enum class FileIcon
	{
		Code,
	    Image,
		Material,
	    Mesh,
	    Scene,
	    Settings,
	    Shader,
		Font,
		Sound,
	    Document,
	    Folder,
	};

	enum class GizmoType
	{
		Translation,
		Rotation,
		Scaling,
	};
	
	// - UserInterface - //
	class Editor
	{
    private:
        Core::Engine*               engine          = nullptr;
		Core::Window*               window          = nullptr;
		Resources::ResourceManager* resourceManager = nullptr;
		Scenes::SceneManager*       sceneManager    = nullptr;
		Render::Renderer*           renderer        = nullptr;
		Render::PostProcessor*      postProcessor   = nullptr;
		ImGuiIO*                    imGuiIO         = nullptr;
		ImGuiStyle*                 imGuiStyle      = nullptr;
		
		Resources::RenderTexture* engineRenderTexture   = nullptr;
		Resources::RenderTexture* viewportRenderTexture = nullptr;
		Resources::RenderTexture* objectRenderTexture   = nullptr;
		Resources::RenderTexture* canvasRenderTexture   = nullptr;
		
        Scenes::GameObject* selectedGameObject = nullptr;
		GizmoType           gizmoType          = GizmoType::Translation;
		Maths::Vector3      gizmoAxis;
		
		Scenes::CanvasObject* selectedCanvas = nullptr;
		Interface::Widget*    selectedWidget = nullptr;
		
		std::filesystem::path explorerPath         = "Resources";
		std::filesystem::path selectedFile         = "";
		bool                  shouldReloadExplorer = false;
		float                 fileIconsSize        = 100;
        
		Maths::Vector2 viewportPos,    viewportSize;
		Maths::Vector2 sceneViewPos,   sceneViewSize;
		Maths::Vector2 canvasPanelPos, canvasPanelSize;
		
		std::unordered_map<std::string, Resources::Texture*> scriptingIcons;
		std::unordered_map<std::string, Resources::Texture*> miscIcons;
		std::unordered_map<std::string, Resources::Texture*> fileIcons;
		std::unordered_map<std::string, Resources::Texture*> objectIcons;
		std::unordered_map<std::string, Resources::Texture*> widgetIcons;
		std::unordered_map<std::string, Resources::Texture*> graphIcons;
		
    public:
        Editor(Core::Engine* _engine);
        ~Editor();

		// -- Methods -- //
		void LockInputsIfUsed() const; // Prevents inputs from registering if the UI is in use.
        void ExecuteFrame();           // Render the whole editor interface.
        void EndFrame() const;         // Finalize user interface rendering.

    private:
		void ShowTransformUi   (Maths::Transform& transform, const std::string& id) const;
		void ShowTextureUi     (const Resources::Texture* texture, const float& size, const bool& isClickable = false, const std::string& tooltip = "") const;
		void ShowMaterialUi    (Resources::Material* material, const std::string& id) const;
		void ShowBoneUi        (const Anim::Bone* bone) const;
        void ShowAnimatorUI    (Anim::Animator& animator, const std::string& id) const;
		void ShowMeshUi        (const Resources::Mesh* mesh, const std::string& id) const;
		void ShowInstancedUi   (Scenes::InstancedObject* instancedObject, const std::string& id) const;
		void ShowLightUi       (Render::Light* light, const std::string& id) const;
		void ShowCameraUi      (Render::Camera* camera, const std::string& id) const;
		void ShowCubemapUi     (Resources::Cubemap* cubemap, const std::string& id) const;
		void ShowSoundUi       (const std::string& id);
		void Show2DTransformUi (Maths::Vector2& position, Maths::Vector2& scale, float& rotation, const std::string& id) const;
		void ShowButtonWidgetUi(Interface::ButtonWidget* widget, const std::string& id) const;
		void ShowTextWidgetUi  (Interface::TextWidget* widget, const std::string& id) const;
		void ShowScriptUI	   (const std::string& id);
		void ShowFileUi        (const std::filesystem::path& path, const float& xMax);
		
        void     ShowObjectTreeNode (Scenes::GameObject* object);
        void     ShowWidgetTreeNode (Interface::Widget* widget);
		FileIcon GetFileIcon        (const std::filesystem::path& path) const;

		void ShowMenuBar        ();
		void ShowRenamePopup     (const char* strId, const char* originalName, const std::function<void(std::string)>& renameCallback) const;
		void ShowDeletePopup     (const char* strId, const char* name,         const std::function<void()>&            deleteCallback) const;
		void ShowFilePopup		 (const char* strId, const std::filesystem::path& file, bool& renameCallback, bool& deleteCallback);
		void ShowSceneGraphPopup ();
        void ShowCanvasGraphPopup();
		void ShowCreateScenePopup();

		void ShowSettingsWindow() const;
		void ShowStatsWindow() const;
		void ShowPostProcessWindow();

		void ShowScriptDropArea ();
		void ShowShaderDropArea();
		
		void SceneGraphWindow     ();
        void ObjectInspectorWindow();
		void CanvasGraphWindow    ();
		void WidgetInspectorWindow();
        void FileExplorerWindow   ();
        void FileInspectorWindow  ();
        void PlayWindow () const;
        void LogsWindow () const;
        void GridDisplay() const;
        void ViewportWindow   ();
		void SceneViewWindow  ();
		void CanvasPanelWindow();

		void DrawObjectOutline() const;
		void DrawObjectGizmo  () const;
		void GizmoBehavior();
		void ObjectPicking();
		void WidgetPicking();
	};
}
