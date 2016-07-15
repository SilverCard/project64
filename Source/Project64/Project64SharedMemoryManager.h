#pragma once
#include <Windows.h>

class Project64SharedMemoryManager
{
public:
	Project64SharedMemoryManager();
	~Project64SharedMemoryManager();
	void MapMemory();
	void StartMemoryCopyTask();
	void StopMemoryCopyTask();

	const char * Name = "Project64";
	const int Size = 4194304;

private:
	HANDLE _Handle;
	LPVOID _Ptr;
	bool _IsRunning;
	bool _ShouldContinue;

	static DWORD WINAPI StaticThreadStarter(void * obj);

};

