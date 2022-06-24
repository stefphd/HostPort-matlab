# HostPort Class

MATLAB class for communication from e to the microcontroller. 

This is the (MATLAB) host-side implementation of the Host Port class. The class makes use of a mex file (generated from C/C++ code) to speed up the communication. The class is thus just a wrapper around a C++ class.

## Contents

* `./src`: C/C++ source files
* `./include`: C/C++ include files
* `./vscode`: setting folder for Visual Studio Code
* `HostPortMex.mexw64`: mex file (Windows x64 version)
* `make.m`: MATLAB function to build the mex file
* `HostPort.m`: implementation of the Host Port class in MATLAB (using the mex file)
* `testing.m`: MATLAB script for testing
* `README.md`: this file

## Quick start

Class usage is:

```matlab
port = 3; %serial port
baud = 115200; %baudrate
len = 256; %size of the buffer to read
buf = single([0 1 2 3 4 5]); %buffer to write

hostport = HostPort(); %instantiate a new HostPort object
hostport.begin(port, baud); %start the communication, return true if success
%hostport.begin(port, baud, header, terminator); %set also header and terminator

exit = hostport.read(len); %read 'len' bytes, return true if success
data = hostport.decode('single'); %decode data into single (i.e. float)

exit = hostport.write(buf); %write buffer
```

To check the connection status:

```matlab
%check initialization
if ~hostport %or hostport.IsInit or hostport.begin(port, baud)
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

The class has two static method (i.e. object instantiation not required):

* `HostPort.getPorts()` to get the available serial ports

* `HostPort.clear()` to clear all the instantiated objects in the mex file.

## Building

If you want to (re)build the mex file, you need to run in MATLAB

```matlab
make
```

Note that building works only in Windows and Linux. MacOS is not supported at the moment. A valid C++17 compiler is required.

## Known issues

* `clear all` makes MATLAB crash. Just use `clear` and `clear mex` instead.
