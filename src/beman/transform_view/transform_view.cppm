module;

#ifndef BEMAN_HAS_STD_MODULES
#include <functional>
#include <iterator>
#include <optional>
#include <ranges>
#else
import std;
#endif

export module beman.transform_view;

extern "C++" {
export {
#include <beman/transform_view/transform_view.hpp>
}
}
