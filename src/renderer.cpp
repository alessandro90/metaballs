#include "renderer.hpp"
#include "SDL2/SDL.h"
#include "texture.hpp"
#include "window.hpp"
#include <cstdint>
#include <utility>

namespace {
[[nodiscard]] Uint8 get_channel(std::uint32_t color, std::uint32_t mask, std::uint32_t shift) noexcept {
    return static_cast<Uint8>((static_cast<Uint32>(color & mask)) >> shift);
}
}  // namespace

Renderer::Renderer(SDL_Renderer *renderer)
    : m_renderer{renderer} {
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
}

std::optional<Renderer> Renderer::make(Window &window, Flag flag) {
    SDL_Renderer *renderer = SDL_CreateRenderer(window.m_window, -1, static_cast<SDL_RendererFlags>(flag));
    if (renderer == NULL) {  // NOLINT(modernize-use-nullptr)
        return std::nullopt;
    }
    return Renderer{renderer};
}

Renderer::Renderer(Renderer &&other) noexcept {
    m_renderer = std::exchange(other.m_renderer, nullptr);
}

Renderer &Renderer::operator=(Renderer &&other) noexcept {
    m_renderer = std::exchange(other.m_renderer, nullptr);
    return *this;
}

Renderer::~Renderer() {
    if (m_renderer != nullptr) {
        SDL_DestroyRenderer(m_renderer);
    }
}

void Renderer::set_draw(std::uint32_t color) {
    SDL_SetRenderDrawColor(m_renderer,
                           get_channel(color, 0xFF'00'00'00U, 24U),  // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
                           get_channel(color, 0x00'FF'00'00U, 16U),  // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
                           get_channel(color, 0x00'00'FF'00U, 8U),  // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
                           get_channel(color, 0x00'00'00'FFU, 0U));  // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
}

void Renderer::clear() {
    SDL_RenderClear(m_renderer);
}

void Renderer::present(Texture &texture) {
    SDL_RenderCopy(m_renderer, texture.m_texture, nullptr, nullptr);
    SDL_RenderPresent(m_renderer);
}
