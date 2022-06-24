
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

%check initialization
if ~hostport %or hostport.IsInit or hostport.begin(port, baud)
    disp('Unable to connect')
end

%check read or write
if ~exit
    disp('Unable to read or write');
end

%shos all properties
disp(hostport);

%close & clear
hostport.close();
clear hostport; %or HostPort.clear() for clear all