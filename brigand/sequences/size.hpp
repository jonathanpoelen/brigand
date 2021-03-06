/*==================================================================================================
  Copyright (c) 2015 Edouard Alligand and Joel Falcou

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
=================================================================================================**/
#pragma once
#include <brigand/algorithms/wrap.hpp>
#include <brigand/algorithms/count.hpp>

namespace brigand
{
  template<class L> using size = wrap<L, count>;
}
