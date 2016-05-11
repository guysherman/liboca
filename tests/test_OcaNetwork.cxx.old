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
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

// 3rd Party Headers
#include <gtest/gtest.h>

// GTK Headers


#include <oca/OcaNetwork.hxx>

static void* worker(void* arg)
{
	if (arg == 0)
	{
		return NULL;
	}
	oca::OcaNetwork* net = reinterpret_cast<oca::OcaNetwork*>(arg);

	net->Start();

	return NULL;
}

static void* sendClient(void* arg)
{
	// Set up a basic client
	boost::asio::io_service svc2;
	boost::asio::ip::tcp::socket socket(svc2);
	boost::asio::ip::tcp::endpoint endpoint(
		boost::asio::ip::address::from_string("127.0.0.1"), 60000);
	socket.connect(endpoint);

	boost::system::error_code error;

	// Data deliberately crafted to cause the other end to terminate
	const uint8_t testData[16] = {0x04, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04 };
	boost::asio::const_buffer buf(testData, 16);

	// Write the data
	boost::asio::write(socket, boost::asio::buffer(buf, 16), error);

	if (error != boost::asio::error::eof && error != 0)
		throw boost::system::system_error(error); // Some other error.

	pthread_detach(pthread_self());
	return NULL;

}



TEST(Suite_OcaNetwork, StartStop)
{
	oca::OcaNetwork network(60000);

	pthread_t t;
	pthread_t u;

	pthread_create(&t, NULL, &worker, &network);
	usleep(1000000);

	pthread_create(&u, NULL, &sendClient, NULL);
	usleep(1000000);

	network.Stop();
	pthread_join(t, NULL);

	//EXPECT_EQ(1, network.Dummy());
}
