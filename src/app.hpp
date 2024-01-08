#ifndef METABALLS_APP_HPP
#define METABALLS_APP_HPP  // NOLINT

#include "app_data.hpp"
#include "renderer.hpp"
#include "texture.hpp"
#include "window.hpp"
#include <cstddef>
#include <optional>

class App {
public:
    [[nodiscard]] static std::optional<App> make();
    void run();

    ~App();
    App(App const &) = delete;
    App &operator=(App const &) = delete;

    App(App &&) noexcept;
    App &operator=(App &&) noexcept;


private:
    App(Window &&, Renderer &&, Texture &&);

    Window m_window;
    Renderer m_renderer;
    Texture m_texture;
    AppData m_data;

    bool m_quit_on_destroy{true};
    bool m_running{true};
    std::size_t m_pattern_index{};

    [[nodiscard]] bool handle_events();
    void quit();
};

#endif
