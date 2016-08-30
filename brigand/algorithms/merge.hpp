/*==================================================================================================
  Copyright (c) 2015 Edouard Alligand and Joel Falcou

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
=================================================================================================**/
#pragma once

#include <brigand/algorithms/wrap.hpp>
#include <brigand/functions/comparison/less.hpp>
#include <brigand/functions/lambda/apply.hpp>
#include <brigand/sequences/append.hpp>
#include <brigand/sequences/clear.hpp>
#include <type_traits>

namespace brigand
{
  namespace detail
  {
    template<class L, class Seq1, class Seq2, class Comp>
    struct merge_impl;

    template<bool, class L, class Seq1, class Seq2, class Comp>
    struct merge_insert;

    template<class... R, class T0, class T1, class... Ts, class U, class... Us, class Comp>
    struct merge_insert<true, list<R...>, list<T0,T1,Ts...>, list<U,Us...>, Comp>
    : merge_insert<::brigand::apply<Comp,T1,U>::value, list<R...,T0>, list<T1,Ts...>, list<U,Us...>, Comp>
    {};

    template<class... R, class T, class U, class... Us, class Comp>
    struct merge_insert<true, list<R...>, list<T>, list<U,Us...>, Comp>
    {
      using list = ::brigand::list<R...,T>;
      using left = ::brigand::list<>;
      using right = ::brigand::list<U,Us...>;
    };

    template<class... R, class T, class... Ts, class U0, class U1, class... Us, class Comp>
    struct merge_insert<false, list<R...>, list<T,Ts...>, list<U0,U1,Us...>, Comp>
    : merge_insert<::brigand::apply<Comp,T,U1>::value, list<R...,U0>, list<T,Ts...>, list<U1,Us...>, Comp>
    {};

    template<class... R, class T, class... Ts, class U, class Comp>
    struct merge_insert<false, list<R...>, list<T,Ts...>, list<U>, Comp>
    {
      using list = ::brigand::list<R...,U>;
      using left = ::brigand::list<T,Ts...>;
      using right = ::brigand::list<>;
    };

    template<bool, class L0, class L1, class Comp>
    struct merge_u9_t0;

    template<
      class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9,
      class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class Comp>
    struct merge_u9_t0<true, list<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>, list<U0,U1,U2,U3,U4,U5,U6,U7,U8,U9>, Comp>
    {
      using list = ::brigand::list<U0,U1,U2,U3,U4,U5,U6,U7,U8,U9,T0,T1,T2,T3,T4,T5,T6,T7,T8>;
      using left = ::brigand::list<T9>;
      using right = ::brigand::list<>;
    };

    template<
      class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9,
      class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class Comp>
    struct merge_u9_t0<false, list<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>, list<U0,U1,U2,U3,U4,U5,U6,U7,U8,U9>, Comp>
    : merge_insert<::brigand::apply<Comp,T0,U0>::value, list<>, list<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>, list<U0,U1,U2,U3,U4,U5,U6,U7,U8,U9>, Comp>
    {};

    template<bool, class L0, class L1, class Comp>
    struct merge_t9_u0;

    template<
      class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9,
      class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class Comp>
    struct merge_t9_u0<true, list<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>, list<U0,U1,U2,U3,U4,U5,U6,U7,U8,U9>, Comp>
    {
      using list = ::brigand::list<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,U0,U1,U2,U3,U4,U5,U6,U7,U8>;
      using left = ::brigand::list<>;
      using right = ::brigand::list<U9>;
    };

    template<
      class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9,
      class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class Comp>
    struct merge_t9_u0<false, list<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>, list<U0,U1,U2,U3,U4,U5,U6,U7,U8,U9>, Comp>
    : merge_u9_t0<::brigand::apply<Comp,U9,T0>::value, list<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>, list<U0,U1,U2,U3,U4,U5,U6,U7,U8,U9>, Comp>
    {};

    template<
      class... R,
      class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class... Ts,
      class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class... Us, class Comp>
    struct merge_impl<list<R...>, list<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,Ts...>, list<U0,U1,U2,U3,U4,U5,U6,U7,U8,U9,Us...>, Comp>
    {
      using sub = merge_t9_u0<::brigand::apply<Comp,T9,U0>::value, list<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>, list<U0,U1,U2,U3,U4,U5,U6,U7,U8,U9>, Comp>;
      using type = typename merge_impl<
        append<list<R...>, typename sub::list>,
        append<typename sub::left, list<Ts...>>,
        append<typename sub::right, list<Us...>>,
        Comp
      >::type;
    };

    template<class... R, class T, class... Ts, class U, class... Us, class Comp>
    struct merge_impl<list<R...>, list<T,Ts...>, list<U,Us...>, Comp>
    : std::conditional<
        ::brigand::apply<Comp,T,U>::value,
        merge_impl<list<R...,T>, list<Ts...>, list<U,Us...>, Comp>,
        merge_impl<list<R...,U>, list<T,Ts...>, list<Us...>, Comp>
    >::type
    {};

    template<class... R, class... Ts, class Comp>
    struct merge_impl<list<R...>, list<Ts...>, list<>, Comp>
    {
      using type = list<R..., Ts...>;
    };

    template<class... R, class... Us, class Comp>
    struct merge_impl<list<R...>, list<>, list<Us...>, Comp>
    {
      using type = list<R..., Us...>;
    };

    template<class... R, class Comp>
    struct merge_impl<list<R...>, list<>, list<>, Comp>
    {
      using type = list<R...>;
    };
  }

  template<class Seq1, class Seq2, class Comp = less<_1,_2>>
  using merge = append<clear<Seq1>, typename detail::merge_impl<list<>, wrap<Seq1, list>, wrap<Seq2, list>, Comp>::type>;
}
