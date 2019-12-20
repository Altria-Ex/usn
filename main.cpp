#include<Windows.h>
#include<iostream>

using namespace std;

int GetFileSystemName(char *FileSystemName, const char *RootPathName = "D:")
{
	char FileSystemNameBuffer[MAX_PATH];
	int status = GetVolumeInformationA(
		RootPathName,
		NULL,
		0,
		NULL,
		NULL,
		NULL,
		FileSystemNameBuffer,
		MAX_PATH
	);

	if (status != 0)
	{
		strcpy_s(FileSystemName, MAX_PATH, FileSystemNameBuffer);
		return 0;
	}
	else
		return -1;
}

int GetVolumeHandle(HANDLE &handle, const char* RootPathName = "D:")
{
	char filename[MAX_PATH] = "\\\\.\\";
	strcat_s(filename, RootPathName);
	HANDLE h = CreateFileA(
		filename,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_READONLY,
		NULL
	);

	if (h != INVALID_HANDLE_VALUE)
	{
		handle = h;
		return 0;
	}
	else
		return -1;
}

int CreateUSNJournal(HANDLE h)
{
	DWORD br;
	CREATE_USN_JOURNAL_DATA cujd;
	cujd.MaximumSize = 0;
	cujd.AllocationDelta = 0;
	int status = DeviceIoControl(
		h,
		FSCTL_CREATE_USN_JOURNAL,
		&cujd,
		sizeof(cujd),
		NULL,
		0,
		&br,
		NULL
	);
	if (status != 0)
		return 0;
	else
		return -1;
}

int GetUSNJornalInfo(HANDLE h, USN_JOURNAL_DATA &d)
{
	DWORD br;
	USN_JOURNAL_DATA USNInfo;
	int status = DeviceIoControl(
		h,
		FSCTL_QUERY_USN_JOURNAL,
		NULL,
		0,
		&USNInfo,
		sizeof(USNInfo),
		&br,
		NULL
	);
	if (status != 0)
	{
		d = USNInfo;
		return 0;
	}
	else
		return -1;

}

int main()
{
	const char* VolumeName = "D:";
	char FileSystemName[MAX_PATH];
	if (GetFileSystemName(FileSystemName, VolumeName) == 0)
		cout << FileSystemName << endl;

	HANDLE h;
	if (GetVolumeHandle(h, VolumeName) == 0)
		cout << h << endl;

	if (CreateUSNJournal(h) == 0)
	{ }

	USN_JOURNAL_DATA data;
	if (GetUSNJornalInfo(h, data) == 0)
	{ }
	
	return 0;
}