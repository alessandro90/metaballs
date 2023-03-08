#ifndef METABALLS_MAP_PIXELS_HPP
#define METABALLS_MAP_PIXELS_HPP  // NOLINT

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <execution>
#include <numeric>
#include <ranges>
#include <type_traits>
#include <utility>


template <typename F, typename R, typename... Args>
concept RegularInvocableR = std::regular_invocable<F, Args...> && std::is_invocable_r_v<R, F, Args...>;

template <typename Range, typename ValueType>
concept RangeWithValueType = std::same_as<std::ranges::range_value_t<Range>, ValueType>;

template <typename Range, typename ValueType>
concept InputValueRange = std::ranges::input_range<Range> && RangeWithValueType<Range, ValueType>;

template <typename C>
concept CoordinateLike =
    requires(C c) {
        { c.x };
        { c.y };
    }
    && std::integral<std::remove_cvref_t<decltype(std::declval<C>().x)>>
    && std::integral<std::remove_cvref_t<decltype(std::declval<C>().y)>>;

void update_pixels(std::size_t cols,
                   std::ranges::random_access_range auto &pixels,
                   std::ranges::input_range auto const &pixel_coordinates,
                   InputValueRange<std::ranges::range_value_t<decltype(pixel_coordinates)>> auto const &centers,
                   RegularInvocableR<float, std::ranges::range_value_t<decltype(pixel_coordinates)>, std::ranges::range_value_t<decltype(pixel_coordinates)>> auto calc_field,
                   RegularInvocableR<std::ranges::range_value_t<decltype(pixels)>, float> auto calc_pixel_value)
    requires CoordinateLike<std::ranges::range_value_t<decltype(pixel_coordinates)>>
{
    using Coordinate_t = std::ranges::range_value_t<decltype(pixel_coordinates)>;
    std::for_each(std::execution::unseq,
                  std::ranges::begin(pixel_coordinates),
                  std::ranges::end(pixel_coordinates),
                  [&](Coordinate_t const &pixel_coordinate) {
                      auto const pixel_field = std::accumulate(
                          centers.begin(),
                          centers.end(),
                          0.0F,
                          [&](float acc, Coordinate_t const &center) {
                              return acc + calc_field(pixel_coordinate, center);
                          });
                      pixels[cols * static_cast<std::size_t>(pixel_coordinate.x) + static_cast<std::size_t>(pixel_coordinate.y)] = calc_pixel_value(pixel_field);
                  });
}

#endif
