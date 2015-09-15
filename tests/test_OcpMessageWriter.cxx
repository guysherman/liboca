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


#include <oca/OcaTypes.hxx>
#include <OcpMessageWriter.hxx>
#include <Ocp1Header.hxx>
#include <Ocp1Parameters.hxx>
#include <Ocp1Command.hxx>

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

TEST(Suite_OcpMessageWriter, WriteParametersToBuffer)
{
	oca::net::Ocp1Parameters params;
	params.parameterCount = 3;
	params.parameters.push_back(0);
	params.parameters.push_back(1);
	params.parameters.push_back(2);

	uint8_t testData[64];
	memset(&testData[0], 0, 64);

	boost::asio::mutable_buffer buf(testData, 32);

	oca::OcpMessageWriter::WriteParametersToBuffer(params, buf);

	EXPECT_EQ(testData[0], 0x03);
	EXPECT_EQ(testData[1], 0x00);
	EXPECT_EQ(testData[2], 0x01);
	EXPECT_EQ(testData[3], 0x02);
}

TEST(Suite_OcpMessageWriter, WriteMethodIdToBuffer)
{
	oca::OcaMethodId id;
	memset(&id, 0, sizeof(oca::OcaMethodId));
	id.treeLevel = 0xDEAD;
	id.methodIndex = 0xF00D;

	uint8_t testData[64];
	memset(&testData[0], 0, 64);

	boost::asio::mutable_buffer buf(testData, 64);

	oca::OcpMessageWriter::WriteMethodIdToBuffer(id, buf);

	EXPECT_EQ(testData[0], 0xDE);
	EXPECT_EQ(testData[1], 0xAD);
	EXPECT_EQ(testData[2], 0xF0);
	EXPECT_EQ(testData[3], 0x0D);
}

TEST(Suite_OcpMessageWriter, WriteCommandToBuffer)
{
	oca::net::Ocp1Parameters params;
	params.parameterCount = 3;
	params.parameters.push_back(0);
	params.parameters.push_back(1);
	params.parameters.push_back(2);

	oca::OcaMethodId id;
	memset(&id, 0, sizeof(oca::OcaMethodId));
	id.treeLevel = 0x0102;
	id.methodIndex = 0x0304;

	uint8_t testData[1024];
	memset(&testData[0], 0, 1024);

	boost::asio::mutable_buffer buf(testData, 1024);

	oca::net::Ocp1Command cmd;
	cmd.commandSize = 20;
	cmd.handle = 0xDEADF00D;
	cmd.targetONo = 0xC01DBEEF;
	cmd.methodId = id;
	cmd.parameters = params;

	oca::OcpMessageWriter::WriteCommandToBuffer(cmd, buf);

	EXPECT_EQ(0x14, testData[3]);
	EXPECT_EQ(0x0D, testData[7]);
	EXPECT_EQ(0xEF, testData[11]);
	EXPECT_EQ(0x04, testData[15]);
	EXPECT_EQ(0x03, testData[16]);

}

TEST(Suite_OcpMessageWriter, ComputeCommandDataSize)
{
	oca::net::Ocp1Parameters params;
	params.parameterCount = 3;
	params.parameters.push_back(0);
	params.parameters.push_back(1);
	params.parameters.push_back(2);

	oca::OcaMethodId id;
	memset(&id, 0, sizeof(oca::OcaMethodId));
	id.treeLevel = 0x0102;
	id.methodIndex = 0x0304;

	uint8_t testData[1024];
	memset(&testData[0], 0, 1024);

	boost::asio::mutable_buffer buf(testData, 1024);

	oca::net::Ocp1Command cmd;
	cmd.commandSize = 0;
	cmd.handle = 0xDEADF00D;
	cmd.targetONo = 0xC01DBEEF;
	cmd.methodId = id;
	cmd.parameters = params;


	oca::OcaUint32 commandDataSize = oca::OcpMessageWriter::ComputeCommandDataSize(cmd);

	EXPECT_EQ(20, commandDataSize);
	EXPECT_EQ(20, cmd.commandSize);

}

TEST(Suite_OcpMessageWriter, ComputeCommandListDataSize)
{
	oca::net::Ocp1Parameters params;
	params.parameterCount = 3;
	params.parameters.push_back(0);
	params.parameters.push_back(1);
	params.parameters.push_back(2);

	oca::OcaMethodId id;
	memset(&id, 0, sizeof(oca::OcaMethodId));
	id.treeLevel = 0x0102;
	id.methodIndex = 0x0304;

	uint8_t testData[1024];
	memset(&testData[0], 0, 1024);

	boost::asio::mutable_buffer buf(testData, 1024);

	oca::net::Ocp1Command cmd;
	cmd.commandSize = 0;
	cmd.handle = 0xDEADF00D;
	cmd.targetONo = 0xC01DBEEF;
	cmd.methodId = id;
	cmd.parameters = params;


	std::vector<oca::net::Ocp1Command> commands;
	commands.push_back(cmd);
	commands.push_back(cmd);

	oca::OcaUint32 commandListDataSize = oca::OcpMessageWriter::ComputeCommandListDataSize(commands);

	EXPECT_EQ(40, commandListDataSize);

}
