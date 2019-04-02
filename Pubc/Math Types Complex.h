/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include <complex>
#include "BlackRoot/Pubc/Math Types.h"

namespace BlackRoot {
namespace Math {

    using  CVec2    = Vector2d<std::complex<float>>;
    using  DCVec2   = Vector2d<std::complex<double>>;

    using  CVec3    = Vector3d<std::complex<float>>;
    using  DCVec3   = Vector3d<std::complex<double>>;

    using  CVec4    = Vector4d<std::complex<float>>;
    using  DCVec4   = Vector4d<std::complex<double>>;

    using  CMat2    = Matrix2x2<std::complex<float>>;
    using  DCMat2   = Matrix2x2<std::complex<double>>;

    using  CMat3    = Matrix3x3<std::complex<float>>;
    using  DCMat3   = Matrix3x3<std::complex<double>>;

    using  CMat4    = Matrix4x4<std::complex<float>>;
    using  DCMat4   = Matrix4x4<std::complex<double>>;
    
    using  CTrMat2   = TransMat2d<std::complex<float>>;
    using  CDTrMat2  = TransMat2d<std::complex<double>>;

    using  CTrMat3   = TransMat3d<std::complex<float>>;
    using  CDTrMat3  = TransMat3d<std::complex<double>>;

    using  CTrMat4   = TransMat4d<std::complex<float>>;
    using  CDTrMat4  = TransMat4d<std::complex<double>>;

}
}