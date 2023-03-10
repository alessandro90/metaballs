#ifndef METABALLS_APP_HPP
#define METABALLS_APP_HPP  // NOLINT

#include "app_data.hpp"
#include "common_types.hpp"
#include "renderer.hpp"
#include "texture.hpp"
#include "window.hpp"
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

    void handle_events();
    void quit();
    void spawn_metaball(Coordinate);
    void maybe_drag_a_metaball(Coordinate);
    void maybe_delete_a_metaball(Coordinate);
};

#endif
