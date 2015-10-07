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
#include <pthread.h>

// Boost Headers
#include <boost/asio.hpp>

// 3rd Party Headers
#include <gtest/gtest.h>

// GTK Headers

#include <TcpConnection.hxx>
#include "scaffold/MockMessageReader.hxx"

class localScaffold
{
public:
	static void handleAccept(oca::net::TcpConnection::pointer conn, const boost::system::error_code& error)
	{
		if (conn != NULL)
		{
			conn->Start();
		}

	}


};

static void* clientRun(void* arg)
{
	if (arg == NULL)
	{
		pthread_detach(pthread_self());
		return NULL;
	}

	uint8_t* value = (uint8_t*)arg;

	boost::asio::io_service svc2;
	boost::asio::ip::tcp::socket socket(svc2);
	boost::asio::ip::tcp::endpoint endpoint(
		boost::asio::ip::address::from_string("127.0.0.1"), 60000);
	socket.connect(endpoint);

	//for (;;)
	//{
	  uint8_t buffer[1024];
	  memset(&buffer[0], 0, 1024);
	  memcpy(&buffer[0], value, 13);
	  boost::array<char, 128> buf;
	  boost::system::error_code error;

	  boost::asio::write(socket, boost::asio::buffer(buffer, 13), error);

	  //socket.read_some(boost::asio::buffer(buf), error);

	  if (error != boost::asio::error::eof && error != 0)
		throw boost::system::system_error(error); // Some other error.
	//}

	pthread_detach(pthread_self());
	return NULL;

}

static void* serverRun(void* arg)
{
	//localScaffold l;
	boost::asio::io_service svc;

	if (arg == NULL)
	{
		pthread_detach(pthread_self());
		return NULL;
	}

	oca_test::MockMessageReader::pointer proc = *(reinterpret_cast<oca_test::MockMessageReader::pointer*>(arg));

	boost::asio::ip::tcp::acceptor acc(svc, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 60000) );

	oca::net::TcpConnection::pointer con = oca::net::TcpConnection::Create(acc.get_io_service(), proc);

	acc.async_accept(con->GetSocket(),
        boost::bind(&localScaffold::handleAccept, con, boost::asio::placeholders::error));

	svc.run();

	pthread_detach(pthread_self());
	return NULL;
}

TEST(Suite_TcpConnection, ReadSyncValue_CorrectValue)
{
	boost::shared_ptr<oca_test::MockMessageReader> sproc(new oca_test::MockMessageReader());

	//					  SV,    PV,        MS,						MT,	  MC			D            SV
	uint8_t testData[16] = {0x3B, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

	pthread_t s, t;
	pthread_create(&s, NULL, &serverRun, &sproc);
	usleep(500000);
	pthread_create(&t, NULL, &clientRun, &testData[0]);
	pthread_join(s, NULL);

	EXPECT_EQ(true, sproc->gotCorrectValue);
	EXPECT_EQ(0x3B, sproc->bufferData[0]);
}

TEST(Suite_TcpConnection, ReadSyncValue_IncorrectValue)
{
	boost::shared_ptr<oca_test::MockMessageReader> sproc(new oca_test::MockMessageReader());

	uint8_t testData[16] = {0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF,0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF };

	pthread_t s, t;
	pthread_create(&s, NULL, &serverRun, &sproc);
	usleep(500000);
	pthread_create(&t, NULL, &clientRun, &testData[0]);
	pthread_join(s, NULL);

	EXPECT_EQ(false, sproc->gotCorrectValue);
	EXPECT_EQ(0xDE, sproc->bufferData[0]);
}

// Can't use a shared_ptr here... it seems to keep the test runner alive indefinitely,
// this is a possible deadlock???
static oca::net::ITcpConnection* extCon = NULL;

static void* sendServer(void* arg)
{
	// Make the thread cancellable
	int oldState = 0;
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &oldState);

	// Make cancellation asynchronous
	int oldType = 0;
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldType);
	boost::asio::io_service svc;

	if (arg == NULL)
	{
		pthread_detach(pthread_self());
		return NULL;
	}

	// Set up a basic TCP server infrastructure
	oca_test::MockMessageReader::pointer proc = *(reinterpret_cast<oca_test::MockMessageReader::pointer*>(arg));
	boost::asio::ip::tcp::acceptor acc(svc, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 60000) );
	oca::net::TcpConnection::pointer con = oca::net::TcpConnection::Create(acc.get_io_service(), proc);
	extCon = con.get();

	acc.async_accept(con->GetSocket(),
        boost::bind(&localScaffold::handleAccept, con, boost::asio::placeholders::error));

	svc.run();

	pthread_detach(pthread_self());
	return NULL;
}

static void* sendClient(void* arg)
{
	if (arg == NULL)
	{
		pthread_detach(pthread_self());
		return NULL;
	}

	// Get our output array
	uint8_t* data = static_cast<uint8_t*>(arg);

	// Set up a basic client
	boost::asio::io_service svc2;
	boost::asio::ip::tcp::socket socket(svc2);
	boost::asio::ip::tcp::endpoint endpoint(
		boost::asio::ip::address::from_string("127.0.0.1"), 60000);
	socket.connect(endpoint);

	boost::array<uint8_t, 128> buf;
	boost::system::error_code error;

	// Wait to read some data
	socket.read_some(boost::asio::buffer(buf), error);

	if (error != boost::asio::error::eof && error != 0)
	throw boost::system::system_error(error); // Some other error.

	// Copy the data we read into the output array
	boost::asio::mutable_buffer b = boost::asio::buffer(buf);
	uint8_t* buffer = boost::asio::buffer_cast<uint8_t*>(b);
	memcpy(data, buffer, 16);

	// Write the data back (so that we deliberately cause the connection to close
	// by sending the wrong sync value)
	boost::asio::write(socket, boost::asio::buffer(buf, 13), error);

	if (error != boost::asio::error::eof && error != 0)
		throw boost::system::system_error(error); // Some other error.

	pthread_detach(pthread_self());
	return NULL;

}


TEST(Suite_TcpConnection, Send)
{
	boost::shared_ptr<oca_test::MockMessageReader> sproc(new oca_test::MockMessageReader());

	uint8_t testData[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint8_t testData2[16] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x00 };
	boost::asio::const_buffer b = boost::asio::buffer(&testData2[0], 16);

	pthread_t s, t;
	pthread_create(&s, NULL, &sendServer, &sproc);
	usleep(100000);
	pthread_create(&t, NULL, &sendClient, &testData[0]);
	usleep(100000);
	extCon->Send(b, 16);
	pthread_join(t, NULL);

	pthread_cancel(s);

	EXPECT_EQ(testData[0], 0x01);
	EXPECT_EQ(testData[5], 0x06);
	EXPECT_EQ(testData[15], 0x00);
}
