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
#include <cstring>

// C Standard Headers


// Boost Headers


// 3rd Party Headers


// Our Headers
#include <oca/OcaDeviceManager.hxx>

namespace oca
{
	namespace managers
	{

		OcaDeviceManager::OcaDeviceManager() : DeviceName("")
		{
			classIdentification.ClassVersion = 1;
			classIdentification.ClassID.FieldCount = 3;
			classIdentification.ClassID.Fields = new OcaClassIDField[3];
			classIdentification.ClassID.Fields[0] = 1;
			classIdentification.ClassID.Fields[1] = 3;
			classIdentification.ClassID.Fields[2] = 1;
		}

		OcaDeviceManager::~OcaDeviceManager()
		{

		}

		OcaDeviceManager::OcaDeviceManager(OcaDeviceManager& rhs)
		{
			classIdentification.ClassVersion = 1;
			classIdentification.ClassID.FieldCount = 3;
			classIdentification.ClassID.Fields = new OcaClassIDField[3];
			classIdentification.ClassID.Fields[0] = 1;
			classIdentification.ClassID.Fields[1] = 3;
			classIdentification.ClassID.Fields[2] = 1;
			DeviceName = rhs.DeviceName;
		}

		OcaClassID& OcaDeviceManager::GetClassId()
		{
			return classIdentification.ClassID;
		}

		OcaClassVersionNumber OcaDeviceManager::GetClassVersion()
		{
			return classIdentification.ClassVersion;
		}

		OcaString OcaDeviceManager::GetDeviceName()
		{
			return DeviceName;
		}

		void OcaDeviceManager::SetDeviceName(OcaString deviceName)
		{
			DeviceName = deviceName;
		}


	}
}
