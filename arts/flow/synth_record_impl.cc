    /*

    Copyright (C) 2000 Stefan Westerfeld
                       stefan@space.twc.de

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.
  
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.
   
    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.

    */

#include "artsflow.h"
#include "debug.h"
#include "convert.h"
#include "objectmanager.h"
#include "audiosubsys.h"
#include "dispatcher.h"
#include "iomanager.h"
#include "flowsystem.h"
#include "stdsynthmodule.h"
#include <stdio.h>
#include <iostream>

using namespace std;
using namespace Arts;

class Synth_RECORD_impl :	virtual public Synth_RECORD_skel,
						virtual public ASConsumer,
						virtual public StdSynthModule
{
protected:
	AudioSubSystem *as;
	bool haveSubSys;

	typedef unsigned char uchar;

	unsigned char *inblock;
	unsigned long maxsamples;
	unsigned long channels;

public:
	/*
	 * functions from the SynthModule interface (which is inherited by
	 * SynthPlay)
	 */
	void streamInit() {
		as = AudioSubSystem::the();

		//cout << "Synth_RECORD: streamInit() called." << endl;
		channels = as->channels();
		maxsamples = 0;
		inblock = 0;

		haveSubSys = as->attachConsumer(this);
		if(!haveSubSys)
		{
			printf("Synth_RECORD: audio subsystem is already used\n");
			return;
		}
	}

	void streamEnd() {
		artsdebug("Synth_RECORD: detaching\n");
		if(haveSubSys) as->detachConsumer();

		if(inblock)
		{
			delete[] inblock;
			inblock = 0;
		}
	}

	void calculateBlock(unsigned long samples)
	{
		// no audio subsystem, no play
		if(!as->running() || !haveSubSys) return;

		if(samples > maxsamples)
		{
			maxsamples = samples;

			if(inblock) delete[] inblock;
			inblock = new uchar[maxsamples * 4]; // 2 channels, 16 bit
		}

		assert(channels);

		as->read(inblock,channels * 2 * samples);

		if(channels == 1)
			convert_mono_16le_float(samples,inblock,left);

		if(channels == 2)
			convert_stereo_i16le_2float(samples,inblock,left,right);
	}

	/**
	 * havemore from the ASConsumer interface (AudioSubSystem)
	 */
	void haveMore()
	{
		_node()->requireFlow();
	}
};

REGISTER_IMPLEMENTATION(Synth_RECORD_impl);
