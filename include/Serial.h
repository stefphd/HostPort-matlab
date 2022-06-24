
#ifndef SERIAL_H
#define SERIAL_H

#define FIFO_BUF_SIZE 262144 //FIFO buffer size (default)

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) || defined(__WIN32__) || defined(WIN64) || defined(_WIN64) || defined(__WIN64) || defined(__WIN64__)
#ifndef OS_WIN
#define OS_WIN
#endif
#elif defined(linux) || defined(__linux) || defined(__linux__) || defined(__GNUC__) || defined(__GNUG__) || defined(unix) || defined(__unix) || defined(__unix__)
#ifndef OS_LINUX
#define OS_LINUX
#endif
#else
#error OS not supported
#endif

#ifdef OS_WIN
#include <windows.h>
#define PORT_STR "\\\\.\\COM%u"
#else 
#include <fcntl.h> // Contains file controls like O_RDWR
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()
#define sprintf_s(buf, len, ...) snprintf((buf), (len), __VA_ARGS__)
#define PORT_STR "/dev/ttyACM%u"
#endif

//common include (both Win and Linux)
#include <stdio.h>
#include <vector>

/*! \brief A class for serial communication.
    \details Class for managing a general-purpose serial communication in Windows (using COM port) and Linux (using ttyACM). 
    This is just a C++ wrapper for the HANDLE type in Windows, and the open, write, read, etc functions in Linux.
    \author Stefano Lovato
    \date 2022
*/
class Serial {
    private:
        #ifdef OS_WIN
        HANDLE _serialPort; //!< serial port
        DCB _serialParams = { 0 };  //!< serial param struct
        COMMTIMEOUTS _serialTimeout = { 0 };  //!< timeout struct
        #else 
        int _serialPort; //!< serial port
        struct termios _serialParams; //!< serial param struct
        #endif
        bool _isInit = false; //!< true if serial has been initialized
        unsigned int _timeout = 10; //!< timeout of the serial port
    public:
        /*! \brief Constructor.
            \details Default constructor for the serial class. No parameter is necessary.
        */
        Serial();

        /*! \brief Desctructor.
            \details Default destructor for the serial class. No parameter is necessary. It automatically closes the communication if open.
        */
        ~Serial();

        /*! \brief Get the available serial ports.
            \details Function to get the available serial ports. Ports from 0 to maxPorts are checked.
            \param maxPortNumber The max port number to be checked.
            \return A std::vector<int> containing the available serial ports.
        */
        static std::vector<int> get_availableSerialPorts(unsigned int maxPortNumber); //static to be available for all

        /*! \brief Begin the serial.
            \details Function to begin the serial communication with the specified configuration (including timeout).
            \param port the port to be used for the serial communication.
            \param baud the baudrate of the serial communication.
            \param timeout the timeout of the serial communication (in ms).
            \return true if the serial communication starts successfully, false otherwise.
        */
        bool begin(unsigned int port, unsigned int baud, unsigned int timeout);

        /*! \brief Begin the serial.
            \details Function to begin the serial communication with the specified configuration (with a default timeout of 10ms).
            \param port the port to be used for the serial communication.
            \param baud the baudrate of the serial communication.
            \return true if the serial communication starts successfully, false otherwise.
        */
        bool begin(unsigned int port, unsigned int baud);

        /*! \brief Flush the serial.
            \details Function to flush the receive buffer of the serial communication. Serial must be opened before.
        */
        void flush();

        /*! \brief Close the serial.
            \details Function to close the serial communication. Serial must be opened before.
            \return true if the serial communication is closed successfully.
        */
        bool end();

        /*! \brief Send a byte.
            \details Function to send a byte via the serial communication. Serial must be opened before.
            \param c The byte to be sent.
            \return true if the byte is sent successfully, false otherwise.
        */
        bool write(unsigned char c);

        /*! \brief Send a buffer.
            \details Function to send a buffer via the serial communication. Serial must be opened before.
            \param buf The pointer to the buffer to be sent.
            \param len The length of the buffer to be sent (in bytes).
            \return The number of bytes sent (equal to len if all buffer is sent successfully).
        */
        size_t write(unsigned char* buf, size_t len);

        /*! \brief Read a byte.
            \details Function to read a byte via the serial communication. Serial must be opened before.
            \return The byte read (null char if no available byte to read).
        */
        unsigned char read();

        /*! \brief Read a buffer.
            \details Function to read a buffer via the serial communication. Serial must be opened before.
            \param buf The pointer to the buffer where the read bytes are stored.
            \param len The number of bytes to read.
            \return The number of bytes actually read (equal to len if successfully).
        */
        size_t read(unsigned char* buf, size_t len);

        /*! \brief Set the timeout.
            \details Function to set the timeout the serial communication.
            \param timeout The timeout to use (in ms).
        */
        void setTimeout(unsigned int timeout);

        /*! \brief Check if serial is open.
            \details Boolean operator to check if the serial communication has been open successfully.
            \return true if the serial is open, false otherwise.
        */
        operator bool() { return _isInit; }
};
#endif
