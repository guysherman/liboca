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
  #ifndef __OCPMESSAGEPROCESSOR_HXX__
  #define __OCPMESSAGEPROCESSOR_HXX__

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
    #include "Ocp1Header.hxx"

	namespace oca
	{
		class OcpMessageProcessor : public boost::enable_shared_from_this<OcpMessageProcessor>
		{
		public:
			typedef boost::shared_ptr<OcpMessageProcessor> pointer;

            OcpMessageProcessor();
            virtual ~OcpMessageProcessor();


            virtual void SyncValueReceived(uint8_t* bufferData, const boost::system::error_code& error, size_t bytesTransferred, boost::function<void(void)> getHeader);

			virtual void Ocp1HeaderReceived(uint8_t* bufferData, const boost::system::error_code& error, size_t bytesTransferred, boost::function<void(uint32_t)> getData);

            virtual void Ocp1DataReceived(uint8_t* bufferData, const boost::system::error_code& error, size_t bytesTransferred);

        private:
            // TODO: it goes against my better judgement to have this here.
            // Since there is currently only one OcpMessageProcessor per OcaNetwork
            // this is actually a bug if there is more than one TcpConnection, ie
            // this class was supposed to be stateless, but I don't really want the
            // TcpConnection to be concerned with the details of the Ocp1Header and I need
            // to keep the header around between Ocp1HeaderReceived and Ocp1DataReceived
            // The solution could be to make a factory for this as well, so that we can
            // have one per connection, without tightly coupling the connection and this
            // class; or, I could make this a map of <TcpConnection*, Ocp1Header> so that the state
            // is stored per connection. I like the latter more. The third option is to pass a callback
            // when calling the callback, but that just feels like the code would be hard to read.
            oca::net::Ocp1Header header;
		};
	}

  #endif // __OCPMESSAGEPROCESSOR_HXX__
