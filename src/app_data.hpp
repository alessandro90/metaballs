#ifndef METABALLS_APP_DATA_HPP
#define METABALLS_APP_DATA_HPP  // NOLINT

#include "common_types.hpp"

#include <cstddef>
#include <vector>

class AppData {
public:
    AppData(std::size_t pixel_width, std::size_t pixel_height);

    void add_metaball(Coordinate coord) { m_centers.push_back(coord); }

    [[nodiscard]] std::vector<PixelValue> &pixels() noexcept { return m_pixels; }
    [[nodiscard]] std::vector<Coordinate> const &coordinates() const noexcept { return m_coordinates; }
    [[nodiscard]] std::vector<Coordinate> &centers() noexcept { return m_centers; }

private:
    std::vector<PixelValue> m_pixels{};
    std::vector<Coordinate> m_coordinates{};
    std::vector<Coordinate> m_centers{};
};

#endif
