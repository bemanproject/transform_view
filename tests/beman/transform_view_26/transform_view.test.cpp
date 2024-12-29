// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/transform_view_26/transform_view.hpp>

#include <gtest/gtest.h>

#include <algorithm>
#include <functional>
#include <string>
#include <vector>

namespace tv26 = beman::transform_view_26;

auto copy_lambda = [](auto x) { return x; };

TEST(transform_view_, copy_func_empty_range_copy_alg) {
    std::vector<int> ints;

    std::vector<int> result;
    std::ranges::copy(tv26::transform_view(ints, copy_lambda),
                      std::back_inserter(result));
    EXPECT_EQ(result, ints);
}

TEST(transform_view_, copy_func_copy_alg) {
    std::vector<int> ints({100, 200, 300, 400, 500, 600, 700});

    std::vector<int> result;
    std::ranges::copy(tv26::transform_view(ints, copy_lambda),
                      std::back_inserter(result));
    EXPECT_EQ(result, ints);
}

TEST(transform_view_, copy_func_empty_range_adaptor_copy_alg) {
    std::vector<int> ints;

    std::vector<int> result;
    std::ranges::copy(ints | tv26::views::transform(copy_lambda),
                      std::back_inserter(result));
    EXPECT_EQ(result, ints);
}

TEST(transform_view_, copy_func_adaptor_copy_alg) {
    std::vector<int> ints({100, 200, 300, 400, 500, 600, 700});

    std::vector<int> result;
    std::ranges::copy(ints | tv26::views::transform(copy_lambda),
                      std::back_inserter(result));
    EXPECT_EQ(result, ints);
}

TEST(transform_view_, copy_func_empty_range_adaptor_to_vec) {
    std::vector<int> ints;

    std::vector<int> result = ints | tv26::views::transform(copy_lambda) |
                              std::ranges::to<std::vector>();
    EXPECT_EQ(result, ints);
}

TEST(transform_view_, copy_func_adaptor_to_vec) {
    std::vector<int> ints({100, 200, 300, 400, 500, 600, 700});

    std::vector<int> result = ints | tv26::views::transform(copy_lambda) |
                              std::ranges::to<std::vector>();
    EXPECT_EQ(result, ints);
}
