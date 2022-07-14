
port = 7; %serial port
baud = 115200; %baudrate
len = 64; %size of the buffer to read
data = single([0 1 2 3 4 5]); %buffer to write

h = HostPort(); %instantiate a new HostPort object
h.begin(port, baud,HostPort.HEADER,HostPort.TERMINATOR,HostPort.TIMEOUT); %start the communication, return true if success
%hostport.begin(port, baud, header, terminator); %set also header and terminator

[data_read, exit] = h.read(len, 'single'); %read 'len' elements of single, return true if success

exit = h.write(data); %write buffer

%check initialization
if ~h %or hostport.isInit or hostport.begin(port, baud)
    disp('Unable to connect')
end

%check read or write
if ~exit
    disp('Unable to read or write');
end

%shos all properties
disp(h);

%close & clear
h.close();
clear h; %or HostPort.clear() for clear all