
#ifndef HOSTPORT_H
#define HOSTPORT_H

#include "Serial.h"
#include <memory> 

/*! \brief A class for host communication via serial port.
    \details Class for host communication via serial port with a specified communication protocol.
    Data packet consists of
    - 4-bytes header
    - data (with specified length)
    - 4-bytes terminator
    No check sum is performed.
    \author Stefano Lovato
    \date 2022
*/
class HostPort {
public:

    /*! \brief Constructor.
		\details Default constructor. 
	*/
    HostPort();

    /*! \brief Desctructor.
		\details Default destructor. It automatically destroy the serial object.
	*/
    ~HostPort();

    /*! \brief Begin the host port.
		\details Function to begin the host port communication with default header, terminator, and timeout.
        \param port The serial port to use.
        \param baud The baudrate of the serial communication.
        \return true if success, false otherwise.
	*/
    bool begin(unsigned int port, unsigned int baud);

    /*! \brief Begin the host port.
		\details Function to begin the host port communication with given header and terminator and default timeout.
        \param port The serial port to use.
        \param baud The baudrate of the serial communication.
        \param header The header to use (as a HEX number of 4-bytes).
        \param terminator The terminator to use (as a HEX number of 4-bytes).
        \return true if success, false otherwise.
	*/
    bool begin(unsigned int port, unsigned int baud, unsigned int header, unsigned int terminator);

    /*! \brief Begin the host port.
		\details Function to begin the host port communication with given header, terminator and timeout.
        \param port The serial port to use.
        \param baud The baudrate of the serial communication.
        \param header The header to use (as a HEX number of 4-bytes).
        \param terminator The terminator to use (as a HEX number of 4-bytes).
        \param timeout The timeout of the serial communcication.
        \return true if success, false otherwise.
	*/
    bool begin(unsigned int port, unsigned int baud, unsigned int header, unsigned int terminator, unsigned int timeout);
    
    /*! \brief Read data buffer.
		\details Function to read a data buffer with given length from the host port.
        Host port must be opened before.
        \param packetPtr The pointer to the data buffer.
        \param size The size of the data buffer.
        \return true if success, false otherwise.
	*/
    bool read(unsigned char* packetPtr, unsigned int size);

    /*! \brief Write data buffer.
		\details Function to write a data buffer with given length to the host port.
        Host port must be opened before.
        \param packetPtr The pointer to the data buffer.
        \param size The size of the data buffer.
        \return true if success, false otherwise.
	*/
    bool write(unsigned char* packetPtr, unsigned int size); 

    /*! \brief Close the host port.
		\details Function to close the host port.
        Host port must be opened before.
        \return true if success, false otherwise.
	*/
    bool close(void); 

    /*! \brief Restart the host port.
		\details Function to restart the host port.
        Host port must be opened before.
        \return true if success, false otherwise.
	*/
    bool restart(void);

    /*! \brief Set the serial port.
		\details Function to set the serial port. The restart function must be called to make the change effective.
        \return true if success, false otherwise.
	*/
    bool setPort(unsigned int port);

    /*! \brief Set the baudrate.
		\details Function to set the baudrate of the serial communiation. The restart function must be called to make the change effective.
        \return true if success, false otherwise.
	*/
    bool setBaud(unsigned int baud); 

    /*! \brief Set the header.
		\details Function to set the header of the host communication. The restart function must be called to make the change effective.
        \return true if success, false otherwise.
	*/
    bool setHeader(unsigned int header); //set the header

    /*! \brief Set the terminator.
		\details Function to set the terminator of the host communication. The restart function must be called to make the change effective.
        \return true if success, false otherwise.
	*/
    bool setTerminator(unsigned int terminator); 

    /*! \brief Get the serial port.
		\details Function to get the serial port used for the host communication. 
        \return The serial port number.
	*/
    unsigned int getPort(void);

    /*! \brief Get the baudate.
		\details Function to get the baudrate used for the serial communication. 
        \return The baudrate.
	*/
    unsigned int getBaud(void);
    
    /*! \brief Get the header.
		\details Function to get the header used for the host communication. 
        \return The header.
	*/
    unsigned int getHeader(void); 

    /*! \brief Get the terminator.
		\details Function to get the terminator used for the host communication. 
        \return The terminator.
	*/
    unsigned int getTerminator(void); 

     /*! \brief Check if host port is open.
        \details Function to check if the host port has been open successfully.
        \return true if the host port is open, false otherwise.
    */
    bool isInit(void); //check if is init
    
    /*! \brief Check if host port is open.
        \details Boolean operator to check if the host port has been open successfully.
        \return true if the host port is open, false otherwise.
    */
    operator bool() { return isInit(); }
    
    static constexpr unsigned int HEADER = 0xFF812345; //!< Default header.
    static constexpr unsigned int TERMINATOR = 0xFF8CABDE; //!< Default terminator.
    static constexpr unsigned int TIMEOUT = 100; //!< Default timeout.

private:
    Serial serial; //!< Serial object.
    unsigned int _port; //!< Serial port.
    unsigned int _baud; //!< Baudrate.
    unsigned int _terminator; //!< Terminator bytes.
    unsigned int _header; //!< Header bytes.
    unsigned int _timeout; //!< Timeout.
    unsigned char _tx_buf[1024]; //!< Tx buffer.
    bool _isFirstRead = false; //!< True if firt packer read.
    bool init(unsigned int port, unsigned int baud, unsigned int timeout); //!< Private initialization function
    static constexpr unsigned int MASK = 0xFF; //!< Mask for parsing.
};

#endif