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
  template<class L, class Comp>
  struct sort_impl;

  template<class Comp>
  struct sort_impl<list<list<>, list<>>, Comp>
  { using type = list<>; };

  template<class T0, class Comp>
  struct sort_impl<list<list<>, list<T0>>, Comp>
  { using type = list<T0>; };

  template<class T0, class T1, class Comp>
  struct sort_impl<list<list<T0>, list<T1>>, Comp>
  : std::conditional<::brigand::apply<Comp,T0,T1>::value, list<T0,T1>, list<T1,T0>>
  {};

  template<class L, class Comp>
  using sort = typename sort_impl<split_at<L, size_t<size<L>::value/2>>, Comp>::type;

  template<class L0, class L1, class Comp>
  struct sort_impl<list<L0, L1>, Comp>
  : merge_impl<list<>, sort<L0, Comp>, sort<L1, Comp>, Comp>
  {};
}

  template <class Seq, class Comp = less<_1,_2>>
  using sort = append<clear<Seq>, detail::sort<wrap<Seq, list>, Comp>>;
}
