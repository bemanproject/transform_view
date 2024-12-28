// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/transform_view_26/identity.hpp>

#include <iostream>

namespace exe = beman::transform_view_26;

int main() {
    std::cout << exe::identity()(2024) << '\n';
    return 0;
}
