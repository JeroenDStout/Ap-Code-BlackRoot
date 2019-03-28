/*
 *
 *  © Stout Games 2019
 *
 */

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