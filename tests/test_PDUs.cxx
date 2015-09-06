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


// C Standard Headers
#include <stdint.h>

// Boost Headers
#include <boost/asio.hpp>

// 3rd Party Headers
#include <gtest/gtest.h>

// GTK Headers


#include <Ocp1Header.hxx>



TEST(Suite_PDUs, Ocp1Header_ToBufferAndBack)
{
	uint8_t data[512];
	memset(&data[0], 0, 512);

	oca::net::Ocp1Header header;
	header.protocolVersion = 1;
	header.messageSize = 128;
	header.messageCount = 1;
	header.messageType = oca::net::OcaCmdRrq;

	boost::asio::mutable_buffer buffer(data, 512);

	header.WriteToBuffer(buffer);


	boost::asio::const_buffer buffer2(data, 512);
	oca::net::Ocp1Header header2 = oca::net::Ocp1Header::FromBuffer(buffer2);

	EXPECT_EQ(1, header2.protocolVersion);
	EXPECT_EQ(128, header2.messageSize);
	EXPECT_EQ(1, header2.messageCount);
	EXPECT_EQ(oca::net::OcaCmdRrq, header2.messageType);

}
