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
#ifndef __OCANETWORK_HXX__
#define __OCANETWORK_HXX__

// C++ Standard Headers


// C Standard Headers
#include <stdint.h>

// Boost Headers
#include <boost/shared_ptr.hpp>

// 3rd Party Headers


// GTK Headers

namespace oca
{
    namespace net
    {
        class TcpServer;
    }

    class OcaNetwork
    {
    public:
        OcaNetwork(uint16_t port);
        void Start();
        void Stop();
        virtual ~OcaNetwork();


    private:
        OcaNetwork(const OcaNetwork& rhs);
        OcaNetwork& operator=(const OcaNetwork& rhs);

        boost::shared_ptr<oca::net::TcpServer> tcpServer;
        bool isRunning;
    };
}



#endif // __OCANETWORK_HXX__
