BattlEye Auto-Exception Generator v 1.0.0
Created by eraser1


This work is protected by Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0). By using, downloading, or copying any of the work contained, you agree to the license included.
http://creativecommons.org/licenses/by-nc-sa/4.0/


The following overview is a human-readable summary of (and not a substitute for) [the full license](http://creativecommons.org/licenses/by-nc-sa/4.0/legalcode).

You are free to:

Share - copy and redistribute the material in any medium or format.
Adapt - remix, transform, and build upon the material.


Under the following terms:

Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the **same license** as the original.
No additional restrictions - You may not apply legal terms or technological measures that legally restrict others from doing anything the license permits.


Notices:

You do not have to comply with the license for elements of the material in the public domain or where your use is permitted by an applicable exception or limitation
No warranties are given. The license may not give you all of the permissions necessary for your intended use. For example, other rights such as publicity, privacy, or moral rights may limit how you use the material.


This software will automatically read the "scripts.log" file, create exceptions from the restriction info, and add those exceptions to the "scripts.txt" file.
This software WILL NOT add exceptions to any other kind of ArmA BattlEye filters (those should always be handled manually).
Also, this software does not (yet) refresh your server's filters, so you will need to do those manually. However, it does have mechanisms to prevent duplicate exceptions from being added into your filters.


INSTRUCTIONS:
1) Place the "BE_AEG_v100.exe" executable in your server's "BattlEye" folder (NEXT TO YOUR SCRIPTS.TXT and SCRIPTS.LOG).
2) Run the "BE_AEG_v100.exe" executable (eg by double-clicking, command line, or via batch file).
3) Generate scripts exceptions! They will be automatically added. You can (should) also reload your script restrictions every time an exception is added.
4) Close the console window whenever you're done.


ADVANCED INSTRUCTIONS:
This software supports the use of certain command line parameters.
These parameters must be separated by (at least) a spacebar. These parameters are CASE SENSITIVE.
Refer to the two provided batch files for a small example.

Supported parameters:
noScriptLogBackup - This will prevent the program from backing up your script logs into the "BE_AEG_scripts.log" file.
noLogs - This will prevent the program from writing logs to "BE_AutoExceptionGenerator.log".
enableDebugConsole - This will cause the program to write debug information to the console. (Keep in mind that the console will NOT produce the same information as the logs)
runOnce - This will cause the program to only generate a single set of exceptions, based on what is in the "scripts.log" file at launch.




The executable was compiled using Microsoft Visual Studio (Ultimate) 2012. I had to add "_CRT_SECURE_NO_WARNINGS" to Preprocessor Definitions because Visual Studio doesn't like "ctime" :p
