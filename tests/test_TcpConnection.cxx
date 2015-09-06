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
#include "scaffold/MockMessageProcessor.hxx"

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

	uint8_t value = *(uint8_t*)arg;

	boost::asio::io_service svc2;
	boost::asio::ip::tcp::socket socket(svc2);
	boost::asio::ip::tcp::endpoint endpoint(
		boost::asio::ip::address::from_string("127.0.0.1"), 60000);
	socket.connect(endpoint);

	for (;;)
	{
	  uint8_t buffer[1];
	  buffer[0] = value;
	  boost::array<char, 128> buf;
	  boost::system::error_code error;

	  boost::asio::write(socket, boost::asio::buffer(buffer, 1), error);

	  socket.read_some(boost::asio::buffer(buf), error);

	  if (error == boost::asio::error::eof)
		break; // Connection closed cleanly by peer.
	  else if (error)
		throw boost::system::system_error(error); // Some other error.
	}

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

	oca_test::MockMessageProcessor::pointer proc = *(reinterpret_cast<oca_test::MockMessageProcessor::pointer*>(arg));

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
	boost::shared_ptr<oca_test::MockMessageProcessor> sproc(new oca_test::MockMessageProcessor());

	uint8_t sv = 0x3B;

	pthread_t s, t;
	pthread_create(&s, NULL, &serverRun, &sproc);
	usleep(500000);
	pthread_create(&t, NULL, &clientRun, &sv);
	pthread_join(t, NULL);

	EXPECT_EQ(true, sproc->gotCorrectValue);
	EXPECT_EQ(0x3B, sproc->bufferData[0]);
}

TEST(Suite_TcpConnection, ReadSyncValue_IncorrectValue)
{
	boost::shared_ptr<oca_test::MockMessageProcessor> sproc(new oca_test::MockMessageProcessor());

	uint8_t sv = 0xDE;

	pthread_t s, t;
	pthread_create(&s, NULL, &serverRun, &sproc);
	usleep(500000);
	pthread_create(&t, NULL, &clientRun, &sv);
	pthread_join(t, NULL);

	EXPECT_EQ(false, sproc->gotCorrectValue);
	EXPECT_EQ(0xDE, sproc->bufferData[0]);
}
