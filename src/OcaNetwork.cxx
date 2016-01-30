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
#include <exception>

// C Standard Headers
#include <stdint.h>

// Boost Headers
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

// 3rd Party Headers


// GTK Headers

#include <oca/OcaException.hxx>
#include <oca/OcaNetwork.hxx>

#include "OcpSessionFactory.hxx"
#include "TcpConnectionFactory.hxx"
#include "TcpServer.hxx"

#include "IOcpSession.hxx"

#include "OcpSession.hxx"


namespace oca
{
    // NB: port is explicitly not initialized here so that we can range check it
    // because OCP.1 Only supports ports in the dynamic range: 49152..65535
    OcaNetwork::OcaNetwork(uint16_t port)
        : isRunning(false)
    {
        // Only have to check the lower bound, because uint16_t won't let us
        // exceed the upper bound. Any overflow will cause us to fall below
        // the lower bound
        if (port < 49152)
        {
            oca::Exception e(__func__, __LINE__, __FILE__, "Port was out of allowed range.");
            throw e;
        }

        sessionFactory = boost::shared_ptr<oca::net::OcpSessionFactory>(new oca::net::OcpSessionFactory());
        boost::shared_ptr<boost::asio::io_service> ioService(new boost::asio::io_service());
        boost::shared_ptr<oca::net::TcpConnectionFactory> factory(new oca::net::TcpConnectionFactory(ioService));
        tcpServer = boost::shared_ptr<oca::net::TcpServer>(
            new oca::net::TcpServer(
                factory,
                ioService,
                port,
                boost::bind(
                    &OcaNetwork::newConnectionCreated,
                    this,
                    _1
                )
            )
        );
    }

    void OcaNetwork::Start()
    {
        isRunning = true;
        tcpServer->Start();
        isRunning = false;
    }

    void OcaNetwork::Stop()
    {
        tcpServer->Stop();
        while (isRunning)
        {
            usleep(32000);
        }
    }

    void OcaNetwork::newConnectionCreated(boost::shared_ptr<oca::net::ITcpConnection> connection)
    {
        oca::net::IOcpSession::pointer session = sessionFactory->CreateSession();
        session->SetTcpConnection(connection);
        session->AddSessionClosedHandler(
            boost::bind(
                &OcaNetwork::sessionEnded,
                this,
                _1
            )
        );
        sessions.insert(std::pair<int, oca::net::IOcpSession::pointer>(session->GetId(), session));
    }

    void OcaNetwork::sessionEnded(boost::shared_ptr<oca::net::IOcpSession> session)
    {
        sessions.erase(session->GetId());
    }

    OcaNetwork::~OcaNetwork()
    {

    }

}
