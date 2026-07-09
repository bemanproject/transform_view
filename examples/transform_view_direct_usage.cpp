// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/transform_view/config.hpp>
#include <beman/transform_view/transform_view.hpp>

#if BEMAN_TRANSFORM_VIEW_USE_MODULES()
import std;
#else
#include <iostream>
#endif

namespace tv26 = beman::transform_view;

int main() {
    auto to_lower = [](char c) { return char(c + 0x20); };

    const std::string upper_str = "LOWER";

    std::cout << (upper_str | tv26::views::transform(to_lower) |
                  std::ranges::to<std::string>())
              << '\n';

    return 0;
}
