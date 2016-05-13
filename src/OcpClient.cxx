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
#include <unistd.h>

// Boost Headers


// 3rd Party Headers


// Our Headers
#include <oca/OcpClient.hxx>
#include "OcpConnectionEndpoint.hxx"

namespace oca
{
	namespace ocp
	{
		Client::Client(const char* hostAddress, const char* port) : clientSocketFileDescriptor(0)
		{
			int status;
			struct addrinfo hints, *candidateAddresses, *currentCandidate;

			memset(&hints, 0, sizeof(struct addrinfo));
			hints.ai_family = AF_UNSPEC;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_flags = AI_PASSIVE;

			status = getaddrinfo(hostAddress, port, &hints, &candidateAddresses);
			if (status != 0)
			{
				fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
				throw new std::exception();
			}

			for (currentCandidate = candidateAddresses;
				currentCandidate != NULL;
				currentCandidate = currentCandidate->ai_next)
			{
				int socketFd = socket(currentCandidate->ai_family,
									currentCandidate->ai_socktype,
									currentCandidate->ai_protocol);
				if(socketFd == -1)
				{
					// TODO: some sort of logging
					continue;
				}

				status = connect(socketFd, currentCandidate->ai_addr, currentCandidate->ai_addrlen);
				if (status == -1)
				{
					close(socketFd);
					fprintf(stderr, "connect failed");
					continue;
				}

				// If we've got this far then we have a socket, and it is bound,
				// so let's store the socket away
				this->clientSocketFileDescriptor = socketFd;
				break;
			}

			freeaddrinfo(candidateAddresses);
			if (currentCandidate == NULL)
			{
				// Shouldn't really get here, but just in case...
				fprintf(stderr, "client connect failed completely");
				throw std::exception();
			}

			this->endpoint = boost::shared_ptr<oca::ocp::ConnectionEndpoint>(new oca::ocp::ConnectionEndpoint(this->clientSocketFileDescriptor));

		}

		Client::~Client()
		{
			if(this->clientSocketFileDescriptor != 0)
			{
				close(clientSocketFileDescriptor);
				this->clientSocketFileDescriptor = 0;
			}
		}
	}
}
