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
#ifndef __MOCKMESSAGEPROCESSOR_HXX__
#define __MOCKMESSAGEPROCESSOR_HXX__

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

  #include <OcpMessageProcessor.hxx>


  namespace oca_test
  {
	  class MockMessageProcessor : public oca::OcpMessageProcessor
	  {
	  public:
		  typedef boost::shared_ptr<oca::OcpMessageProcessor> pointer;

		  MockMessageProcessor()
		  {
			  gotCorrectValue = false;
              bufferData = 0;
		  }

		  virtual ~MockMessageProcessor()
		  {
              if (bufferData != 0)
              {
                  free(bufferData);
              }
		  }


		  virtual void SyncValueReceived(uint8_t* bufferData, const boost::system::error_code& error, size_t bytesTransferred, boost::function<void(void)> )
		  {
			  if (bufferData == 0)
              {
                  gotCorrectValue = false;
                  return;
              }

              gotCorrectValue = (bufferData[0] == 0x3B);
              this->bufferData = (uint8_t*)malloc(sizeof(uint8_t) * (bytesTransferred * 2));

              memcpy(this->bufferData, bufferData, bytesTransferred);
              this->error = error;


		  }

		  // void Ocp1HeaderReceived(const boost::system::error_code& error, size_t bytesTransferred);
		  // void Ocp1DataReceived(/* header, */const boost::system::error_code& error, size_t bytesTransferred);

		  bool gotCorrectValue;
          uint8_t* bufferData;
          boost::system::error_code error;

	  };
  }

#endif // __MOCKMESSAGEPROCESSOR_HXX__
