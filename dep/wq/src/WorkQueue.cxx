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
#include <deque>

// C Standard Headers


// Boost Headers
#include <boost/function.hpp>

// 3rd Party Headers


// GTK Headers


// Our Headers
#include <wq/ScopedLock.hxx>
#include <wq/WorkQueue.hxx>

namespace oca
{
	namespace util
	{
		WorkQueue::WorkQueue()
		{
			memset(&worker, 0, sizeof(pthread_t));
			memset(&mutex, 0, sizeof(pthread_mutex_t));

			pthread_mutex_init(&mutex, NULL);

			shouldContinueRunning = true;
			isRunning = false;
		}

		WorkQueue::~WorkQueue()
		{
			Stop();
			pthread_mutex_destroy(&mutex);
		}

		void WorkQueue::Start()
		{
			if (!isRunning)
			{
				ScopedLock lock(&mutex);
				if (!isRunning)
				{
					pthread_create(&worker, NULL, &WorkQueue::run, this);
				}
			}

		}

		void* WorkQueue::run(void* arg)
		{
			WorkQueue* instance = static_cast<WorkQueue*>(arg);
			instance->runInstance();

			return NULL;
		}

		void WorkQueue::runInstance()
		{
			isRunning = true;
			while (shouldContinueRunning)
			{
				WorkItem item = NULL;

				if (deque.begin() != deque.end())
				{
					ScopedLock lock(&mutex);
					if (deque.begin() != deque.end())
					{
						item = deque.front();
					}

				}

				if (item != NULL)
				{
					item();
					{
						ScopedLock lock(&mutex);
						deque.pop_front();
					}
				}
			}
			isRunning = false;
		}

		void WorkQueue::Stop()
		{
			if (shouldContinueRunning)
			{
				ScopedLock lock(&mutex);
				if (shouldContinueRunning)
				{
					shouldContinueRunning = false;
				}

			}
			pthread_join(worker, NULL);

		}

		void WorkQueue::QueueWork(WorkItem workItem)
		{
			if (shouldContinueRunning)
			{
				ScopedLock lock(&mutex);
				if (shouldContinueRunning)
				{
					deque.push_back(workItem);
				}
			}
		}


	}
}
