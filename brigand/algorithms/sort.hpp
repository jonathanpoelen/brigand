/*==================================================================================================
Copyright (c) 2015 Edouard Alligand and Joel Falcou

Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
=================================================================================================**/
#pragma once

#include <brigand/algorithms/merge.hpp>
#include <brigand/algorithms/transform.hpp>
#include <brigand/functions/comparison/less.hpp>
#include <brigand/functions/lambda/apply.hpp>
#include <brigand/sequences/append.hpp>
#include <brigand/sequences/range.hpp>
#include <brigand/sequences/size.hpp>

namespace brigand
{
namespace detail
{

  template<class Ints, class L>
  struct map_sort_impl;

  template<class... Ints, class... Ts>
  struct map_sort_impl<list<Ints...>, list<Ts...>>
  : list<Ints, Ts>...
  {};

  template<class L>
  using map_sort = map_sort_impl<range<std::size_t, 0, size<L>::value>, L>;

  template<std::size_t Int, class T>
  T get_val_fn(list<size_t<Int>, T>*);

  template<class Comp, template<class...> class Tpl, std::size_t Int, class T, class U>
  Tpl<Comp, T, U> get_val_fn2(list<size_t<Int>, T>*, list<size_t<Int+1>, U>*);

  template<class M, std::size_t Int>
  using get_val = decltype(get_val_fn<Int>(static_cast<M*>(nullptr)));

  template<class Comp, template<class...> class Tpl, class M, class Int>
  //using get_group = Tpl<Comp, get_val<M, Int::value * 2>, get_val<M, Int::value * 2 + 1>>;
  using get_group = decltype(get_val_fn2<Comp, Tpl, Int::value * 2>(static_cast<M*>(nullptr), static_cast<M*>(nullptr)));

  template<bool odd, class Comp, template<class...> class Tpl, template<class...> class Tpl2, class M, class Ints>
  struct groups_impl;

  template<class Comp, template<class...> class Tpl, template<class...> class Tpl2, class M, class... Ints>
  struct groups_impl<false, Comp, Tpl, Tpl2, M, list<Ints...>>
  {
    using type = list<get_group<Comp, Tpl, M, Ints>...>;
  };

  template<class Comp, template<class...> class Tpl, template<class...> class Tpl2, class M, class... Ints>
  struct groups_impl<true, Comp, Tpl, Tpl2, M, list<Ints...>>
  {
    using type = list<get_group<Comp, Tpl, M, Ints>..., Tpl2<get_val<M, sizeof...(Ints)*2>>>;
  };

  template<class L, class Comp, template<class...> class Tpl, template<class...> class Tpl2>
  using groups = typename groups_impl<bool(size<L>::value&1), Comp, Tpl, Tpl2, map_sort<L>, range<std::size_t, 0, size<L>::value/2>>::type;

  template<class Comp, class T, class U>
  using cmp_l2 = typename merge_impl<list<>, list<T>, list<U>, Comp>::type;

  template<class Comp, class L0, class L1>
  using merge_l2 = typename merge_impl<list<>, L0, L1, Comp>::type;

  template<class T> using ident = T;

  template<class L, class Comp>
  struct sort_impl
  : sort_impl<groups<L, Comp, merge_l2, ident>, Comp>
  {};

  template<class L, class Comp>
  struct sort_impl<list<L>, Comp>
  { using type = L; };

  template<class Comp>
  struct sort_impl<list<>, Comp>
  { using type = list<>; };

  template<class L, class Comp>
  using sort = typename sort_impl<groups<L, Comp, cmp_l2, list>, Comp>::type;
}

  template <class Seq, class Comp = less<_1,_2>>
  using sort = append<clear<Seq>, detail::sort<wrap<Seq, list>, Comp>>;
}
