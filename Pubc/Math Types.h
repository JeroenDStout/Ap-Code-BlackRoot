/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include <cstddef>
#include "BlackRoot/Pubc/Tensor.h"
#include "BlackRoot/Pubc/Rotor.h"
#include "BlackRoot/Pubc/Transformation.h"

namespace BlackRoot {
namespace Math {

    template<typename n>
    struct Vector2d : public Math::OrthoVectorDef<n, 2> {
        using Vector2dType     = Vector2d<n>;
        using OrthoVectorType  = Math::OrthoVectorDef<n, 2>;

        BR_MATH_F_ORTHOVECTOR(Vector2dType, OrthoVectorType);

        ScalarType  x, y;
    };

    template<typename n>
    struct Vector3d : public Math::OrthoVectorDef<n, 3> {
        using Vector3dType     = Vector3d<n>;
        using OrthoVectorType  = Math::OrthoVectorDef<n, 3>;
        
        BR_MATH_F_ORTHOVECTOR(Vector3dType, OrthoVectorType);

        ScalarType  x, y, z;
        
    };

    template<typename n>
    struct Vector4d : public Math::OrthoVectorDef<n, 4> {
        using Vector4dType     = Vector4d<n>;
        using OrthoVectorType  = Math::OrthoVectorDef<n, 4>;
        
        BR_MATH_F_ORTHOVECTOR(Vector4dType, OrthoVectorType);

        ScalarType  x, y, z, w;
    };

    template<typename n, int s>
    struct VectorNd : public Math::OrthoVectorDef<n, s> {
        using VectorNdType     = VectorNd<n, s>;
        using OrthoVectorType  = Math::OrthoVectorDef<n, s>;

        BR_MATH_F_ORTHOVECTOR(VectorNdType, OrthoVectorType);

        ScalarType  elem[s];
    };

    template<typename n>
    struct Colour3d : public Math::OrthoVectorDef<n, 3> {
        using Colour3dType     = Colour3d<n>;
        using OrthoVectorType  = Math::OrthoVectorDef<n, 3>;

        BR_MATH_F_ORTHOVECTOR(Colour3dType, OrthoVectorType);

        ScalarType  r, g, b;
    };

    template<typename n>
    struct Colour4d : public Math::OrthoVectorDef<n, 4> {
        using Colour4dType     = Colour4d<n>;
        using OrthoVectorType  = Math::OrthoVectorDef<n, 4>;

        BR_MATH_F_ORTHOVECTOR(Colour4dType, OrthoVectorType);

        ScalarType  r, g, b, a;
    };

    template<typename n>
    struct Matrix2x2 : public Math::MatrixDef<n, 2, 2> {
        using Matrix2x2Type    = Matrix2x2<n>;
        using MatrixType       = Math::MatrixDef<n, 2, 2>;

        BR_MATH_F_MATRIX(Matrix2x2Type, MatrixType);

        ScalarType  xx, xy,
                    yx, yy;
    };

    template<typename n>
    struct Matrix3x3 : public Math::MatrixDef<n, 3, 3> {
        using Matrix3x3Type    = Matrix3x3<n>;
        using MatrixType       = Math::MatrixDef<n, 3, 3>;

        BR_MATH_F_MATRIX(Matrix3x3Type, MatrixType);

        ScalarType  xx, xy, xz,
                    yx, yy, yz,
                    zx, zy, zz;
    };

    template<typename n>
    struct Matrix4x4 : public Math::MatrixDef<n, 4, 4> {
        using Matrix4x4Type    = Matrix4x4<n>;
        using MatrixType       = Math::MatrixDef<n, 4, 4>;

        BR_MATH_F_MATRIX(Matrix4x4Type, MatrixType);

        ScalarType  xx, xy, xz, xw,
                    yx, yy, yz, yw,
                    zx, zy, zz, zw,
                    wx, wy, wz, ww;
    };

    template<typename n, int rows, int columns>
    struct MatrixNxN : public Math::MatrixDef<n, rows, columns> {
        using MatrixNxNType    = MatrixNxN<n, rows, columns>;
        using MatrixType       = Math::MatrixDef<n, rows, columns>;

        BR_MATH_F_MATRIX(MatrixNxNType, MatrixType);

        ScalarType  elem[rows][columns];
    };

    template<typename n>
    struct Quaternion : public Math::QuaternionDef<n> {
        using QuaternionType   = Math::QuaternionDef<n>;

        BR_MATH_F_QUATERNION(Quaternion, QuaternionType);

        ScalarType  x, y, z, w;
    };
    
    template<int n>              using Vec         = VectorNd<float, n>;
    template<int n>              using DVec        = VectorNd<double, n>;
    template<int n>              using I32Vec      = VectorNd<int, n>;
    
    template<int r, int c>       using Mat         = MatrixNxN<float, r, c>;
    
    template<typename t>         using TransMat2d  = TranslatedMap<Matrix2x2<t>, Vector2d<t>>;
    template<typename t>         using TransMat3d  = TranslatedMap<Matrix3x3<t>, Vector3d<t>>;
    template<typename t>         using TransMat4d  = TranslatedMap<Matrix4x4<t>, Vector4d<t>>;

    template<int n>              using TrMat       = TranslatedMap<MatrixNxN<float, n, n>, VectorNd<float, n>>;

    using  Vec2     = Vector2d<float>;
    using  DVec2    = Vector2d<double>;
    using  UVec2    = Vector2d<uint8_t>;
    using  IVec2    = Vector2d<int>;

    using  Vec3     = Vector3d<float>;
    using  DVec3    = Vector3d<double>;
    using  UVec3    = Vector3d<uint8_t>;
    using  IVec3    = Vector3d<int>;

    using  Vec4     = Vector4d<float>;
    using  DVec4    = Vector4d<double>;
    using  UVec4    = Vector4d<uint8_t>;
    using  IVec4    = Vector4d<int>;

    using  Mat2     = Matrix2x2<float>;
    using  DMat2    = Matrix2x2<double>;

    using  Mat3     = Matrix3x3<float>;
    using  DMat3    = Matrix3x3<double>;

    using  Mat4     = Matrix4x4<float>;
    using  DMat4    = Matrix4x4<double>;

    using  Col3     = Colour3d<float>;
    using  DCol3    = Colour3d<double>;
    using  UCol3    = Colour3d<uint8_t>;

    using  Col4     = Colour4d<float>;
    using  DCol4    = Colour4d<double>;
    using  UCol4    = Colour4d<uint8_t>;

    //using  Rect     = Area2d<float>;
    //using  DRect    = Area2d<double>;
    //using  URect    = Area2d<uint8_t>;
    //using  IRect    = Area2d<int>;

    //using  Box      = Area3d<float>;
    //using  DBox     = Area3d<double>;
    //using  UBox     = Area3d<uint8_t>;
    //using  IBox     = Area3d<int>;

    //using  Box4     = Area4d<float>;
    //using  DBox4    = Area4d<double>;
    //using  UBox4    = Area4d<uint8_t>;
    //using  IBox4    = Area4d<int>;

    using  Quat     = Quaternion<float>;
    using  DQuat    = Quaternion<double>;

    using  TrMat2   = TransMat2d<float>;
    using  DTrMat2  = TransMat2d<double>;

    using  TrMat3   = TransMat3d<float>;
    using  DTrMat3  = TransMat3d<double>;

    using  TrMat4   = TransMat4d<float>;
    using  DTrMat4  = TransMat4d<double>;

    using  TrQuat   = TranslatedMap<Quaternion<float>, Vector3d<float>>;
    using  DTrQuat  = TranslatedMap<Quaternion<double>, Vector3d<double>>;

};
};