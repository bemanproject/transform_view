// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/transform_view_26/transform_view.hpp>

#include <iostream>

namespace tv26 = beman::transform_view_26;

int main() {
    auto to_lower = [](char c) { return char(c + 0x20); };

    const std::string upper_str = "LOWER";

    std::cout << (upper_str | tv26::views::transform(to_lower) |
                  std::ranges::to<std::string>())
              << '\n';

    return 0;
}
