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

#define RANGELIB_VERSION L"1.29"

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
// TODO refactor so we don't need to expose external structure.. make internal 'automagic' version
typedef struct _REGSAVESTATE
{
	HKEY hive;
	std::wstring original_keypath;
	std::wstring original_name;
	std::wstring original_stringvalue;
	DWORD		 original_valuetype;

	RESTOREFLAG restore_keypath = RESTOREFLAG::RESTORE_NONE;
	RESTOREFLAG restore_namevalue = RESTOREFLAG::RESTORE_NONE;
	
} REGSAVESTATE, *PREGSAVESTATE;

typedef enum _REGBITS
{
	BITS_DEFAULT = 0,	// if 32bit exe on 64bit os, use WoW6432Node 
	BITS_FORCE32,		// force use of WoW6432Node on 64 bit, even if 64 bit exe	
	BITS_FORCE64		// force use of 64 bit registry on 64 bit OS, even if 32 bit exe
} REGBITS;

// registry
BOOL ToolRegInit();
BOOL ToolRegSaveState(HKEY hKeyRoot, wstring keypath, wstring valuename, PREGSAVESTATE save_state, REGBITS bits = REGBITS::BITS_DEFAULT);
BOOL ToolRegRestoreState(PREGSAVESTATE save_state, REGBITS bits = REGBITS::BITS_DEFAULT);
BOOL ToolRegAppendStringValue(HKEY hKeyRoot, wstring keyname, wstring valuename, wstring file_path, REGBITS bits = REGBITS::BITS_DEFAULT);
BOOL ToolRegAddStringValue(HKEY hKeyRoot, wstring keyname, wstring valuename, wstring new_value, bool ensureCreated = false, REGBITS bits = REGBITS::BITS_DEFAULT);
BOOL ToolRegDeleteValue(HKEY hKeyRoot, wstring keyname, wstring valuename, REGBITS bits = REGBITS::BITS_DEFAULT);
BOOL ToolRegDeleteLastKeyOfPath(HKEY hive, std::wstring thePath, REGBITS bits = REGBITS::BITS_DEFAULT);
BOOL ToolAddRegCLSID(std::wstring clsid, std::wstring dllPath, REGBITS bits = REGBITS::BITS_DEFAULT);
BOOL ToolDelRegCLSID(std::wstring clsid, REGBITS bits = REGBITS::BITS_DEFAULT);

// Services
BOOL ToolCreatePersistentService(WCHAR *theBinaryPath, WCHAR *theServiceName, WCHAR *theServiceDisplayName);
BOOL ToolDeletePersistentService(WCHAR *theServiceName);
BOOL ToolHijackService(HKEY hRootKey, WCHAR *theServiceName, WCHAR *theHijackBinaryPath);
BOOL ToolSaveServiceImagePath(wstring servicename, PREGSAVESTATE save_state);

// process
BOOL ToolLaunchProcess(WCHAR* theProcessPath);
BOOL ToolLaunchProcessAsToken(WCHAR* theProcessPath, HANDLE hToken);
BOOL ToolDebugProcess(std::wstring target_process);
BOOL ToolOpenProcess(std::wstring target_process);
BOOL ToolEnumProcii();
BOOL ToolDetachDebugger();

// tokens
BOOL ToolEnableTokenPrivilege(std::wstring privilege_name);
BOOL ToolDisableTokenPrivilege(std::wstring privilege_name);
BOOL ToolImpersonateProcessToken(std::wstring process_name);
BOOL ToolDuplicateProcessToken(std::wstring target_process, HANDLE *pTokenHandle);

// keylogger
BOOL ToolSetKeylogger();
BOOL ToolRemoveKeylogger();

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

// User Messages
BOOL ShowMessageBoxOk(std::wstring message, std::wstring caption);
BOOL ShowMessageBoxYesNo(std::wstring message, std::wstring caption);

// Tasks
BOOL ToolCreateTask(WCHAR *theTaskName, WCHAR *theFilePath);
BOOL ToolDeleteTask(WCHAR *theTaskName);

// Synchronization
DWORD WaitGlobalEventAndTerminate(std::wstring event_name);
bool SignalGlobalEvent(std::wstring event_name);
bool WaitGlobalEvent(std::wstring event_name);

BOOL ToolSetVerbose(int level);


