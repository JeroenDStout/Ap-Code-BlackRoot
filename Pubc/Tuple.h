/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

/* TODO:
 *  - Add easy conversions (i.e., tuple<i,3> ->tuple<f, 3>)
 */

#include <algorithm>
#include <cstddef>
#include <cmath>
#include <iterator>
#include <functional>
#include <initializer_list>
#include <type_traits>

namespace BlackRoot {
namespace Math {

    // Note that tuples *do not* store any memory by themselves, a derived object always
    //  needs to have the actual members
    
    template<typename t, int columns, int rows>
    struct Tuple2dDef;

    template<typename t, int size>
    struct Tuple1dDef {
        struct Tuple1dAbstract;

            // -- Type

        using ScalarType = t;
        using TupleType  = Tuple1dDef<t, size>;
        using TupleImpl  = Tuple1dAbstract;

        static const std::size_t Size   = size;

            // -- Downcast

        TupleType& as_tuple() {
            return *(TupleType*)(this);
        }
        const TupleType& as_tuple() const {
            return *(const TupleType*)(this);
        }

        template <int sub, typename = std::enable_if_t<sub <= Size>>
        Tuple1dDef<t, sub>& as_sub_tuple() {
            return *(Tuple1dDef<t, sub>*)(this);
        }
        template <int sub, typename = std::enable_if_t<sub <= Size>>
        const Tuple1dDef<t, sub>& as_sub_tuple() const {
            return *(const Tuple1dDef<t, sub>*)(this);
        }

            // -- Assignment
        
        void set_to(const ScalarType &sc) {
            std::fill(this->begin(), this->end(), sc);
        }
        TupleType& set_to(const TupleType &rh) {
            return (*this = rh);
        }
        
            // -- Operators

        TupleType& operator=(const TupleType &rh) {
            std::copy(rh.begin(), rh.end(), this->begin());
            return *this;
        }
        bool operator ==(const TupleType &rh) const {
            return std::equal(this->begin(), this->end(), rh.begin());
        }
        bool operator !=(const TupleType &rh) const {
            return !std::equal(this->begin(), this->end(), rh.begin());
        }

        ScalarType& operator[](size_t i) {
            return (this->begin())[i];
        }
        const ScalarType& operator[](size_t i) const {
            return (this->begin())[i];
        }

            // -- Iterators

        ScalarType * begin() {
            return (ScalarType*)(this);
        }
        ScalarType * end() {
            return begin() + Size;
        }
        const ScalarType * begin() const {
            return (const ScalarType*)(this);
        }
        const ScalarType * end() const {
            return begin() + Size;
        }

            // -- Functional

        bool is_real() const {
            return !std::is_floating_point<t>::value ||
                !std::any_of(this->begin(), this->end(), [](auto e) { return std::isnan(e); });
        }

#define BR_MATH_F_TUPLE(t, p) \
        t() { ; } \
        t(const t &rh) { *this = rh; } \
        template<typename... Args, typename = typename std::enable_if<sizeof...(Args) == Size>::type> \
            t(Args... e) { int i = 0; for (const ScalarType p : std::initializer_list<ScalarType>({e...})) { (*this)[i++] = p; } } \
        t& operator=(const t &rh) { return t::interpret(this->as_tuple() = rh.as_tuple()); } \
        static t& interpret(TupleType & rh) { return *(t*)(&rh); } \
        static t && interpret(TupleType && rh) { return std::move(*(t*)(&rh)); } \
        static const t& interpret(const TupleType & rh) { return *(const t*)(&rh); }
        
        struct Tuple1dAbstractMem {
            t e[size];
        };
        struct Tuple1dAbstract : public Tuple1dDef<t, size>, public Tuple1dAbstractMem {
            BR_MATH_F_TUPLE(Tuple1dAbstract, TupleType);
        };
    };
    
    template<typename t, int rows, int columns>
    struct Tuple2dDef : Tuple1dDef<t, columns * rows> {
        struct Tuple2dAbstract;

        typedef Tuple1dDef<t, columns>       TupleRowType;
        typedef Tuple2dDef<t, rows, columns> Tuple2dType;

            // -- Type

        static const std::size_t Column_Count = columns;
        static const std::size_t Row_Count    = rows;
        static const bool        Is_Square    = Column_Count == Row_Count;

        template <int sub, typename = std::enable_if_t<sub <= Row_Count>>
        Tuple2dDef<t, sub, Column_Count>& AsSubTuple2d() {
            return *(Tuple2dDef<t, sub, Column_Count>*)(this);
        }
        template <int sub, typename = std::enable_if_t<sub <= Row_Count>>
        const Tuple2dDef<t, sub, Column_Count>& AsSubTuple2d() const {
            return *(const Tuple2dDef<t, sub, Column_Count>*)(this);
        }
        
            // -- Operators

        Tuple2dType& operator=(const Tuple2dType &rh) {
            TupleType::operator=(rh); return *this;
        }
        bool operator ==(const TupleType &rh) const {
            return TupleType::operator==(rh);
        }
        bool operator !=(const TupleType &rh) const {
            return TupleType::operator!=(rh);
        }

        TupleRowType& operator[](size_t i) {
            return *(TupleRowType*)((ScalarType*)(this) + Column_Count*i);
        }
        const TupleRowType& operator[](size_t i) const {
            return *(const TupleRowType*)((const ScalarType*)(this) + Column_Count*i);
        }

            // -- Iterators

        TupleRowType * begin() {
            return (TupleRowType*)(this);
        }
        TupleRowType * end() {
            return (TupleRowType*)((ScalarType*)(this) + Size);
        }
        const TupleRowType * begin() const {
            return (TupleRowType*)(this);
        }
        const TupleRowType * end() const {
            return (TupleRowType*)((ScalarType*)(this) + Size);
        }

            // -- Inheritance

#define BR_MATH_F_TUPLE_2D(t, p) \
    BR_MATH_F_TUPLE(t, p)
        
        struct Tuple2dAbstractMem {
            t e[rows][columns];
        };
        struct Tuple2dAbstract : public Tuple2dAbstractMem, public Tuple2dDef<t, rows, columns> {
            BR_MATH_F_TUPLE(Tuple2dAbstract, Tuple2dType);
        };
    };

}
}