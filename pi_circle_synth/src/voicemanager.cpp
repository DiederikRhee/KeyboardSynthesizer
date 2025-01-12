//
// voicemanager.cpp
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
#include "voicemanager.h"
#include <assert.h>

CVoiceManager::CVoiceManager (CMemorySystem *pMemorySystem)
{
	for (unsigned i = 0; i < VOICES; i++)
	{
		m_pVoice[i] = new CVoice(i);
		assert (m_pVoice[i] != 0);
	}
}

CVoiceManager::~CVoiceManager (void)
{
	for (unsigned i = 0; i < VOICES; i++)
	{
		delete m_pVoice[i];
		m_pVoice[i] = 0;
	}
}

boolean CVoiceManager::Initialize (void)
{
	return TRUE;
}

void CVoiceManager::NoteOn (u8 ucKeyNumber, u8 ucVelocity)
{
	int sampleKey = MidiKeyToSampleKey(ucKeyNumber);
	if (sampleKey < 0) return;

	m_pVoice[sampleKey]->NoteOn (ucVelocity);
}

void CVoiceManager::NoteOff (u8 ucKeyNumber)
{
	int sampleKey = MidiKeyToSampleKey(ucKeyNumber);
	if (sampleKey < 0) return;

	m_pVoice[sampleKey]->NoteOff ();
}


float CVoiceManager::Sample(void)
{
	float fLevel = 0.0;
	for (unsigned i = 0; i <= VOICES; i++)
	{
		if (m_pVoice[i]->GetState () != VoiceStateIdle)
		{
			fLevel += m_pVoice[i]->Sample();
		}
	}

	return fLevel;
}


int CVoiceManager::MidiKeyToSampleKey(u8 ucKeyNumber)
{
	if (ucKeyNumber < NOTE_KEY_OFFSET) return -1;

	int result = ucKeyNumber - NOTE_KEY_OFFSET;

	if (result >= VOICES) return -1;

	return result;
}
