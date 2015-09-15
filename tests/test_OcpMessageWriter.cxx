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


// Boost Headers


// 3rd Party Headers
#include <gtest/gtest.h>

// GTK Headers



#include <OcpMessageWriter.hxx>
#include <Ocp1Header.hxx>

TEST(Suite_OcpMessageWriter, WriteHeaderToBuffer)
{
	oca::net::Ocp1Header header;
	header.protocolVersion = 1;
	header.messageSize = 2;
	header.messageType = oca::net::OcaCmdRrq;
	header.messageCount = 1;

	uint8_t testData[32];
	memset(&testData[0], 0, 32);

	boost::asio::mutable_buffer buf(testData, 32);

	oca::OcpMessageWriter::WriteHeaderToBuffer(header, buf);

	EXPECT_EQ(testData[1], 0x01);
	EXPECT_EQ(testData[5], 0x02);
	EXPECT_EQ(testData[6], 0x01);
	EXPECT_EQ(testData[8], 0x01);
}
