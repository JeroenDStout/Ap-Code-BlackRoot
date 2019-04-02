/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include <cstddef>
#include "BlackRoot\Pubc\Tuple.h"

namespace BlackRoot {
namespace Math {

    template<typename t>
    struct GaussianDistDef : Math::Tuple1dDef<t, 2> {
        typedef GaussianDistDef<t>         GaussianDistType;
        
        GaussianDistType() { ; }
        GaussianDistType(TupleType &rh) : TupleType(rh) { ; }

#define BR_MATH_F_GAUSSIAN_DIST(t, p) \
    BR_MATH_F_TUPLE(t, p)

        static GaussianDistType&       Interpret(TupleType &t)        { return *((VectorType*)(&t)); }
        static const GaussianDistType& Interpret(const TupleType &t)  { return *((VectorType*)(&t)); }
    };

}
}