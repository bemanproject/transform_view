// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef BEMAN_TRANSFORM_VIEW_HPP
#define BEMAN_TRANSFORM_VIEW_HPP

#include <functional>
#include <ranges>

namespace beman::transform_view_26 {

namespace detail {
template <bool Const, typename T>
using maybe_const = std::conditional_t<Const, const T, T>;

template <typename T>
using with_ref = T&;
template <typename T>
concept can_ref = requires { typename with_ref<T>; };

template <typename Base>
constexpr auto concept_tag() {
    if constexpr (std::ranges::random_access_range<Base>) {
        return std::random_access_iterator_tag{};
    } else if constexpr (std::ranges::bidirectional_range<Base>) {
        return std::bidirectional_iterator_tag{};
    } else if constexpr (std::ranges::forward_range<Base>) {
        return std::forward_iterator_tag{};
    } else {
        return std::input_iterator_tag{};
    }
}

template <typename Base, typename F>
constexpr auto category_tag() {
    if constexpr (!std::ranges::forward_range<Base>) {
        return 0; // int means "no tag"
    } else {
        using C = typename std::iterator_traits<
            std::ranges::iterator_t<Base>>::​iterator_category;
        constexpr bool call_result_is_ref = std::is_reference_v<
            std::invoke_result_t<F&, std::ranges::range_reference_t<Base>>>;
        if constexpr (call_result_is_ref) {
            if constexpr (std::derived_from<C, std::contiguous_iterator_tag>) {
                return std::random_access_iterator_tag{};
            } else {
                return C();
            }
        } else {
            return std::input_iterator_tag{};
        }
    }
}
template <typename Base,
          typename F,
          typename IteratorCategory = decltype(detail::category_tag<Base, F>())>
struct iterator_category_base {
    using iterator_category = IteratorCategory;
};
template <typename Base, typename F>
struct iterator_category_base<Base, F, int> {};

template <typename T>
concept boxable = std::move_constructible<T> && std::is_object_v<T>;
template <boxable T>
struct movable_box : std::optional<T> {
    constexpr movable_box() noexcept(std::is_nothrow_default_constructible_v<T>)
        requires std::default_initializable<T>
        : std::optional<T>{std::in_place} {}

    movable_box(const movable_box&) = default;
    movable_box(movable_box&&)      = default;

    constexpr movable_box& operator=(const movable_box& rhs) noexcept(
        std::is_nothrow_copy_constructible_v<T>)
        requires(!std::copyable<T>) && std::copy_constructible<T>
    {
        if (std::addressof(rhs) == this)
            return *this;
        if (rhs)
            this->emplace(*rhs);
        else
            this->reset();
        return *this;
    }

    constexpr movable_box& operator=(movable_box&& rhs) noexcept(
        std::is_nothrow_move_constructible_v<T>)
        requires(!std::movable<T>)
    {
        if (std::addressof(rhs) == this)
            return *this;
        if (rhs)
            this->emplace(std::move(*rhs));
        else
            this->reset();
        return *this;
    }

    using std::optional<T>::optional;
    using std::optional<T>::operator=;
};
} // namespace detail

template <std::ranges::input_range V, std::move_constructible F>
    requires std::ranges::view<V> && std::is_object_v<F> &&
             std::regular_invocable<F&, std::ranges::range_reference_t<V>> &&
             detail::can_ref<
                 std::invoke_result_t<F&, std::ranges::range_reference_t<V>>>
class transform_view
    : public std::ranges::view_interface<transform_view<V, F>> {
    template <bool Const>
    class iterator
        : public detail::iterator_category_base<detail::maybe_const<Const, V>,
                                                detail::maybe_const<Const, F>> {
        using Parent = detail::maybe_const<Const, transform_view>;
        using Base   = detail::maybe_const<Const, V>;
        std::ranges::iterator_t<Base> current_ =
            std::ranges::iterator_t<Base>();
        Parent* parent_ = nullptr;

      public:
        using iterator_concept = decltype(detail::concept_tag<Base>());
        using value_type       = std::remove_cvref_t<
                  std::invoke_result_t<detail::maybe_const<Const, F>&,
                                       std::ranges::range_reference_t<Base>>>;
        using difference_type = std::ranges::range_difference_t<Base>;

        iterator()
            requires std::default_initializable<std::ranges::iterator_t<Base>>
        = default;
        constexpr iterator(Parent&                       parent,
                           std::ranges::iterator_t<Base> current)
            : current_(std::move(current)),
              parent_(std::addressof(parent)) {}
        constexpr iterator(iterator<!Const> i)
            requires Const && std::convertible_to<std::ranges::iterator_t<V>,
                                                  std::ranges::iterator_t<Base>>
            : current_(std::move(i.current_)), parent_(i.parent_) {}

        constexpr const std::ranges::iterator_t<Base>& base() const& noexcept {
            return current_;
        }
        constexpr std::ranges::iterator_t<Base> base() && {
            return std::move(current_);
        }

        constexpr decltype(auto) operator*() const
            noexcept(noexcept(std::invoke(*parent_->fun_, *current_))) {
            return std::invoke(*parent_->fun_, *current_);
        }

        constexpr iterator& operator++() {
            ++current_;
            return *this;
        }
        constexpr void     operator++(int) { ++current_; }
        constexpr iterator operator++(int)
            requires std::ranges::forward_range<Base>
        {
            auto tmp = *this;
            ++*this;
            return tmp;
        }

        constexpr iterator& operator--()
            requires std::ranges::bidirectional_range<Base>
        {
            --current_;
            return *this;
        }
        constexpr iterator operator--(int)
            requires std::ranges::bidirectional_range<Base>
        {
            auto tmp = *this;
            --*this;
            return tmp;
        }

        constexpr iterator& operator+=(difference_type n)
            requires std::ranges::random_access_range<Base>
        {
            current_ += n;
            return *this;
        }
        constexpr iterator& operator-=(difference_type n)
            requires std::ranges::random_access_range<Base>
        {
            current_ -= n;
            return *this;
        }

        constexpr decltype(auto) operator[](difference_type n) const
            requires std::ranges::random_access_range<Base>
        {
            return std::invoke(*parent_->fun_, current_[n]);
        }

        friend constexpr bool operator==(const iterator& x, const iterator& y)
            requires std::equality_comparable<std::ranges::iterator_t<Base>>
        {
            return x.current_ == y.current_;
        }

        friend constexpr bool operator<(const iterator& x, const iterator& y)
            requires std::ranges::random_access_range<Base>
        {
            return x.current_ < y.current_;
        }
        friend constexpr bool operator>(const iterator& x, const iterator& y)
            requires std::ranges::random_access_range<Base>
        {
            return y < x;
        }
        friend constexpr bool operator<=(const iterator& x, const iterator& y)
            requires std::ranges::random_access_range<Base>
        {
            return !(y < x);
        }
        friend constexpr bool operator>=(const iterator& x, const iterator& y)
            requires std::ranges::random_access_range<Base>
        {
            return !(x < y);
        }
        friend constexpr auto operator<=>(const iterator& x, const iterator& y)
            requires std::ranges::random_access_range<Base> &&
                     std::three_way_comparable<std::ranges::iterator_t<Base>>
        {
            return x.current_ <=> y.current_;
        }

        friend constexpr iterator operator+(iterator i, difference_type n)
            requires std::ranges::random_access_range<Base>
        {
            return iterator{*i.parent_, i.current_ + n};
        }
        friend constexpr iterator operator+(difference_type n, iterator i)
            requires std::ranges::random_access_range<Base>
        {
            return iterator{*i.parent_, i.current_ + n};
        }

        friend constexpr iterator operator-(iterator i, difference_type n)
            requires std::ranges::random_access_range<Base>
        {
            return iterator{*i.parent_, i.current_ - n};
        }
        friend constexpr difference_type operator-(const iterator& x,
                                                   const iterator& y)
            requires std::sized_sentinel_for<std::ranges::iterator_t<Base>,
                                             std::ranges::iterator_t<Base>>
        {
            return x.current_ - y.current_;
        }
    };

    template <bool Const>
    class sentinel {
      private:
        using Parent = detail::maybe_const<Const, transform_view>;
        using Base   = detail::maybe_const<Const, V>;
        std::ranges::sentinel_t<Base> end_ = std::ranges::sentinel_t<Base>();

      public:
        sentinel() = default;
        constexpr explicit sentinel(std::ranges::sentinel_t<Base> end)
            : end_(end) {}
        constexpr sentinel(sentinel<!Const> i)
            requires Const && std::convertible_to<std::ranges::sentinel_t<V>,
                                                  std::ranges::sentinel_t<Base>>
            : end_(std::move(i.end_)) {}

        constexpr std::ranges::sentinel_t<Base> base() const { return end_; }

        template <bool OtherConst>
            requires std::sentinel_for<
                std::ranges::sentinel_t<Base>,
                std::ranges::iterator_t<detail::maybe_const<OtherConst, V>>>
        friend constexpr bool operator==(const iterator<OtherConst>& x,
                                         const sentinel&             y) {
            return x.current_ == y.end_;
        }

        template <bool OtherConst>
            requires std::sized_sentinel_for<
                std::ranges::sentinel_t<Base>,
                std::ranges::iterator_t<detail::maybe_const<OtherConst, V>>>
        friend constexpr std::ranges::range_difference_t<
            detail::maybe_const<OtherConst, V>>
        operator-(const iterator<OtherConst>& x, const sentinel& y) {
            return x.current_ - y.end_;
        }

        template <bool OtherConst>
            requires std::sized_sentinel_for<
                std::ranges::sentinel_t<Base>,
                std::ranges::iterator_t<detail::maybe_const<OtherConst, V>>>
        friend constexpr std::ranges::range_difference_t<
            detail::maybe_const<OtherConst, V>>
        operator-(const sentinel& y, const iterator<OtherConst>& x) {
            return y.end_ - x.current_;
        }
    };

    V                                            base_ = V();
    [[no_unique_address]] detail::movable_box<F> fun_;

  public:
    transform_view()
        requires std::default_initializable<V> && std::default_initializable<F>
    = default;
    constexpr explicit transform_view(V base, F fun)
        : base_(std::move(base)), fun_(std::move(fun)) {}

    constexpr V base() const&
        requires std::copy_constructible<V>
    {
        return base_;
    }
    constexpr V base() && { return std::move(base_); }

    constexpr iterator<false> begin() {
        return iterator<false>{*this, std::ranges::begin(base_)};
    }
    constexpr iterator<true> begin() const
        requires std::ranges::range<const V> &&
                 std::regular_invocable<const F&,
                                        std::ranges::range_reference_t<const V>>
    {
        return iterator<true>{*this, std::ranges::begin(base_)};
    }

    constexpr sentinel<false> end() {
        return sentinel<false>{std::ranges::end(base_)};
    }
    constexpr iterator<false> end()
        requires std::ranges::common_range<V>
    {
        return iterator<false>{*this, std::ranges::end(base_)};
    }
    constexpr sentinel<true> end() const
        requires std::ranges::range<const V> &&
                 std::regular_invocable<const F&,
                                        std::ranges::range_reference_t<const V>>
    {
        return sentinel<true>{std::ranges::end(base_)};
    }
    constexpr iterator<true> end() const
        requires std::ranges::common_range<const V> &&
                 std::regular_invocable<const F&,
                                        std::ranges::range_reference_t<const V>>
    {
        return iterator<true>{*this, std::ranges::end(base_)};
    }

    constexpr auto size()
        requires std::ranges::sized_range<V>
    {
        return std::ranges::size(base_);
    }
    constexpr auto size() const
        requires std::ranges::sized_range<const V>
    {
        return std::ranges::size(base_);
    }
};

template <typename R, typename F>
transform_view(R&&, F) -> transform_view<std::ranges::views::all_t<R>, F>;

namespace views {
namespace detail {
template <typename Range, typename F>
concept can_transform_view =
    requires { transform_view(std::declval<Range>(), std::declval<F>()); };
} // namespace detail

struct transform_impl : std::ranges::range_adaptor_closure<transform_impl> {
    template <std::ranges::viewable_range Range, typename F>
        requires detail::can_transform_view<Range, F>
    constexpr auto operator() [[nodiscard]] (Range&& r, F&& f) const {
        return transform_view((Range&&)r, (F&&)f);
    }

    // TODO using std::ranges::range_adaptor_closure<transform_impl>::operator();
};

inline constexpr transform_impl transform;
} // namespace views

} // namespace beman::transform_view_26

#endif // BEMAN_TRANSFORM_VIEW_HPP
