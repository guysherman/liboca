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


// Our Headers
#include <OcpSession.hxx>
#include <OcpMessageReader.hxx>
#include <Ocp1Header.hxx>
#include <Ocp1Parameters.hxx>
#include <Ocp1Command.hxx>
#include <Ocp1Response.hxx>
#include <Ocp1EventData.hxx>
#include <Ocp1NtfParams.hxx>

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


TEST(Suite_OcpSession, SyncValueReceived_GetHeaderCalledForCorrectSyncValue)
{
	oca::net::OcpSession processor;
	CallbackCheck cbc;

	uint8_t buffer[1024];
	memset(&buffer[0], 0, 1024);
	buffer[0] = 0x3B;

	boost::system::error_code ec = boost::system::errc::make_error_code(boost::system::errc::success);

	processor.SyncValueReceived(&buffer[0], ec, 1, boost::bind(&CallbackCheck::Fire, &cbc));

	EXPECT_EQ(true, cbc.fired);
}

TEST(Suite_OcpSession, SyncValueReceived_GetHeaderNotCalledForIncorrectSyncValue)
{
	oca::net::OcpSession processor;
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

TEST(Suite_OcpSession, SyncValueReceived_GetHeaderNotCalledForNullData)
{
	oca::net::OcpSession processor;
	CallbackCheck cbc;

	boost::system::error_code ec = boost::system::errc::make_error_code(boost::system::errc::success);

	processor.SyncValueReceived(0, ec, 1, boost::bind(&CallbackCheck::Fire, &cbc));

	EXPECT_EQ(false, cbc.fired);
}


TEST(Suite_OcpSession, SyncValueReceived_GetHeaderNotCalledForLtOneByte)
{
	oca::net::OcpSession processor;
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

TEST(Suite_OcpSession, SyncValueReceived_GetHeaderNotCalledForGtOneByte)
{
	oca::net::OcpSession processor;
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

TEST(Suite_OcpSession, SyncValueReceived_GetHeaderNotCalledForErrCode)
{
	oca::net::OcpSession processor;
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
