#ifndef METABALLS_APP_DATA_HPP
#define METABALLS_APP_DATA_HPP  // NOLINT

#include "common_types.hpp"

#include <algorithm>
#include <cstddef>
#include <span>
#include <vector>

class AppData {
public:
    AppData(std::size_t pixel_width, std::size_t pixel_height);

    void add_metaball(Coordinate coord) { m_centers.push_back(coord); }

    void drag(Coordinate coord) {
        auto closest_center = find_closest(coord);
        if (closest_center != m_centers.end()) {
            *closest_center = coord;
        }
    }

    void remove(Coordinate coord) {
        auto closest_center = find_closest(coord);
        if (closest_center != m_centers.end()) {
            std::erase(m_centers, Coordinate{*closest_center});
        }
    }

    void clear_metaballs() { m_centers.clear(); }

    [[nodiscard]] std::span<PixelValue> pixels() noexcept { return m_pixels; }
    [[nodiscard]] std::span<Coordinate const> coordinates() const noexcept { return m_coordinates; }
    [[nodiscard]] std::span<Coordinate const> centers() const noexcept { return m_centers; }

private:
    std::vector<PixelValue> m_pixels{};
    std::vector<Coordinate> m_coordinates{};
    std::vector<Coordinate> m_centers{};

    [[nodiscard]] std::vector<Coordinate>::iterator find_closest(Coordinate coord) {
        static constexpr auto min_px_from_center = 20;
        return std::find_if(
            m_centers.begin(),
            m_centers.end(),
            [&](Coordinate center) {
                return std::abs(center.x - coord.x) < min_px_from_center
                       && std::abs(center.y - coord.y) < min_px_from_center;
            });
    }
};

#endif
