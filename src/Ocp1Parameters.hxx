/*
  Copyright (C) 2015 Guy Sherman, Shermann Innovations Limited

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/
#ifndef __OCP1PARAMETERS_HXX__
#define __OCP1PARAMETERS_HXX__

// C++ Standard Headers
#include <vector>

// C Standard Headers


// Boost Headers


// 3rd Party Headers


// GTK Headers


#include <oca/OcaTypes.hxx>

namespace oca
{
    namespace net
    {
        // Notably, this class does not decode the parameters, it simply wraps them,
        // It is up to the object executing the command to decode the parameters
        struct Ocp1Parameters
        {

            Ocp1Parameters();
            Ocp1Parameters(const Ocp1Parameters&);

            Ocp1Parameters& operator=(const Ocp1Parameters&);

            ~Ocp1Parameters();

            OcaUint8    parameterCount;             // the number of parameters, not the number of bytes below
            std::vector<OcaUint8>   parameters;    // Hence we use a vector, because managing raw arrays sucks.
        };
    }
}

#endif // __OCP1PARAMETERS_HXX__
