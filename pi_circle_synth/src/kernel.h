//
// kernel.h
//
// MiniSynth Pi - A virtual analogue synthesizer for Raspberry Pi
// Copyright (C) 2017-2022  R. Stange <rsta2@o2online.de>
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
#ifndef _kernel_h
#define _kernel_h

#include <circle/actled.h>
#include <circle/koptions.h>
#include <circle/devicenameservice.h>
#include <circle/cputhrottle.h>
#include <circle/nulldevice.h>
#include <circle/screen.h>
#include <circle/serial.h>
#include <circle/exceptionhandler.h>
#include <circle/interrupt.h>
#include <circle/timer.h>
#include <circle/logger.h>
#include <circle/i2cmaster.h>
#include <circle/usb/usbhcidevice.h>
#include <SDCard/emmc.h>
#include <circle/input/rpitouchscreen.h>
#include <lvgl/lvgl.h>
#include <fatfs/ff.h>
#include <circle/types.h>
#include "synthconfig.h"
#include "minisynth.h"

enum TShutdownMode
{
	ShutdownNone,
	ShutdownHalt,
	ShutdownReboot
};

class CKernel
{
public:
	CKernel (void);
	~CKernel (void);

	boolean Initialize (void);

	TShutdownMode Run (void);

private:
#ifdef SCREENSHOT_AFTER_SECS
	void SaveScreenshot (void);
#endif

private:
	// do not change this order
	CActLED			m_ActLED;
	CKernelOptions		m_Options;
	CDeviceNameService	m_DeviceNameService;
	CCPUThrottle		m_CPUThrottle;
	CNullDevice		m_NullDevice;
	CScreenDevice		m_Screen;
	//CSerialDevice		m_Serial;
	CExceptionHandler	m_ExceptionHandler;
	CInterruptSystem	m_Interrupt;
	CTimer			m_Timer;
	CLogger			m_Logger;
	CI2CMaster		m_I2CMaster;
	CUSBHCIDevice		m_USBHCI;
	CEMMCDevice		m_EMMC;
	CRPiTouchScreen		m_RPiTouchScreen;
	CLVGL			m_GUI;

	FATFS			m_FileSystem;
	CSynthConfig		m_Config;
	CMiniSynthesizer	*m_pSynthesizer;
};

#endif
