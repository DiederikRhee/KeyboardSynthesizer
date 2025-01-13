//
// minisynth.cpp
//
// MiniSynth Pi - A virtual analogue synthesizer for Raspberry Pi
// Copyright (C) 2017-2024  R. Stange <rsta2@o2online.de>
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
#include "minisynth.h"
#include "math.h"
#include "config.h"
#include <circle/timer.h>
#include <circle/synchronize.h>
#include <circle/memory.h>
#include <circle/logger.h>
#include <assert.h>

static const char FromMiniSynth[] = "synth";

CMiniSynthesizer::CMiniSynthesizer (CInterruptSystem *pInterrupt)
:	m_SerialMIDI (this, pInterrupt),
	m_bUseSerial (FALSE),
	m_nConfigRevisionWrite (0),
	m_nConfigRevisionRead (0),
	m_VoiceManager (CMemorySystem::Get ()),
	m_fVolume (0.4)
{
}

CMiniSynthesizer::~CMiniSynthesizer (void)
{
}

boolean CMiniSynthesizer::Initialize (void)
{
	if (m_SerialMIDI.Initialize ())
	{
		CLogger::Get ()->Write (FromMiniSynth, LogNotice, "Serial MIDI interface enabled");

		m_bUseSerial = TRUE;

		return m_VoiceManager.Initialize ();
	}

	return FALSE;
}

void CMiniSynthesizer::Process (boolean bPlugAndPlayUpdated)
{
	if (m_bUseSerial)
	{
		m_SerialMIDI.Process ();
	}
}


void CMiniSynthesizer::NoteOn (u8 ucKeyNumber, u8 ucVelocity)
{
	GlobalLock ();

	m_VoiceManager.NoteOn (ucKeyNumber, ucVelocity);

	GlobalUnlock ();
}

void CMiniSynthesizer::NoteOff (u8 ucKeyNumber)
{
	GlobalLock ();

	m_VoiceManager.NoteOff (ucKeyNumber);

	GlobalUnlock ();
}

boolean CMiniSynthesizer::ConfigUpdated (void)
{
	unsigned nConfigRevisionWrite = m_nConfigRevisionWrite;
	if (nConfigRevisionWrite != m_nConfigRevisionRead)
	{
		m_nConfigRevisionRead = nConfigRevisionWrite;

		return TRUE;
	}

	return FALSE;
}

void CMiniSynthesizer::ControlChange (u8 ucFunction, u8 ucValue)
{
	/*assert (m_pConfig != 0);
	TSynthParameter Parameter = m_pConfig->MapMIDICC (ucFunction);
	if (Parameter >= SynthParameterUnknown)
	{
		return;
	}

	GlobalLock ();

	CPatch *pPatch = m_pConfig->GetActivePatch ();
	assert (pPatch != 0);

	pPatch->SetMIDIParameter (Parameter, ucValue);
	SetPatch (pPatch);

	m_nConfigRevisionWrite++;

	GlobalUnlock ();*/
}

void CMiniSynthesizer::ProgramChange (u8 ucProgram)
{
	GlobalLock ();

	/*if (ucProgram < PATCHES)
	{
		//m_pConfig->SetActivePatchNumber (ucProgram);
		//SetPatch (m_pConfig->GetActivePatch ());
		m_nConfigRevisionWrite++;
	}*/

	GlobalUnlock ();
}


void CMiniSynthesizer::GlobalLock (void)
{
	EnterCritical (IRQ_LEVEL);
}

void CMiniSynthesizer::GlobalUnlock (void)
{
	LeaveCritical ();
}


//// I2S //////////////////////////////////////////////////////////////////////

CMiniSynthesizerI2S::CMiniSynthesizerI2S (CInterruptSystem *pInterrupt, CI2CMaster *pI2CMaster)
:	CMiniSynthesizer (pInterrupt),
	CI2SSoundBaseDevice (pInterrupt, SAMPLE_RATE, 2048, FALSE, pI2CMaster, DAC_I2C_ADDRESS),
	m_nMinLevel (GetRangeMin ()+1),
	m_nMaxLevel (GetRangeMax ()-1),
	m_bChannelsSwapped (AreChannelsSwapped ())
{
}

boolean CMiniSynthesizerI2S::Start (void)
{
	return CI2SSoundBaseDevice::Start ();
}

boolean CMiniSynthesizerI2S::IsActive (void)
{
	return CI2SSoundBaseDevice::IsActive ();
}

unsigned CMiniSynthesizerI2S::GetChunk (u32 *pBuffer, unsigned nChunkSize)
{
	GlobalLock ();

	unsigned nResult = nChunkSize;

	float fVolumeLevel = m_fVolume * m_nMaxLevel;

	for (; nChunkSize > 0; nChunkSize -= 2)		// fill the whole buffer
	{
		float fsample = m_VoiceManager.Sample ();

		int nLevel = (int) (fsample*fVolumeLevel);
		if (nLevel > (int) m_nMaxLevel)
		{
			nLevel = m_nMaxLevel;
		}
		else if (nLevel < m_nMinLevel)
		{
			nLevel = m_nMinLevel;
		}

		*pBuffer++ = (u32) nLevel;
		*pBuffer++ = (u32) nLevel;
	}

	GlobalUnlock ();

	return nResult;
}
