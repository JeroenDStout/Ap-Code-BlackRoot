/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include <cstddef>
#include "BlackRoot/Pubc/Tuple.h"
#include "BlackRoot/Pubc/TemplateUtility.h"

#include "BlackRoot/Pubc/Matrix Operations.h"

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
        
        static const bool   Is_Vector                 = true;
        static const size_t Operating_Dimensionality  = s;

            // -- Downcast

        VectorType& as_vector() {
            return *(VectorType*)(this);
        }
        const VectorType& as_vector() const {
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
        t& operator+= (const t& rhs) { return t::interpret(this->as_vector() += rhs.as_vector()); } \
        friend t operator+ (t lhs, const t& rhs) { return t::interpret(lhs.as_vector() += rhs.as_vector()); } \
        t& operator-= (const t& rhs) { return t::interpret(this->as_vector() -= rhs.as_vector()); } \
        friend t operator- (t lhs, const t& rhs) { return t::interpret(lhs.as_vector() -= rhs.as_vector()); } \
        t& operator*= (const ScalarType& rhs) { return t::interpret(this->as_vector() *= rhs); } \
        friend t operator* (t lhs, const ScalarType& rhs) { return t::interpret(lhs.as_vector() *= rhs); } \
        t& operator/= (const ScalarType& rhs) { return t::interpret(this->as_vector() /= rhs); } \
        friend t operator/ (t lhs, const ScalarType& rhs) { return t::interpret(lhs.as_vector() /= rhs); }
        
        struct VectorAbstract : public VectorType, public Tuple1dAbstractMem {
            BR_MATH_F_VECTOR(VectorAbstract, VectorDef);
        };
    };

    template<typename t, int s>
    struct CovectorDef : VectorDef<t, s> {

            // -- Downcast

        CovectorType& as_covector() {
            return *(VectorType*)(this);
        }
        const CovectorType& as_covector() const {
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

        struct OrthovectorAbstract;
        
        OrthoVectorType() { ; }
        OrthoVectorType(TupleType &rh) : VectorType(rh) { ; }

            // -- Downcast

        OrthoVectorType& as_ortho_vector() {
            return *(OrthoVectorType*)(this);
        }
        const OrthoVectorType& as_ortho_vector() const {
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
        OrthovectorAbstract operator^ (const OrthoVectorType& rhs) const {
            OrthoVectorType::OrthovectorAbstract tmp;
                // xyzzy !
            tmp[0] = (*this)[1] * rhs[2] - (*this)[2] * rhs[1];
            tmp[1] = (*this)[2] * rhs[0] - (*this)[0] * rhs[2];
            tmp[2] = (*this)[0] * rhs[1] - (*this)[1] * rhs[0];
            return tmp;
        }

            // Functional

        ScalarType  length_squared() const {
            return (*this) * (*this);
        }
        ScalarType  length() const {
            return sqrt(this->length_squared());
        }

        void normalise(ScalarType t = (ScalarType)(1)) {
            *this *= (t / this->length());
        }
        void normalise_safe(ScalarType t = (ScalarType)(1)) {
            auto l = this->length_squared();
            if (l > (ScalarType)(0)) {
                *this *= (t / sqrt(l));
            }
        }
        
        template<typename = typename std::enable_if<3 == Size>>
        OrthovectorAbstract cross(const OrthoVectorType &rhs) {
            return std::move((*this) ^ rhs);
        }

            // -- Inheritance
        
#define BR_MATH_F_ORTHOVECTOR(t, p) \
    BR_MATH_F_VECTOR(t, p) \
        template<typename = typename std::enable_if<3 == Size>> \
        t operator^ (const t& rhs) { auto q = this->as_ortho_vector() ^ rhs.as_ortho_vector(); return t::interpret(q); }

        struct OrthovectorAbstract : public OrthoVectorType, public Tuple1dAbstractMem {
            BR_MATH_F_ORTHOVECTOR(OrthovectorAbstract, OrthoVectorType);
        };
    };

        // Matrices
    
    template<typename t, int rows, int columns>
    struct MatrixDef : Math::Tuple2dDef<t, rows, columns> {
        typedef MatrixDef<t, Row_Count, Column_Count>   MatrixType;
        typedef MatrixDef<t, Row_Count, Column_Count>   EquivalentMatrixType;

            // -- Type

        static const size_t  Operating_Dimensionality  = Column_Count;

            // -- Downcast
        
        MatrixType& as_matrix() {
            return *(MatrixType*)(this);
        }
        const MatrixType& as_matrix() const {
            return *(const MatrixType*)(this);
        }

            // -- Multiplications

        template< typename out = Math::VectorDef<ScalarType, Row_Count>::VectorAbstract,
                  typename = std::enable_if_t<std::is_same<out::VectorType, Math::VectorDef<ScalarType, Row_Count>>::value> >
        out mul(const Math::VectorDef<ScalarType, Column_Count>& rhs) const {
            out o;
            for (int r = 0; r < Row_Count; r++) {
                o[r] = (ScalarType)0;
                for (int i = 0; i < Column_Count; i++) {
                    o[r] += (*this)[r][i] * rhs[i];
                }
            }
            return o;
        }

        template< typename in, typename out = Math::MatrixDef<ScalarType, Column_Count, in::Row_Count>::MatrixAbstract,
                  typename = std::enable_if_t<Column_Count == in::Row_Count> >
        out operator* (in rhs) const {
            out o;
            for (int r = 0; r < out::Row_Count; r++) {
                for (int c = 0; c < out::Column_Count; c++) {
                    o[r][c] = (ScalarType)0;
                    for (int i = 0; i < Column_Count; i++) {
                        o[r][c] += (*this)[r][i] * rhs[i][c];
                    }
                }
            }
            return o;
        }

        template< typename v,
                  typename = typename std::enable_if_t<Is_Square>,
                  typename = std::enable_if_t<std::is_same<v::VectorType, Math::VectorDef<ScalarType, Column_Count>>::value> >
        v operator* (v rhs) const {
            return this->mul<v>(rhs);
        }

            // -- Operations
		
        template< typename = typename std::enable_if_t<Is_Square>,
                  typename = typename std::enable_if_t<Column_Count <= Meta::Meta_Largest_Determinant> >
        ScalarType det() const {
			return Determinant<ScalarType, Column_Count>((ScalarType*)(this));
        }

            // -- Inheritance

#define BR_MATH_F_MATRIX(t, p) \
    BR_MATH_F_TUPLE_2D(t, p)
        
        struct MatrixAbstract : public MatrixType, public Tuple2dAbstractMem {
            BR_MATH_F_MATRIX(MatrixAbstract, MatrixType);
        };
    };

        // Volumes

    template<typename t, int s>
    struct OrthoOrthotopeDef : Math::VectorDef<t, s> {
        typedef OrthoOrthotopeDef<t, s> OrthoOrthotopeType;
        
        OrthoOrthotopeType() { ; }
        OrthoOrthotopeType(TupleType &rh) : VectorType(rh) { ; }

        ScalarType  volume() const;
    };

}
}