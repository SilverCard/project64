#include "StdAfx.h"
#include "Project64SharedMemoryManager.h"


Project64SharedMemoryManager::Project64SharedMemoryManager()
{
	this->_IsRunning = false;
	this->_ShouldContinue = true;
}

void Project64SharedMemoryManager::MapMemory()
{
	this->_Handle = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file, Creating Named Shared Memory
		NULL,                    // default security
		PAGE_READWRITE,          // read/write access
		0,                       // max. object size
		this->Size,                // buffer size
		TEXT(this->Name));

	if (this->_Handle == NULL || this->_Handle == INVALID_HANDLE_VALUE)
	{
		throw "Failed to CreateFileMapping.";
	}	

	this->_Ptr = MapViewOfFile(this->_Handle,   // handle to map object
		FILE_MAP_ALL_ACCESS, // read/write permission
		0,
		0,
		this->Size);

	if (this->_Ptr == NULL)
	{
		throw "Failed to MapViewOfFile.";
	}
}

void Project64SharedMemoryManager::StopMemoryCopyTask()
{
	this->_ShouldContinue = false;
}

void Project64SharedMemoryManager::StartMemoryCopyTask()
{
	if (!this->_IsRunning)
	{
		this->_ShouldContinue = true;
		CreateThread(NULL, 0, StaticThreadStarter, this, 0, NULL);
		this->_IsRunning = true;
	}
}

DWORD WINAPI Project64SharedMemoryManager::StaticThreadStarter(void * obj)
{
	Project64SharedMemoryManager * manager = (Project64SharedMemoryManager *)obj;

	while (manager->_ShouldContinue)
	{
		if (g_MMU != NULL)
		{
			CopyMemory(manager->_Ptr , g_MMU->Rdram(), min(g_MMU->RdramSize(), manager->Size));
		}

		Sleep(50);
	}

	manager->_IsRunning = false;
	return 0;
}

Project64SharedMemoryManager::~Project64SharedMemoryManager()
{
}
