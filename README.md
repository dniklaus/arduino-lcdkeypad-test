# arduino-lcdkeypad-test
Arduino LCD Keypad Shield test project.


Clone this repository recursively into:

    C:\git\arduino-projects\arduino-lcdkeypad-test

using this command:

    git clone --recursive git@github.com:dniklaus/arduino-lcdkeypad-test.git C:\git\arduino-projects\arduino-lcdkeypad-test

After cloning, run the following batch:

    C:\git\arduino-projects\arduino-lcdkeypad-test\prepare-workspace.bat

This downloads and installs all needed tools to build this and work on this project using Eclipse CDT together with the Eclipse Arduino Plugin, unless the tools are already installed at the following locations:

* Arduino IDE Version 1.5.6-r2:
        
        C:\git\arduino-projects\Tools\arduino_revs\arduino-1.5.6-r2

* Eclipse Arduino Workbench Version 2014-12-28_02-06-00:
        
        C:\git\arduino-projects\Tools\eclipseArduino_revs\win64.2014-12-28_02-06-00\eclipseArduino

The workspace is now prepared and the project is built now already.

Run the Eclipse Arduino Workbench with this batch:

    C:\git\arduino-projects\arduino-lcdkeypad-test\eclipseArduino.bat

In the Eclipse Arduino Workbench select the lintilla_embedded entry in the Project Explorer (on the left-hand side) and press the Verify button, this compiles the project (and the red error icons will disappear). 

![](https://github.com/dniklaus/arduino-lcdkeypad-test/wiki/pictures/select_proj_and_press_verify.png)
