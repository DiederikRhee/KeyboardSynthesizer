//
// voice.cpp
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
#include "voice.h"
#include <assert.h>


CVoice::CVoice (u8 aSampleKeyNumber)
:	CKeySampleFatFsFile(aSampleKeyNumber)
{
	mEnvelopeGen.SetAttack (100);
	mEnvelopeGen.SetDecay (400);
	mEnvelopeGen.SetSustain (1);
	mEnvelopeGen.SetRelease (100);
}

CVoice::~CVoice (void)
{
}

void CVoice::NoteOn (u8 ucVelocity)
{
	float fVelocityLevel = ucVelocity / 127.0;
	mEnvelopeGen.NoteOn (fVelocityLevel);
}

void CVoice::NoteOff (void)
{
	mEnvelopeGen.NoteOff ();
}

TVoiceState CVoice::GetState (void) const
{
	switch (mEnvelopeGen.GetState ())
	{
	case EnvelopeStateIdle:
		return VoiceStateIdle;

	case EnvelopeStateAttack:
	case EnvelopeStateDecay:
	case EnvelopeStateSustain:
		return VoiceStateActive;

	case EnvelopeStateRelease:
		return VoiceStateRelease;

	default:
		assert (0);
		return VoiceStateActive;
	}
}


float CVoice::Sample (void)
{
	mEnvelopeGen.NextSample();

	float sampleLevel = 0;
	return mEnvelopeGen.GetOutputLevel() * sampleLevel;
}
