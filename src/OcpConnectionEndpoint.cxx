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


// Our Headers
#include "OcpConnectionEndpoint.hxx"

namespace oca
{
	namespace ocp
	{
		ConnectionEndpoint::ConnectionEndpoint(int socketFileDescriptor) :
			socketFileDescriptor(socketFileDescriptor),
			shouldContinue(false)

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
				
				pthread_yield();
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
