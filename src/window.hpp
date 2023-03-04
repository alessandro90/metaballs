#ifndef METABALLS_WINDOW_HPP
#define METABALLS_WINDOW_HPP  // NOLINT

#include <SDL2/SDL.h>
#include <cstddef>
#include <optional>
#include <string_view>

class Window {
    friend class Renderer;

public:
    enum class Position : int {
        UNDEFINED = SDL_WINDOWPOS_UNDEFINED,
        CENTERED = SDL_WINDOWPOS_CENTERED,
    };

    [[nodiscard]] static std::optional<Window> make(std::string_view title,
                                                    Position x,
                                                    Position y,
                                                    std::size_t w,
                                                    std::size_t h);

    Window(Window const &) = delete;
    Window &operator=(Window const &) = delete;

    Window(Window &&) noexcept;
    Window &operator=(Window &&) noexcept;

    ~Window();

private:
    explicit Window(SDL_Window *window);

    SDL_Window *m_window{nullptr};
};


#endif
