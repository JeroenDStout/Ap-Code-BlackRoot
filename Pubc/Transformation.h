/*
 *
 *  © Stout Games 2019
 *
 */

#pragma once

#include <cstddef>
#include "BlackRoot/Pubc/TemplateUtility.h"

namespace BlackRoot {
namespace Math {

    template<typename map, typename translate>
    struct TranslatedMap {
        using MapType            = map;
        using TranslateType      = translate;
        using ScalarType         = typename map::ScalarType;
        using TranslatedMapType  = TranslatedMap<map, translate>;
        
        static_assert(Math::has_equivalent_matrix_type<map>::value, "Map must be representable as a square matrix");
        static_assert(Math::has_vector_type<translate>::value, "Translation must be a vector");
        static_assert(map::OperatingDimensionality == translate::OperatingDimensionality, "Operating dimensionality of map and translation must be equal");
        static_assert(map::EquivalentMatrixType::IsSquare, "Map must be representable as a square matrix");

            // -- Type
        
        static const size_t Size                     = map::Size + translate::Size;
        static const size_t OperatingDimensionality  = map::OperatingDimensionality;

            // -- Members

        MapType         Map;
        TranslateType   Translate;

            // -- Operators

        template< typename vType,
                  typename = std::enable_if_t<std::is_same<vType::VectorType, Math::VectorDef<map::ScalarType, OperatingDimensionality>>::value> >
        vType operator*(vType rhs) const {
            return (rhs = (this->Map * rhs) + this->Translate);
        }

    };

}
}