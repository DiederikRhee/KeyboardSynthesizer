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

	//mEnvelopeGen.SetAttack (pPatch->GetParameter (EGVCAAttack));
	//mEnvelopeGen.SetDecay (pPatch->GetParameter (EGVCADecay));
	//mEnvelopeGen.SetSustain (pPatch->GetParameter (EGVCASustain) / 100.0);
	//mEnvelopeGen.SetRelease (pPatch->GetParameter (EGVCARelease));

	//mAmp.SetModulationVolume (pPatch->GetParameter (VCAModulationVolume) / 100.0);
}

CVoice::~CVoice (void)
{
}

void CVoice::NoteOn (u8 ucVelocity)
{
	//mEnvelopeGen.NoteOn (fVelocityLevel);
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

u8 CVoice::GetKeyNumber (void) const
{
	return mEnvelopeGen.GetState () != EnvelopeStateIdle ? m_ucKeyNumber : KEY_NUMBER_NONE;
}

void CVoice::NextSample (void)
{
	mEnvelopeGen.NextSample ();
	mAmp.NextSample ();
}

float CVoice::GetOutputLevel (void) const
{
	return mAmp.GetOutputLevel ();
}
