# RangeForge
Hardening Your Security

Please see our project website at [RangeForge.com](https://www.rangeforge.com/).

## Version 1.30

- added LAUNCH shellexec
- added additional file extension hijacking
- added sethc and utilman hijacking via debugger key

## Version 1.29

- added TOKEN runas
- added LAUNCH --runas

## Version 1.28

- added TOKEN impersonate
- added TOKEN priv enable/disable

## Version 1.27

- added support for browser helper objects
- added --force64 for registry keys
- added support for AppInitDLLs
- added support for launching DLL's via rundll32.exe
- added USER command with net, net1, and wmic support
- added KEYLOG command

## Version 1.24
- added service and task persistence
- added service hijacking

## Version 1.23
- added REGKEY command to persist at arbitrary location
- minor bugfixes

## Version 1.22
- demo of encoded powershell inside of reflective injected DLL
- fixed compiler warnings due to conflicting debug flags

## Version 1.21
- demo of encoded powershell script
- demo of account enumeration
- demo of adding local admin account

## Version 1.2
- added REGKEY command for arbitrary key creation
- added cleanup to kill launched processes
- added feature to CMD to use last STRINGS as batch file

## Version 1.1
- added seed to variable and filename generation
- added CMD to run arbitrary command line
- added LAUNCH feature to use proxy through secondary command-line exe

## Version 1.0
- got wait command working with command-line
- save and cleanup added
- RUNKEY persistence

## Version 0.4
- INJECT command
- added classic and reflective DLL injection

## Version 0.3
- embedding STRINGS
- embedding DLL's now possible
- added resource packing for DROP and embedded EXE/DLL
- LAUNCH command

## Version 0.2
- significant work on recursive decent parser
- proper order of operations

## Version 0.1
- added basic PRINT command
- comments working
- hello world launch of compiler/linker

