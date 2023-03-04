#ifndef METABLASS_RENDERER_HPP
#define METABLASS_RENDERER_HPP  // NOLINT

#include "SDL2/SDL.h"
#include "window.hpp"
#include <optional>


class Texture;

class Renderer {
    friend Texture;

public:
    enum Flag {
        SOFTWARE = SDL_RENDERER_SOFTWARE,
        ACCELERATED = SDL_RENDERER_ACCELERATED,
        PRESENTVSYNC = SDL_RENDERER_PRESENTVSYNC,
        TARGETTEXTURE = SDL_RENDERER_TARGETTEXTURE,
    };

    [[nodiscard]] static std::optional<Renderer> make(Window &window, Flag flag);

    void set_draw(std::uint32_t color);
    void clear();
    void present(Texture &);

    Renderer(Renderer const &) = delete;
    Renderer &operator=(Renderer const &) = delete;

    Renderer(Renderer &&) noexcept;
    Renderer &operator=(Renderer &&) noexcept;

    ~Renderer();

private:
    explicit Renderer(SDL_Renderer *renderer);

    SDL_Renderer *m_renderer{nullptr};
};

#endif
