#ifndef YARPINTERFACE_HPP_
#define YARPINTERFACE_HPP_

#include <string>
#include <boost/asio.hpp>
#include <boost/mpl/vector.hpp>
#include <iSpike/YarpPortDetails.hpp>
#include <iSpike/Bitmap.hpp>

using boost::asio::ip::tcp;

/**
 * @class YarpConnection
 * @brief Yarp Connection
 *
 * This is the main interface used for communication with Yarp
 *
 * @author Edgars Lazdins
 *
 */

class YarpConnection {

private:
  std::map<std::string, YarpPortDetails*>* portMap;
  tcp::socket* connectionSocket;
  boost::asio::io_service io_service;

  /**
   * Read text from a previously connected socket
   * @return The text that has been read
   */
  std::string read_text();

  /**
   * Read binary contents from a previously opened socket
   * as per http://eris.liralab.it/yarpdoc/yarp_protocol.html
   * @param buffer Contents will go here
   * @param length How much of the contents to read
   */
  int read_binary(unsigned char* buffer, int length);

  /**
   * Write binary contents to a previously opened socket
   * as per http://eris.liralab.it/yarpdoc/yarp_protocol.html
   * @param buffer The contents to write
   */
  int write_binary(unsigned char* buffer, int length);

  /**
   * Disconnect the given socket
   */
  void disconnect();

  /**
   * Reads a Yarp binary data header
   * @return Size of the header
   */
  int read_data_header();

  /**
   * Reads an integer value from the Yarp connection
   * @return the value read
   */
  int read_int(unsigned char* buf);

public:

  /**
   * Default constructor, creates a new Yarp Connection
   * @param ip IP address of a Yarp nameserver
   * @param port Yarp nameserver port
   */
  YarpConnection(std::string ip, std::string port);

  /**
   * Retrieves a map of Yarp ports available on the nameserver
   * @return A map of Yarp ports
   */
  std::map<std::string, YarpPortDetails*>* getPortMap()
  {
    return portMap;
  }

  /**
   * Read text until a given string is received
   * @param until The string where we stop
   * @return The text that has been read including the stop characters
   */
  std::string read_until(std::string until);

  /**
   * Write text contents to a previously opened socket
   * @param message The message to be written
   * @return something
   */
  int write_text(std::string message);

  /**
   * Connect to a given YARP port
   * @param ip The IP address of the Yarp server
   * @param port The port number to connect to
   * @return true if connected, false otherwise
   */
  int connect_to_port(std::string ip, std::string port);

  /**
   * Reverses the connection to enable text to be read
   * It will no longer be possible to write to this
   * port
   */
  void prepare_to_read_text();

  /**
   * Reverses the connection to enable binary contents to be
   * read. It will no longer be possible to write to this
   * port
   */
  void prepare_to_read_binary();

  /**
   * Retrieves a bitmap image across a Yarp connection
   * @return The retrieved Bitmap
   */
  Bitmap* read_image();

};

#endif /* YARPINTERFACE_HPP_ */
