
// Copyright (c) 2022 Tristan Brindle (tcbrindle at gmail dot com)
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "catch.hpp"

#include <flux.hpp>

#include <sstream>
#include <vector>

TEST_CASE("write to")
{
    SECTION("Basic write_to")
    {
        std::vector vec{1, 2, 3, 4, 5};

        std::ostringstream oss;

        flux::write_to(vec, oss);

        REQUIRE(oss.str() == "[1, 2, 3, 4, 5]");
    }

    SECTION("Nested sequences")
    {
        std::vector<std::vector<std::vector<int>>> vec{
            { {1, 2}, {3, 4} },
            { {5, 6}, {7, 8} },
            { {9, 10}, {11, 12} }
        };

        std::ostringstream oss;

        flux::ref(vec).write_to(oss);

        REQUIRE(oss.str() == "[[[1, 2], [3, 4]], [[5, 6], [7, 8]], [[9, 10], [11, 12]]]");
    }

    SECTION("Reading and writing streams")
    {
        std::istringstream iss("1 2 3 4 5");
        std::ostringstream oss;

        flux::from_istream<int>(iss).write_to(oss) << "\n";

        REQUIRE(oss.str() == "[1, 2, 3, 4, 5]\n");
    }
}