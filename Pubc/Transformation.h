/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

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
        
        static_assert(Math::has_equivalent_matrix_type<map>::value, "Map must be representable as a matrix");
        static_assert(Math::has_vector_type<translate>::value, "Translation must be a vector");
        static_assert(map::Operating_Dimensionality == translate::Operating_Dimensionality, "Operating dimensionality of map and translation must be equal");
        static_assert(map::EquivalentMatrixType::Is_Square, "Map must be representable as a square matrix");

            // -- Type
        
        static const size_t Size                     = map::Size + translate::Size;
        static const size_t Operating_Dimensionality = map::Operating_Dimensionality;

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