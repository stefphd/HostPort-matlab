#include "Serial.h"

Serial::Serial() {}

Serial::~Serial() {
    end();
}

std::vector<int> Serial::get_availableSerialPorts(unsigned int maxPorts) {
	std::vector<int> portsAvailable;
	for (int i = 0; i <= maxPorts; ++i) {
		Serial serial;
		serial.begin(i, 115200); //start serial with a default baudrate
		if (serial) portsAvailable.push_back(i);
		serial.end();
	}
	return portsAvailable;
}

bool Serial::begin(unsigned int port, unsigned int baud, unsigned int timeout) {
    setTimeout(timeout);
    return begin(port, baud);
}

bool Serial::begin(unsigned int port, unsigned int baud) {
    if (_isInit) {
        return false;
    }

    #ifdef OS_WIN
    char str[256];
    sprintf_s(str, 256, PORT_STR, port);
    _serialPort = CreateFile(str, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (_serialPort == INVALID_HANDLE_VALUE) {
        _isInit = false;
        return false;
    }
    // Do some basic settings
    _serialParams.DCBlength = sizeof(_serialParams);
    GetCommState(_serialPort, &_serialParams);
    _serialParams.BaudRate = baud;
    _serialParams.ByteSize = 8;
    _serialParams.StopBits = 1;
    _serialParams.Parity = 0;
    SetCommState(_serialPort, &_serialParams);
    // Set timeouts
    _serialTimeout.ReadIntervalTimeout = 0;
    _serialTimeout.ReadTotalTimeoutConstant = _timeout;
    _serialTimeout.ReadTotalTimeoutMultiplier = 0;
    _serialTimeout.WriteTotalTimeoutConstant = _timeout;
    _serialTimeout.WriteTotalTimeoutMultiplier = 0;
    SetCommTimeouts(_serialPort, &_serialTimeout);
    SetupComm(_serialPort, FIFO_BUF_SIZE, FIFO_BUF_SIZE);
    Sleep(500);
    #else
    char str[256];
    sprintf_s(str, 256, PORT_STR, port);
    _serialPort = open(str, O_RDWR);
    //check if serial valid
    if (_serialPort < 0) { return false; } 
    // read in existing settings, and handle any error
    if(tcgetattr(_serialPort, &_serialParams) != 0) { printf("1"); return false; }
    //set serial params
    _serialParams.c_cflag &= ~PARENB;
    _serialParams.c_cflag &= CSTOPB;
    _serialParams.c_cflag &= ~CSIZE;
    _serialParams.c_cflag |= CS8;
    _serialParams.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    _serialParams.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)
    _serialParams.c_lflag &= ~ICANON;
    _serialParams.c_lflag &= ~ECHO; // Disable echo
    _serialParams.c_lflag &= ~ECHOE; // Disable erasure
    _serialParams.c_lflag &= ~ECHONL; // Disable new-line echo
    _serialParams.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    _serialParams.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    _serialParams.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes
    _serialParams.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    _serialParams.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
    unsigned char vtime = (unsigned char) double(_timeout)/100.0; //prevent timeout to be zero
    if (vtime==0) vtime = 1;
    _serialParams.c_cc[VTIME] = vtime; // Wait for up (in deciseconds), returning as soon as any data is received.
    _serialParams.c_cc[VMIN] = 0;
    cfsetispeed(&_serialParams, baud);
    cfsetospeed(&_serialParams, baud);
    //set param to serial port
    if (tcsetattr(_serialPort, TCSANOW, &_serialParams) != 0) { return false; }
    //usleep(500000);
    #endif
    //all ok, returning true
    _isInit = true;
    return true;
}

void Serial::flush() {
    #ifdef OS_WIN
    if (_isInit) PurgeComm(_serialPort, PURGE_RXCLEAR);
    #else 
    if (_isInit) tcflush(_serialPort, TCIFLUSH);
    #endif
}

bool Serial::end() {
    #ifdef OS_WIN
    if ((_serialPort != INVALID_HANDLE_VALUE) & _isInit) {
        _isInit = false;
        CloseHandle(_serialPort);
        return true;
    }
    #else 
    if (!(_serialPort < 0) & _isInit) {
        _isInit = false;
        close(_serialPort);
        return true;
    }
    #endif
    return false;
}

bool Serial::write(unsigned char c) {
    if (!_isInit) {
        return false;
    }

    #ifdef OS_WIN
    return WriteFile(_serialPort, &c, 1, NULL, NULL);
    #else 
    return (::write(_serialPort, &c, 1) > 0);
    #endif
}

size_t Serial::write(unsigned char* buf, size_t len) {
    if (!_isInit) {
        return 0;
    }

    #ifdef OS_WIN
    DWORD n;
    if (WriteFile(_serialPort, buf, len, &n, NULL)) {
        return (size_t)n;
    }
    else {
        return 0;
    }
    #else 
    return ::write(_serialPort, buf, len);
    #endif
}

unsigned char Serial::read() {
    if (!_isInit) {
        return '\0';
    }

    #ifdef OS_WIN
    unsigned char c;
    if (ReadFile(_serialPort, &c, 1, NULL, NULL)) {
        return c;
    }
    else {
        return '\0';
    }

    #else 
    unsigned char c = '\0';
    ::read(_serialPort, &c, 1);
    return c;
    #endif
}

size_t Serial::read(unsigned char* buf, size_t len) {
    if (!_isInit) {
        return 0;
    }

    #ifdef OS_WIN
    DWORD n;
    if (ReadFile(_serialPort, buf, len, &n, NULL)) {
        return (size_t)n;
    }
    else {
        return 0;
    }
    #else 
    return ::read(_serialPort, buf, len);
    #endif
}

void Serial::setTimeout(unsigned int timeout) {
    _timeout = timeout;
}