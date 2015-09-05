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


	namespace oca
	{
		class OcpMessageProcessor : public boost::enable_shared_from_this<OcpMessageProcessor>
		{
		public:
			typedef boost::shared_ptr<OcpMessageProcessor> pointer;

            OcpMessageProcessor();
            virtual ~OcpMessageProcessor();


            virtual void SyncValueReceived(uint8_t* bufferData, const boost::system::error_code& error, size_t bytesTransferred, boost::function<void(void)> getHeader );

			// void Ocp1HeaderReceived(const boost::system::error_code& error, size_t bytesTransferred);
			// void Ocp1DataReceived(/* header, */const boost::system::error_code& error, size_t bytesTransferred);
		};
	}

  #endif // __OCPMESSAGEPROCESSOR_HXX__
