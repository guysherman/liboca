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
#include <oca/OcpServer.hxx>


namespace oca
{
	namespace ocp
	{
		Server::Server(const char* listenAddress, const char* port) : listenSocketFileDescriptor(0)
		{
			int status;
			int yes = 1;
			struct addrinfo hints, *candidateAddresses, *currentCandidate;


			memset(&hints, 0, sizeof(struct addrinfo));
			hints.ai_family = AF_UNSPEC;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_flags = AI_PASSIVE;

			status = getaddrinfo(listenAddress, port, &hints, &candidateAddresses);
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

				status = setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
				if (status == -1)
				{
					fprintf(stderr, "setsockopt failed");
					// TODO: use our exception here so we get useful info
					freeaddrinfo(candidateAddresses);
					throw new std::exception();
				}

				status = bind(socketFd, currentCandidate->ai_addr, currentCandidate->ai_addrlen);
				if (status == -1)
				{
					close(socketFd);
					fprintf(stderr, "bind failed");
					continue;
				}

				// If we've got this far then we have a socket, and it is bound,
				// so let's store the socket away
				this->listenSocketFileDescriptor = socketFd;
				break;
			}

			freeaddrinfo(candidateAddresses);
			if (currentCandidate == NULL)
			{
				// Shouldn't really get here, but just in case...
				fprintf(stderr, "bind failed completely");
				throw new std::exception();
			}

		}

		// TODO: listen, spawn accept thread, spawn connection threads

		// TODO: stop accept thread, stop connection threads, unbind, close

		Server::~Server()
		{
			if(this->listenSocketFileDescriptor != 0)
			{
				close(listenSocketFileDescriptor);
				this->listenSocketFileDescriptor = 0;
			}
		}
	}
}
