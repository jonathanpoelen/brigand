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

  template<class Comp, template<class...> class Tpl, std::size_t Int, class T0, class T1, class T2, class T3>
  Tpl<Comp, T0, T1, T2, T3> get_val_fn4(list<size_t<Int>, T0>*, list<size_t<Int+1>, T1>*, list<size_t<Int+2>, T2>*, list<size_t<Int+3>, T3>*);

  template<class M, std::size_t Int>
  using get_val = decltype(get_val_fn<Int>(static_cast<M*>(nullptr)));

  template<class Comp, template<class...> class Tpl, class M, class Int>
  //using get_group = Tpl<Comp, get_val<M, Int::value * 2>, get_val<M, Int::value * 2 + 1>>;
  using get_group = decltype(get_val_fn4<Comp, Tpl, Int::value * 4>(static_cast<M*>(nullptr), static_cast<M*>(nullptr), static_cast<M*>(nullptr), static_cast<M*>(nullptr)));

  template<std::size_t, class Comp, template<class...> class Tpl, template<class...> class Tpl2, class M, class Ints>
  struct groups_impl;

  template<class Comp, template<class...> class Tpl, template<class...> class Tpl2, class M, class... Ints>
  struct groups_impl<0, Comp, Tpl, Tpl2, M, list<Ints...>>
  {
    using type = list<get_group<Comp, Tpl, M, Ints>...>;
  };

  template<class Comp, template<class...> class Tpl, template<class...> class Tpl2, class M, class... Ints>
  struct groups_impl<1, Comp, Tpl, Tpl2, M, list<Ints...>>
  {
    using type = list<get_group<Comp, Tpl, M, Ints>..., Tpl2<Comp, get_val<M, sizeof...(Ints)*4>>>;
  };

  template<class Comp, template<class...> class Tpl, template<class...> class Tpl2, class M, class... Ints>
  struct groups_impl<2, Comp, Tpl, Tpl2, M, list<Ints...>>
  {
    using type = list<get_group<Comp, Tpl, M, Ints>..., Tpl2<Comp, get_val<M, sizeof...(Ints)*4>, get_val<M, sizeof...(Ints)*4+1>>>;
  };

  template<class Comp, template<class...> class Tpl, template<class...> class Tpl2, class M, class... Ints>
  struct groups_impl<3, Comp, Tpl, Tpl2, M, list<Ints...>>
  {
    using type = list<get_group<Comp, Tpl, M, Ints>..., Tpl2<Comp, get_val<M, sizeof...(Ints)*4>, get_val<M, sizeof...(Ints)*4+1>, get_val<M, sizeof...(Ints)*4+2>>>;
  };

  template<class L, class Comp, template<class...> class Tpl, template<class...> class Tpl2>
  using groups = typename groups_impl<size<L>::value%4, Comp, Tpl, Tpl2, map_sort<L>, range<std::size_t, 0, size<L>::value/4>>::type;

  template<class Comp, class L0, class L1, class L2, class L3>
  using merge_l2 = typename merge_impl<list<>, typename merge_impl<list<>, L0, L1, Comp>::type, typename merge_impl<list<>, L2, L3, Comp>::type, Comp>::type;

  template<class Comp, class T0, class T1, class T2, class T3>
  using cmp_l2 = merge_l2<Comp, list<T0>, list<T1>, list<T2>, list<T3>>;

  template<class Comp, class... L>
  struct merge_3_impl;

  template<class Comp, class L>
  struct merge_3_impl<Comp, L>
  {
    using type = L;
  };

  template<class Comp, class L0, class L1>
  struct merge_3_impl<Comp, L0, L1>
  : merge_impl<list<>, L0, L1, Comp>
  {};

  template<class Comp, class L0, class L1, class L2>
  struct merge_3_impl<Comp, L0, L1, L2>
  : merge_impl<list<>, typename merge_impl<list<>, L0, L1, Comp>::type, L2, Comp>
  {};

  template<class Comp, class... L>
  using merge_3 = typename merge_3_impl<Comp, L...>::type;

  template<class L, class Comp>
  struct sort_impl
  : sort_impl<groups<L, Comp, merge_l2, merge_3>, Comp>
  {};

  template<class L, class Comp>
  struct sort_impl<list<L>, Comp>
  { using type = L; };

  template<class L0, class L1, class Comp>
  struct sort_impl<list<L0, L1>, Comp>
  : merge_impl<list<>, L0, L1, Comp>
  {};

  template<class L0, class L1, class L2, class Comp>
  struct sort_impl<list<L0, L1, L2>, Comp>
  : merge_impl<list<>, typename merge_impl<list<>, L0, L1, Comp>::type, L2, Comp>
  {};

  template<class Comp>
  struct sort_impl<list<>, Comp>
  { using type = list<>; };

  template<class Comp, class... Ts> using list_merge_3 = merge_3<Comp, list<Ts>...>;

  template<class L, class Comp>
  using sort = typename sort_impl<groups<L, Comp, cmp_l2, list_merge_3>, Comp>::type;
}

  template <class Seq, class Comp = less<_1,_2>>
  using sort = append<clear<Seq>, detail::sort<wrap<Seq, list>, Comp>>;
}
