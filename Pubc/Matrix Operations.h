/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include <cstddef>
#include "BlackRoot/Pubc/Tuple.h"

namespace BlackRoot {
namespace Math {

    namespace Meta {
        static const int Meta_Largest_Determinant = 3;
    }
	
	template<typename scalar>
	inline scalar Determinant(scalar x0y0) {
		return x0y0;
	}
	
	template<typename scalar>
	inline scalar Determinant(scalar x0y0, scalar x1y0,
							  scalar x0y1, scalar x1y1) {
		return (x0y0 * x1y1) - (x1y0 * x0y1);
	}
	
	template<typename scalar>
	inline scalar Determinant(scalar x0y0, scalar x1y0, scalar x2y0,
							  scalar x0y1, scalar x1y1, scalar x2y1,
							  scalar x0y2, scalar x1y2, scalar x2y2) {
		return   x0y0 * (x1y1 * x2y2 - x2y1 * x1y2)
			   - x1y0 * (x0y1 * x2y2 - x2y1 * x0y2)
			   + x2y0 * (x0y1 * x1y2 - x1y1 * x0y2);
	}

	template<typename scalar, int dimensionality>
	inline scalar Determinant(scalar * dat) {
		if (dimensionality == 1) {
			return Determinant(dat[0]);
		}
		if (dimensionality == 2) {
			return Determinant(dat[0], dat[1],
							   dat[2], dat[3]);
		}
		if (dimensionality == 3) {
			return Determinant(dat[0], dat[1], dat[2],
							   dat[3], dat[4], dat[5],
                               dat[6], dat[7], dat[8]);
		}
	}

    template<typename mat,
             typename = std::enable_if_t<mat::Is_Square>>
    inline mat & Set_Identity(mat & in)
    {
        auto & tup = in.as_tuple();

        if (mat::Row_Count == 1) {
            tup[0] = 1.f;
            return in;
        }
        if (mat::Row_Count == 2) {
            tup[0] = 1.f;
            tup[1] = 0.f;
            tup[2] = 0.f;
            tup[3] = 1.f;
            return in;
        }
        if (mat::Row_Count == 3) {
            tup[0] = 1.f;
            tup[1] = 0.f;
            tup[2] = 0.f;
            tup[3] = 0.f;
            tup[4] = 1.f;
            tup[5] = 0.f;
            tup[6] = 0.f;
            tup[7] = 0.f;
            tup[8] = 1.f;
            return in;
        }
        if (mat::Row_Count == 4) {
            tup[ 0] = 1.f;
            tup[ 1] = 0.f;
            tup[ 2] = 0.f;
            tup[ 3] = 0.f;
            tup[ 4] = 0.f;
            tup[ 5] = 1.f;
            tup[ 6] = 0.f;
            tup[ 7] = 0.f;
            tup[ 8] = 0.f;
            tup[ 9] = 0.f;
            tup[10] = 1.f;
            tup[11] = 0.f;
            tup[12] = 0.f;
            tup[13] = 0.f;
            tup[14] = 0.f;
            tup[15] = 1.f;
            return in;
        }

        int indx = 0;
        for (int y = 0; y < mat::Row_Count; y++) {
            for (int x = 0; x < mat::Column_Count; x++, indx++) {
                tup[indx] = (x == y)? 1.f : 0.f;
            }
        }
        
        return in;
    }

    template<typename mat,
             typename = std::enable_if_t<mat::Is_Square>>
    inline mat Identity()
    {
        mat ret;
        Set_Identity(ret);
        return ret;
    }

}
}