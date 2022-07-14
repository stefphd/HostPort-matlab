# HostPort-matlab

MATLAB binding of the HostPort class for communication from and to the microcontroller.

This is the (MATLAB) host-side implementation of the Host Port class. The class is internally implemented in C++ and interfaced to MATLAB using a mex file (generated from C/C++ code). The MATLAB class is thus just a wrapper around a C++ class.

Tested on Windows 10 and Linux (ArchLinux) with MATLAB 2022a.

## Contents

* `./src`: C/C++ source files
* `./include`: C/C++ include files
* `./.vscode`: setting folder for Visual Studio Code
* `HostPortMex.mexw64`: mex file for the Host Port class (Windows x64 version)
* `HostPortMex.mexa64`: mex file for the Host Port class (Linux x64 version)
* `make.m`: MATLAB function to build the mex file
* `HostPort.m`: implementation of the Host Port class in MATLAB (using the mex file)
* `Serial.m`: implementation of the Serial class in MATLAB (using the mex file)
* `testing.m`: MATLAB script for testing
* `README.md`: this file

## Quick start

Usage for the Host Port class is:

```matlab
port = 3; %serial port
baud = 115200; %baudrate
len = 64; %size of the buffer to read
buf = single([0 1 2 3 4 5]); %buffer to write

hostport = HostPort(); %instantiate a new HostPort object
hostport.begin(port, baud); %start the communication, return true if success
%hostport.begin(port, baud, header, terminator); %set also header and terminator

%possibilities are: 'single', 'double', 'uint8', 'int8', 'uint16', 'int16', 
%                   'uint32', 'int32', 'uint64', 'int64'
exit = hostport.read(len,'single'); %read 'len' single values, return true if success

exit = hostport.write(buf); %write buffer
```

To check the connection status:

```matlab
%check initialization
if ~hostport %or hostport.isInit or hostport.begin(port, baud)
    error('Unable to connect')
end

%check read or write
if ~exit
    error('Unable to read or write');
end
```

To show all properties:

```matlab
%shos all properties
disp(hostport);
```

To flush and restart the port:

```matlab
hostport.flush();
hostport.restart();
```

Finally, to close and clear

```matlab
%close & clear
hostport.close();
clear hostport; %or HostPort.clear() for clear all
```

Editable object properties are:

* `Port`

* `Baud`

* `Header`

* `Terminator`

* `Timeout`

Note that the port must be restarted with `hostport.restart()` after having set one of the editable properties to make the change effective.

The class has two static method (i.e. object instantiation not required):

* `HostPort.getPorts()` to get the available serial ports

* `HostPort.clear()` to clear all the instantiated objects in the mex file.

Default header, terminator, and timeout can be obtained using

* `HostPort.HEADER`

* `HostPort.TERMINATOR`

* `HostPort.TIMEOUT`

## Building

If you want to (re)build the mex file for the Host Port class, you need to run in MATLAB

```matlab
make
```

Note that building works only in Windows and Linux. MacOS is not supported at the moment. A valid C++17 compiler is required.
