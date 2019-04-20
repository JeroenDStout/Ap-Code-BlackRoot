/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include <cstddef>
#include "BlackRoot/Pubc/Tuple.h"
#include "BlackRoot/Pubc/TemplateUtility.h"

namespace BlackRoot {
namespace Math {

        // Vector Definitions
    
    template<typename t, int s>
    struct VectorDef;
    template<typename t, int s>
    struct CovectorDef;
    template<typename t, int s>
    struct OrthoVectorDef;
    template<typename t, int s>
    struct OrthoRegionDef;
    template<typename t, int rows, int columns>
    struct MatrixDef;

// -----------------------------------------------------------------------------------------------------
//          Utility
// -----------------------------------------------------------------------------------------------------
        
    BR_U_TYPE_CHECK(equivalent_matrix_type, EquivalentMatrixType)
    BR_U_TYPE_CHECK(vector_type, VectorType)

// -----------------------------------------------------------------------------------------------------
//          Vector, Co-vector
// -----------------------------------------------------------------------------------------------------

    template<typename t, int s>
    struct VectorDef : Math::Tuple1dDef<t, s> {
        typedef VectorDef<t, s>     VectorType;
        typedef CovectorDef<t, s>   CovectorType;

            // -- Type
        
        static const bool   IsVector                 = true;
        static const size_t OperatingDimensionality  = s;

            // -- Downcast

        VectorType& AsVector() {
            return *(VectorType*)(this);
        }
        const VectorType& AsVector() const {
            return *(const VectorType*)(this);
        }

            // -- Operators

        VectorType& operator+= (const VectorType &rhs) {
            std::transform(this->begin(), this->end(), rhs.begin(), this->begin(), std::plus<>());
            return *this;
        }
        friend VectorType operator+ (VectorType lhs, const VectorType& rhs) {
            return (lhs += rhs);
        }
        
        VectorType & operator-= (const VectorType &rhs) {
            std::transform(this->begin(), this->end(), rhs.begin(), this->begin(), std::minus<>());
            return *this;
        }
        friend VectorType operator- (VectorType lhs, const VectorType& rhs) {
            return (lhs -= rhs);
        }
        
        VectorType & operator*= (const ScalarType &rhs) {
            std::transform(this->begin(), this->end(), this->begin(), std::bind(std::multiplies<>(), std::placeholders::_1, rhs));
            return *this;
        }
        friend VectorType operator* (VectorType lhs, const ScalarType& rhs) {
            return (lhs *= rhs);
        }
        
        VectorType & operator/= (const ScalarType &rhs) {
            std::transform(this->begin(), this->end(), this->begin(), std::bind(std::divides<>(), std::placeholders::_1, rhs));
            return *this;
        }
        friend VectorType operator/ (VectorType lhs, const ScalarType& rhs) {
            return (lhs /= rhs);
        }

            // -- Inheritance

#define BR_MATH_F_VECTOR(t, p) \
    BR_MATH_F_TUPLE(t, p) \
        t& operator+= (const t& rhs) { return t::Interpret(this->AsVector() += rhs.AsVector()); } \
        friend t operator+ (t lhs, const t& rhs) { return t::Interpret(lhs.AsVector() += rhs.AsVector()); } \
        t& operator-= (const t& rhs) { return t::Interpret(this->AsVector() -= rhs.AsVector()); } \
        friend t operator- (t lhs, const t& rhs) { return t::Interpret(lhs.AsVector() -= rhs.AsVector()); } \
        t& operator*= (const ScalarType& rhs) { return t::Interpret(this->AsVector() *= rhs); } \
        friend t operator* (t lhs, const ScalarType& rhs) { return t::Interpret(lhs.AsVector() *= rhs); } \
        t& operator/= (const ScalarType& rhs) { return t::Interpret(this->AsVector() /= rhs); } \
        friend t operator/ (t lhs, const ScalarType& rhs) { return t::Interpret(lhs.AsVector() /= rhs); }
        
        struct VectorAbstract : public VectorType, public Tuple1dAbstractMem {
            BR_MATH_F_VECTOR(VectorAbstract, VectorDef);
        };
    };

    template<typename t, int s>
    struct CovectorDef : VectorDef<t, s> {

            // -- Downcast

        CovectorType& AsCovector() {
            return *(VectorType*)(this);
        }
        const CovectorType& AsCovector() const {
            return *(const VectorType*)(this);
        }

            // -- Inheritance

#define BR_MATH_F_COVECTOR(t, p) \
    BR_MATH_F_VECTOR(t, p)
        
        struct CovectorAbstract : public TupleAbstractMem, public CovectorType {
            BR_MATH_F_COVECTOR(CovectorAbstract, CovectorType);
        };
    };

// -----------------------------------------------------------------------------------------------------
//          Orthonormal Vector, Orthonormal Co-vector
// -----------------------------------------------------------------------------------------------------

    template<typename t, int s>
    struct OrthoVectorDef : Math::VectorDef<t, s> {
        typedef OrthoVectorDef<t, s> OrthoVectorType;
        
        OrthoVectorType() { ; }
        OrthoVectorType(TupleType &rh) : VectorType(rh) { ; }

            // -- Downcast

        OrthoVectorType& AsOrthoVector() {
            return *(OrthoVectorType*)(this);
        }
        const OrthoVectorType& AsOrthoVector() const {
            return *(const OrthoVectorType*)(this);
        }

            // -- Operators

        ScalarType operator * (const OrthoVectorType &rh) const {
            ScalarType r = (ScalarType)(0);
            for (int i = 0; i < Size; i++) {
                r += (*this)[i] * rh[i];
            }
            return r;
        }

        template<typename = typename std::enable_if<3 == Size>>
        OrthoVectorType operator^ (const OrthoVectorType& rhs) const {
            OrthoVectorType::OrthovectorAbstract tmp;
            tmp[0] = (*this)[1] * rhs[2] - (*this)[2] * rhs[1];
            tmp[1] = (*this)[0] * rhs[2] - (*this)[2] * rhs[0];
            tmp[2] = (*this)[0] * rhs[1] - (*this)[1] * rhs[0];
            return std::move(tmp);
        }

            // Functional

        ScalarType  LengthSquared() const {
            return (*this) * (*this);
        }
        ScalarType  Length() const {
            sqrt(this->LengthSquared());
        }

        void Normalise(ScalarType &t = (ScalarType)(1)) {
            *this *= (t / this->Length());
        }
        void NormaliseSafe(ScalarType &t = (ScalarType)(1)) {
            auto l = this->LengthSquared();
            if (l > (ScalarType)(0)) {
                *this *= (t / sqrt(l));
            }
        }
        
        template<typename = typename std::enable_if<3 == Size>>
        OrthoVectorType Cross(const OrthoVectorType &rhs) {
            return std::move((*this) ^ rhs);
        }

            // -- Inheritance
        
#define BR_MATH_F_ORTHOVECTOR(t, p) \
    BR_MATH_F_VECTOR(t, p) \
        template<typename = typename std::enable_if<3 == Size>> \
        t operator^ (const t& rhs) { return t::Interpret(this->AsOrthoVector() ^ rhs.AsOrthoVector()); }

        struct OrthovectorAbstract : public OrthoVectorType {//, public TupleAbstract {
            BR_MATH_F_ORTHOVECTOR(OrthovectorAbstract, OrthoVectorType);
        };
    };

        // Matrices
    
    template<typename t, int rows, int columns>
    struct MatrixDef : Math::Tuple2dDef<t, rows, columns> {
        typedef MatrixDef<t, RowCount, ColumnCount>   MatrixType;
        typedef MatrixDef<t, RowCount, ColumnCount>   EquivalentMatrixType;

            // -- Type

        static const size_t OperatingDimensionality  = ColumnCount;

            // -- Downcast
        
        MatrixType& AsMatrix() {
            return *(MatrixType*)(this);
        }
        const MatrixType& AsMatrix() const {
            return *(const MatrixType*)(this);
        }

            // -- Multiplications

        template< typename out = Math::VectorDef<ScalarType, RowCount>::VectorAbstract,
                  typename = std::enable_if_t<std::is_same<out::VectorType, Math::VectorDef<ScalarType, RowCount>>::value> >
        out Mul(const Math::VectorDef<ScalarType, ColumnCount>& rhs) const {
            out o;
            for (int r = 0; r < RowCount; r++) {
                o[r] = (ScalarType)0;
                for (int i = 0; i < ColumnCount; i++) {
                    o[r] += (*this)[r][i] * rhs[i];
                }
            }
            return o;
        }

        template< typename in, typename out = Math::MatrixDef<ScalarType, ColumnCount, in::RowCount>::MatrixAbstract,
                  typename = std::enable_if_t<ColumnCount == in::RowCount> >
        out operator* (in rhs) const {
            out o;
            for (int r = 0; r < out::RowCount; r++) {
                for (int c = 0; c < out::ColumnCount; c++) {
                    o[r][c] = (ScalarType)0;
                    for (int i = 0; i < ColumnCount; i++) {
                        o[r][c] += (*this)[r][i] * rhs[i][c];
                    }
                }
            }
            return o;
        }

        template< typename v,
                  typename = typename std::enable_if_t<IsSquare>,
                  typename = std::enable_if_t<std::is_same<v::VectorType, Math::VectorDef<ScalarType, ColumnCount>>::value> >
        v operator* (v rhs) const {
            return this->Mul<v>(rhs);
        }

            // -- Inheritance

#define BR_MATH_F_MATRIX(t, p) \
    BR_MATH_F_TUPLE_2D(t, p)
        
        struct MatrixAbstract : public Tuple2dAbstractMem, public MatrixType {
            BR_MATH_F_MATRIX(MatrixAbstract, MatrixType);
        };
    };

        // Volumes

    template<typename t, int s>
    struct OrthoOrthotopeDef : Math::VectorDef<t, s> {
        typedef OrthoOrthotopeDef<t, s> OrthoOrthotopeType;
        
        OrthoOrthotopeType() { ; }
        OrthoOrthotopeType(TupleType &rh) : VectorType(rh) { ; }

        ScalarType  Volume() const;
    };

}
}