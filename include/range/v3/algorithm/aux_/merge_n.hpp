// Range v3 library
//
//  Copyright Eric Niebler 2014
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3
//
// Copyright (c) 2009 Alexander Stepanov and Paul McJones
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without
// fee, provided that the above copyright notice appear in all copies
// and that both that copyright notice and this permission notice
// appear in supporting documentation. The authors make no
// representations about the suitability of this software for any
// purpose. It is provided "as is" without express or implied
// warranty.
//
// Algorithms from
// Elements of Programming
// by Alexander Stepanov and Paul McJones
// Addison-Wesley Professional, 2009
#ifndef RANGES_V3_ALGORITHM_AUX_MERGE_N_HPP
#define RANGES_V3_ALGORITHM_AUX_MERGE_N_HPP

#include <tuple>
#include <range/v3/range_fwd.hpp>
#include <range/v3/begin_end.hpp>
#include <range/v3/distance.hpp>
#include <range/v3/range_concepts.hpp>
#include <range/v3/range_traits.hpp>
#include <range/v3/utility/functional.hpp>
#include <range/v3/utility/iterator.hpp>
#include <range/v3/utility/iterator_traits.hpp>
#include <range/v3/algorithm/copy_n.hpp>

namespace ranges
{
    inline namespace v3
    {
        namespace aux
        {
            struct merge_n_fn
            {
                template<typename I0, typename I1, typename O, typename C = ordered_less,
                    typename P0 = ident, typename P1 = ident,
                    CONCEPT_REQUIRES_(
                        Mergeable<I0, I1, O, C, P0, P1>()
                    )>
                std::tuple<I0, I1, O>
                operator()(I0 begin0, iterator_difference_t<I0> n0,
                           I1 begin1, iterator_difference_t<I1> n1,
                           O out, C r = C{}, P0 p0 = P0{}, P1 p1 = P1{}) const
                {
                    using T = std::tuple<I0, I1, O>;
                    auto &&ir = invokable(r);
                    auto &&ip0 = invokable(p0);
                    auto &&ip1 = invokable(p1);
                    auto n0orig = n0;
                    auto n1orig = n1;
                    auto b0 = uncounted(begin0);
                    auto b1 = uncounted(begin1);
                    while(true)
                    {
                        if(0 == n0)
                        {
                            auto res = copy_n(b1, n1, out);
                            begin0 = recounted(begin0, b0, n0orig);
                            begin1 = recounted(begin1, res.first, n1orig);
                            return T{begin0, begin1, res.second};
                        }
                        if(0 == n1)
                        {
                            auto res = copy_n(b0, n0, out);
                            begin0 = recounted(begin0, res.first, n0orig);
                            begin1 = recounted(begin1, b1, n1orig);
                            return T{begin0, begin1, res.second};
                        }
                        if(ir(ip1(*b1), ip0(*b0)))
                        {
                            *out = *b1;
                            ++b1; ++out; --n1;
                        }
                        else
                        {
                            *out = *b0;
                            ++b0; ++out; --n0;
                        }
                    }
                }
            };

            constexpr merge_n_fn merge_n{};
        }
    } // namespace v3
} // namespace ranges

#endif // include guard
