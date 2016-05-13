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
#include <stdint.h>

// Boost Headers


// 3rd Party Headers


// Our Headers
#include "OcpConnectionEndpoint.hxx"
#include "Ocp1Header.hxx"
#include "OcpMessageReader.hxx"

namespace oca
{
	namespace ocp
	{
		ConnectionEndpoint::ConnectionEndpoint(int socketFileDescriptor) :
			socketFileDescriptor(socketFileDescriptor),
			shouldContinue(false),
			heartbeatTime(0),
			supervisorActivated(false),
			lastMessageSentAt(0),
			lastMessageReceivedAt(0)

		{
			shouldContinue = true;
			pthread_create(&this->receiveThread, NULL, &ConnectionEndpoint::receiveWrapper, (void*)this);
			//pthread_detach(this->receiveThread);

			pthread_create(&this->sendThread, NULL, &ConnectionEndpoint::sendWrapper, (void*)this);
			//pthread_detach(this->sendThread);
		}

		void* ConnectionEndpoint::receiveWrapper(void *arg)
		{
			if (arg == NULL)
			{
				// TODO: return proper exit codes with pthread_exit #correctness
				return NULL;
			}

			ConnectionEndpoint* me = static_cast<ConnectionEndpoint*>(arg);
			if (me->socketFileDescriptor == 0)
			{
				// TODO: return proper exit codes with pthread_exit #correctness
				return NULL;
			}

			me->receiveLoop(NULL);

			// TODO: return proper exit codes with pthread_exit #correctness
			return NULL;
		}

		void* ConnectionEndpoint::receiveLoop(void *arg)
		{
			while(shouldContinue)
			{
				uint8_t syncValue = (uint8_t)0;
				int bytesReceived = recv(this->socketFileDescriptor, &syncValue, sizeof(uint8_t), 0);
				if (bytesReceived == 0)
				{
					// TODO: return values #correctness
					return NULL;
				}

				// TODO: move the magic number to a defs.h or something #smell
				if (syncValue != 0x3B)
				{
					// TODO: return values #correctness
					return NULL;
				}

				uint8_t headerBuffer[9];
				memset(&headerBuffer, 0, 9);
				bytesReceived = recv(this->socketFileDescriptor, &headerBuffer[0], 9 * sizeof(uint8_t), 0);
				if (bytesReceived == 0)
				{
					// TODO: return values #correctness
					return NULL;
				}

				Ocp1Header header;
				OcpMessageReader::HeaderFromBuffer(&headerBuffer[0], header);

				this->lastMessageReceivedAt = time(NULL);

				switch (header.messageType) {
					case oca::ocp::OcaKeepAlive:
						processKeepAlive(header);
						break;
					case oca::ocp::OcaCmd:
					case oca::ocp::OcaCmdRrq:
					case oca::ocp::OcaNtf:
					case oca::ocp::OcaRsp:
					default:
						break;

				}



				pthread_yield();
			}

			return NULL;
		}

		void ConnectionEndpoint::processKeepAlive(oca::ocp::Ocp1Header &header)
		{
			uint8_t buffer[2];
			memset(&buffer, 0, 2);
			fprintf(stderr, "Received KeepAlive\n");
			int bytesReceived = recv(this->socketFileDescriptor, &buffer[0], sizeof(OcaUint16), 0);
			if (bytesReceived == 2)
			{
				this->heartbeatTime = ntohs(* reinterpret_cast<uint16_t*>(&buffer[0]));
				if (!this->supervisorActivated)
				{
					this->supervisorActivated = false;
					pthread_create(&this->supervisorThread, NULL, &ConnectionEndpoint::supervisorWrapper, (void*)this);
				}

			}

		}

		void* ConnectionEndpoint::supervisorWrapper(void* arg)
		{
			if (arg == NULL)
			{
				// TODO: return proper exit codes with pthread_exit #correctness
				return NULL;
			}

			ConnectionEndpoint* me = static_cast<ConnectionEndpoint*>(arg);
			me->supervisorLoop(NULL);

			// TODO: return proper exit codes with pthread_exit #correctness
			return NULL;
		}

		void* ConnectionEndpoint::supervisorLoop(void* arg)
		{
			time_t heartbeatPeriod = (time_t) this->heartbeatTime;
			time_t timeoutPeriod = (time_t)(this->heartbeatTime * 3);

			while(this->shouldContinue)
			{
				time_t now = time(NULL);
				time_t receiveDiff = now - this->lastMessageReceivedAt;
				time_t sendDiff = now - this->lastMessageSentAt;

				if (sendDiff >= heartbeatPeriod)
				{
					// TODO: We seem to  be idle, send a message so the other
					// side knows we're still here
				}

				if (receiveDiff >= timeoutPeriod)
				{
					this->shouldContinue = false;
					fprintf(stderr, "Idled for too long, terminating.\n");
					// TODO: tell somebody that we've stopped
				}
			}

			return NULL;
		}

		void* ConnectionEndpoint::sendWrapper(void *arg)
		{
			if (arg == NULL)
			{
				// TODO: return proper exit codes with pthread_exit
				return NULL;
			}

			ConnectionEndpoint* me = static_cast<ConnectionEndpoint*>(arg);
			if (me->socketFileDescriptor == 0)
			{
				// TODO: return proper exit codes with pthread_exit #correctness
				return NULL;
			}

			me->sendLoop(NULL);

			// TODO: return proper exit codes with pthread_exit
			return NULL;
		}

		void* ConnectionEndpoint::sendLoop(void *arg)
		{
			while(shouldContinue)
			{
				pthread_yield();
			}

			return NULL;
		}

		ConnectionEndpoint::~ConnectionEndpoint()
		{
			shouldContinue = false;
			pthread_join(this->receiveThread, NULL);
			pthread_join(this->sendThread, NULL);
			socketFileDescriptor = 0;
		}
	}
}
