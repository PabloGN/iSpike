#ifndef YARPINTERFACE_HPP_
#define YARPINTERFACE_HPP_

#include <string>
#include <boost/asio.hpp>
#include <boost/mpl/vector.hpp>
#include <SpikeAdapter/YarpPortDetails.hpp>
#include <SpikeAdapter/Bitmap.hpp>

using boost::asio::ip::tcp;

/**
 * @class YarpInterface
 * @brief Yarp Interface
 *
 * This is the main interface used for communication with Yarp
 *
 * @author Edgars Lazdins
 *
 */

class YarpInterface {

private:
  std::map<std::string, YarpPortDetails*>* portMap;
  static YarpInterface* pInstance;
  bool initialised;
  tcp::socket* connectionSocket;
  boost::asio::io_service io_service;

protected:
  YarpInterface(std::string ip, std::string port);

public:

  static YarpInterface* Instance()
  {
    return pInstance;
  }

  std::map<std::string, YarpPortDetails*>* getPortMap()
  {
    return portMap;
  }

  static void initialise(std::string ip, std::string port)
  {
    pInstance = new YarpInterface(ip, port);
  }

  /**
   * Read text from a previously connected socket
   * @param socket The socket to read from
   * @return The text that has been read
   */
  std::string read_text();

  /**
   * Read text until a given string is received
   * @param socket The socket to read from
   * @param until The string where we stop
   * @return The text that has been read including the stop characters
   */
  std::string read_until(std::string until);

  /**
   * Write text contents to a previously opened socket
   * @param socket The socket to write to
   * @param message The message to be written
   * @return something
   */
  int write_text(std::string message);

  /**
   * Register a new port with the nameserver?
   */
  std::string register_port(std::string port);

  /**
   * Read binary contents from a previously opened socket
   * as per http://eris.liralab.it/yarpdoc/yarp_protocol.html
   * @param socket A previously opened socket
   * @param buffer Contents will go here
   * @param length How much of the contents to read
   */
  int read_binary(unsigned char* buffer, int length);

  /**
   * Write binary contents to a previously opened socket
   * as per http://eris.liralab.it/yarpdoc/yarp_protocol.html
   * @param socket A previously opened socket
   * @param buffer The contents to write
   */
  int write_binary(unsigned char* buffer, int length);

  /**
   * Connect to a given YARP port
   * @param ip The IP address of the Yarp server
   * @param port The port number to connect to
   * @param socket The socket to use for connection
   * @return true if connected, false otherwise
   */
  int connect_to_port(std::string ip, std::string port);

  /**
   * Disconnect the given socket
   * @param socket The socket to disconnect
   */
  void disconnect();

  /**
   * Reverses the connection to enable text to be read
   * It will no longer be possible to write to this
   * port
   * @param socket The connection to be reversed
   */
  void prepare_to_read_text();

  /**
   * Reverses the connection to enable binary contents to be
   * read. It will no longer be possible to write to this
   * port
   * @param socket The connection to be reversed
   */
  void prepare_to_read_binary();

  Bitmap* read_image();

  int read_data_header();

  int read_int(unsigned char* buf);

  bool getInitialised() const
  {
      return initialised;
  }

};

#endif /* YARPINTERFACE_HPP_ */
