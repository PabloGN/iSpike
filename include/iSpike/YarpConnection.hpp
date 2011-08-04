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
			int connect_to_port(std::string ip, unsigned port);
			map<string, YarpPortDetails>& getPortMap() { return portMap; }
			string getSocketString();
			void prepare_to_read_binary();
			void prepare_to_read_text();
			bool read_image(Bitmap& bitmap);
			std::string read_until(string until);
			int write_text(std::string message);


		private:
			//==========================  VARIABLES  ===========================
			/** Map of available ip addresses and ports */
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
