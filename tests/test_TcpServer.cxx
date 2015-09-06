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

#include <TcpServer.hxx>

#include "scaffold/MockTcpConnection.hxx"
#include "scaffold/MockTcpConnectionFactory.hxx"

struct ThreadArgWrapper
{
	ThreadArgWrapper(boost::asio::io_service& svc, boost::shared_ptr<oca::net::MockTcpConnectionFactory> cf, oca::net::TcpServer& srv)
		:	svc(svc), cf(cf), srv(srv)
	{

	}

	boost::asio::io_service& svc;
	boost::shared_ptr<oca::net::MockTcpConnectionFactory> cf;
	oca::net::TcpServer& srv;
};


static void* clientRun(void* arg)
{
	if (arg == NULL)
	{
		pthread_detach(pthread_self());
		return NULL;
	}

	uint16_t value = *(uint16_t*)arg;

	boost::asio::io_service svc2;
	boost::asio::ip::tcp::socket socket(svc2);
	boost::asio::ip::tcp::endpoint endpoint(
		boost::asio::ip::address::from_string("127.0.0.1"), value);
	socket.connect(endpoint);

	usleep(500000);
	socket.close();
	pthread_detach(pthread_self());
	return NULL;

}


static void* clientRunFail(void* arg)
{
	if (arg == NULL)
	{
		pthread_detach(pthread_self());
		return NULL;
	}

	uint16_t value = *(uint16_t*)arg;

	boost::asio::io_service svc2;
	boost::asio::ip::tcp::socket socket(svc2);
	boost::asio::ip::tcp::endpoint endpoint(
		boost::asio::ip::address::from_string("127.0.0.1"), value);

	try
	{
		socket.connect(endpoint);
	}
	catch (std::exception)
	{
		return NULL;
	}

 	std::exception e;
	throw e;
	return NULL;

}

static void* serverRun(void* arg)
{
	//localScaffold l;


	if (arg == NULL)
	{
		pthread_detach(pthread_self());
		return NULL;
	}

	ThreadArgWrapper* w = reinterpret_cast<ThreadArgWrapper*>(arg);

	w->svc.run();


	pthread_detach(pthread_self());
	return NULL;
}



TEST(Suite_TcpServer, TcpServer_AcceptsCorrectPort)
{
	boost::asio::io_service svc;
	boost::shared_ptr<oca::net::MockTcpConnectionFactory> cf(new oca::net::MockTcpConnectionFactory(svc, 1));
	oca::net::TcpServer srv(cf, svc, 60000);

	ThreadArgWrapper wrp(svc, cf, srv);

	pthread_t s, t;
	pthread_create(&s, NULL, &serverRun, &wrp);

	usleep(500000);
	int a = 60000;

	pthread_create(&t, NULL, &clientRun, &a);
	pthread_join(t, 0);
	EXPECT_EQ(1, cf->connections.size());
	EXPECT_EQ(true, cf->connections.at(0)->started);

	pthread_join(s, NULL);
}

TEST(Suite_TcpServer, TcpServer_RefusesOtherPorts)
{
	boost::asio::io_service svc;
	boost::shared_ptr<oca::net::MockTcpConnectionFactory> cf(new oca::net::MockTcpConnectionFactory(svc, 1));
	oca::net::TcpServer srv(cf, svc, 60000);

	ThreadArgWrapper wrp(svc, cf, srv);

	pthread_t s, t;
	pthread_create(&s, NULL, &serverRun, &wrp);

	usleep(500000);
	int a = 60000;
	int b = 60001;

	pthread_create(&t, NULL, &clientRunFail, &b);
	pthread_join(t, 0);

	pthread_create(&t, NULL, &clientRun, &a);
	pthread_join(t, 0);
	EXPECT_EQ(1, cf->connections.size());
	EXPECT_EQ(true, cf->connections.at(0)->started);

	pthread_join(s, NULL);


}

TEST(Suite_TcpServer, TcpServer_AcceptsMultiple)
{
	boost::asio::io_service svc;
	boost::shared_ptr<oca::net::MockTcpConnectionFactory> cf(new oca::net::MockTcpConnectionFactory(svc, 2));
	oca::net::TcpServer srv(cf, svc, 60000);

	ThreadArgWrapper wrp(svc, cf, srv);

	pthread_t s, t;
	pthread_create(&s, NULL, &serverRun, &wrp);

	usleep(500000);
	int a = 60000;

	EXPECT_EQ(1, cf->connections.size());
	pthread_create(&t, NULL, &clientRun, &a);
	pthread_join(t, 0);
	EXPECT_EQ(2, cf->connections.size());
	EXPECT_EQ(true, cf->connections.at(0)->started);

	pthread_create(&t, NULL, &clientRun, &a);
	pthread_join(t, 0);

	EXPECT_EQ(true, cf->connections.at(0)->started);

	pthread_join(s, NULL);
}
