/*
  Copyright (C) 2016 Guy Sherman

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

// Our Headers
#include <oca/OcpServer.hxx>
#include <oca/OcpClient.hxx>
#include "../src/OcpConnectionEndpoint.hxx"


TEST(GrindConstructors, OcaOcpServer)
{
	try
	{
		oca::ocp::Server* server = new oca::ocp::Server(NULL, "49152");
		delete server;
	}
	catch (std::exception e)
	{

	}

}

TEST(GrindConstructors, OcaOcpClient)
{
		try
		{
			oca::ocp::Client* client = new oca::ocp::Client("localhost", "49152");
			delete client;
		}
		catch (std::exception e)
		{

		}
}

TEST(GrindConstructors, OcpConnectionEndpoint)
{
		try
		{
			oca::ocp::ConnectionEndpoint* endpoint = new oca::ocp::ConnectionEndpoint(0);
			delete endpoint;
		}
		catch (std::exception e)
		{

		}
}
