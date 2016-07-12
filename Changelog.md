# BE_AEG Changelog:
#### July 12, 2016 v1.0.2a
* The program should now actually sleep (oops lol)

#### July 12, 2016 v1.0.2
* The program will now "sleep" for 0.5 seconds whenever the "script.log" file is empty (whenever the program generated exceptions, or there are no new exceptions)
* The program will also "sleep" 0.25 seconds if the program fails to access the "scripts.txt" (instead of repeatedly accessing it as fast as possible)
* Slight optimization regarding "duplicate" exceptions; the program will now immediately check if the restriction code matches any exceptions instead of generating the exception, and THEN checking for duplicate exceptions.
* Added version number to the launch log.
* The changelog is now in a separate file to reduce clutter in the ReadMe.

#### June 27, 2016 v1.0.1
* Double quotes (") and backslashes ( \ ) will now be escaped.
* A different method is used to get time and date, so if you want to compile the program, you no longer need to set the preprocessor definition to allow the use of "ctime".
* Consequently, time and date is now in the "MM/DD/YY HH:MM:SS" format (sorry non-Americans :p ).

#### June 25, 2016 v1.0.0 Initial Release
