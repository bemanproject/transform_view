// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/transform_view_26/transform_view.hpp>

#include <gtest/gtest.h>

#include <algorithm>
#include <forward_list>
#include <functional>
#include <list>
#include <string>
#include <vector>

namespace tv26 = beman::transform_view_26;

auto copy_lambda  = [](auto x) { return x; };
auto ref_lambda   = [](auto& x) -> decltype(auto) { return x; };
auto lower_lambda = [](char x) { return char(x + 0x20); };
auto upper_lambda = [](char x) { return char(x - 0x20); };

template <typename T>
concept has_iter_cat = requires { typename T::iterator_category; };

TEST(transform_view_, iter_concept_categroy) {
    // non-ref returned from f()
    {
        int arr[1] = {42};
        static_assert(std::ranges::contiguous_range<decltype(arr)>);
        using iterator_type =
            decltype(tv26::transform_view(arr, copy_lambda).begin());
        static_assert(std::same_as<typename iterator_type::iterator_concept,
                                   std::random_access_iterator_tag>);
        static_assert(std::same_as<typename iterator_type::iterator_category,
                                   std::input_iterator_tag>);
    }
    {
        using iterator_type =
            decltype(tv26::transform_view(std::list<int>(), copy_lambda)
                         .begin());
        static_assert(std::same_as<typename iterator_type::iterator_concept,
                                   std::bidirectional_iterator_tag>);
        static_assert(std::same_as<typename iterator_type::iterator_category,
                                   std::input_iterator_tag>);
    }
    {
        using iterator_type =
            decltype(tv26::transform_view(std::forward_list<int>(), copy_lambda)
                         .begin());
        static_assert(std::same_as<typename iterator_type::iterator_concept,
                                   std::forward_iterator_tag>);
        static_assert(std::same_as<typename iterator_type::iterator_category,
                                   std::input_iterator_tag>);
    }
    {
        std::istringstream str("");
        auto r = std::ranges::subrange(std::istream_iterator<int>(str),
                                       std::istream_iterator<int>());
        using iterator_type =
            decltype(tv26::transform_view(r, copy_lambda).begin());
        static_assert(std::same_as<typename iterator_type::iterator_concept,
                                   std::input_iterator_tag>);
        static_assert(!has_iter_cat<iterator_type>);
    }

    // ref returned from f()
    {
        int arr[1] = {42};
        static_assert(std::ranges::contiguous_range<decltype(arr)>);
        using iterator_type =
            decltype(tv26::transform_view(arr, ref_lambda).begin());
        static_assert(std::same_as<typename iterator_type::iterator_concept,
                                   std::random_access_iterator_tag>);
        static_assert(std::same_as<typename iterator_type::iterator_category,
                                   std::random_access_iterator_tag>);
    }
    {
        using iterator_type =
            decltype(tv26::transform_view(std::list<int>(), ref_lambda)
                         .begin());
        static_assert(std::same_as<typename iterator_type::iterator_concept,
                                   std::bidirectional_iterator_tag>);
        static_assert(std::same_as<typename iterator_type::iterator_category,
                                   std::bidirectional_iterator_tag>);
    }
    {
        using iterator_type =
            decltype(tv26::transform_view(std::forward_list<int>(), ref_lambda)
                         .begin());
        static_assert(std::same_as<typename iterator_type::iterator_concept,
                                   std::forward_iterator_tag>);
        static_assert(std::same_as<typename iterator_type::iterator_category,
                                   std::forward_iterator_tag>);
    }
    {
        std::istringstream str("");
        auto r = std::ranges::subrange(std::istream_iterator<int>(str),
                                       std::istream_iterator<int>());
        using iterator_type =
            decltype(tv26::transform_view(r, ref_lambda).begin());
        static_assert(std::same_as<typename iterator_type::iterator_concept,
                                   std::input_iterator_tag>);
        static_assert(!has_iter_cat<iterator_type>);
    }
}

TEST(transform_view_, default_ctor) {
    tv26::transform_view<std::ranges::empty_view<int>, decltype(copy_lambda)>
        view;
    std::vector<int> result;
    std::ranges::copy(view, std::back_inserter(result));
    EXPECT_TRUE(result.empty());
}

TEST(transform_view_, base_) {
    {
        std::vector<int>     ints;
        tv26::transform_view view(ints, copy_lambda);

        auto base = view.base();
        static_assert(std::same_as<decltype(base),
                                   std::ranges::ref_view<std::vector<int>>>);
        std::vector<int>& ints_ref = base.base();
        EXPECT_EQ(ints_ref, ints);
        EXPECT_EQ(std::addressof(ints_ref), std::addressof(ints));
    }
    {
        std::vector<int>     ints({100, 200, 300, 400, 500, 600, 700});
        tv26::transform_view view(ints, copy_lambda);

        auto base = view.base();
        static_assert(std::same_as<decltype(base),
                                   std::ranges::ref_view<std::vector<int>>>);
        std::vector<int>& ints_ref = base.base();
        EXPECT_EQ(ints_ref, ints);
        EXPECT_EQ(std::addressof(ints_ref), std::addressof(ints));
    }
    {
        std::vector<int>     ints;
        tv26::transform_view view(std::move(ints), copy_lambda);

        auto base = std::move(view).base();
        static_assert(std::same_as<decltype(base),
                                   std::ranges::owning_view<std::vector<int>>>);
        std::vector<int> ints_2 = std::move(base).base();
        EXPECT_EQ(ints, std::vector<int>());
        EXPECT_EQ(ints_2, std::vector<int>());
        EXPECT_NE(std::addressof(ints_2), std::addressof(ints));
    }
    {
        std::vector<int>     ints({100, 200, 300, 400, 500, 600, 700});
        tv26::transform_view view(std::move(ints), copy_lambda);

        auto base = std::move(view).base();
        static_assert(std::same_as<decltype(base),
                                   std::ranges::owning_view<std::vector<int>>>);
        std::vector<int> ints_2 = std::move(base).base();
        EXPECT_EQ(ints, std::vector<int>());
        EXPECT_EQ(ints_2,
                  std::vector<int>({100, 200, 300, 400, 500, 600, 700}));
        EXPECT_NE(std::addressof(ints_2), std::addressof(ints));
    }
}

struct null_sentinel_t {
    template <std::input_iterator I>
        requires std::default_initializable<std::iter_value_t<I>> &&
                 std::equality_comparable_with<std::iter_reference_t<I>,
                                               std::iter_value_t<I>>
    friend constexpr bool operator==(I it, null_sentinel_t) {
        return *it == std::iter_value_t<I>{};
    }
};

inline constexpr null_sentinel_t null_sentinel;

template <typename CharT>
constexpr auto null_term(CharT* ptr) {
    return std::ranges::subrange(ptr, null_sentinel);
}

TEST(transform_view_, begin_end) {
    {
        auto view = tv26::transform_view(std::string(), copy_lambda);
        EXPECT_EQ(view.begin(), view.end());
        EXPECT_EQ(view.begin() - view.end(), 0);
        EXPECT_EQ(view.end() - view.begin(), 0);
#if !defined(_MSC_VER)
        auto underlying_first = view.begin().base().base();
        auto underlying_last  = view.end().base().base();
        static_assert(std::same_as<decltype(underlying_first),
                                   decltype(underlying_last)>);
#endif

        {
            const auto const_view = std::move(view);
            EXPECT_EQ(const_view.begin(), const_view.end());
            EXPECT_EQ(const_view.begin() - const_view.end(), 0);
            EXPECT_EQ(const_view.end() - const_view.begin(), 0);
#if !defined(_MSC_VER)
            auto underlying_first = const_view.begin().base().base();
            auto underlying_last  = const_view.end().base().base();
            static_assert(std::same_as<decltype(underlying_first),
                                       decltype(underlying_last)>);
#endif
        }
    }
    {
        auto view = tv26::transform_view(std::string("str"), copy_lambda);
        EXPECT_NE(view.begin(), view.end());
        EXPECT_EQ(view.begin() - view.end(), -3);
        EXPECT_EQ(view.end() - view.begin(), 3);

        {
            auto it = view.begin();
            for (auto it2 = view.begin(), last = view.end(); it2 != last;
                 ++it2) {
                EXPECT_EQ(*it2, *it++);
            }
        }
        {
            auto it = view.end();
            for (auto it2 = view.end(), first = view.begin(); it2 != first;
                 --it2) {
                EXPECT_EQ(*std::ranges::prev(it2), *std::ranges::prev(it--));
            }
        }

        EXPECT_FALSE(view.begin() != view.begin());
        EXPECT_TRUE(view.begin() != view.end());
        EXPECT_TRUE(view.begin() == view.begin());
        EXPECT_FALSE(view.begin() == view.end());
        EXPECT_FALSE(view.begin() < view.begin());
        EXPECT_TRUE(view.begin() < view.end());
        EXPECT_TRUE(view.begin() <= view.begin());
        EXPECT_TRUE(view.begin() <= view.end());
        EXPECT_TRUE(view.begin() >= view.begin());
        EXPECT_FALSE(view.begin() >= view.end());
        EXPECT_FALSE(view.begin() > view.begin());
        EXPECT_FALSE(view.begin() > view.end());

        EXPECT_TRUE(view.begin() <=> view.begin() == 0);
        EXPECT_TRUE(view.begin() <=> view.end() < 0);
        EXPECT_TRUE(view.end() <=> view.begin() > 0);

        EXPECT_EQ(view.begin() + 1, view.end() - 2);
        EXPECT_EQ(view.begin() + 2, view.end() - 1);
        EXPECT_EQ(1 + view.begin(), view.end() - 2);
        EXPECT_EQ(2 + view.begin(), view.end() - 1);
        EXPECT_EQ(view.begin() += 1, view.end() -= 2);
        EXPECT_EQ(view.begin() += 2, view.end() -= 1);

        EXPECT_EQ(view.begin()[0], 's');
        EXPECT_EQ(view.begin()[1], 't');
        EXPECT_EQ(view.begin()[2], 'r');

        {
            const auto const_view = std::move(view);
            EXPECT_NE(const_view.begin(), const_view.end());
            EXPECT_EQ(const_view.begin() - const_view.end(), -3);
            EXPECT_EQ(const_view.end() - const_view.begin(), 3);

            {
                auto it = const_view.begin();
                for (auto it2 = const_view.begin(), last = const_view.end();
                     it2 != last;
                     ++it2) {
                    EXPECT_EQ(*it2, *it++);
                }
            }
            {
                auto it = const_view.end();
                for (auto it2 = const_view.end(), first = const_view.begin();
                     it2 != first;
                     --it2) {
                    EXPECT_EQ(*std::ranges::prev(it2),
                              *std::ranges::prev(it--));
                }
            }

            EXPECT_FALSE(const_view.begin() != const_view.begin());
            EXPECT_TRUE(const_view.begin() != const_view.end());
            EXPECT_TRUE(const_view.begin() == const_view.begin());
            EXPECT_FALSE(const_view.begin() == const_view.end());
            EXPECT_FALSE(const_view.begin() < const_view.begin());
            EXPECT_TRUE(const_view.begin() < const_view.end());
            EXPECT_TRUE(const_view.begin() <= const_view.begin());
            EXPECT_TRUE(const_view.begin() <= const_view.end());
            EXPECT_TRUE(const_view.begin() >= const_view.begin());
            EXPECT_FALSE(const_view.begin() >= const_view.end());
            EXPECT_FALSE(const_view.begin() > const_view.begin());
            EXPECT_FALSE(const_view.begin() > const_view.end());

            EXPECT_TRUE(const_view.begin() <=> const_view.begin() == 0);
            EXPECT_TRUE(const_view.begin() <=> const_view.end() < 0);
            EXPECT_TRUE(const_view.end() <=> const_view.begin() > 0);

            EXPECT_EQ(const_view.begin() + 1, const_view.end() - 2);
            EXPECT_EQ(const_view.begin() + 2, const_view.end() - 1);
            EXPECT_EQ(1 + const_view.begin(), const_view.end() - 2);
            EXPECT_EQ(2 + const_view.begin(), const_view.end() - 1);
            EXPECT_EQ(const_view.begin() += 1, const_view.end() -= 2);
            EXPECT_EQ(const_view.begin() += 2, const_view.end() -= 1);

            EXPECT_EQ(const_view.begin()[0], 's');
            EXPECT_EQ(const_view.begin()[1], 't');
            EXPECT_EQ(const_view.begin()[2], 'r');
        }
    }
    {
        const char* str  = "";
        auto        view = tv26::transform_view(null_term(str), copy_lambda);
        EXPECT_EQ(view.begin(), view.end());
        auto underlying_first = view.begin().base();
        auto underlying_last  = view.end().base();
        static_assert(!std::same_as<decltype(underlying_first),
                                    decltype(underlying_last)>);
        static_assert(std::same_as<decltype(underlying_last), null_sentinel_t>);

        {
            const auto const_view = std::move(view);
            EXPECT_EQ(const_view.begin(), const_view.end());
            auto underlying_first = const_view.begin().base();
            auto underlying_last  = const_view.end().base();
            static_assert(!std::same_as<decltype(underlying_first),
                                        decltype(underlying_last)>);
            static_assert(
                std::same_as<decltype(underlying_last), null_sentinel_t>);
        }
    }
    {
        const char* str  = "str";
        auto        view = tv26::transform_view(null_term(str), copy_lambda);
        EXPECT_NE(view.begin(), view.end());
    }
}

TEST(transform_view_, copy_func_empty_range_copy_alg) {
    std::vector<int> ints;

    std::vector<int> result;
    std::ranges::copy(tv26::transform_view(ints, copy_lambda),
                      std::back_inserter(result));
    EXPECT_EQ(result, ints);

    {
        auto view = tv26::transform_view(ints, copy_lambda);
        EXPECT_EQ(view.size(), ints.size());
    }
    {
        const auto view = tv26::transform_view(ints, copy_lambda);
        EXPECT_EQ(view.size(), ints.size());

        std::vector<int> result;
        std::ranges::copy(view, std::back_inserter(result));
        EXPECT_EQ(result, ints);
    }
}

TEST(transform_view_, copy_func_copy_alg) {
    std::vector<int> ints({100, 200, 300, 400, 500, 600, 700});

    std::vector<int> result;
    std::ranges::copy(tv26::transform_view(ints, copy_lambda),
                      std::back_inserter(result));
    EXPECT_EQ(result, ints);

    {
        auto view = tv26::transform_view(ints, copy_lambda);
        EXPECT_EQ(view.size(), ints.size());
    }
    {
        const auto view = tv26::transform_view(ints, copy_lambda);
        EXPECT_EQ(view.size(), ints.size());

        std::vector<int> result;
        std::ranges::copy(view, std::back_inserter(result));
        EXPECT_EQ(result, ints);
    }
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

TEST(transform_view_, lower_func_copy_alg) {
    std::string str = "LOWER";
    std::string result;
    std::ranges::copy(tv26::transform_view(str, lower_lambda),
                      std::back_inserter(result));
    EXPECT_EQ(result, "lower");
}

TEST(transform_view_, lower_func_range_adaptor) {
    std::string str = "LOWER";
    std::string result;
    std::ranges::copy(str | tv26::views::transform(lower_lambda),
                      std::back_inserter(result));
    EXPECT_EQ(result, "lower");
}

TEST(transform_view_, lower_func_adaptor_to_vec) {
    std::string str = "LOWER";
    std::string result = str | tv26::views::transform(lower_lambda) |
                         std::ranges::to<std::basic_string>();
    EXPECT_EQ(result, "lower");
}

TEST(transform_view_, lower_upper_func_copy_alg) {
    std::string str = "UPPER";
    std::string result;
    std::ranges::copy(
        tv26::transform_view(tv26::transform_view(str, lower_lambda),
                             upper_lambda),
        std::back_inserter(result));
    EXPECT_EQ(result, "UPPER");
}

TEST(transform_view_, lower_upper_func_range_adaptor) {
    std::string str = "UPPER";
    std::string result;
    std::ranges::copy(str | tv26::views::transform(lower_lambda) |
                          tv26::views::transform(upper_lambda),
                      std::back_inserter(result));
    EXPECT_EQ(result, "UPPER");
}

TEST(transform_view_, lower_upper_func_adaptor_to_vec) {
    std::string str = "UPPER";
    std::string result = str | tv26::views::transform(lower_lambda) |
                         tv26::views::transform(upper_lambda) |
                         std::ranges::to<std::basic_string>();
    EXPECT_EQ(result, "UPPER");
}

TEST(transform_view_, sentinel_lower_func_copy_alg) {
    const char* str = "LOWER";
    std::string result;
    std::ranges::copy(tv26::transform_view(null_term(str), lower_lambda),
                      std::back_inserter(result));
    EXPECT_EQ(result, "lower");
}

TEST(transform_view_, sentinel_lower_func_range_adaptor) {
    const char* str = "LOWER";
    std::string result;
    std::ranges::copy(null_term(str) | tv26::views::transform(lower_lambda),
                      std::back_inserter(result));
    EXPECT_EQ(result, "lower");
}

TEST(transform_view_, sentinel_lower_func_adaptor_to_vec) {
    const char* str    = "LOWER";
    std::string result = null_term(str) | tv26::views::transform(lower_lambda) |
                         std::ranges::to<std::basic_string>();
    EXPECT_EQ(result, "lower");
}

TEST(transform_view_, sentinel_lower_upper_func_copy_alg) {
    const char* str = "UPPER";
    std::string result;
    std::ranges::copy(
        tv26::transform_view(tv26::transform_view(null_term(str), lower_lambda),
                             upper_lambda),
        std::back_inserter(result));
    EXPECT_EQ(result, "UPPER");
}

TEST(transform_view_, sentinel_lower_upper_func_range_adaptor) {
    const char* str = "UPPER";
    std::string result;
    std::ranges::copy(null_term(str) | tv26::views::transform(lower_lambda) |
                          tv26::views::transform(upper_lambda),
                      std::back_inserter(result));
    EXPECT_EQ(result, "UPPER");
}

TEST(transform_view_, sentinel_lower_upper_func_adaptor_to_vec) {
    const char* str    = "UPPER";
    std::string result = null_term(str) | tv26::views::transform(lower_lambda) |
                         tv26::views::transform(upper_lambda) |
                         std::ranges::to<std::basic_string>();
    EXPECT_EQ(result, "UPPER");
}

TEST(transform_view_, borrowability) {
    {
        auto view = std::string() | tv26::views::transform(lower_lambda);
        static_assert(!std::ranges::borrowed_range<decltype(view)>);
    }
    {
        const char* str = "";
        auto view       = null_term(str) | tv26::views::transform(lower_lambda);
        static_assert(std::ranges::borrowed_range<decltype(null_term(str))>);
        static_assert(std::ranges::borrowed_range<decltype(view)>);
    }
    {
        const char* str = "";
        auto view = null_term(str) | tv26::views::transform(lower_lambda) |
                    tv26::views::transform(upper_lambda);
        static_assert(std::ranges::borrowed_range<decltype(null_term(str))>);
        static_assert(std::ranges::borrowed_range<decltype(view)>);
    }
}

auto make_transform_view_subrange(const char* str) {
    auto view = null_term(str) | tv26::views::transform(lower_lambda);
    return std::ranges::subrange(view.begin(), view.end());
}

auto make_nested_transform_view_subrange(const char* str) {
    auto view = null_term(str) | tv26::views::transform(lower_lambda) |
                tv26::views::transform(upper_lambda);
    return std::ranges::subrange(view.begin(), view.end());
}

TEST(transform_view_, borrowability_safety) {
    {
        const char* str    = "LOWER";
        auto        view   = make_transform_view_subrange(str);
        std::string result = view | std::ranges::to<std::basic_string>();
        EXPECT_EQ(result, "lower");
    }
    {
        const char* str  = "UPPER";
        auto        view = make_transform_view_subrange(str) |
                    tv26::views::transform(upper_lambda);
        std::string result = view | std::ranges::to<std::basic_string>();
        EXPECT_EQ(result, "UPPER");
    }
    {
        const char* str    = "UPPER";
        auto        view   = make_nested_transform_view_subrange(str);
        std::string result = view | std::ranges::to<std::basic_string>();
        EXPECT_EQ(result, "UPPER");
    }
}

#if 0 // Enable this to see ASan catch the memory unsafety of using a
      // non-borrowable range.
auto make_dangling_transform_view_subrange(const char* str) {
    auto view = std::string(str) | tv26::views::transform(lower_lambda);
    return std::ranges::subrange(view.begin(), view.end());
}

TEST(transform_view_, borrowability_safety_failure) {
    const char* str    = "LOWER";
    auto        view   = make_dangling_transform_view_subrange(str);
    std::string result = view | std::ranges::to<std::basic_string>();
    EXPECT_EQ(result, "lower");
}
#endif
