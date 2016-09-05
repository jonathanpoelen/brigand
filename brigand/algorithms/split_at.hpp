/*==================================================================================================
Copyright (c) 2015 Edouard Alligand and Joel Falcou

Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
=================================================================================================**/
#pragma once

#include <brigand/algorithms/wrap.hpp>
#include <brigand/sequences/clear.hpp>
#include <brigand/sequences/list.hpp>

namespace brigand
{
namespace detail
{
  constexpr std::size_t rk_ge_16_1_0(std::size_t i) { return i >= 16 ? 2 : i != 0 ? 1 : 0; }

  template<std::size_t> struct drop
  {
    template<template<class...> class F, class Ctx, std::size_t i,
      class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class... Ts>
    using invoke = typename drop<rk_ge_16_1_0(i-16)>::template invoke<F, Ctx, i-16, Ts...>;
  };

  template<> struct drop<1>
  {
    template<template<class...> class F, class Ctx, std::size_t i, class T, class... Ts>
    using invoke = typename drop<rk_ge_16_1_0(i-1)>::template invoke<F, Ctx, i-1, Ts...>;
  };

  template<> struct drop<0>
  {
    template<template<class...> class F, class Ctx, std::size_t i, class... Ts>
    using invoke = F<Ctx, Ts...>;
  };

  template<std::size_t> struct rotate
  {
    template<template<class...> class F, class Ctx, std::size_t i,
      class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class... Ts>
    using invoke = typename rotate<rk_ge_16_1_0(i-16)>::template invoke<F, Ctx, i-16, Ts..., T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>;
  };

  template<> struct rotate<1>
  {
    template<template<class...> class F, class Ctx, std::size_t i, class T, class... Ts>
    using invoke = typename rotate<rk_ge_16_1_0(i-1)>::template invoke<F, Ctx, i-1, Ts..., T>;
  };

  template<> struct rotate<0>
  {
    template<template<class...> class F, class Ctx, std::size_t i, class... Ts>
    using invoke = F<Ctx, Ts...>;
  };

  template<class Ctx, class... Ts>
  using list_ = list<Ts...>;

  template<class I, class... Ts>
  using drop_i = typename drop<rk_ge_16_1_0(I::value)>::template invoke<list_, void, I::value, Ts...>;

  template<class I, class... Ts>
  using take_impl = typename rotate<rk_ge_16_1_0(I::value)>::template invoke<drop_i, std::integral_constant<std::size_t,sizeof...(Ts)-I::value>, I::value, Ts...>;

  template<class I, class... Ts>
  using drop_impl = typename drop<rk_ge_16_1_0(I::value)>::template invoke<list_, void, I::value, Ts...>;

  template<class L, class I>
  struct split_at_impl;

  template<template<class... T> class L, class... Ts, class I>
  struct split_at_impl<L<Ts...>, I>
  {
    using type = L<
      ::brigand::wrap<take_impl<I, Ts...>, L>,
      ::brigand::wrap<drop_impl<I, Ts...>, L>
    >;
  };
}
namespace lazy
{
    template <typename L, typename I>
    using split_at = ::brigand::detail::split_at_impl<L, I>;
}
template <typename L, typename I>
using split_at = typename lazy::split_at<L, I>::type;
}
