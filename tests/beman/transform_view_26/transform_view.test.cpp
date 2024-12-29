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
