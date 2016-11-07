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


// C Standard Headers
#include <pthread.h>

// Boost Headers
#include <boost/function.hpp>
#include <boost/bind.hpp>

// 3rd Party Headers
#include <gtest/gtest.h>

// GTK Headers


// Our Headers
#include <wq/WorkQueue.hxx>


int counter = 0;

void increment()
{
	counter++;
}



TEST(WorkQueue, BasicRun)
{
	oca::util::WorkQueue wq;

	oca::util::WorkItem f(&increment);
	wq.Start();
	wq.QueueWork(f);
	wq.QueueWork(f);
	wq.QueueWork(f);

	usleep(5000000);

	wq.Stop();

	wq.QueueWork(f);
	wq.QueueWork(f);
	wq.QueueWork(f);

	EXPECT_EQ(3, counter);

}
