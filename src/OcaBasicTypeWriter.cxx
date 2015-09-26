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
#include "OcaBasicTypeWriter.hxx"

namespace oca
{
	void OcaBasicTypeWriter::WriteBlobToBuffer(OcaBlob& blob, boost::asio::mutable_buffer& buffer)
	{
		assert(blob.size() < 0xFFFF);
		WriteUint16ToBuffer((OcaUint16)blob.size(), buffer);
		WriteVectorUint8ToBuffer(blob, buffer);
	}

	void OcaBasicTypeWriter::WriteUint16ToBuffer(OcaUint16 value, boost::asio::mutable_buffer& buffer)
	{
		OcaUint16* dest = boost::asio::buffer_cast<OcaUint16*>(buffer);
		*dest = htons(value);
		buffer = buffer + sizeof(OcaUint16);
	}

	void OcaBasicTypeWriter::WriteVectorUint8ToBuffer(std::vector<OcaUint8>& vec, boost::asio::mutable_buffer& buffer)
	{
		OcaUint8* dest = boost::asio::buffer_cast<OcaUint8*>(buffer);
		assert(dest != NULL);

		memcpy(dest, &vec[0], vec.size());

		buffer = buffer + (vec.size() * sizeof(OcaUint8));
	}
}
