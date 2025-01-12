//
// voice.h
//
// One voice in a polyphonic choir
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
#ifndef _voice_h
#define _voice_h

#include "envelopegenerator.h"
#include "keysamplefatfsfile.h"

#include <circle/types.h>

enum TVoiceState
{
	VoiceStateIdle,
	VoiceStateActive,
	VoiceStateRelease,
	VoiceStateUnknown
};

class CVoice : public CKeySampleFatFsFile
{
public:
	CVoice (u8 aSampleKeyNumber);
	~CVoice (void);

	void NoteOn (u8 ucVelocity);	// MIDI key number and velocity
	void NoteOff (void);

	TVoiceState GetState (void) const;
	u8 GetSampleKeyNumber (void) const;			// returns KEY_NUMBER_NONE if voice is unused
	void NextSample (void);
	float GetOutputLevel (void) const;

private:
	CEnvelopeGenerator mEnvelopeGen;
};

#endif
