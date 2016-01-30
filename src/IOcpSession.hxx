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
#ifndef __IOCPSESSION_HXX__
#define __IOCPSESSION_HXX__

// C++ Standard Headers


// C Standard Headers


// Boost Headers
#include <boost/system/error_code.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

// 3rd Party Headers


// GTK Headers


// Our Headers
#include "Ocp1Header.hxx"


namespace oca
{
	namespace net
	{
		class ITcpConnection;

		class IOcpSession
		{
		public:
			typedef boost::shared_ptr<IOcpSession> pointer;
			typedef boost::function< void(boost::shared_ptr<IOcpSession>) > SessionEventHandler;

			virtual void SetTcpConnection(boost::shared_ptr<ITcpConnection> connection) = 0;

			virtual void SyncValueReceived(uint8_t* bufferData, const boost::system::error_code& error, size_t bytesTransferred, boost::function<void(void)> getHeader) = 0;
			virtual void Ocp1HeaderReceived(uint8_t* bufferData, uint64_t connectionIdentifier, const boost::system::error_code& error, size_t bytesTransferred, boost::function<void(uint32_t)> getData) = 0;
            virtual void Ocp1DataReceived(uint8_t* bufferData, uint64_t connectionIdentifier, const boost::system::error_code& error, size_t bytesTransferred) = 0;

			virtual void AddSessionClosedHandler(SessionEventHandler handler) = 0;
			virtual int GetId() = 0;
		};
	}
}


#endif // __IOCPSESSION_HXX__
