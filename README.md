# MySusu-Strings
This code is a program for extracting strings from any given executable file. It can be used for a variety of purposes, such as malware analysis, forensics investigations, or searching for passwords. The program takes a target executable file as an argument, and an optional set of flags. The flags tell the program which types of strings to output: Unicode, ASCII, links, and/or paths. The program will then output the strings to a file called output.txt. If the -all flag is specified, the program will create a folder with the same name as the target executable and save the strings to separate files in the folder. 

Using this program can help you save time by quickly extracting strings from an executable file. It can also help you discover useful information such as passwords or links contained in the executable file.

Usage with target: 

MySusu.exe target.exe [flags]

Usage without target: 

MySusu.exe [flags]



Commands:

-u - Unicode Strings

-a - Ascii Strings

-l - Links that are being taken from strings (foundable)

-all - Creating a folder with the .exe name, putting all the txt inside of 3 difirent .txt files (links, unicode, ascii) and gives the strings (efficient way, but takes alot of time)

-s -save result as the given name (optional)

-av -  check for viruses

-h = show all commands

-runs = check for scheculated running applications on startup.


-spoof = Spoofing a selected title to another title you put the name.


