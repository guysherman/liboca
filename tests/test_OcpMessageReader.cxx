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

#include <OcpMessageReader.hxx>
#include <Ocp1Header.hxx>
#include <Ocp1Parameters.hxx>
#include <Ocp1Command.hxx>

class CallbackCheck
{
public:

	CallbackCheck() : fired(false) {}

	void Fire()
	{
		fired = true;
	}

	bool fired;

};


TEST(Suite_OcpMessageReader, SyncValueReceived_GetHeaderCalledForCorrectSyncValue)
{
	oca::OcpMessageReader processor;
	CallbackCheck cbc;

	uint8_t buffer[1024];
	memset(&buffer[0], 0, 1024);
	buffer[0] = 0x3B;

	boost::system::error_code ec = boost::system::errc::make_error_code(boost::system::errc::success);

	processor.SyncValueReceived(&buffer[0], ec, 1, boost::bind(&CallbackCheck::Fire, &cbc));

	EXPECT_EQ(true, cbc.fired);
}

TEST(Suite_OcpMessageReader, SyncValueReceived_GetHeaderNotCalledForIncorrectSyncValue)
{
	oca::OcpMessageReader processor;
	CallbackCheck cbc;

	uint8_t buffer[1024];
	memset(&buffer[0], 0, 1024);
	buffer[0] = 0xDE;
	buffer[1] = 0xAD;
	buffer[2] = 0xBE;
	buffer[3] = 0xEF;


	boost::system::error_code ec = boost::system::errc::make_error_code(boost::system::errc::success);

	processor.SyncValueReceived(&buffer[0], ec, 1, boost::bind(&CallbackCheck::Fire, &cbc));

	EXPECT_EQ(false, cbc.fired);
}

TEST(Suite_OcpMessageReader, SyncValueReceived_GetHeaderNotCalledForNullData)
{
	oca::OcpMessageReader processor;
	CallbackCheck cbc;

	boost::system::error_code ec = boost::system::errc::make_error_code(boost::system::errc::success);

	processor.SyncValueReceived(0, ec, 1, boost::bind(&CallbackCheck::Fire, &cbc));

	EXPECT_EQ(false, cbc.fired);
}


TEST(Suite_OcpMessageReader, SyncValueReceived_GetHeaderNotCalledForLtOneByte)
{
	oca::OcpMessageReader processor;
	CallbackCheck cbc;

	uint8_t buffer[1024];
	memset(&buffer[0], 0, 1024);
	buffer[0] = 0xDE;
	buffer[1] = 0xAD;
	buffer[2] = 0xBE;
	buffer[3] = 0xEF;

	boost::system::error_code ec = boost::system::errc::make_error_code(boost::system::errc::success);

	processor.SyncValueReceived(&buffer[0], ec, 0, boost::bind(&CallbackCheck::Fire, &cbc));

	EXPECT_EQ(false, cbc.fired);
}

TEST(Suite_OcpMessageReader, SyncValueReceived_GetHeaderNotCalledForGtOneByte)
{
	oca::OcpMessageReader processor;
	CallbackCheck cbc;

	uint8_t buffer[1024];
	memset(&buffer[0], 0, 1024);
	buffer[0] = 0xDE;
	buffer[1] = 0xAD;
	buffer[2] = 0xBE;
	buffer[3] = 0xEF;

	boost::system::error_code ec = boost::system::errc::make_error_code(boost::system::errc::success);

	processor.SyncValueReceived(&buffer[0], ec, 22346, boost::bind(&CallbackCheck::Fire, &cbc));

	EXPECT_EQ(false, cbc.fired);
}

TEST(Suite_OcpMessageReader, SyncValueReceived_GetHeaderNotCalledForErrCode)
{
	oca::OcpMessageReader processor;
	CallbackCheck cbc;

	uint8_t buffer[1024];
	memset(&buffer[0], 0, 1024);
	buffer[0] = 0xDE;
	buffer[1] = 0xAD;
	buffer[2] = 0xBE;
	buffer[3] = 0xEF;

	boost::system::error_code ec = boost::system::errc::make_error_code(boost::system::errc::io_error);

	processor.SyncValueReceived(&buffer[0], ec, 1, boost::bind(&CallbackCheck::Fire, &cbc));

	EXPECT_EQ(false, cbc.fired);

	ec = boost::system::errc::make_error_code(boost::system::errc::host_unreachable);

	processor.SyncValueReceived(&buffer[0], ec, 1, boost::bind(&CallbackCheck::Fire, &cbc));

	EXPECT_EQ(false, cbc.fired);
}

TEST(Suite_OcpMessageReader, HeaderFromBuffer)
{
	const uint8_t testData[16] = {0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	boost::asio::const_buffer buf(testData, 16);

	const oca::net::Ocp1Header header = oca::OcpMessageReader::HeaderFromBuffer(buf);

	EXPECT_EQ(1, header.protocolVersion);
	EXPECT_EQ(2, header.messageSize);
	EXPECT_EQ(oca::net::OcaCmdRrq, (oca::net::OcaMessageType)header.messageType);
	EXPECT_EQ(1, header.messageCount);


}

TEST(Suite_OcpMessageReader, ParametersFromBuffer)
{
	const uint8_t testData[16] = {0x04, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04 };
	boost::asio::const_buffer buf(testData, 16);

	oca::net::Ocp1Parameters params;
	oca::OcpMessageReader::ParametersFromBuffer(buf, 16, params);

	EXPECT_EQ(4, params.parameterCount);
	EXPECT_EQ(15, params.parameters.size());
	EXPECT_EQ(1, params.parameters.at(0));
	EXPECT_EQ(2, params.parameters.at(2));
	EXPECT_EQ(3, params.parameters.at(6));
	EXPECT_EQ(4, params.parameters.at(14));
}

TEST(Suite_OcpMessageReader, MethodIdFromBuffer)
{
	const uint8_t testData[16] = {0x04, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04 };
	boost::asio::const_buffer buf(testData, 16);

	oca::OcaMethodId id;
	memset(&id, 0, sizeof(oca::OcaMethodId));
	oca::OcpMessageReader::MethodIdFromBuffer(buf, id);

	EXPECT_EQ(0x0401, id.treeLevel);
	EXPECT_EQ(0x0002, id.methodIndex);
}

TEST(Suite_OcpMessageReader, CommandFromBuffer)
{
	const uint8_t testData[32] = {	0x00, 0x00, 0x00, 0x20, 0xDE, 0xAD, 0xF0, 0x0D, 0xC0, 0x1D, 0xBE, 0xEF, 0x01, 0x02, 0x03, 0x04,
									0x04, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04 };
	boost::asio::const_buffer buf(testData, 32);

	oca::net::Ocp1Command cmd;
	oca::OcpMessageReader::CommandFromBuffer(buf, cmd);

	EXPECT_EQ(32, cmd.commandSize);
	EXPECT_EQ(0xDEADF00D, cmd.handle);
	EXPECT_EQ(0xC01DBEEF, cmd.targetONo);
	EXPECT_EQ(0x0102, cmd.methodId.treeLevel);
	EXPECT_EQ(0x0304, cmd.methodId.methodIndex);
	EXPECT_EQ(15, cmd.parameters.parameters.size());

}

TEST(Suite_OcpMessageReader, CommandListFromBuffer)
{
	const uint8_t testData[64] = {	0x00, 0x00, 0x00, 0x20, 0xDE, 0xAD, 0xF0, 0x0D, 0xC0, 0x1D, 0xBE, 0xEF, 0x01, 0x02, 0x03, 0x04,
									0x04, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04,
									0x00, 0x00, 0x00, 0x20, 0xDE, 0xAD, 0xF0, 0x0D, 0xC0, 0x1D, 0xBE, 0xEF, 0x01, 0x02, 0x03, 0x04,
									0x04, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04 };
	boost::asio::const_buffer buf(testData, 64);

	oca::net::Ocp1Header header;
	header.protocolVersion = 1;
	header.messageSize = 74;
	header.messageType = oca::net::OcaCmdRrq;
	header.messageCount = 2;

	std::vector<oca::net::Ocp1Command> commands;
	oca::OcpMessageReader::CommandListFromBuffer(buf, header, commands);

	EXPECT_EQ(2, commands.size());

	oca::net::Ocp1Command cmd = commands[0];

	EXPECT_EQ(32, cmd.commandSize);
	EXPECT_EQ(0xDEADF00D, cmd.handle);
	EXPECT_EQ(0xC01DBEEF, cmd.targetONo);
	EXPECT_EQ(0x0102, cmd.methodId.treeLevel);
	EXPECT_EQ(0x0304, cmd.methodId.methodIndex);
	EXPECT_EQ(15, cmd.parameters.parameters.size());

}
