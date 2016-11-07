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
#include "../src/OcpMessageWriter.hxx"
#include "../src/Ocp1Header.hxx"
#include "../src/Ocp1Command.hxx"
#include "../src/Ocp1Parameters.hxx"
#include "../src/Ocp1EventData.hxx"
#include "../src/Ocp1Response.hxx"

TEST(Suite_OcpMessageWriter, WriteHeaderToBuffer)
{
	oca::ocp::Ocp1Header header;
	header.protocolVersion = 1;
	header.messageSize = 2;
	header.messageType = oca::ocp::OcaCmdRrq;
	header.messageCount = 1;

	uint8_t testData[32];
	memset(&testData[0], 0, 32);

	uint8_t* buf = &testData[0];

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

	uint8_t* buf = &testData[0];

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

	uint8_t* buf = &testData[0];

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

	uint8_t* buf = &testData[0];

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

TEST(Suite_OcpMessageWriter, WriteCommandListToBuffer)
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

	uint8_t* buf = &testData[0];

	oca::net::Ocp1Command cmd;
	cmd.commandSize = 20;
	cmd.handle = 0xDEADF00D;
	cmd.targetONo = 0xC01DBEEF;
	cmd.methodId = id;
	cmd.parameters = params;

	std::vector<oca::net::Ocp1Command> commands;
	commands.push_back(cmd);
	commands.push_back(cmd);

	oca::OcpMessageWriter::WriteCommandListToBuffer(commands, buf);

	EXPECT_EQ(0x14, testData[3]);
	EXPECT_EQ(0x0D, testData[7]);
	EXPECT_EQ(0xEF, testData[11]);
	EXPECT_EQ(0x04, testData[15]);
	EXPECT_EQ(0x03, testData[16]);

	EXPECT_EQ(0x14, testData[23]);
	EXPECT_EQ(0x0D, testData[27]);
	EXPECT_EQ(0xEF, testData[31]);
	EXPECT_EQ(0x04, testData[35]);
	EXPECT_EQ(0x03, testData[36]);

}

TEST(Suite_OcpMessageWriter, WriteResponseToBuffer)
{
	oca::net::Ocp1Parameters params;
	params.parameterCount = 3;
	params.parameters.push_back(0);
	params.parameters.push_back(1);
	params.parameters.push_back(2);

	uint8_t testData[1024];
	memset(&testData[0], 0, 1024);

	uint8_t* buf = &testData[0];

	oca::net::Ocp1Response resp;
	resp.responseSize = 13;
	resp.handle = 0xDEADF00D;
	resp.statusCode = oca::BufferOverflow;
	resp.parameters = params;

	oca::OcpMessageWriter::WriteResponseToBuffer(resp, buf);

	EXPECT_EQ(0x0D, testData[3]);
	EXPECT_EQ(0x0D, testData[7]);
	EXPECT_EQ(0x0E, testData[8]);
	EXPECT_EQ(0x03, testData[9]);
	EXPECT_EQ(0x02, testData[12]);

}

TEST(Suite_OcpMessageWriter, ComputeResponseDataSize)
{
	oca::net::Ocp1Parameters params;
	params.parameterCount = 3;
	params.parameters.push_back(0);
	params.parameters.push_back(1);
	params.parameters.push_back(2);

	uint8_t testData[1024];
	memset(&testData[0], 0, 1024);

	oca::net::Ocp1Response resp;
	resp.responseSize = 13;
	resp.handle = 0xDEADF00D;
	resp.statusCode = oca::BufferOverflow;
	resp.parameters = params;


	oca::OcaUint32 respDataSize = oca::OcpMessageWriter::ComputeResponseDataSize(resp);

	EXPECT_EQ(13, respDataSize);
	EXPECT_EQ(13, resp.responseSize);

}

TEST(Suite_OcpMessageWriter, ComputeResponseListDataSize)
{
	oca::net::Ocp1Parameters params;
	params.parameterCount = 3;
	params.parameters.push_back(0);
	params.parameters.push_back(1);
	params.parameters.push_back(2);

	uint8_t testData[1024];
	memset(&testData[0], 0, 1024);

	oca::net::Ocp1Response resp;
	resp.responseSize = 13;
	resp.handle = 0xDEADF00D;
	resp.statusCode = oca::BufferOverflow;
	resp.parameters = params;


	oca::net::ResponseList responses;
	responses.push_back(resp);
	responses.push_back(resp);

	oca::OcaUint32 responseListDataSize = oca::OcpMessageWriter::ComputeResponseListDataSize(responses);

	EXPECT_EQ(26, responseListDataSize);

}

TEST(Suite_OcpMessageWriter, WriteResponseListToBuffer)
{
	oca::net::Ocp1Parameters params;
	params.parameterCount = 3;
	params.parameters.push_back(0);
	params.parameters.push_back(1);
	params.parameters.push_back(2);

	uint8_t testData[1024];
	memset(&testData[0], 0, 1024);

	uint8_t* buf = &testData[0];

	oca::net::Ocp1Response resp;
	resp.responseSize = 13;
	resp.handle = 0xDEADF00D;
	resp.statusCode = oca::BufferOverflow;
	resp.parameters = params;


	oca::net::ResponseList responses;
	responses.push_back(resp);
	responses.push_back(resp);

	oca::OcpMessageWriter::WriteResponseListToBuffer(responses, buf);

	EXPECT_EQ(0x0D, testData[3]);
	EXPECT_EQ(0x0D, testData[7]);
	EXPECT_EQ(0x0E, testData[8]);
	EXPECT_EQ(0x03, testData[9]);
	EXPECT_EQ(0x02, testData[12]);

	EXPECT_EQ(0x0D, testData[16]);
	EXPECT_EQ(0x0D, testData[20]);
	EXPECT_EQ(0x0E, testData[21]);
	EXPECT_EQ(0x03, testData[22]);
	EXPECT_EQ(0x02, testData[25]);

}

TEST(Suite_OcpMessageWriter, WriteEventIdToBuffer)
{
	oca::OcaEventId id;
	memset(&id, 0, sizeof(oca::OcaEventId));
	id.treeLevel = 0xDEAD;
	id.eventIndex = 0xF00D;

	uint8_t testData[64];
	memset(&testData[0], 0, 64);

	uint8_t* buf = &testData[0];

	oca::OcpMessageWriter::WriteEventIdToBuffer(id, buf);

	EXPECT_EQ(testData[0], 0xDE);
	EXPECT_EQ(testData[1], 0xAD);
	EXPECT_EQ(testData[2], 0xF0);
	EXPECT_EQ(testData[3], 0x0D);
}

TEST(Suite_OcpMessageWriter, WriteEventToBuffer)
{
	oca::OcaEvent event;
	memset(&event, 0, sizeof(oca::OcaEvent));
	event.emitterONo = 0xC01DBEEF;
	event.eventId.treeLevel = 0xDEAD;
	event.eventId.eventIndex = 0xF00D;

	uint8_t testData[64];
	memset(&testData[0], 0, 64);

	uint8_t* buf = &testData[0];

	oca::OcpMessageWriter::WriteEventToBuffer(event, buf);

	EXPECT_EQ(testData[0], 0xC0);
	EXPECT_EQ(testData[1], 0x1D);
	EXPECT_EQ(testData[2], 0xBE);
	EXPECT_EQ(testData[3], 0xEF);
	EXPECT_EQ(testData[4], 0xDE);
	EXPECT_EQ(testData[5], 0xAD);
	EXPECT_EQ(testData[6], 0xF0);
	EXPECT_EQ(testData[7], 0x0D);
}

TEST(Suite_OcpMessageWriter, WriteEventDataToBuffer)
{
	oca::OcaEvent event;
	memset(&event, 0, sizeof(oca::OcaEvent));
	event.emitterONo = 0xC01DBEEF;
	event.eventId.treeLevel = 0xDEAD;
	event.eventId.eventIndex = 0xF00D;

	oca::net::Ocp1EventData edata;
	edata.event = event;
	edata.eventParameters.push_back(0xCA);
	edata.eventParameters.push_back(0xFE);
	edata.eventParameters.push_back(0xBA);
	edata.eventParameters.push_back(0xBE);
	edata.eventParameters.push_back(0xAB);
	edata.eventParameters.push_back(0xCD);
	edata.eventParameters.push_back(0xEF);
	edata.eventParameters.push_back(0xFE);


	uint8_t testData[64];
	memset(&testData[0], 0, 64);

	uint8_t* buf = &testData[0];

	oca::OcpMessageWriter::WriteEventDataToBuffer(edata, buf);

	EXPECT_EQ(testData[0], 0xC0);
	EXPECT_EQ(testData[8], 0xCA);
	EXPECT_EQ(testData[9], 0xFE);
	EXPECT_EQ(testData[10], 0xBA);
	EXPECT_EQ(testData[11], 0xBE);
	EXPECT_EQ(testData[12], 0xAB);
	EXPECT_EQ(testData[13], 0xCD);
	EXPECT_EQ(testData[14], 0xEF);
	EXPECT_EQ(testData[15], 0xFE);
}

TEST(Suite_OcpMessageWriter, WriteNtf1ParamsToBuffer)
{
	uint8_t testData[64];
	memset(&testData[0], 0, 64);
	uint8_t* buf = &testData[0];

	oca::net::Ocp1NtfParams ntfParams;
	ntfParams.parameterCount = 1;
	ntfParams.context.push_back(0xBA);
	ntfParams.context.push_back(0xBE);

	oca::OcaEvent event;
	memset(&event, 0, sizeof(oca::OcaEvent));
	event.emitterONo = 0xC01DBEEF;
	event.eventId.treeLevel = 0xDEAD;
	event.eventId.eventIndex = 0xF00D;

	oca::net::Ocp1EventData edata;
	edata.event = event;
	edata.eventParameters.push_back(0xCA);
	edata.eventParameters.push_back(0xFE);
	edata.eventParameters.push_back(0xBA);
	edata.eventParameters.push_back(0xBE);
	edata.eventParameters.push_back(0xAB);
	edata.eventParameters.push_back(0xCD);
	edata.eventParameters.push_back(0xEF);
	edata.eventParameters.push_back(0xFE);

	ntfParams.eventData = edata;


	oca::OcpMessageWriter::WriteNtfParamsToBuffer(ntfParams, buf);

	// parameter count
	EXPECT_EQ(testData[0], 0x01);

	// context.size
	EXPECT_EQ(testData[2], 0x02);
	// context.data
	EXPECT_EQ(testData[3], 0xBA);
	EXPECT_EQ(testData[4], 0xBE);

	// event.emitterONo
	EXPECT_EQ(testData[5], 0xC0);
	EXPECT_EQ(testData[6], 0x1D);
	EXPECT_EQ(testData[7], 0xBE);
	EXPECT_EQ(testData[8], 0xEF);
	// eventId.treeLevel
	EXPECT_EQ(testData[9], 0xDE);
	EXPECT_EQ(testData[10], 0xAD);
	// eventId.eventIndex
	EXPECT_EQ(testData[11], 0xF0);
	EXPECT_EQ(testData[12], 0x0D);

	EXPECT_EQ(testData[13], 0xCA);
	EXPECT_EQ(testData[14], 0xFE);
	EXPECT_EQ(testData[15], 0xBA);
	EXPECT_EQ(testData[16], 0xBE);
	EXPECT_EQ(testData[17], 0xAB);
	EXPECT_EQ(testData[18], 0xCD);
	EXPECT_EQ(testData[19], 0xEF);
	EXPECT_EQ(testData[20], 0xFE);

}

TEST(Suite_OcpMessageWriter, WriteNotificationToBuffer)
{
	uint8_t testData[64];
	memset(&testData[0], 0, 64);
	uint8_t* buf = &testData[0];


	oca::net::Ocp1Notification notification;
	notification.notificationSize = 33;
	notification.targetONo = 0xDEADBEEF;
	notification.methodId.treeLevel = 0x1234;
	notification.methodId.methodIndex = 0x5678;

	oca::net::Ocp1NtfParams ntfParams;
	ntfParams.parameterCount = 1;
	ntfParams.context.push_back(0xBA);
	ntfParams.context.push_back(0xBE);

	oca::OcaEvent event;
	memset(&event, 0, sizeof(oca::OcaEvent));
	event.emitterONo = 0xC01DBEEF;
	event.eventId.treeLevel = 0xDEAD;
	event.eventId.eventIndex = 0xF00D;

	oca::net::Ocp1EventData edata;
	edata.event = event;
	edata.eventParameters.push_back(0xCA);
	edata.eventParameters.push_back(0xFE);
	edata.eventParameters.push_back(0xBA);
	edata.eventParameters.push_back(0xBE);
	edata.eventParameters.push_back(0xAB);
	edata.eventParameters.push_back(0xCD);
	edata.eventParameters.push_back(0xEF);
	edata.eventParameters.push_back(0xFE);

	ntfParams.eventData = edata;
	notification.parameters = ntfParams;


	oca::OcpMessageWriter::WriteNotificationToBuffer(notification, buf);

	// notification size
	EXPECT_EQ(testData[0], 0x00);
	EXPECT_EQ(testData[1], 0x00);
	EXPECT_EQ(testData[2], 0x00);
	EXPECT_EQ(testData[3], 0x21);

	// ONo
	EXPECT_EQ(testData[4], 0xDE);
	EXPECT_EQ(testData[5], 0xAD);
	EXPECT_EQ(testData[6], 0xBE);
	EXPECT_EQ(testData[7], 0xEF);

	// method Id
	EXPECT_EQ(testData[8],  0x12);
	EXPECT_EQ(testData[9],  0x34);
	EXPECT_EQ(testData[10], 0x56);
	EXPECT_EQ(testData[11], 0x78);

	// parameter count
	EXPECT_EQ(testData[12], 0x01);

	// context.size
	EXPECT_EQ(testData[14], 0x02);

	// context.data
	EXPECT_EQ(testData[15], 0xBA);
	EXPECT_EQ(testData[16], 0xBE);

	// event.emitterONo
	EXPECT_EQ(testData[17], 0xC0);
	EXPECT_EQ(testData[18], 0x1D);
	EXPECT_EQ(testData[19], 0xBE);
	EXPECT_EQ(testData[20], 0xEF);

	// eventId.treeLevel
	EXPECT_EQ(testData[21], 0xDE);
	EXPECT_EQ(testData[22], 0xAD);

	// eventId.eventIndex
	EXPECT_EQ(testData[23], 0xF0);
	EXPECT_EQ(testData[24], 0x0D);

	// params
	EXPECT_EQ(testData[25], 0xCA);
	EXPECT_EQ(testData[26], 0xFE);
	EXPECT_EQ(testData[27], 0xBA);
	EXPECT_EQ(testData[28], 0xBE);
	EXPECT_EQ(testData[29], 0xAB);
	EXPECT_EQ(testData[30], 0xCD);
	EXPECT_EQ(testData[31], 0xEF);
	EXPECT_EQ(testData[32], 0xFE);

}

TEST(Suite_OcpMessageWriter, ComputeNotificationSize)
{
	uint8_t testData[1024];
	memset(&testData[0], 0, 1024);

	oca::net::Ocp1Notification notification;
	notification.notificationSize = 0;
	notification.targetONo = 0xDEADBEEF;
	notification.methodId.treeLevel = 0x1234;
	notification.methodId.methodIndex = 0x5678;

	oca::net::Ocp1NtfParams ntfParams;
	ntfParams.parameterCount = 1;
	ntfParams.context.push_back(0xBA);
	ntfParams.context.push_back(0xBE);

	oca::OcaEvent event;
	memset(&event, 0, sizeof(oca::OcaEvent));
	event.emitterONo = 0xC01DBEEF;
	event.eventId.treeLevel = 0xDEAD;
	event.eventId.eventIndex = 0xF00D;

	oca::net::Ocp1EventData edata;
	edata.event = event;
	edata.eventParameters.push_back(0xCA);
	edata.eventParameters.push_back(0xFE);
	edata.eventParameters.push_back(0xBA);
	edata.eventParameters.push_back(0xBE);
	edata.eventParameters.push_back(0xAB);
	edata.eventParameters.push_back(0xCD);
	edata.eventParameters.push_back(0xEF);
	edata.eventParameters.push_back(0xFE);

	ntfParams.eventData = edata;
	notification.parameters = ntfParams;

	oca::OcaUint32 size = oca::OcpMessageWriter::ComputeNotificationDataSize(notification);

	EXPECT_EQ(size, 33);
	EXPECT_EQ(notification.notificationSize, 33);

}

TEST(Suite_OcpMessageWriter, ComputeNotificationListSize)
{
	uint8_t testData[1024];
	memset(&testData[0], 0, 1024);

	oca::net::Ocp1Notification notification;
	notification.notificationSize = 0;
	notification.targetONo = 0xDEADBEEF;
	notification.methodId.treeLevel = 0x1234;
	notification.methodId.methodIndex = 0x5678;

	oca::net::Ocp1NtfParams ntfParams;
	ntfParams.parameterCount = 1;
	ntfParams.context.push_back(0xBA);
	ntfParams.context.push_back(0xBE);

	oca::OcaEvent event;
	memset(&event, 0, sizeof(oca::OcaEvent));
	event.emitterONo = 0xC01DBEEF;
	event.eventId.treeLevel = 0xDEAD;
	event.eventId.eventIndex = 0xF00D;

	oca::net::Ocp1EventData edata;
	edata.event = event;
	edata.eventParameters.push_back(0xCA);
	edata.eventParameters.push_back(0xFE);
	edata.eventParameters.push_back(0xBA);
	edata.eventParameters.push_back(0xBE);
	edata.eventParameters.push_back(0xAB);
	edata.eventParameters.push_back(0xCD);
	edata.eventParameters.push_back(0xEF);
	edata.eventParameters.push_back(0xFE);

	ntfParams.eventData = edata;
	notification.parameters = ntfParams;

	oca::net::NotificationList notifs;
	notifs.push_back(notification);
	notifs.push_back(notification);
	notifs.push_back(notification);

	oca::OcaUint32 size = oca::OcpMessageWriter::ComputeNotificationListDataSize(notifs);

	EXPECT_EQ(size, 99);

}

TEST(Suite_OcpMessageWriter, WriteNotificationListToBuffer)
{
	uint8_t testData[1024];
	memset(&testData[0], 0, 1024);
	uint8_t* buf = &testData[0];


	oca::net::Ocp1Notification notification;
	notification.notificationSize = 33;
	notification.targetONo = 0xDEADBEEF;
	notification.methodId.treeLevel = 0x1234;
	notification.methodId.methodIndex = 0x5678;

	oca::net::Ocp1NtfParams ntfParams;
	ntfParams.parameterCount = 1;
	ntfParams.context.push_back(0xBA);
	ntfParams.context.push_back(0xBE);

	oca::OcaEvent event;
	memset(&event, 0, sizeof(oca::OcaEvent));
	event.emitterONo = 0xC01DBEEF;
	event.eventId.treeLevel = 0xDEAD;
	event.eventId.eventIndex = 0xF00D;

	oca::net::Ocp1EventData edata;
	edata.event = event;
	edata.eventParameters.push_back(0xCA);
	edata.eventParameters.push_back(0xFE);
	edata.eventParameters.push_back(0xBA);
	edata.eventParameters.push_back(0xBE);
	edata.eventParameters.push_back(0xAB);
	edata.eventParameters.push_back(0xCD);
	edata.eventParameters.push_back(0xEF);
	edata.eventParameters.push_back(0xFE);

	ntfParams.eventData = edata;
	notification.parameters = ntfParams;

	oca::net::NotificationList notifs;
	notifs.push_back(notification);
	notifs.push_back(notification);
	notifs.push_back(notification);

	oca::OcpMessageWriter::WriteNotificationListToBuffer(notifs, buf);


	EXPECT_EQ(testData[3], 0x21);
	EXPECT_EQ(testData[32], 0xFE);

	EXPECT_EQ(testData[36], 0x21);
	EXPECT_EQ(testData[65], 0xFE);

	EXPECT_EQ(testData[69], 0x21);
	EXPECT_EQ(testData[98], 0xFE);

}
