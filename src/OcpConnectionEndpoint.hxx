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
#include <deque>

// C Standard Headers
#include <sys/socket.h>
#include <pthread.h>

// Boost Headers
#include <boost/shared_ptr.hpp>

// 3rd Party Headers


// Our Headers
#include <oca/OcaTypes.hxx>
#include <wq/WorkQueue.hxx>

namespace oca
{
	namespace ocp
	{
		struct Ocp1Header;

		struct Message
		{
			boost::shared_ptr<const uint8_t> buffer;
			size_t bufferLength;

			Message() : bufferLength(0) {}
		};

		class ConnectionEndpoint
		{
		public:
			explicit ConnectionEndpoint(int socketFileDescriptor);
			virtual ~ConnectionEndpoint();

			void StartSupervision(OcaUint16 hearbeatTime);

		private:
			static void* receiveWrapper(void* arg);
			static void* sendWrapper(void* arg);
			void* receiveLoop(void* arg);
			void* sendLoop(void* arg);
			void processKeepAlive(Ocp1Header& header);
			static void* supervisorWrapper(void* arg);
			void* supervisorLoop(void* arg);
			void queueMessageForSend(Message message);
			void sendHeartbeat();


			int socketFileDescriptor;
			pthread_t receiveThread;
			pthread_t sendThread;
			bool shouldContinue;
			OcaUint16 heartbeatTime;
			bool supervisorActivated;
			pthread_t supervisorThread;
			time_t lastMessageSentAt;
			time_t lastMessageReceivedAt;
			pthread_mutex_t mutex;
			std::deque< Message > messageQueue;
		};
	}
}
