/*==================================================================================================
  Copyright (c) 2015 Edouard Alligand and Joel Falcou

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
=================================================================================================**/
#pragma once

#include <brigand/functions/lambda/bind.hpp>
#include <brigand/types/args.hpp>

namespace brigand
{

namespace detail
{
  template<template<class...> class F> struct call_impl { using type = call_impl; };
  template<template<class> class F> struct call_impl<F> { using type = bind<F, _1>; };
  template<template<class,class> class F> struct call_impl<F> { using type = bind<F, _1, _2>; };
  template<template<class,class,class> class F> struct call_impl<F> { using type = bind<F, _1, _2, _3>; };
  template<template<class,class,class,class> class F> struct call_impl<F> { using type = bind<F, _1, _2, _3, _4>; };
}

template<template<class...> class F> using call = typename detail::call_impl<F>::type;

}

