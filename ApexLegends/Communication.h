#include <Windows.h>
#include <TlHelp32.h>
#include <tchar.h>
#include <vector>
#include <string>
#include <iostream>
#include <array>
#include <string>
#include <sstream>
#include <chrono>
#include <cstdio>
#include <vector>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <tlhelp32.h>
#include <direct.h>
#include <stdio.h>
#include <cstring>
#include <Psapi.h>
#include <string_view>
#include <thread>
#include "Structs.h"
#include <urlmon.h>
#pragma comment(lib, "urlmon.lib")

using namespace std;

#include "Main.h"

using namespace std;

uintptr_t BaseAddress = 0;
uint32_t ProcessId = 0;
uint32_t CheckProcess = 0;

typedef struct _SERAPH_MEMORY
{
	void* BufferAddress;
	UINT_PTR Address;
	ULONGLONG Size;
	ULONG ProcessId;
	BOOLEAN Write;
	BOOLEAN Read;
	BOOLEAN RequestBase;
	void* Output;
	const char* ModuleName;
	ULONG64 BaseAddress;
	BOOLEAN ClearPiddb;
	BOOLEAN Mmunload;
}SERAPH_MEMORY;

template<typename ... Arg>
uint64_t call_hook(const Arg ... args)
{
	void* hooked_func = GetProcAddress(LoadLibrary("win32u.dll"), "NtQueryCompositionSurfaceStatistics");

	auto func = static_cast<uint64_t(_stdcall*)(Arg...)>(hooked_func);

	return func(args ...);
}

struct HandleDisposer
{
	using pointer = HANDLE;
	void operator()(HANDLE handle) const
	{
		if (handle != NULL || handle != INVALID_HANDLE_VALUE)
		{
			CloseHandle(handle);
		}
	}
};

using unique_handle = std::unique_ptr<HANDLE, HandleDisposer>;

std::uint32_t _GetProcessId(std::string_view process_name)
{
	PROCESSENTRY32 processentry;
	const unique_handle snapshot_handle(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL));

	if (snapshot_handle.get() == INVALID_HANDLE_VALUE)
		return NULL;

	processentry.dwSize = sizeof(MODULEENTRY32);

	while (Process32Next(snapshot_handle.get(), &processentry) == TRUE)
	{
		if (process_name.compare(processentry.szExeFile) == NULL)
		{
			return processentry.th32ProcessID;
		}
	}
	return NULL;
}

static ULONG64 GetModuleBase(const char* module_name)
{
	SERAPH_MEMORY Seraph = { 0 };
	Seraph.ProcessId = ProcessId;
	Seraph.RequestBase = TRUE;
	Seraph.Read = FALSE;
	Seraph.Write = FALSE;
	Seraph.ClearPiddb = FALSE;
	Seraph.Mmunload = FALSE;
	Seraph.ModuleName = module_name;
	call_hook(&Seraph);

	ULONG64 base = NULL;
	base = Seraph.BaseAddress;
	return base;
}

void Piddbclear()
{
	SERAPH_MEMORY Seraph;
	Seraph.Read = FALSE;
	Seraph.Write = FALSE;
	Seraph.ClearPiddb = TRUE;
	Seraph.Mmunload = FALSE;
	Seraph.RequestBase = FALSE;
	call_hook(&Seraph);

	std::cout << XorString("[+] Clear The Caches") << std::endl;

	return;
}

void Mmunload()
{
	SERAPH_MEMORY Seraph;
	Seraph.Read = FALSE;
	Seraph.Write = FALSE;
	Seraph.ClearPiddb = FALSE;
	Seraph.Mmunload = TRUE;
	Seraph.RequestBase = FALSE;
	call_hook(&Seraph);

	std::cout << XorString("[+] Clear The Threads") << std::endl;
	
	return;
}

template <class T>
T Read(UINT_PTR read_address)
{
	T Response{};
	SERAPH_MEMORY Seraph;
	Seraph.ProcessId = ProcessId;
	Seraph.Size = sizeof(T);
	Seraph.Address = read_address;
	Seraph.Read = TRUE;
	Seraph.Write = FALSE;
	Seraph.ClearPiddb = FALSE;
	Seraph.Mmunload = FALSE;
	Seraph.RequestBase = FALSE;
	Seraph.Output = &Response;
	call_hook(&Seraph);

	return Response;
}

template <class T>
T ReadString(UINT_PTR read_address)
{

	std::vector<char> buffer(sizeof(DWORD64), char{ 0 });

	T Response{};
	SERAPH_MEMORY Seraph;
	Seraph.ProcessId = ProcessId;
	Seraph.Size = sizeof(DWORD64);
	Seraph.Address = read_address;
	Seraph.Read = TRUE;
	Seraph.Write = FALSE;
	Seraph.ClearPiddb = FALSE;
	Seraph.Mmunload = FALSE;
	Seraph.RequestBase = FALSE;
	Seraph.Output = static_cast<void*>(&buffer[0]);
	call_hook(&Seraph);

	return std::string(buffer.data());
}

template <class T>
T ReadName(UINT_PTR read_address)
{

	std::vector<char> buffer(24, char{ 0 });

	T Response{};
	SERAPH_MEMORY Seraph;
	Seraph.ProcessId = ProcessId;
	Seraph.Size = 24;
	Seraph.Address = read_address;
	Seraph.Read = TRUE;
	Seraph.Write = FALSE;
	Seraph.ClearPiddb = FALSE;
	Seraph.Mmunload = FALSE;
	Seraph.RequestBase = FALSE;
	Seraph.Output = static_cast<void*>(&buffer[0]);
	call_hook(&Seraph);

	return std::string(buffer.data());
}

bool write_memory(UINT_PTR write_address, UINT_PTR source_address, SIZE_T write_size)
{
	SERAPH_MEMORY Seraph;
	Seraph.Address = write_address;
	Seraph.ProcessId = ProcessId;
	Seraph.Write = TRUE;
	Seraph.Read = FALSE;
	Seraph.ClearPiddb = FALSE;
	Seraph.Mmunload = FALSE;
	Seraph.RequestBase = FALSE;
	Seraph.BufferAddress = (void*)source_address;
	Seraph.Size = write_size;

	call_hook(&Seraph);

	return true;
}

template<typename S>
bool write(UINT_PTR write_address, const S& value)
{
	return write_memory(write_address, (UINT_PTR)&value, sizeof(S));
}


template<typename T>
std::string read_string(uint64_t address, SIZE_T Size)
{
	std::string blank = "CAN'T";

	std::vector<char> buffer(Size, char{ 0 });

	SERAPH_MEMORY Seraph = { 0 };
	Seraph.ProcessId = ProcessId;
	Seraph.Address = address;
	Seraph.Size = buffer.size();
	Seraph.Output = static_cast<void*>(&buffer[0]);

	if (call_hook(&Seraph)) {
		return std::string(buffer.data());
	}
	else {
		return blank;
	}
}