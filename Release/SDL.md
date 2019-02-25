# Scenario Definition Language

Scenario Definition Language (SDL) is the driving force behind the forge.  An easy to understand, almost batch-file-like language that can be used by anyone with command-line experience.  No programming skills are required.  Yet, a user of SDL can easily create a wide variety of attack behavior, malware decoys, droppers within droppers, memory injection, persistence, fileless attacks, and living off the land behaviors.

SDL Files contain one or more SDL commands.  Command are typically one line and follow this format:
```batch
COMMAND <modifier> <argument(s)> --flag <value>
```
A simple example that launched notepad and wait for user input:
```batch
LAUNCH "c:\windows\notepad.exe"
USERWAIT
```

## Commands

### Comments

Comments are prefixed with a hash sign #

```batch
# this is a comment
```

### Flags

Flags are passed to a command using the dashdash --flag.  These modify the behavior of the command.  Flags can optionally take an argument.

```batch
RUNKEY --noclean  # --noclean disables registry key cleanup after test
```

### PRINT

PRINT simply outputs the string to stdout.  Keep in mind that of you use PRINT from an dropped EXE or an injected DLL, the output may be in another process space.

```batch
PRINT "this is a string"
```

### MESSAGEBOX

Pops up a modal message box.  This will pause execution at this point until the user dismisses the message box.

```batch
PRINT "this is line one"
MESSAGEBOX "this is a message box"
PRINT "this line prints after the message box is dismissed"
```

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

### LAUNCH

Launch the previously dropped executable.

```batch
EXE
	PRINT "hello world"
ENDEXE
DROP
LAUNCH
```

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

### RUNKEY

Persist the previously dropped EXE as a run key in the registry.

```batch
EXE
	MESSAGEBOX "I am persisting"
ENDEXE
DROP
RUNKEY
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



