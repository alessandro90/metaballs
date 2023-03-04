#ifndef METABALLS_COMMON_TYPES_HPP
#define METABALLS_COMMON_TYPES_HPP  // NOLINT

#include <cstdint>

using Int = std::int32_t;

struct Coordinate {
    Int x;
    Int y;

    [[nodiscard]] constexpr bool operator==(Coordinate const &other) const noexcept {
        return x == other.x && y == other.y;
    }
};

using PixelValue = std::uint32_t;


#endif
