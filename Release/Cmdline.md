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
  -v[b]          Verbose mode. Use postfix '-vb' to apply -v to test binary.
  -vv[b]         Extra-verbose mode (use for debug tracing)
  -q             Quiet mode. Consider use with -w for no user input req.
  -w <ms>        Wait time in ms. Disables user input. Use with scripts or -q
  --audit        Produce an audit only of what would be created. No compilation.
  --pretty       Use pretty print.
  --noclean      Don't clean up test artifacts.  Exit after applying changes.
                 * warning --noclean will leave artifacts behind.
  --expert       Enable unrestricted commands / disable decoy-only mode.
                 * warning --expert allows arbitrary behavior/payloads.
  --leavesrc     Leave auto-generated source code files behind.
  --leavebin     Leave copies of embedded binaries/strings.
  -o <filename>  Manually specify the name of the output file on disk
  -s <seed>      Specify test seed (use for reproducibility)
