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
