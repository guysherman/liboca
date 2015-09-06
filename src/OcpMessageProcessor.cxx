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


// Boost Headers


// 3rd Party Headers


// GTK Headers

#include "OcpMessageProcessor.hxx"

namespace oca
{
	OcpMessageProcessor::OcpMessageProcessor()
	{

	}

	OcpMessageProcessor::~OcpMessageProcessor()
	{

	}


	void OcpMessageProcessor::SyncValueReceived(uint8_t* bufferData,
		const boost::system::error_code& error,
		size_t bytesTransferred,
		boost::function<void(void)> getHeader )
	{
		if (error.value() != boost::system::errc::success)
		{
			// TODO: consider an exception here
			return;
		}


		if (bytesTransferred != 1)
		{
			// TODO: consider an exception here
			return;
		}


		// TODO: logging


		if (bufferData != 0)
		{
			if (bufferData[0] == 0x3B)
			{
				getHeader();
			}
		}
	}

}
