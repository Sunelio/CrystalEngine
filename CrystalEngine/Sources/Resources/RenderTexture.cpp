#include "Resources/RenderTexture.h"
#include "Core/Engine.h"
#include "Render/Renderer.h"

using namespace Resources;
using namespace Utils;

RenderTexture::RenderTexture(const std::string& _name)
{
    name = _name;
    type = ResourceType::RenderTexture;
    renderer = Core::Engine::Get()->GetRenderer();
}

RenderTexture::~RenderTexture()
{
    if (WasSentToPipeline())
        renderer->DeleteResource<RenderTexture>(this);
}

void RenderTexture::Load()
{
    if (IsLoaded() || width <= 0 || height <= 0)
        return;
    loaded = true;
}

void RenderTexture::SendToPipeline()
{
    if (!IsLoaded() || WasSentToPipeline())
        return;

    renderer->SendResourceToPipeline<RenderTexture>(this);
	sentToPipeline = true;
}

void RenderTexture::BeginRender() const
{
    renderer->BindRenderTexture(this);
    renderer->ClearBindedRenderTexture(clearColor);
}

void RenderTexture::EndRender() const
{
    renderer->UnbindRenderTexture();
    // TODO: this requires a rendertexture stack to work with nested render textures.
}

void RenderTexture::SetWidth(const int& _width )
{
    width = _width;
    if (WasSentToPipeline())
    {
        renderer->DeleteResource<RenderTexture>(this);
        pipelineData = RenderTexturePipelineData();
        sentToPipeline = false;
        SendToPipeline();
    }
}

void RenderTexture::SetHeight(const int& _height)
{
    height = _height;
    if (WasSentToPipeline())
    {
        renderer->DeleteResource<RenderTexture>(this);
        pipelineData = RenderTexturePipelineData();
        sentToPipeline = false;
        SendToPipeline();
    }
}

void RenderTexture::SetSize(const int& _width, const int& _height)
{
    width  = _width;
    height = _height;
    if (WasSentToPipeline())
    {
        width  = _width;
        height = _height;
        renderer->DeleteResource<RenderTexture>(this);
        pipelineData = RenderTexturePipelineData();
        sentToPipeline = false;
        SendToPipeline();
    }
}

void RenderTexture::SetPipelineData(const RenderTexturePipelineData& data)
{
    if (!pipelineData.IsEmpty())
        return;
    pipelineData = data;
}
