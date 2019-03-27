/*
 *
 *  © Stout Games 2019
 *
 */

#pragma once

#include <algorithm>
#include <cstddef>
#include <cmath>
#include <iterator>
#include <functional>
#include <initializer_list>

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

        TupleType& AsTuple() {
            return *(TupleType*)(this);
        }
        const TupleType& AsTuple() const {
            return *(const TupleType*)(this);
        }

        template <int sub, typename = std::enable_if_t<sub <= Size>>
        Tuple1dDef<t, sub>& AsSubTuple() {
            return *(Tuple1dDef<t, sub>*)(this);
        }
        template <int sub, typename = std::enable_if_t<sub <= Size>>
        const Tuple1dDef<t, sub>& AsSubTuple() const {
            return *(const Tuple1dDef<t, sub>*)(this);
        }

            // -- Assignment
        
        void SetTo(const ScalarType &sc) {
            std::fill(this->begin(), this->end(), sc);
        }
        TupleType& SetTo(const TupleType &rh) {
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
            return std::not_equal_to(this->begin(), this->end(), rh.begin());
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

        bool IsReal() const {
            return std::all_of(this->begin(), this->end(), [](auto e) { return std::isnan(e); });
        }

#define BR_MATH_F_TUPLE(t, p) \
        t() { ; } \
        t(const t &rh) { *this = rh; } \
        template<typename... Args, typename = typename std::enable_if<sizeof...(Args) == Size>::type> \
            t(Args... e) { int i = 0; for (const ScalarType p : std::initializer_list<ScalarType>({e...})) { (*this)[i++] = p; } } \
        t& operator=(const t &rh) { return t::Interpret(this->AsTuple() = rh.AsTuple()); } \
        static t& Interpret(TupleType & rh) { return *(t*)(&rh); } \
        static t && Interpret(TupleType && rh) { return std::move(*(t*)(&rh)); } \
        static const t& Interpret(const TupleType & rh) { return *(const t*)(&rh); }
        
        struct TupleAbstractMem {
            t e[size];
        };
        struct TupleAbstract : public TupleAbstractMem, public Tuple1dDef<t, size> {
            BR_MATH_F_TUPLE(TupleAbstract, TupleType);
        };
    };
    
    template<typename t, int rows, int columns>
    struct Tuple2dDef : Tuple1dDef<t, columns * rows> {
        typedef Tuple1dDef<t, columns>       TupleRowType;
        typedef Tuple2dDef<t, rows, columns> Tuple2dType;

            // -- Type

        static const std::size_t ColumnCount = columns;
        static const std::size_t RowCount    = rows;
        static const bool        IsSquare    = ColumnCount == RowCount;

        template <int sub, typename = std::enable_if_t<sub <= RowCount>>
        Tuple2dDef<t, sub, ColumnCount>& AsSubTuple2d() {
            return *(Tuple2dDef<t, sub, ColumnCount>*)(this);
        }
        template <int sub, typename = std::enable_if_t<sub <= RowCount>>
        const Tuple2dDef<t, sub, ColumnCount>& AsSubTuple2d() const {
            return *(const Tuple2dDef<t, sub, ColumnCount>*)(this);
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
            return *(TupleRowType*)((ScalarType*)(this) + ColumnCount*i);
        }
        const TupleRowType& operator[](size_t i) const {
            return *(const TupleRowType*)((const ScalarType*)(this) + ColumnCount*i);
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