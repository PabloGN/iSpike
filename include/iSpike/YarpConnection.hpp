#ifndef YARPINTERFACE_HPP_
#define YARPINTERFACE_HPP_

//iSpike includes
#include <iSpike/YarpPortDetails.hpp>
#include <iSpike/Bitmap.hpp>

//Other includes
#include <string>
#include <boost/asio.hpp>
#include <boost/mpl/vector.hpp>
using namespace std;
using boost::asio::ip::tcp;

namespace ispike {

	/** Main interface used for communication with Yarp */
	class YarpConnection {
		public:
			YarpConnection(std::string ip, unsigned port);
			~YarpConnection();

			/** Connect to a given YARP port
		   * @param ip The IP address of the Yarp server
		   * @param port The port number to connect to
		   * @return true if connected, false otherwise  */
			int connect_to_port(std::string ip, unsigned port);

			/** Retrieves a map of Yarp ports available on the nameserver
		   * @return A map of Yarp ports  */
			map<string, YarpPortDetails> getPortMap() {	return portMap; }
			string getSocketString();

			/** Read text until a given string is received
		   * @param until The string where we stop
		   * @return The text that has been read including the stop characters */
			std::string read_until(string until);

			/** Write text contents to a previously opened socket
		   * @param message The message to be written
		   * @return something */
			int write_text(std::string message);

			/** Reverses the connection to enable text to be read
		   * It will no longer be possible to write to this
		   * port  */
			void prepare_to_read_text();

			/** Reverses the connection to enable binary contents to be
		   * read. It will no longer be possible to write to this
		   * port  */
			void prepare_to_read_binary();

			/** Retrieves a bitmap image across a Yarp connection
		   * @return The retrieved Bitmap  */
			Bitmap* read_image();


		private:
			//==========================  VARIABLES  ===========================
			map<string, YarpPortDetails> portMap;
			tcp::socket* connectionSocket;
			boost::asio::io_service io_service;

			//===========================  METHODS  ============================
			void disconnect();
			int read_binary(unsigned char* buffer, int length);
			unsigned char read_char();
			int read_data_header();
			int read_int(unsigned char* buf);
			string read_text();
			int write_binary(unsigned char* buffer, int length);

	};

}

#endif /* YARPINTERFACE_HPP_ */
