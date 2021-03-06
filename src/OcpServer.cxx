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
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>

// Boost Headers


// 3rd Party Headers


// Our Headers
#include <oca/OcpServer.hxx>
#include "OcpConnectionEndpoint.hxx"

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

namespace oca
{
	namespace ocp
	{
		Server::Server(const char* listenAddress, const char* port) :
			listenSocketFileDescriptor(0),
			continueAccepting(true)
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
				throw std::exception();
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
					fprintf(stderr, "setsockopt failed.\n");
					// TODO: use our exception here so we get useful info
					freeaddrinfo(candidateAddresses);
					throw std::exception();
				}

				status = bind(socketFd, currentCandidate->ai_addr, currentCandidate->ai_addrlen);
				if (status == -1)
				{
					close(socketFd);
					fprintf(stderr, "bind failed.\n");
					continue;
				}

				status = listen(socketFd, 10);  // TODO: magic number #smell
				if (status == -1)
				{
					close(socketFd);
					fprintf(stderr, "listen failed.\n");
					throw std::exception();
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
				fprintf(stderr, "bind failed completely.\n");
				throw std::exception();
			}

			pthread_create(&this->acceptThread, NULL, &Server::acceptWrapper, (void* )this);

		}

		// TODO: listen, spawn accept thread, spawn connection threads
		void* Server::acceptWrapper(void *arg)
		{
			if (arg == NULL)
			{
				// TODO: return proper exit codes with pthread_exit
				return NULL;
			}

			Server* me = static_cast<Server*>(arg);
			me->acceptLoop(NULL);

			// TODO: proper return values #correctness
			return NULL;
		}

		void* Server::acceptLoop(void *arg)
		{


			while(continueAccepting)
			{
				struct sockaddr_storage clientAddress; // connector's address information
	    		socklen_t clientAddressSize = sizeof clientAddress;
				char s[INET6_ADDRSTRLEN];
				int newSocketFileDescriptor = accept(this->listenSocketFileDescriptor,
												(struct sockaddr*)&clientAddress,
												&clientAddressSize);
				if (newSocketFileDescriptor == -1)
				{
					fprintf(stderr, "accept failed with errno=%d {%d, %d}.\n", errno, this->listenSocketFileDescriptor, clientAddressSize);
					pthread_yield();
					continue;
				}

				inet_ntop(clientAddress.ss_family,
            				get_in_addr((struct sockaddr *)&clientAddress),
            				s, sizeof s);
        		printf("server: got connection from %s\n", s);
				boost::shared_ptr<ConnectionEndpoint> endpoint(new ConnectionEndpoint(newSocketFileDescriptor));
				this->endpoints.push_back(endpoint);

				pthread_yield();
			}

			return NULL;
		}

		// TODO: stop accept thread, stop connection threads, unbind, close

		Server::~Server()
		{
			this->continueAccepting = false;
			pthread_join(acceptThread, NULL);
			if(this->listenSocketFileDescriptor != 0)
			{
				close(listenSocketFileDescriptor);
				this->listenSocketFileDescriptor = 0;
			}
		}
	}
}
