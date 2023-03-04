#ifndef METABALLS_TEXTURE_HPP
#define METABALLS_TEXTURE_HPP  // NOLINT

#include "SDL2/SDL.h"
#include <cstddef>
#include <optional>

class Renderer;

class Texture {
    friend Renderer;

public:
    [[nodiscard]] static std::optional<Texture> make(Renderer &,
                                                     SDL_PixelFormatEnum,
                                                     SDL_TextureAccess,
                                                     std::size_t w,
                                                     std::size_t h);

    void update(const void *pixels, std::size_t bytes_per_row);

    Texture(Texture const &) = delete;
    Texture &operator=(Texture const &) = delete;

    Texture(Texture &&) noexcept;
    Texture &operator=(Texture &&) noexcept;

    ~Texture();

private:
    explicit Texture(SDL_Texture *texture);

    SDL_Texture *m_texture{nullptr};
};

#endif
