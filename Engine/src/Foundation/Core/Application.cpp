#include "Application.h"

#include <memory>

#include <glad/glad.h>

namespace fnd {

  template<> Application* Singleton<Application>::m_singleton = nullptr;

  Application::Application() {
    m_window = Window::create();
    m_window->setEventCallback(FND_BIND_EVENT_FN(Application::onEvent));

    m_running = true;

    // Initialise systems
    if (!Log::getSingletonPtr()) {
      new Log();
    }
    m_logPtr = UniquePtr<Log>(Log::getSingletonPtr());
    m_layerManagerPtr = std::make_unique<LayerManager>();

    // Create default ImGui layer
    m_ImGuiLayer = new ImGuiLayer();
    pushOverlay(m_ImGuiLayer);
  }

  void Application::run() {
    while (m_running) {
      // TODO: remove this testing code
      {
        static bool faze = false;
        faze = !faze;
        if (faze) {
          glClearColor(1, 0, 1, 1);
        } else {
          glClearColor(0.9, 0.1, 0.9, 1);
        }
        glClear(GL_COLOR_BUFFER_BIT);
      }

      // Update layers in order
      for (Layer* layer : *m_layerManagerPtr) {
        layer->onUpdate();
      }

      // Update ImGui
      m_ImGuiLayer->begin();
      for (Layer* layer : *m_layerManagerPtr) {
        layer->onImGui();
      }
      m_ImGuiLayer->end();


      m_window->onUpdate();
    }
  }

  void Application::onEvent(Event& e) {
    FND_DEBUG(e.toString());

    EventDispatcher dispatcher(e);
    dispatcher.dispatch<WindowCloseEvent>(FND_BIND_EVENT_FN(Application::onWindowClose));

    // Propagate event inversely through layers
    for (auto it = m_layerManagerPtr->rbegin(); it != m_layerManagerPtr->rend() && !e.handled; ++it) {
      (*it)->onEvent(e);
    }
  }

  // LayerManager wrapper
  void Application::pushLayer(fnd::Layer* layer) {
    m_layerManagerPtr->pushLayer(layer);
  }

  void Application::pushOverlay(fnd::Layer* overlay) {
    m_layerManagerPtr->pushOverlay(overlay);
  }

  void Application::popLayer(fnd::Layer* layer) {
    m_layerManagerPtr->popLayer(layer);
  }

  void Application::popOverlay(fnd::Layer* overlay) {
    m_layerManagerPtr->popOverlay(overlay);
  }

  bool Application::onWindowClose(fnd::WindowCloseEvent &e) {
    m_running = false;
    return true;
  }

}