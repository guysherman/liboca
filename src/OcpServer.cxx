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
#include <exception>

// C Standard Headers
#include <string.h>
#include <stdio.h>
// Boost Headers


// 3rd Party Headers


// Our Headers
#include <oca/OcpServer.hxx>


namespace oca
{
	namespace ocp
	{
		Server::Server(const char* listenAddress, const char* port) : addressInfo(NULL)
		{
			int status;
			struct addrinfo hints;

			memset(&hints, 0, sizeof(struct addrinfo));
			hints.ai_family = AF_UNSPEC;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_flags = AI_PASSIVE;

			status = getaddrinfo(listenAddress, port, &hints, &this->addressInfo);
			if (status != 0)
			{
				fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
				throw new std::exception();
			}


		}

		// TODO: create socket, bind, listen, spawn accept thread, spawn connection threads

		// TODO: stop accept thread, stop connection threads, unbind, close

		Server::~Server()
		{
			if (this->addressInfo != NULL)
			{
				freeaddrinfo(this->addressInfo);
				this->addressInfo = NULL;
			}
		}
	}
}
