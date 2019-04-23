/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include <cstddef>
#include "BlackRoot/Pubc/Tuple.h"
#include "BlackRoot/Pubc/Tensor.h"

namespace BlackRoot {
namespace Math {

        // Vector Definitions
    
    template<typename t>
    struct QuaternionDef;

    template<typename t>
    struct QuaternionDef : Math::Tuple1dDef<t, 4> {
        typedef QuaternionDef<t>     QuaternionType;
        typedef MatrixDef<t, 3, 3>   EquivalentMatrixType;

    protected:
        QuaternionDef() { ; }

    public:

            // -- Type

        static const size_t Operating_Dimensionality  = 3;

            // -- Downcast
        
        QuaternionType& as_quaternion() {
            return *(QuaternionType*)(this);
        }
        const QuaternionType& as_quaternion() const {
            return *(const QuaternionType*)(this);
        }

            // -- Operators

        template <typename v, typename = std::enable_if_t<std::is_same<v::OrthoVectorType, Math::OrthoVectorDef<ScalarType, 3>>::value>>
        v operator* (v rhs) const {
            const auto & quat = *this;
            ScalarType num = quat[0] * (ScalarType)2;
            ScalarType num2 = quat[1] * (ScalarType)2;
            ScalarType num3 = quat[2] * (ScalarType)2;
            ScalarType num4 = quat[0] * num;
            ScalarType num5 = quat[1] * num2;
            ScalarType num6 = quat[2] * num3;
            ScalarType num7 = quat[0] * num2;
            ScalarType num8 = quat[0] * num3;
            ScalarType num9 = quat[1] * num3;
            ScalarType num10 = quat[3] * num;
            ScalarType num11 = quat[3] * num2;
            ScalarType num12 = quat[3] * num3;
            v result;
            result[0] = ((ScalarType)(1) - (num5 + num6)) * rhs[0] + (num7 - num12) * rhs[1] + (num8 + num11) * rhs[2];
            result[1] = (num7 + num12) * rhs[0] + ((ScalarType)(1) - (num4 + num6)) * rhs[1] + (num9 - num10) * rhs[2];
            result[2] = (num8 - num11) * rhs[0] + (num9 + num10) * rhs[1] + ((ScalarType)(1) - (num4 + num5)) * rhs[2];
            return result;
        }
        
        template<typename v, typename = typename std::enable_if<std::is_same<v::OrthoVectorType, Math::OrthoVectorDef<ScalarType, 3>>>>
        v& operator* (const v & rhs) {
            return (rhs = *this * rhs);
        }

            // -- Inheritance

#define BR_MATH_F_QUATERNION(t, p) \
    BR_MATH_F_TUPLE(t, p)
        
        struct QuaternionAbstract : public QuaternionType, public Tuple1dAbstractMem {
            BR_MATH_F_QUATERNION(QuaternionAbstract, QuaternionType);
        };
    };

}
}