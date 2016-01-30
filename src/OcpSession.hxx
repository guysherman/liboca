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
#ifndef __OCPSESSION_HXX__
#define __OCPSESSION_HXX__

// C++ Standard Headers
#include <vector>

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
#include "IOcpSession.hxx"

namespace oca
{
	namespace net
	{
		class ITcpConnection;

		class OcpSession : public IOcpSession, public boost::enable_shared_from_this<OcpSession>
		{
		public:


			OcpSession();
			virtual ~OcpSession();

			virtual void SetTcpConnection(boost::shared_ptr<ITcpConnection> connection);

			virtual void SyncValueReceived(uint8_t* bufferData, const boost::system::error_code& error, size_t bytesTransferred, boost::function<void(void)> getHeader);
			virtual void Ocp1HeaderReceived(uint8_t* bufferData, uint64_t connectionIdentifier, const boost::system::error_code& error, size_t bytesTransferred, boost::function<void(uint32_t)> getData);
            virtual void Ocp1DataReceived(uint8_t* bufferData, uint64_t connectionIdentifier, const boost::system::error_code& error, size_t bytesTransferred);

			void AddSessionClosedHandler(SessionEventHandler handler);
			int GetId();

		private:



			OcpSession(const OcpSession& rhs);
			OcpSession& operator=(const OcpSession& rhs);

			void sessionClosed();

			boost::shared_ptr<oca::net::Ocp1Header> stashedHeader;

			std::vector<IOcpSession::SessionEventHandler> sessionClosedHandlers;
			static int nextId;
			int id;

		protected:
			boost::shared_ptr<ITcpConnection> tcpConnection;


		};
	}
}


#endif // __OCPSESSION_HXX__
