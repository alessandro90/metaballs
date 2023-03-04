#include "app_data.hpp"
#include "common_types.hpp"
#include <cstddef>

AppData::AppData(std::size_t pixel_width, std::size_t pixel_height) {
    m_pixels.resize(pixel_width * pixel_height);
    m_coordinates.reserve(pixel_width * pixel_height);
    for (std::size_t i = 0; i < pixel_height; ++i) {
        for (std::size_t j = 0; j < pixel_width; ++j) {
            m_coordinates.emplace_back(static_cast<Int>(i), static_cast<Int>(j));
        }
    }
    // m_coordinates.resize(pixel_width * pixel_height);
    // for (std::size_t i = 0; i < pixel_height; ++i) {
    //     for (std::size_t j = 0; j < pixel_width; ++j) {
    //         m_coordinates[pixel_width * i + j] = Coordinate{static_cast<Int>(i), static_cast<Int>(j)};
    //     }
    // }
}
