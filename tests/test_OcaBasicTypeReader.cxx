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
#include <cstdio>

// Boost Headers
#include <boost/asio.hpp>

// 3rd Party Headers
#include <gtest/gtest.h>

// GTK Headers


// Our Headers
#include <oca/OcaTypes.hxx>
#include "../src/OcaBasicTypeReader.hxx"

TEST(Suite_OcpBasicTypeReader, Uint8FromBuffer)
{
	const uint8_t testData[16] = {0xBA, 0xBE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	const uint8_t* buf = (uint8_t *)testData;

	oca::OcaUint8 value = oca::OcaBasicTypeReader::Uint8FromBuffer(&buf);
	const uint8_t* bufferLocation = buf;

	EXPECT_EQ(0xBA, value);
	EXPECT_EQ(&testData[1], bufferLocation);
}

TEST(Suite_OcpBasicTypeReader, Uint16FromBuffer)
{
	const uint8_t testData[16] = {0xBA, 0xBE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	const uint8_t* buf = (uint8_t *)testData;

	oca::OcaUint16 value = oca::OcaBasicTypeReader::Uint16FromBuffer(&buf);
	const uint8_t* bufferLocation = buf;

	EXPECT_EQ(0xBABE, value);
	EXPECT_EQ(&testData[2], bufferLocation);
}

TEST(Suite_OcpBasicTypeReader, Uint32FromBuffer)
{
	const uint8_t testData[16] = {0xBA, 0xBE, 0xCA, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	const uint8_t* buf = (uint8_t *)testData;

	oca::OcaUint32 value = oca::OcaBasicTypeReader::Uint32FromBuffer(&buf);
	const uint8_t* bufferLocation = buf;

	EXPECT_EQ(0xBABECAFE, value);
	EXPECT_EQ(&testData[4], bufferLocation);
}

TEST(Suite_OcpBasicTypeReader, BlobFromBuffer)
{
	const uint8_t testData[20] = {0x00, 0x0E, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0xDE, 0xAD, 0xBE, 0xEF };
	const uint8_t* buf = (uint8_t *)testData;

	oca::OcaBlob blob;
	oca::OcaBasicTypeReader::BlobFromBuffer(&buf, blob);

	const uint8_t* bufferLocation = buf;

	EXPECT_EQ(0x02, blob[1]);
	EXPECT_EQ(0x04, blob[13]);
	EXPECT_EQ(&testData[16], bufferLocation);
}
