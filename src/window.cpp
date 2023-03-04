#include "window.hpp"
#include "SDL2/SDL.h"
#include <cassert>
#include <cstddef>
#include <limits>
#include <optional>
#include <string_view>
#include <utility>

Window::Window(SDL_Window *window)
    : m_window{window} {}

std::optional<Window> Window::make(std::string_view title,
                                   Position x,
                                   Position y,
                                   std::size_t w,
                                   std::size_t h) {
    assert(w <= static_cast<std::size_t>(std::numeric_limits<int>::max()));
    assert(h <= static_cast<std::size_t>(std::numeric_limits<int>::max()));

    SDL_Window *window = SDL_CreateWindow(title.data(),
                                          static_cast<int>(x),
                                          static_cast<int>(y),
                                          static_cast<int>(w),
                                          static_cast<int>(h),
                                          SDL_WINDOW_SHOWN);
    if (window == NULL) {  // NOLINT(modernize-use-nullptr)
        return std::nullopt;
    }
    return Window{window};
}

Window::Window(Window &&other) noexcept {
    m_window = std::exchange(other.m_window, nullptr);
}

Window &Window::operator=(Window &&other) noexcept {
    m_window = std::exchange(other.m_window, nullptr);
    return *this;
}

Window::~Window() {
    if (m_window != nullptr) {
        SDL_DestroyWindow(m_window);
    }
}