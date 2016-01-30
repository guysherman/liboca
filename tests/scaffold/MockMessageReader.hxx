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
#ifndef __MOCKMESSAGEREADER_HXX__
#define __MOCKMESSAGEREADER_HXX__

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

#include <ITcpConnection.hxx>
#include <IOcpSession.hxx>


  namespace oca_test
  {
	  class MockMessageReader : public oca::net::IOcpSession, public boost::enable_shared_from_this<MockMessageReader>
	  {
	  public:

		  MockMessageReader() : tcpConnection(boost::shared_ptr<oca::net::ITcpConnection>())
		  {
			  gotCorrectValue = false;
              dataSize = 0;
              memset(&bufferData[0], 0, 1024);
              memset(&bufferData2[0], 0, 1024);
              memset(&bufferData3[0], 0, 1024);
		  }

		  virtual ~MockMessageReader()
		  {

		  }

          virtual void SetTcpConnection(boost::shared_ptr<oca::net::ITcpConnection> connection)
  		  {
  			  tcpConnection = connection;
  			  tcpConnection->SetOcpSession(shared_from_this());
  		  }


		  virtual void SyncValueReceived(uint8_t* bufferData, const boost::system::error_code& error, size_t bytesTransferred, boost::function<void(void)> getHeader)
		  {
			  if (bufferData == 0)
              {
                  gotCorrectValue = false;
                  return;
              }



              if (!gotCorrectValue)
              {
                gotCorrectValue = (bufferData[0] == 0x3B);
                memcpy(&this->bufferData[0], bufferData, bytesTransferred);
                this->error = error;
                this->bytesTransferred = bytesTransferred;
              }



              // We're second time through, time to bail out for our unit test
              // Might have to think of how we facilitate quitting for other tests

              if (bufferData[0] == 0x3B)
              {
                    getHeader();
              }

		  }

		  virtual void Ocp1HeaderReceived(uint8_t* bufferData, uint64_t identifier, const boost::system::error_code& error, size_t bytesTransferred, boost::function<void(uint32_t)> getData)
          {
              memcpy(&this->bufferData2[0], bufferData, bytesTransferred);
              this->error = error2;
              this->bytesTransferred2 = bytesTransferred;

              getData(2);
          }

		  virtual void Ocp1DataReceived(uint8_t* bufferData, uint64_t identifier, const boost::system::error_code& error, size_t bytesTransferred)
          {
              memcpy(&this->bufferData3[0], bufferData, bytesTransferred);
              this->error = error3;
              this->bytesTransferred3 = bytesTransferred;


          }


          virtual void AddSessionClosedHandler(SessionEventHandler handler)
          {

          }

          virtual int GetId()
          {

          }

		  bool gotCorrectValue;

          uint32_t dataSize;

          uint8_t bufferData[1024];
          uint8_t bufferData2[1024];
          uint8_t bufferData3[1024];

          boost::system::error_code error;
          boost::system::error_code error2;
          boost::system::error_code error3;

          size_t bytesTransferred;
          size_t bytesTransferred2;
          size_t bytesTransferred3;

          boost::shared_ptr<oca::net::ITcpConnection> tcpConnection;

	  };
  }

#endif // __MOCKMESSAGEREADER_HXX__
