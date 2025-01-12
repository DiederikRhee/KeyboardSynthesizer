#include "keysamplefatfsfile.h"
#include <circle/string.h>

CKeySampleFatFsFile::CKeySampleFatFsFile (u8 aSampleKeyNumber)
:	mSampleKeyNumber(aSampleKeyNumber)
{
	mFileReadingSucceeded = Load();
}

CKeySampleFatFsFile::~CKeySampleFatFsFile (void)
{
	delete [] mrgpSamples;
}

boolean CKeySampleFatFsFile::Load (void)
{
	FRESULT Result;
	FIL File;

	CString fileName;
	fileName.Format("%d.bin", (int)mSampleKeyNumber);

	Result = f_open (&File, fileName.c_str(), FA_READ | FA_OPEN_EXISTING);
	if (Result != FR_OK) return FALSE;

	FSIZE_t sizeBytes = f_size(&File);
	mSamplesSize = sizeBytes / sizeof(float);

	mrgpSamples = new float[mSamplesSize]();

	unsigned int nBytesRead = 0;

	Result = f_read (&File, mrgpSamples, sizeBytes, &nBytesRead);
	if (Result != FR_OK)
	{
		return FALSE;
	}

	return (sizeBytes == nBytesRead);
}
