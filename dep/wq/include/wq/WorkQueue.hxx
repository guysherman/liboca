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
#ifndef __WORKQUEUE_HXX__
#define __WORKQUEUE_HXX__

// C++ Standard Headers
#include <deque>

// C Standard Headers
#include <pthread.h>

// Boost Headers
#include <boost/function.hpp>

// 3rd Party Headers


// GTK Headers


// Our Headers

namespace oca
{
	namespace util
	{
		typedef boost::function<void(void)> WorkItem;

		class WorkQueue
		{
		public:
			WorkQueue();
			virtual ~WorkQueue();

			void Start();
			void Stop();
			void QueueWork(WorkItem workItem);


		private:

			typedef std::deque<WorkItem> WorkList;

			WorkQueue(const WorkQueue& rhs);
			WorkQueue& operator=(const WorkQueue& rhs);

			static void* run(void* arg);
			void runInstance();

			bool shouldContinueRunning;
			bool isRunning;

			WorkList deque;
			pthread_t worker;
			pthread_mutex_t mutex;



		};
	}
}
#endif // __WORKQUEUE_HXX__
