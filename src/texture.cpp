#include "texture.hpp"
#include "renderer.hpp"
#include <cassert>
#include <limits>
#include <optional>
#include <utility>

std::optional<Texture> Texture::make(Renderer &renderer,
                                     SDL_PixelFormatEnum pixel_format,
                                     SDL_TextureAccess access,
                                     std::size_t w,
                                     std::size_t h) {
    assert(w <= static_cast<std::size_t>(std::numeric_limits<int>::max()));
    assert(h <= static_cast<std::size_t>(std::numeric_limits<int>::max()));
    auto *texture = SDL_CreateTexture(renderer.m_renderer,
                                      pixel_format,
                                      access,
                                      static_cast<int>(w),
                                      static_cast<int>(h));
    if (texture == NULL) {  // NOLINT(modernize-use-nullptr)
        return std::nullopt;
    }
    return Texture{texture};
}

void Texture::update(const void *pixels, std::size_t bytes_per_row) {
    SDL_UpdateTexture(m_texture, nullptr, pixels, static_cast<int>(bytes_per_row));
}

Texture::Texture(Texture &&other) noexcept {
    m_texture = std::exchange(other.m_texture, nullptr);
}

Texture &Texture::operator=(Texture &&other) noexcept {
    m_texture = std::exchange(other.m_texture, nullptr);
    return *this;
}

Texture::Texture(SDL_Texture *texture)
    : m_texture{texture} {}

Texture::~Texture() {
    if (m_texture != nullptr) {
        SDL_DestroyTexture(m_texture);
    }
}
