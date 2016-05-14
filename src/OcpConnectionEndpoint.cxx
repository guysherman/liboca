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
#include <wq/ScopedLock.hxx>
#include "OcpConnectionEndpoint.hxx"
#include "Ocp1Header.hxx"
#include "OcpMessageReader.hxx"
#include "OcpMessageWriter.hxx"
#include "OcaBasicTypeWriter.hxx"

namespace oca
{
	namespace ocp
	{
		ConnectionEndpoint::ConnectionEndpoint(int socketFileDescriptor) :
			socketFileDescriptor(socketFileDescriptor),
			shouldContinue(false),
			heartbeatTime(2),
			supervisorActivated(false),
			lastMessageSentAt(0),
			lastMessageReceivedAt(0)

		{
			shouldContinue = true;
			lastMessageReceivedAt = time(NULL);
			lastMessageSentAt = time(NULL);
			memset(&mutex, 0, sizeof(pthread_mutex_t));
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
			fprintf(stderr, "Received KeepAlive.\n");
			int bytesReceived = recv(this->socketFileDescriptor, &buffer[0], sizeof(OcaUint16), 0);
			if (bytesReceived == 2)
			{
				this->heartbeatTime = ntohs(* reinterpret_cast<uint16_t*>(&buffer[0]));
				if (!this->supervisorActivated)
				{
					fprintf(stderr, "Activating supervisor thread.\n");
					this->supervisorActivated = true;
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
					sendHeartbeat();

				}

				if (receiveDiff >= timeoutPeriod)
				{
					this->shouldContinue = false;
					fprintf(stderr, "Idled for too long, terminating.\n");
					// TODO: tell somebody that we've stopped
				}

				sleep(this->heartbeatTime / 2);
			}

			return NULL;
		}

		void ConnectionEndpoint::sendHeartbeat()
		{
			Ocp1Header h;
			h.messageCount = 1;
			h.messageSize = 11;
			h.messageType = ocp::OcaKeepAlive;
			h.protocolVersion = 0x0103;

			uint8_t* buffer = reinterpret_cast<uint8_t*>(malloc(12 * sizeof(uint8_t)));
			buffer[0] = 0x3B;
			OcpMessageWriter::WriteHeaderToBuffer(h, &buffer[1]);

			uint8_t* temp = &buffer[10];
			OcaBasicTypeWriter::WriteUint16ToBuffer(this->heartbeatTime, &temp);

			boost::shared_ptr<const uint8_t> bufferOwner(buffer);

			Message m;
			m.buffer = bufferOwner;
			m.bufferLength = 12;

			queueMessageForSend(m);
			fprintf(stderr, "Queued heartbeat.\n");
		}

		void ConnectionEndpoint::queueMessageForSend(oca::ocp::Message message)
		{
			if (shouldContinue)
			{
				util::ScopedLock(&this->mutex);
				if (shouldContinue)
				{
					this->messageQueue.push_back(message);
				}

			}

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
				Message m;
				if (this->messageQueue.begin() != this->messageQueue.end())
				{
					util::ScopedLock lock(&this->mutex);
					if (this->messageQueue.begin() != this->messageQueue.end())
					{
						m = this->messageQueue.front();
					}
				}

				if (m.bufferLength > 0 && m.buffer != NULL)
				{
					int bytesSent = 0;
					bytesSent = send(this->socketFileDescriptor, m.buffer.get(), m.bufferLength, 0);
					if (bytesSent != m.bufferLength)
					{
						fprintf(stderr, "Didn't send whole buffer, only sent %d bytes of %d.\n", bytesSent, static_cast<int>(m.bufferLength));
					}
					{
						util::ScopedLock(&this->mutex);
						this->messageQueue.pop_front();
					}
					this->lastMessageSentAt = time(NULL);
				}

				pthread_yield();
			}

			return NULL;
		}

		void ConnectionEndpoint::StartSupervision(OcaUint16 heartbeatTime)
		{
			this->heartbeatTime = heartbeatTime;
			sendHeartbeat();
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
