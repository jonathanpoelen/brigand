/*==================================================================================================
Copyright (c) 2015 Edouard Alligand and Joel Falcou

Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
=================================================================================================**/
#pragma once

#include <brigand/algorithms/merge.hpp>
#include <brigand/algorithms/split_at.hpp>
#include <brigand/functions/comparison/less.hpp>
#include <brigand/functions/lambda/apply.hpp>
#include <brigand/sequences/size.hpp>

namespace brigand
{
namespace detail
{
  template<class L0, class L1, class Comp>
  struct sort_impl;

  template<class Comp>
  struct sort_impl<list<>, list<>, Comp>
  { using type = list<>; };

  template<class T0, class Comp>
  struct sort_impl<list<>, list<T0>, Comp>
  { using type = list<T0>; };

  template<class T0, class T1, class Comp>
  struct sort_impl<list<T0>, list<T1>, Comp>
  : std::conditional<!::brigand::apply<Comp,T1,T0>::value, list<T0,T1>, list<T1,T0>>
  {};

  template<template<class...> class F, class L, class... Args>
  struct take_and_drop;

  template<template<class...> class F, class... Ts, class... Args>
  struct take_and_drop<F, list<Ts...>, Args...>
  {
    using type = F<take_impl<size_t<sizeof...(Ts)/2>, Ts...>, drop_impl<size_t<sizeof...(Ts)/2>, Ts...>, Args...>;
  };

  template<template<class...> class F, class T, class U, class... Args>
  struct take_and_drop<F, list<T, U>, Args...>
  {
    using type = F<list<T>, list<U>, Args...>;
  };

  template<template<class...> class F, class T, class... Args>
  struct take_and_drop<F, list<T>, Args...>
  {
    using type = F<list<>, list<T>, Args...>;
  };

  template<class L>
  struct exhibit;
  template<class... Ts>
  struct exhibit<list<Ts...>>
  {
    template<template<class...> class F, class... Args>
    using invoke = F<Args..., Ts...>;
  };

  template<class L, class Comp>
  using sort = typename take_and_drop<sort_impl, L, Comp>::type::type;
  //using sort = typename exhibit<L>::template invoke<sort_impl, Comp>::type;

  template<class L0, class L1, class Comp>
  struct sort_impl
  : merge_impl<list<>, sort<L0, Comp>, sort<L1, Comp>, Comp>
  {};
}

  template <class Seq, class Comp = less<_1,_2>>
  using sort = append<clear<Seq>, detail::sort<wrap<Seq, list>, Comp>>;

  template <class Seq, class Comp = less<_1,_2>>
  using stable_sort = sort<Seq, Comp>;
}
