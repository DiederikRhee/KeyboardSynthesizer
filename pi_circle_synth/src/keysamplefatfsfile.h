//
// propertiesfatfsfile.h
//
// Provides access to configuration properties saved in a file (FatFs version)
//
// Circle - A C++ bare metal environment for Raspberry Pi
// Copyright (C) 2016-2018  R. Stange <rsta2@o2online.de>
//
// FatFs support by Steve Maynard
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
#ifndef _keysamplefatfsfile_h
#define _keysamplefatfsfile_h

#include <fatfs/ff.h>
#include <circle/string.h>
#include <circle/types.h>

class CKeySampleFatFsFile
{
public:
	CKeySampleFatFsFile (const char *apFileName);
	~CKeySampleFatFsFile (void);

	boolean Get_FileReadingSucceeded(void) { return mFileReadingSucceeded; }

private:
	boolean Load (void);		// may be partially loaded on error


private:
	CString mFileName;

	boolean mFileReadingSucceeded;

	float* mrgpSamples;

	size_t mrgpSamplesSize;
};

#endif
