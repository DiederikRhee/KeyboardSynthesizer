//
// voicemanager.h
//
// Manages the polyphonic voices and available CPU cores
//
// MiniSynth Pi - A virtual analogue synthesizer for Raspberry Pi
// Copyright (C) 2017-2020  R. Stange <rsta2@o2online.de>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
#ifndef _voicemanager_h
#define _voicemanager_h

#include <circle/multicore.h>
#include <circle/memory.h>
#include <circle/types.h>
#include "patch.h"
#include "voice.h"
#include "reverbmodule.h"
#include "config.h"

#define VOICES		61
#define NOTE_KEY_OFFSET 32

class CVoiceManager
{
public:
	CVoiceManager (CMemorySystem *pMemorySystem);
	~CVoiceManager (void);

	boolean Initialize (void);

	void NoteOn (u8 ucKeyNumber, u8 ucVelocity);	// MIDI key number and velocity
	void NoteOff (u8 ucKeyNumber);

	float Sample(void);

private:
	int MidiKeyToSampleKey(u8 ucKeyNumber);

private:
	CVoice *m_pVoice[VOICES];
};

#endif
