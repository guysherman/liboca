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


// 3rd Party Headers
#include <gtest/gtest.h>

// GTK Headers


#include <oca/OcaNetwork.hxx>

static void* worker(void* arg)
{
	if (arg == 0)
	{
		return NULL;
	}
	oca::OcaNetwork* net = reinterpret_cast<oca::OcaNetwork*>(arg);

	net->Start();

	return NULL;
}




TEST(Suite_OcaNetwork, StartStop)
{
	oca::OcaNetwork network(60000);

	pthread_t t;

	pthread_create(&t, NULL, &worker, &network);
	usleep(1000000);

	network.Stop();
	pthread_join(t, NULL);

	//EXPECT_EQ(1, network.Dummy());
}
