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

// C++ Standard Headers
#include <vector>

// C Standard Headers
#include <cstdio>
#include <arpa/inet.h>

// Boost Headers
#include <boost/asio.hpp>

// 3rd Party Headers


// GTK Headers


// Our Headers
#include <oca/OcaTypes.hxx>
#include "OcaBasicTypeReader.hxx"

namespace oca
{
	void OcaBasicTypeReader::BlobFromBuffer(const uint8_t** buffer, OcaBlob& blob)
	{
		OcaUint16 blobSize = Uint16FromBuffer(buffer);
		BufferToUint8Vector(buffer, blobSize, blob);

	}

	OcaUint8 OcaBasicTypeReader::Uint8FromBuffer(const uint8_t** buffer)
	{
		OcaUint8 result = *(*buffer);
		*buffer = *buffer + sizeof(OcaUint8);
		return result;
	}

	OcaUint16 OcaBasicTypeReader::Uint16FromBuffer(const uint8_t** buffer)
	{
		OcaUint16 result = ntohs(* reinterpret_cast<const uint16_t*>(*buffer));
		*buffer = *buffer + sizeof(OcaUint16);
		return result;
	}

	OcaUint32 OcaBasicTypeReader::Uint32FromBuffer(const uint8_t** buffer)
	{
		OcaUint32 result = ntohl(* reinterpret_cast<const uint32_t*>(*buffer));
		*buffer = *buffer + sizeof(OcaUint32);
		return result;
	}

	void OcaBasicTypeReader::BufferToUint8Vector(const uint8_t** buffer, size_t numBytes, std::vector<OcaUint8>& vec)
	{
		assert (vec.size() == 0);

		const OcaUint8* bytes = *buffer;

		// We ask the vector to grow in one hit, so we can memcpy straight after into it
		// We reserve then resize, rather than just resizing, so that we only get one
		// allocation, rather than log(n) allocations;
		// We resize so that the size()  value is up to date, and so that the
		// memory gets initialized to 0.
		vec.reserve(numBytes);
		vec.resize(numBytes, 0);
		// TODO: security issue: we trust that the buffer actually has the right number of bytes #security
		memcpy(&vec[0], bytes, numBytes);

		*buffer = *buffer + (numBytes * sizeof(OcaUint8));
	}
}
