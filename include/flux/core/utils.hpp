
// Copyright (c) 2023 Tristan Brindle (tcbrindle at gmail dot com)
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef FLUX_CORE_UTILS_HPP_INCLUDED
#define FLUX_CORE_UTILS_HPP_INCLUDED

#include <flux/core/assert.hpp>

#include <concepts>
#include <cstdio>
#include <exception>
#include <source_location>
#include <stdexcept>
#include <type_traits>

#include <flux/core/numeric.hpp>

namespace flux {

/*
 * Useful helpers
 */
template <typename From, typename To>
concept decays_to = std::same_as<std::remove_cvref_t<From>, To>;

namespace detail {

struct copy_fn {
    template <typename T>
    [[nodiscard]]
    constexpr auto operator()(T&& arg) const
        noexcept(std::is_nothrow_convertible_v<T, std::decay_t<T>>)
        -> std::decay_t<T>
    {
        return FLUX_FWD(arg);
    }
};

} // namespace detail

inline constexpr auto copy = detail::copy_fn{};

namespace detail {

template <std::integral To>
struct checked_cast_fn {
    template <std::integral From>
    [[nodiscard]]
    constexpr auto operator()(From from) const -> To
    {
        if constexpr (requires { To{from}; }) {
            return To{from}; // not a narrowing conversion
        } else {
            To to = static_cast<To>(from);
            FLUX_DEBUG_ASSERT(static_cast<From>(to) == from);
            if constexpr (std::is_signed_v<From> != std::is_signed_v<To>) {
                FLUX_DEBUG_ASSERT((to < To{}) == (from < From{}));
            }
            return to;
        }
    }
};

} // namespace detail

template <std::integral To>
inline constexpr auto checked_cast = detail::checked_cast_fn<To>{};

} // namespace flux

#endif
