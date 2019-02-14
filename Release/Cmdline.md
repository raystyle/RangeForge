# Command line documentation

## Rangeforge.exe

Usage: RangeForge [-hnq] [-v|vv] [-NOCLEAN] <file.SDL>

  examples
  -------------
  'RangeForge <file.SDL>' // Compile the SDL file with default behavior
  'RangeForge <file.SDL> -o myfile.bin' // Output test exe as 'myfile.bin'

  options
  -------
  -h             Display this help message and exit
  -n             Display version number and exit
  -v             Verbose mode
  -vv            Extra-verbose mode (use for debug tracing)
  -q             Quiet mode. Consider use with -w for no user input req.
  -w <ms>        Wait time in ms. Disables user input. Use with scripts or -q
  --pretty       Use pretty print.
  --NOCLEAN      Don't clean up test artifacts.  Exit after applying changes.
                 * warning --NOCLEAN will leave artifacts behind!
  --NOSAFE       Turn restricted safety/decoy mode off.
                 * warning --NOSAFE allows arbitrary commands!
  --leavesrc     Leave auto-generated source code files behind.
  -o <filename>  Manually specify the name of the output file on disk
  -s <seed>      Specify test seed (use for reproducibility)
  -s TIME        Seed with tickcount (DEFAULT)
