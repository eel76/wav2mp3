Task:

Write a C/C++ commandline application that encodes a set of WAV files to MP3
 
Requirements:
 
(1) application is called with pathname as argument, e.g.
 
<applicationname> F:\MyWavCollection all WAV-files contained directly in that folder are to be encoded to MP3
 
(2) use all available CPU cores for the encoding process in an efficient way by utilizing multi-threading
 
(3) statically link to lame encoder library
 
(4) application should be compilable and runnable on Windows and Linux
 
(5) the resulting MP3 files are to be placed within the same directory as the source WAV files, the filename extension should be changed appropriately to .MP3
 
(6) non-WAV files in the given folder shall be ignored
 
(7) multithreading shall be implemented by means of using Posix Threads (there exist implementations for Windows)
 
(8) the Boost library shall not be used
 
(9) the LAME encoder should be used with reasonable standard settings (e.g. quality based encoding with quality level "good")

Implementation notes:

(-) i didn't use <thread> due to (7)
