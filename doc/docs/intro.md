---
sidebar_position: 1
---

# Description

The view `std::ranges::transform_view` (<Stdref ref="range.transform"/>)
introduced with C++20 is unconditionally a non-borrowed range (see <Stdref
ref="range.range"/>).  <Paper num="P3117"/> was written to address this,
making `transform_view` conditionally borrowable.

The condition for its borrowability is that the callable `F` given to
`std::ranges::transform_view` must have properties that allow its trivial
construction and destruction, on-demand.  The paper calls this property being
"tidy".  This is the exposition-only concept that determines tidiness:

```cpp
template<class F>
  constexpr bool tidy-func =
    is_empty_v<F> && is_trivially_default_constructible_v<F> && is_trivially_destructible_v<F>;
```

# Usage

To use the revised version of `transform_view`, simply use it as you would
`std::ranges::transform_view`.  If the template parameter `F` you use is
"tidy", your `transform_view` will be a borrowed range.
