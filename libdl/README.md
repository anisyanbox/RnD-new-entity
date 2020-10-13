# Experiments with dynamic runtime linking

`dlopen()` will look for a library in the $LD_LIBRARY_PATH.
Don't forget to export your secret library path to one.

Or just use linker options to add relative paths: `-Wl,-rpath,.`
