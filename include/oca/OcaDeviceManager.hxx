/*
  Copyright (C) 2016 Guy Sherman

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


// Our Headers
#include "OcaTypes.hxx"
#include "OcaManager.hxx"


namespace oca
{
	namespace net
	{
		class Ocp1Parameters;
	}

	namespace managers
	{
		class OcaDeviceManager : public OcaManager
		{
		public:
			OcaDeviceManager();
			virtual ~OcaDeviceManager();
			OcaDeviceManager(OcaDeviceManager& rhs);

			virtual OcaClassID& GetClassId();
			virtual OcaClassVersionNumber GetClassVersion();

			OcaString GetDeviceName();
			void SetDeviceName(OcaString deviceName);
		protected:
			OcaString DeviceName;
		private:
			OcaClassIdentification classIdentification;


			// Binders
		public:
			static OcaStatus bind_GetDeviceName(void *instance, OcaUint32 handle, net::Ocp1Parameters *parameters);
		};
	}
}
