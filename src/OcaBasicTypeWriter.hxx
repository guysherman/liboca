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
#ifndef __OCABASICTYPEWRITER_HXX__
#define __OCABASICTYPEWRITER_HXX__

// C++ Standard Headers


// C Standard Headers


// Boost Headers
#include <boost/asio.hpp>

// 3rd Party Headers


// GTK Headers


// Our Headers
#include <oca/OcaTypes.hxx>

namespace oca
{
	class OcaBasicTypeWriter
	{
	public:

		static void WriteBlobToBuffer(OcaBlob& blob, boost::asio::mutable_buffer& buffer);
		static void WriteUint16ToBuffer(OcaUint16 value, boost::asio::mutable_buffer& buffer);

		static void WriteVectorUint8ToBuffer(std::vector<OcaUint8>& vec, boost::asio::mutable_buffer& buffer);
	};
}

#endif // __OCABASICTYPEWRITER_HXX__
