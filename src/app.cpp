#include "app.hpp"
#include "SDL2/SDL.h"
#include "app_data.hpp"
#include "common_types.hpp"
#include "renderer.hpp"
#include "texture.hpp"
#include "update_pixels.hpp"
#include "window.hpp"

#include <SDL_mouse.h>
#include <SDL_scancode.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <optional>
#include <utility>
#include <vector>

// #include <chrono>
// #include <iostream>

namespace {

constexpr std::size_t g_screen_witdh = 1200;
constexpr std::size_t g_screen_height = 900;

constexpr auto g_small = 1e-8F;


// [[nodiscard]] float calc_field(Coordinate c1, Coordinate c2) {
//     auto const r = (c1.x - c2.x) * (c1.x - c2.x) + (c1.y - c2.y) * (c1.y - c2.y);
//     return r == 0 ? 1.0F : (1.0F / std::sqrt(static_cast<float>(r)));
// }


[[nodiscard]] float calc_field_approx(Coordinate c1, Coordinate c2) {
    static constexpr float g_metaball_radius = 100.0F;
    auto const r = (c1.x - c2.x) * (c1.x - c2.x) + (c1.y - c2.y) * (c1.y - c2.y);
    return g_metaball_radius / (static_cast<float>(r) + g_small);
}

[[nodiscard]] PixelValue calc_pixel_full(float pixel_field) {
    static constexpr float threshold = 0.01F;
    return pixel_field > threshold ? 0xFDDB27FFU : 0x00B1D2FFU;  // NOLINT
}

[[nodiscard]] PixelValue calc_pixel_value_bw(float pixel_field) {
    const auto i = static_cast<std::uint8_t>(std::lerp(0.F, 255.F, std::min(50.F * pixel_field, 1.F)));
    return static_cast<PixelValue>(
        static_cast<PixelValue>(i << 24U)  // NOLINT
        | static_cast<PixelValue>(i << 16U)  // NOLINT
        | static_cast<PixelValue>(i << 8U)  // NOLINT
        | 0xFFU);  // NOLINT
}

template <std::size_t BitShift>  // 8==blue; 16==green; 24==red
[[nodiscard]] PixelValue calc_pixel_value_single_color(float pixel_field) {
    static_assert(BitShift == 8 || BitShift == 16 || BitShift == 24);  // NOLINT
    const auto i = static_cast<std::uint8_t>(std::lerp(0.F, 255.F, std::min(50.F * pixel_field, 1.F)));
    return static_cast<PixelValue>(i << BitShift);
}

[[nodiscard]] PixelValue calc_pixel_ring(float pixel_field) {
    if (pixel_field < 0.013F && pixel_field > 0.01F) { return 0xD6ED17FFU; }  // NOLINT
    return 0x606060FFU;  // NOLINT
}

[[nodiscard]] PixelValue calc_pixel_ring_full(float pixel_field) {
    if (pixel_field > 0.013F) { return 0xA8D5BAFFU; }  // NOLINT
    if (pixel_field > 0.01F) { return 0xD7A9E3FFU; }  // NOLINT
    return 0x8BBEE8FFU;  // NOLINT
}

constexpr auto g_patterns = std::array{
    &calc_pixel_full,
    &calc_pixel_value_bw,
    &calc_pixel_value_single_color<8>,
    &calc_pixel_value_single_color<16>,
    &calc_pixel_value_single_color<24>,
    &calc_pixel_ring,
    &calc_pixel_ring_full};

[[nodiscard]] auto find_closest(std::vector<Coordinate> &centers, Coordinate coord) {
    static constexpr auto min_px_from_center = 20;
    return std::find_if(
        centers.begin(),
        centers.end(),
        [&](Coordinate center) {
            return std::abs(center.x - coord.x) < min_px_from_center
                   && std::abs(center.y - coord.y) < min_px_from_center;
        });
}
}  // namespace

std::optional<App> App::make() {
    auto maybe_window = Window::make("METABALLS",
                                     Window::Position::CENTERED,
                                     Window::Position::CENTERED,
                                     g_screen_witdh,
                                     g_screen_height);
    if (!maybe_window.has_value()) {
        return std::nullopt;
    }
    auto &window = maybe_window.value();
    auto maybe_renderer = Renderer::make(window, Renderer::Flag::ACCELERATED);
    if (!maybe_renderer.has_value()) {
        return std::nullopt;
    }
    auto &renderer = maybe_renderer.value();

    auto maybe_texture = Texture::make(renderer,
                                       SDL_PIXELFORMAT_RGBA8888,
                                       SDL_TEXTUREACCESS_STREAMING,
                                       g_screen_witdh,
                                       g_screen_height);

    if (!maybe_texture.has_value()) {
        return std::nullopt;
    }
    auto &texture = maybe_texture.value();

    return App{std::move(window), std::move(renderer), std::move(texture)};
}

App::App(Window &&window, Renderer &&renderer, Texture &&texture)
    : m_window{std::move(window)}
    , m_renderer{std::move(renderer)}
    , m_texture{std::move(texture)}
    , m_data{g_screen_witdh, g_screen_height} {
    SDL_Init(SDL_INIT_EVERYTHING);
}

App &App::operator=(App &&other) noexcept {
    other.m_quit_on_destroy = false;
    m_window = std::move(other.m_window);
    m_renderer = std::move(other.m_renderer);
    m_texture = std::move(other.m_texture);
    m_data = std::move(other.m_data);
    return *this;
}

App::App(App &&other) noexcept
    : m_window{std::move(other.m_window)}
    , m_renderer{std::move(other.m_renderer)}
    , m_texture{std::move(other.m_texture)}
    , m_data{std::move(other.m_data)} {
    other.m_quit_on_destroy = false;
}

App::~App() {
    if (m_quit_on_destroy) {
        SDL_Quit();
    }
}

void App::quit() { m_running = false; }

void App::spawn_metaball(Coordinate c) {
    m_data.add_metaball(c);
}

void App::maybe_drag_a_metaball(Coordinate coord) {
    auto &centers = m_data.centers();
    auto closest_center = find_closest(centers, coord);
    if (closest_center != centers.end()) {
        *closest_center = coord;
    }
}

void App::maybe_delete_a_metaball(Coordinate coord) {
    auto &centers = m_data.centers();
    auto closest_center = find_closest(centers, coord);
    if (closest_center != centers.end()) {
        std::erase(centers, Coordinate{*closest_center});
    }
}

void App::handle_events() {
    for (SDL_Event ev; SDL_PollEvent(&ev) != 0;) {
        switch (ev.type) {
        case SDL_QUIT:
            quit();
            break;
        case SDL_KEYDOWN:
            if (ev.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                quit();
            } else if (ev.key.keysym.scancode == SDL_SCANCODE_X) {
                m_pattern_index = (m_pattern_index + 1U) % g_patterns.size();
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (ev.button.clicks != 1) {
                break;
            }
            switch (ev.button.button) {
            case SDL_BUTTON_RIGHT:
                spawn_metaball({ev.button.y, ev.button.x});
                break;
            case SDL_BUTTON_MIDDLE:
                maybe_delete_a_metaball({ev.button.y, ev.button.x});
                break;
            default:
                break;
            }
            break;
        case SDL_MOUSEMOTION:  // Not the more efficient approach. A state-based logic would prpbably be more efficient.
            if ((ev.motion.state & static_cast<Uint32>(SDL_BUTTON_LMASK)) != 0) {
                maybe_drag_a_metaball({ev.motion.y, ev.motion.x});
            }
            break;
        default:
            break;
        }
    }
}

void App::run() {
    while (m_running) {
        handle_events();

        auto &pixels = m_data.pixels();

        // auto const x = std::chrono::system_clock::now();

        update_pixels(g_screen_witdh,
                      pixels,
                      m_data.coordinates(),
                      m_data.centers(),
                      calc_field_approx,
                      g_patterns[m_pattern_index]);  // NOLINT

        // auto const y = std::chrono::system_clock::now();

        // std::cout << "Elapsed: " << std::chrono::duration_cast<std::chrono::milliseconds>(y - x) << '\n';

        m_texture.update(pixels.data(), g_screen_witdh * 4U);
        m_renderer.present(m_texture);
    }
}
