#pragma once

///////////////////////////////////////////////////////////////////////////////////
// This header file should co-exist in the same directory as the Release/x86
// build of RangeLib.lib and RangeForge.exe to enable RangeForge to link
// executables properly.  RangeForge.exe can be built in debug mode, but the
// directory-resident version of RangeLib.lib must be in Release/x86 mode.
///////////////////////////////////////////////////////////////////////////////////

#include <Windows.h>
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;  // TODO remove this and fix all the references to std

#pragma comment (lib, "User32.lib")

#define RANGELIB_VERSION L"1.2"

#ifndef _WIN64
#define PLATFORM_STRING "Win32"
#else
#define PLATFORM_STRING "x64"
#endif

#ifdef _DEBUG
#define CONFIG_STRING "Debug"
#else
#define CONFIG_STRING "Release"
#endif

// TODO fix this to work on all project files
#define LIB_PATH(lib) "..\\Output\\Lib\\" PLATFORM_STRING "\\" CONFIG_STRING "\\" lib

//utilities
std::vector<BYTE> AssembleHeaderArray(CHAR ** theResourcePointer, INT * theResourceSizes, INT Element);
std::wstring expand_env(std::wstring str);

// drop
BOOL ToolDropFileEmpty(WCHAR *theFilePath);
BOOL ToolDropFileClone(WCHAR *theFilePath);
BOOL ToolDropFileFromResource(WCHAR *theFilePath, LPCWSTR theResourceID);
BOOL ToolDropFileFromString(std::wstring theFilePath, std::string theContent);
BOOL ToolCleanFile(WCHAR *theFilePath);
//BOOL ToolDropFileFromHeader(WCHAR *theFilePath, BYTE *theResourcePointer, SIZE_T ResourceSize);
BOOL ToolDropFileFromHeader(WCHAR *theFilePath, CHAR ** theResourcePointer, INT * theResourceSizes, INT Elements);
std::vector<BYTE> OpenAndLockResource(LPCWSTR theResourceID);

typedef enum _RESTOREFLAG
{
	RESTORE_NONE,		// don't restore, just leave the value
	RESTORE_DELETE,		// delete the item entirely
	RESTORE_ORIGINAL	// put back the original value
} RESTOREFLAG;

// Registry persistence
typedef struct _REGSAVESTATE
{
	HKEY hive;
	std::wstring original_keypath;
	std::wstring original_name;
	std::wstring original_stringvalue;

	RESTOREFLAG restore_keypath = RESTOREFLAG::RESTORE_NONE;
	RESTOREFLAG restore_namevalue = RESTOREFLAG::RESTORE_NONE;
	
} REGSAVESTATE, *PREGSAVESTATE;

BOOL ToolRegSaveState(HKEY hKeyRoot, wstring keypath, wstring valuename, PREGSAVESTATE save_state);
BOOL ToolRegRestoreState(PREGSAVESTATE save_state);

BOOL ToolRegAppendStringValue(HKEY hKeyRoot, wstring keyname, wstring valuename, wstring file_path);
BOOL ToolRegAddStringValue(HKEY hKeyRoot, wstring keyname, wstring valuename, wstring new_value, bool ensureCreated = false);
BOOL ToolRegDeleteValue(HKEY hKeyRoot, wstring keyname, wstring valuename);

// TODO refactor
BOOL PersistRunKey(std::wstring value_name, std::wstring target_file_path, bool run_once, bool user_hive, bool use_wow64);
BOOL DeleteRunKeyPersistence(std::wstring value_name, bool run_once, bool user_hive, bool wow64);

// process
BOOL ToolLaunchProcess(WCHAR* theProcessPath);

// debugging
BOOL ToolOpenProcess(std::wstring target_process);

// cmd shell
BOOL ToolLaunchCommandShell(WCHAR* szCommandLine);

// inject
BOOL InjectDll(std::wstring inject_source, std::wstring target_process, std::wstring injection_method);
BOOL InjectDll(std::vector<BYTE> data, std::wstring target_process, std::wstring injection_method);

// unit testing
BOOL ToolFunctionOne(CHAR *theArg);
BOOL ToolFunctionTwo(CHAR *theArg);
BOOL ToolFunctionThree(CHAR *theArg);
BOOL ToolFunctionFour(CHAR *theArg);

//persistance
BOOL PersistRunKey(std::wstring value_name, std::wstring target_file_path, bool run_once, bool user_hive, bool use_wow64);
BOOL PersistService(std::wstring svc_name, std::wstring display_name, std::wstring target_file_path, std::wstring user_name, std::wstring password);
BOOL PersistSvcHost(std::wstring svc_name, std::wstring group_name, std::wstring target_dll_path);
BOOL PersistAppInitDll(std::wstring target_dll_path);

// User Messages
BOOL ShowMessageBoxOk(std::wstring message, std::wstring caption);
BOOL ShowMessageBoxYesNo(std::wstring message, std::wstring caption);

// Services
BOOL ToolCreatePersistentService(WCHAR *theBinaryPath, WCHAR *theServiceName, WCHAR *theServiceDisplayName);
BOOL ToolDeletePersistentService(WCHAR *theServiceName);

// Tasks
BOOL ToolCreateTask(WCHAR *theTaskName, WCHAR *theFilePath);
BOOL ToolDeleteTask(WCHAR *theTaskName, WCHAR *theFilePath);

// AppInit DLL
//BOOL ToolReadOriginalAppInitDLL(wstring &orig_value);
BOOL ToolAddAppInitDLL(wstring theFilePath);
//BOOL ToolRestoreAppInitDLL(wstring orig_value);

// Run Key
BOOL ToolAddRunKey(wstring theName, wstring theFilePath);
BOOL ToolDeleteRunKey(wstring theName);
BOOL ToolAddRunOnceKey(wstring theName, wstring theFilePath);
BOOL ToolDeleteRunOnceKey(wstring theName);

// Synchronization
DWORD WaitGlobalEventAndTerminate(std::wstring event_name);
bool SignalGlobalEvent(std::wstring event_name);
bool WaitGlobalEvent(std::wstring event_name);
