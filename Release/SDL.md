# Scenario Definition Language

Scenario Definition Language (SDL) is the driving force behind the Range Forge.  An easy to understand, almost batch-file-like language that can be used by anyone with command-line experience.  No programming skills are required.  Yet, a user of SDL can easily create a wide variety of attack behavior, malware decoys, droppers within droppers, memory injection, persistence, file-less attacks, and living off the land behaviors.

## Table of Contents

[TOC]

## Basic SDL

SDL Files contain one or more SDL commands.  Command are typically one line and follow this format:

```batch
COMMAND <modifier> <argument(s)> --flag <value>
```
Here is a simple SDL example that launches notepad and then waits for user input:
```batch
LAUNCH "c:\windows\notepad.exe"
USERWAIT
```

The beauty of SDL is the simplicity.  Most commands have built-in behavior and don't require arguments.  In many cases a command will try to figure out what you mean to do.  

```
EXE
	PRINT "Hello World"
ENDEXE
DROP
LAUNCH
```

The above SDL creates an executable that prints "Hello World" to the screen.  The DROP and LAUNCH commands are simple and will figure out what to do based on their position in the SDL file.   This example will drop the EXE to disk and launch it. 

***How does it work?*** Most commands can take action on the output of a preceding command.  In our example, the DROP command will look backwards in the file for the first thing that can be written to disk as a file (aka *dropped*).  In this case, the immediately preceding executable (defined by the ```EXE/ENDEXE``` block) qualifies as an object that can be written to disk.  The file will be written as a standard PE executable that can be executed. 

In similar fashion, the LAUNCH command also looks through the command history to find something to launch.  In this case, the immediately preceding DROP will have dropped a PE executable to disk.  The LAUNCH command will then launch that file.

***What will it do?*** If you want to figure out what actions an SDL will take (without actually executing a test) you can perform an **audit**.  Using the ```RangeForge.exe``` utility, simply pass the ```--audit``` flag and the tool will list out the sequence of behavior to expect from the SDL.

For example, for the following SDL file:

```
EXE
	STRINGS
		"cmd /c del C:\MALWARE_01~1.EXE > nul "
		" AddressFamily "
		" cmd.exe "
		" Com+Enabled "
		" command "
		" ComputerName "
		" COMSPEC "
		" CreatePipe "
		" CreateProcessA "
		" DelegateExecute "
		" Description "
		" DisableLocalOverride "
		" DisconnectNamedPipe "
		" GetComputerNameA "
		" GetCurrentProcess "
		" GetCurrentThread "
		" GetEnvironmentVariableA "
		" GetModuleFileNameA "
		" GetModuleHandleA "
		" GetShortPathNameA "
		" GetStartupInfoA "
	ENDSTRINGS
ENDEXE
DROP --path "%Appdata%"
REGKEY run
USERWAIT
```

Here is the ```RangeForge.exe --audit``` output:

```
c:\Temp\Tools>rangeforge appdata_runkey.SDL --audit
#  Range Forge
#  -------------------------------------------
#  Version: 1.27
#  Authors: Hoglund,Butterworth 2018, 2019
#  -------------------------------------------
# Audit mode only. Logging to stdout. No compilation.
@ audit: Logging to stdout. No compilation.
# Starting Range Forge...
@ audit: START scenario CRT931d0a51
@ audit: EMBEDDING child scenario CRTa4c3fa63
@ audit: Dropping embedded scenario CRTa4c3fa63 as ("%Appdata%\\CRT56a922be.exe")...
@ audit: Running write regkey here with key path ("Software\\Microsoft\\Windows\\CurrentVersion\\Run") and key name ("key_CRT441c8d50") and value ("%Appdata%\\CRT56a922be.exe")
@ audit: Waiting for user to press ENTER key here...
@ audit: END scenario CRT931d0a51
# Done creating SDL scenario...
# Range Forge completed successfully.

c:\Temp\Tools>
```

The **audit** feature is a key part of the Range Forge architecture.  It helps illustrate behavior and log static observables and IOC's associated with a test scenario.  Also, auditing illustrates that Range Forge calculates all pseudo-random values before compilation time.  This is an important feature that allows Range Forge to clean downstream artifacts after a test is complete.  And, when using **seeded polymorphism**, it allows you to see that Range Forge generated tests that can be reproduced in a QA lab or across multiple teams.

For more information on how to use the ```RangeForge.exe``` command line, see [Cmdline.md]()

# SDL Commands

### Comments

Comments are prefixed with a hash sign #

```batch
# this is a comment
```

Comments need to be one to a line.  Also, they can appear at the end of a line after a legitimate command...

```
PRINT "Hello"  # putting a comment here is OK
```

Comments are a useful way to keep track of your tests.

### Flags

Flags are passed to a command using the dashdash ```--flag```.  These modify the behavior of the command.  Flags can optionally take an argument.

```batch
REGKEY --noclean  # --noclean disables registry key cleanup after test
```

The following flags can be applied universally to most commands:

#### --noclean

This will disable artifact cleanup for that specific command.  By default, Range Forge generated tests will clean up after themselves.  But, in some cases, you may want the test artifacts to be left behind.

```
DLL
	STRING "Hello BHO"
ENDDLL   
DROP --noclean  # leave the DLL
REGKEY bho --noclean   # leave the bho registry keys
```

In the above example, a browser helper object will be registered and left behind after the test has exited.  Alternatively, you can force the entire test to skip cleanup by passing ```---noclean``` to ```RangeForge.exe```...

```
c:\Temp\Tools>rangeforge appdata_runkey.SDL --noclean
# noclean enabled.  Test artifacts will be left behind after test.
# Starting Range Forge...
...
```

In the above example, the test won't perform any cleanup.  This is the easiest way to disable cleanup for a given test.  

**Please be aware that disabling cleanup will mean test artifacts are left behind after the test executable has completed.  This MAY NOT be what you want.  For example, if you have hijacked a service with a test binary and you don't clean up, that hijacked service will remain.  At that point the only way to remove it may be manually or by restoring a VM snapshot.**

If you only want to disable cleanup behavior within a given executable or DLL you can pass the ```--noclean``` flag to the ```EXE/ENDEXE``` or ```DLL/ENDDLL``` build block...

```batch
EXE
	STRINGS
		"This is my text file"
	ENDSTRINGS
	DROP
ENDEXE --noclean
DROP
LAUNCH
USERWAIT
```

In the example an executable is dropped and launched.  All of the behavior within the exe has ```--noclean``` applied to it.  That exe then drops a text file to disk.  When the test is complete the exe will be deleted but the dropped text file will remain.

**Some flags are specific to commands and are documented below.**

### PRINT

PRINT simply outputs the string to stdout.  Keep in mind that of you use PRINT from an dropped EXE or an injected DLL, the output may be in another process space.

```batch
PRINT "this is a string"
```

When printing from an injected DLL, often times you won't be able to see the output.  The reason is the PRINT command is sending data to the standard output file handle ```stdout``` that is present in nearly all processes.  But, processes that don't have command windows (i.e., running from ```cmd.exe```) won't display the ```stdout``` data anywhere.

### MESSAGEBOX

Pops up a modal message box.  This will pause execution at this point until the user dismisses the message box.

```batch
PRINT "this is line one"
MESSAGEBOX "this is a message box"
PRINT "this line prints after the message box is dismissed"
```

The MESSAGEBOX is a good alternative to PRINT when dealing with programs that have graphical user interfaces (GUI's).  But, unlike PRINT, remember that MESSAGEBOX will pause execution at that line in the SDL.  And, until the user responds to the MESSAGEBOX, the execution will not continue past that point.  This is actually a good feature when you want the test to wait until you have taken some action (like launching an external program or measurement tool).

### EXE / ENDEXE

Embed and express an executable.  This will cause the executable to be embedded into the main test executable.  Also, you can embed EXE's within EXE's.

```batch
EXE
	EXE
		PRINT "I am exe two"
	ENDEXE
	DROP # exe one drops exe two
	PRINT "I am exe one"
ENDEXE
DROP 	# drop the embedded exe one to disk
LAUNCH 	# launch exe one
USERWAIT
```

### DLL / ENDDLL

Embed and express a DLL. This will cause the executable DLL to be embedded into the main test executable.  Like EXE's, DLL's can be embedded in DLL's and EXE's.

```batch
DLL
	MESSAGEBOX "I am DLL one"
ENDDLL
DROP
LAUNCH "notepad.exe" 
INJECT "notepad.exe"
USERWAIT
```

### STRINGS / ENDSTRINGS

Embed a set of strings into a binary.

```batch
EXE
	STRINGS
		"Hello world"
		"I am pretending to be a rootkit"
		"screencapture"
		"melt"
		"remote shell"
	ENDSTRINGS
ENDEXE
```

### SELECT

SELECT is a modifier on STRINGS.  Use STRINGS and SELECT together to select a subset of the strings.  This is controlled by the --seed value.  SELECT takes the number of strings to select.  In the following example, ten strings are selected.

```batch
EXE
	STRINGS SELECT(10)
		" !!! FLUSH cache !!! "
		" !!! parts after public exponent are process encrypted !!! "
		" !@#$%^&*()qwertyUIOPAzxcvbnmQQQQQQQQQQQQ)(*@&% "
		" !http://crl.certum.pl/cscasha2.crl0q "
		" $http://blog.gentilkiwi.com/mimikatz 0 "
		" Suspend a process "
		" Suspend service "
		" Switch (or reinit) to LSASS minidump context "
		" Switch (or reinit) to LSASS process context "
		" Switch to MINIDUMP : "
		" Switch to PROCESS "
		" SysKey : "
		" Remote : %s "
		" Remote server: %s "
		" Remote time (local): "
		" RemoteInteractive "
		" remotemodify "
		" Remove service "
		" Resume a process "
		" Resume service "
		" RF module for SR98 device and T5577 target "
		" RID : %08x (%u)User : %wZ "
		" RIGHT and FULL OUTER JOINs are not currently supported "
	ENDSTRINGS
	PRINT "I am fake mimikatz"
ENDEXE
```

### DROP

DROP will find the previous embedded DLL or EXE and drop it to disk.  By default it will drop the current working directory.  Optionally you can specify a path.

```batch
DROP --path "%WINDIR%\system32"  # drop to the system32 directory
```

### HOLDOPEN

HOLDOPEN simply holds the process open.  The process will only be killed once the test exits.

```batch
EXE
	HOLDOPEN
ENDEXE
DROP
LAUNCH
USERWAIT
```

### USERWAIT

Wait for user input before exiting the test.  This is important because tests will clean up after themselves upon exit.  You must keep the test open with USERWAIT if you want the artifacts to stay resident.

```batch
EXE
	MESSAGEBOX "hello"
ENDEXE
DROP
LAUNCH
USERWAIT
```

### CMD

Run a command shell.  CMD takes a command-line command as an argument.

```batch
CMD "net localgroup administrators"
```

Optionally, you can specify your commands in a STRINGS block.

```batch
STRINGS
	"net user"
	"wmic useraccount get /all /format:list"
	"wmic useraccount list full"
	"net localgroup administrators"
	"net group 'administrators'"
ENDSTRINGS
CMD 
USERWAIT
```

### USER

The USER command allows you to add and query user accounts.  It also allows you to specify different ways to query user accounts.

The format is:
```USER [action required] [filter optional] [additional arguments] --flags [flag args]```

The USER command requires an action.  There is no default behavior for an action-less USER command.

#### USER add

This action will add a user to the system.

The format is:

```USER add "username" "password"```

After the test completes the user will be removed.  To leave the user behind, specify ```--noclean```

```USER add "username" "password" --noclean```

Users can be added to the system in numerous ways.  Use one of the following flags to control how the user will be added:

##### --net

Use the ```net.exe``` command to add the user.  This is an existing command available on Windows.  The USER command will execute roughly the following:

```net.exe user "username" "password" /ADD```

##### --net1

Similar to the above but using ```net1.exe```

In fact, ```net.exe``` is just a wrapper to ```net1.exe``` 

Some attackers will use ```net1.exe``` to confuse security audits or behavioral detection.

**Currently there is no support to add users using ```--wmic```.**

#### USER list

This action will list users or administrators on a system.  This is a common command-line behavior when attackers have a shell or when malware is performing an inventory of a system.  There are many ways to query users both using tools and making API calls.

The format is:

```USER list```

To list admins, specify the ```admins``` filter:

```USER list admins```

You can specify numerous flags to control how the user list will be queried:

##### --net       

Use the ```net.exe``` command to list users.  

The format is:

```
USER list --net  # list users
USER list admins --net # list admins
```

This is equivalent to running the command line:

```net.exe user```

If you specify ```admins``` then the command will be:

```net.exe localgroup administrators```

Under the hood, ```net.exe``` uses basic win32 API calls to query the user list.

##### --net1

Uses ```net1.exe``` instead of ```net.exe``` but is otherwise equivalent.

The format is:

```
USER list --net1
USER list admins --net1
```

##### --wmic      

This uses the ```wmic.exe``` utility that ships with Windows.  ```wmic.exe``` is a complicated command that can be used in many ways.  

The format is:

```
USER list --wmic
USER list admins --wmic
```

By default, listing users is done with the following command line:

```wmic useraccount```

However, listing admins is done with a far more complicated command line:

```wmic /Node:"COMPUTERNAME" path win32_groupuser where (groupcomponent="win32_group.name=\"administrators\",domain=\"COMPUTERNAME\"")```

In the above, ```COMPUTERNAME``` is replaced with the local computer name.

**When using ```--wmic```, you can specify variations of the command as follows...**

##### --wmic ldap		

Use an ldap directory.  

The format is:

```
USER list --wmic ldap
USER list admins --wmic ldap
```

In the case of listing users, the command line will be similar to the following:

```wmic /NAMESPACE:\\root\directory\ldap PATH ds_user GET ds_smaccountname```

And, when listings admins, the command will be similar to:

```wmic /NAMESPACE:\\root\directory\ldap PATH ds_group where "ds_samccountname='Domain Admins'" Get ds_member /Value```

##### --wmic class	

Use a query against a WMI class.  

The format is:

```
USER list --wmic class
```

WMI provides many different information classes that can be queried.  In the case of listing users, the command will be similar to:

```wmic path win32_useraccount```

**Currently there is no support to list admins using ```-wmic class```.**

### TOKEN

Ability to enable privileges and impersonate process tokens.

#### TOKEN priv [enable|disable] "privilege_name"

Enable or disable privileges.  

```
TOKEN priv enable "debug"
```

The above command would enable the debug privilege (SE_DEBUG_PRIV) on the current token.

#### TOKEN impersonate "process_name"

Impersonate the token of another process.

```
TOKEN priv enable "debug"
TOKEN impersonate "lsass.exe"
```

The above would impersonate the token of lsass.exe - thus granting NT_AUTHORITY/SYSTEM access.  Note that SE_DEBUG_PRIV is enabled first in order to allow the impersonation to succeed.

You can follow the token impersonation with LAUNCH or INJECT to use the stolen token for subsequent operations.

#### TOKEN runas "process_name"

In this case, the token will be stolen from the target process but saved for later.  The code will not call impersonate.  This command can be used in conjunction with LAUNCH --runas to launch a process with the stolen token.  This is distinct from impersonation.  Using runas, the test only stores the stolen token and later uses it to launch a child process without changing the security context of the current thread.

```
TOKEN priv enable "debug"
TOKEN runas "lsass.exe"
LAUNCH "notepad.exe" --runas
USERWAIT
```

The above example would launch a copy of notepad.exe running under the user account NT_AUTHORITY/SYSTEM.

### LAUNCH

Launch the previously dropped executable.

```batch
EXE
	PRINT "hello world"
ENDEXE
DROP
LAUNCH
```

#### LAUNCH [proxy executable]

Optionally, you can proxy the launch through a secondary executable.  This can be a form of bypass.

```batch
EXE
	PRINT "Hello world"
ENDEXE
DROP
LAUNCH wmic
```

The above would launch the dropped EXE by calling to wmic, as opposed to launching the EXE directly.

The following launch proxies are supported:

- wmic
- atbroker
- cmd
- start
- explorer
- forfiles
- hh
- pcalua
- pcwrun

#### LAUNCH "process_path" --runas

This will cause the test to look for a previous call to TOKEN runas to find a stolen token to use with LAUNCH.  In the code this translates to a call to the win32 function ```CreateProcessWithTokenW(...)```

```
TOKEN priv enable "debug"
TOKEN runas "lsass.exe"
LAUNCH "notepad.exe" --runas
USERWAIT
```

In the above example, LAUNCH --runas causes the test to find the token stolen from lsass.exe and launch notepad.exe with this stolen token.

#### LAUNCH "process_path"

You can specify the name of an existing exe in the $PATH or the full path to an executable.

```
LAUNCH "calc.exe"
```

#### LAUNCH shellexec

This will cause the LAUNCH command to use ShellExecute to launch the file.  The command is not vectored through cmd.exe but launched directly with the verb "open".

```
LAUNCH shellexec "myfile.bat"
```

### OPENPROCESS

This is will open a remote handle to a process.  This is a behavioral pattern indicitive of malware.

```batch
OPENPROCESS "notepad.exe" # open a process handle to notepad.  Notepad must be running.
```

### INJECT

Inject a DLL into a remote process.

```batch
DLL
	STRINGS
		"I am a bad DLL"
	ENDSTRINGS
	MESSAGEBOX "Hello from the bad DLL"
ENDDLL
DROP
INJECT "explorer.exe"
USERWAIT
```

### REGKEY

Write a a specific regkey and persist the previously dropped EXE.

```batch
EXE
	MESSAGEBOX "I am persisting"
ENDEXE
DROP
REGKEY "Software\Microsoft\Windows\CurrentVersion\Policies\Explorer\Run"  # APT29 / CozyCar style persistence
```

REGKEY takes options.  You can specify preset locations to persist an executable:

REGKEY run          # persist the EXE using the Run key
REGKEY runonce      # persist the EXE using the RunOnce key
REGKEY appinit      # register a DLL as an AppInitDLL
REGKEY bho          # register a DLL as a Browser Helper Object (BHO)

You can also force a regkey to be created in the 64 bit hive:

REGKEY bho --force64  # forces the regkey to be created in the 64 bit hive, as opposed to the Wow6432Node (32 bit) hive.

### SERVICE

Persist through a service.  A service can be created or hijacked.  Used stand-alone, the service will use an auto-generated name and the most recent qualifying binary in the SDL history.

#### SERVICE "name"

Specify the name of the service.  The most recently dropped executable in the SDL history will be used.

#### SERVICE "name" "path"

Specify the name of the service and the path to the binary on disk.

#### SERVICE hijack "name"

Hijack an existing service, specified by "name".  This must be an existing service that uses a stand-alone exe (Type 0x16 or Win32OwnProcess).

#### SERVICE hijack <preset name>

Hijack a service by a preset name.  Supported services are:

- alg
- alerter


