    /*

    Copyright (C) 2001 Jeff Tranter
                       tranter@kde.org

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

    Permission is also granted to link this program with the Qt
    library, treating Qt like a library that normally accompanies the
    operating system kernel, whether or not that is in fact the case.

    */

#ifndef SOUNDSERVERV2_IMPL_H
#define SOUNDSERVERV2_IMPL_H

#include "soundserver.h"
#include "simplesoundserver_impl.h"
#include "soundserver_impl.h"
#include "soundserver.h"

namespace Arts {
	class SoundServerV2_impl : virtual public SoundServerV2_skel,
							   public SoundServer_impl
	{
		long autoSuspendSeconds();
		void autoSuspendSeconds(long newValue);
		string audioMethod();
		long samplingRate();
		long channels();
		long bits();
		bool fullDuplex();
		string audioDevice();
		long fragments();
		long fragmentSize();
		long secondsUntilSuspend();
		void notifyTime();
		long _autoSuspendSeconds;
		
	public:
		SoundServerV2_impl();
	};
};

#endif /* SOUNDSERVERV2_IMPL_H */
