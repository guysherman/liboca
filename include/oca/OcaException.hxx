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
#ifndef __OCAEXCEPTION_HXX__
#define __OCAEXCEPTION_HXX__

// C++ Standard Headers
#include <exception>

// C Standard Headers
#include <cstring>


// Boost Headers


// 3rd Party Headers


// GTK Headers

namespace oca
{
	class Exception : public std::exception
	{
	public:
		Exception(const char* excFunc, int excLine, const char* excFile, const char* excCustom)
		{
			// This is slow, and it makes my exception take over 4k... BUT:
			// by copying only a known, maximum number of chars into each
			// buffer, it prevents abuse by malicious users.
			memset(&func[0], 0, 1024);
			memset(&file[0], 0, 1024);
			memset(&custom[0], 0, 1024);

			line = excLine;

			// we copy in one less byte, so the buffer always ends up as a null-terminated
			// string, no matter what.
			strncpy(&func[0], excFunc, 1023);
			strncpy(&file[0], excFile, 1023);
			strncpy(&custom[0], excCustom, 1023);
		}

		const char* GetFunction() const
		{
			return &func[0];
		}

		const int GetLine() const
		{
			return line;
		}

		const char* GetFilename() const
		{
			return &file[0];
		}

		const char* GetCustom() const
		{
			return &custom[0];
		}

	private:
		char func[1024];
		int line;
		char file[1024];
		char custom[1024];

	};
}



#endif // __OCAEXCEPTION_HXX__
