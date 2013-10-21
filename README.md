# kinect-pd-instrument

#### Final project of KTH Sound in Interation course (DT2300)

This project has been developed on Windows over **MS VS 2013** using **SFML 2.0** graphics library and **Kinect SDK** for windows

It consists of 2 programs, one program in C++ that retrieves Kinect information and a program in **PureData** that handles the information that it is sent to it and generates sound.

##### Disclaimer
This software uses a light implementation of OSC (http://opensoundcontrol.org/) developed by Julien Pommier at http://gruntthepeon.free.fr/oscpkt/ for communication between the main program in C++ and PureData

##### Compiling
To compile we have the .sln project for VS 2013.
Note that you need to have SFML2.X and KinectSDK and have the enviorment variables ***KINECTSDK10_DIR*** and ***SFML32_DIR*** defined on your path.
You can always change those parameters on the VS project settings

##### Executing
You need to have the kinect sdk installed and the kinect plugged in to the computer.
Execute the file **pd/instrument.pd** in PureData first.
Execute the VS executable then.
You can change the chords that will be played using numbers *1 - 5* in the keyboard

Remember to add the corresponding *dll*s to the folder where you are executing (if you are compiling dynamically), for instance the sfml *dll*s.
