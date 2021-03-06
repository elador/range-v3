// Range v3 library
//
//  Copyright Eric Niebler 2013-2014
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3
//

#ifndef RANGES_V3_ACTION_TAKE_HPP
#define RANGES_V3_ACTION_TAKE_HPP

#include <functional>
#include <range/v3/range_fwd.hpp>
#include <range/v3/action/action.hpp>
#include <range/v3/action/erase.hpp>
#include <range/v3/utility/iterator.hpp>
#include <range/v3/utility/functional.hpp>
#include <range/v3/utility/iterator_concepts.hpp>
#include <range/v3/utility/iterator_traits.hpp>

namespace ranges
{
    inline namespace v3
    {
        /// \addtogroup group-actions
        /// @{
        namespace action
        {
            struct take_fn
            {
            private:
                friend action_access;
                template<typename Int, CONCEPT_REQUIRES_(Integral<Int>())>
                static auto bind(take_fn take, Int n)
                RANGES_DECLTYPE_AUTO_RETURN
                (
                    std::bind(take, std::placeholders::_1, n)
                )
            public:
                template<typename Rng, typename Int,
                    typename I = range_iterator_t<Rng>,
                    CONCEPT_REQUIRES_(ForwardIterable<Rng>() && Integral<Int>() &&
                        EraseableIterable<Rng, I, I>())>
                void operator()(Rng & rng, Int n) const
                {
                    RANGES_ASSERT(n >= 0);
                    ranges::action::erase(rng, next_bounded(begin(rng), n, end(rng)), end(rng));
                }
            };

            /// \ingroup group-actions
            /// \sa take_fn
            /// \sa action
            constexpr action<take_fn> take{};
        }
        /// @}
    }
}

#endif
