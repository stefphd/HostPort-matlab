
port = 3; %serial port
baud = 115200; %baudrate
len = 64; %size of the buffer to read
data = single([0 1 2 3 4 5]); %buffer to write

hostport = HostPort(); %instantiate a new HostPort object
hostport.begin(port, baud); %start the communication, return true if success
%hostport.begin(port, baud, header, terminator); %set also header and terminator

[data_read, exit] = hostport.read(len, 'single'); %read 'len' elements of single, return true if success

exit = hostport.write(data); %write buffer

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