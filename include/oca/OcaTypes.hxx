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
#ifndef __OCATYPES_HXX__
#define __OCATYPES_HXX__

// C++ Standard Headers


// C Standard Headers
#include <stdint.h>

// Boost Headers


// 3rd Party Headers


// GTK Headers


namespace oca
{
	typedef uint8_t OcaUint8;
	typedef uint16_t OcaUint16;
	typedef uint32_t OcaUint32;
	typedef uint64_t OcaUint64;

	typedef OcaUint32 	OcaONo;
	typedef OcaUint8	OcaStatus;

	typedef struct
	{
		OcaUint16 treeLevel;
		OcaUint16 methodIndex;
	} OcaMethodId;

	typedef struct
	{
		OcaUint16 treeLevel;
		OcaUint16 eventIndex;
	} OcaEventId;

}

#endif // __OCATYPES_HXX__
