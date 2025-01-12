#include "keysamplefatfsfile.h"

CKeySampleFatFsFile::CKeySampleFatFsFile (const char *apFileName)
:	mFileName (apFileName)
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

	if (mFileName.GetLength () == 0) return FALSE;

	Result = f_open (&File, mFileName, FA_READ | FA_OPEN_EXISTING);
	if (Result != FR_OK) return FALSE;

	FSIZE_t sizeBytes = f_size(&File);
	mrgpSamplesSize = sizeBytes / sizeof(float);

	mrgpSamples = new float[mrgpSamplesSize]();

	unsigned int nBytesRead = 0;

	Result = f_read (&File, mrgpSamples, sizeBytes, &nBytesRead);
	if (Result != FR_OK)
	{
		return FALSE;
	}

	return (sizeBytes == nBytesRead);
}
