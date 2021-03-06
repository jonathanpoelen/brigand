/*==================================================================================================
Copyright (c) 2015 Edouard Alligand and Joel Falcou

Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
=================================================================================================**/
#pragma once

// configuration macros for brigand

#if defined(_MSC_VER) && !defined(__GNUC__) && !defined(__clang__)
#define BRIGAND_COMP_MSVC
#if _MSC_VER == 1900
#define BRIGAND_COMP_MSVC_2015
#elif _MSC_VER == 1800
#define BRIGAND_COMP_MSVC_2013
#endif
#elif __GNUC__
#ifndef __clang__
#define BRIGAND_COMP_GCC
#else
#define BRIGAND_COMP_CLANG
#endif
#endif
