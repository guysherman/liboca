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
#include <OcaBasicTypeWriter.hxx>

TEST(Suite_OcaBasicTypeWriter, WriteUint8ToBuffer)
{
	oca::OcaUint8 value =  0xBA;

	uint8_t testData[64];
	memset(&testData[0], 0, 64);

	boost::asio::mutable_buffer buf(testData, 64);

	oca::OcaBasicTypeWriter::WriteUint8ToBuffer(value, buf);

	const uint8_t* bufferLocation = boost::asio::buffer_cast<const uint8_t*>(buf);

	EXPECT_EQ(testData[0], 0xBA);
	EXPECT_EQ(&testData[1], bufferLocation);
}

TEST(Suite_OcaBasicTypeWriter, WriteUint16ToBuffer)
{
	oca::OcaUint16 value =  0xBABE;

	uint8_t testData[64];
	memset(&testData[0], 0, 64);

	boost::asio::mutable_buffer buf(testData, 64);

	oca::OcaBasicTypeWriter::WriteUint16ToBuffer(value, buf);

	const uint8_t* bufferLocation = boost::asio::buffer_cast<const uint8_t*>(buf);

	EXPECT_EQ(testData[0], 0xBA);
	EXPECT_EQ(testData[1], 0xBE);
	EXPECT_EQ(&testData[2], bufferLocation);
}

TEST(Suite_OcaBasicTypeWriter, WriteUint32ToBuffer)
{
	oca::OcaUint32 value =  0xBABECAFE;

	uint8_t testData[64];
	memset(&testData[0], 0, 64);

	boost::asio::mutable_buffer buf(testData, 64);

	oca::OcaBasicTypeWriter::WriteUint32ToBuffer(value, buf);

	const uint8_t* bufferLocation = boost::asio::buffer_cast<const uint8_t*>(buf);

	EXPECT_EQ(testData[0], 0xBA);
	EXPECT_EQ(testData[1], 0xBE);
	EXPECT_EQ(testData[2], 0xCA);
	EXPECT_EQ(testData[3], 0xFE);
	EXPECT_EQ(&testData[4], bufferLocation);
}


TEST(Suite_OcaBasicTypeWriter, WriteBlobToBuffer)
{

	oca::OcaBlob blob;
	blob.push_back(0xCA);
	blob.push_back(0xFE);
	blob.push_back(0xBA);
	blob.push_back(0xBE);
	blob.push_back(0xAB);
	blob.push_back(0xCD);
	blob.push_back(0xEF);
	blob.push_back(0xFE);


	uint8_t testData[64];
	memset(&testData[0], 0, 64);

	boost::asio::mutable_buffer buf(testData, 64);

	oca::OcaBasicTypeWriter::WriteBlobToBuffer(blob, buf);
	const uint8_t* bufferLocation = boost::asio::buffer_cast<const uint8_t*>(buf);

	EXPECT_EQ(0x00, testData[0]);
	EXPECT_EQ(0x08, testData[1]);
	EXPECT_EQ(0xCA, testData[2]);
	EXPECT_EQ(0xFE, testData[3]);
	EXPECT_EQ(0xBA, testData[4]);
	EXPECT_EQ(0xBE, testData[5]);
	EXPECT_EQ(0xAB, testData[6]);
	EXPECT_EQ(0xCD, testData[7]);
	EXPECT_EQ(0xEF, testData[8]);
	EXPECT_EQ(0xFE, testData[9]);

	EXPECT_EQ(&testData[10], bufferLocation);
}
