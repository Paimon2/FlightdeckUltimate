#include <Ultralight/Renderer.h>
#include <Ultralight/View.h>
#include <Ultralight/platform/Platform.h>
#include <Ultralight/platform/Config.h>
#include <Ultralight/platform/GPUDriver.h>
#include "../framework/minimal/FontLoaderMinimal.h"
#include <iostream>
#include <string>

using namespace ultralight;

static bool finished = false;

class GPUDriverStub : public GPUDriver {
public:
  GPUDriverStub() {}
  virtual ~GPUDriverStub() {}

  // Synchronization

  virtual void BeginSynchronize() {}

  virtual void EndSynchronize() {}

  // Textures

  virtual uint32_t NextTextureId() { return 0; }

  virtual void CreateTexture(uint32_t texture_id,
    Ref<Bitmap> bitmap) {}

  virtual void UpdateTexture(uint32_t texture_id,
    Ref<Bitmap> bitmap) {}

  virtual void BindTexture(uint8_t texture_unit,
    uint32_t texture_id) {}

  virtual void DestroyTexture(uint32_t texture_id) {}

  // Offscreen Rendering

  virtual uint32_t NextRenderBufferId() { return 0; }

  virtual void CreateRenderBuffer(uint32_t render_buffer_id,
    const RenderBuffer& buffer) {}

  virtual void BindRenderBuffer(uint32_t render_buffer_id) {}

  virtual void SetRenderBufferViewport(uint32_t render_buffer_id,
    uint32_t width,
    uint32_t height) {}

  virtual void ClearRenderBuffer(uint32_t render_buffer_id) {}

  virtual void DestroyRenderBuffer(uint32_t render_buffer_id) {}

  // Geometry

  virtual uint32_t NextGeometryId() { return 0; }

  virtual void CreateGeometry(uint32_t geometry_id,
    const VertexBuffer& vertices,
    const IndexBuffer& indices) {}

  virtual void UpdateGeometry(uint32_t geometry_id,
    const VertexBuffer& buffer,
    const IndexBuffer& indices) {}


  virtual void DrawGeometry(uint32_t geometry_id,
    uint32_t indices_count,
    uint32_t indices_offset,
    const GPUState& state) {}


  virtual void DestroyGeometry(uint32_t geometry_id) {}

  // Command Queue

  virtual void UpdateCommandList(const CommandList& queue) {
    std::cout << "UpdateCommandList: " << queue.size << "." << std::endl;
  }

  virtual bool HasCommandsPending() { return false; }

  virtual void DrawCommandList() {}
};

class LoadListenerImpl : public LoadListener {
public:
    virtual ~LoadListenerImpl() {}
    
    // Called when the page begins loading new URL into main frame
    virtual void OnBeginLoading(ultralight::View* caller) {
        std::cout << "OnBeginLoading" << std::endl;
    }
    
    // Called when the page finishes loading URL into main frame
    virtual void OnFinishLoading(ultralight::View* caller) {
        std::cout << "OnFinishLoading" << std::endl;
        finished = true;
    }
    
    // Called when the history (back/forward state) is modified
    virtual void OnUpdateHistory(ultralight::View* caller) {
        std::cout << "OnUpdateHistory" << std::endl;
    }
    
    // Called when all JavaScript has been parsed and the document is ready.
    // This is the best time to make any initial JavaScript calls to your page.
    virtual void OnDOMReady(ultralight::View* caller) {
        std::cout << "OnDOMReady" << std::endl;
    }
};

int main() {
  std::cout << "Minimal Sample starting..." << std::endl;
  ultralight::Platform& platform = ultralight::Platform::instance();
  platform.set_font_loader(new FontLoaderMinimal());
  platform.set_gpu_driver(new GPUDriverStub());

  std::cout << "Creating renderer." << std::endl;

  auto renderer = ultralight::Renderer::Create();

  std::cout << "Creating view." << std::endl;

  auto view = renderer->CreateView(100, 100, false);
  view->set_load_listener(new LoadListenerImpl());
  view->LoadHTML("<html><head><title>Hello World</title></head><body></body></html>");

  std::cout << "Start!" << std::endl;
  
  while (!finished) {
    renderer->Update();
    renderer->Render();
  }

  std::wcout << L"Loaded page with title: " << 
    std::wstring(view->title().utf16().data()) << std::endl;
  
  std::cout << "Done.";

  return 0;
}
