//iSpike includes
#include <iSpike/YarpConnection.hpp>
#include <iSpike/YarpPortDetails.hpp>
#include <iSpike/ISpikeException.hpp>
#include <iSpike/Log/Log.hpp>
using namespace ispike;

//Other includes
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <map>
#include <utility>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/mpl/vector.hpp>
using namespace std;
using boost::asio::ip::tcp;

/** Constructor */
YarpConnection::YarpConnection(string ip, unsigned port){
	//boost::asio::io_service io_service;
	LOG(LOG_DEBUG) << "creating a socket";
	this->connectionSocket = new tcp::socket(this->io_service);
	LOG(LOG_DEBUG) << "connecting to port";
	int result = connect_to_port(ip, port);
	LOG(LOG_DEBUG) << "connected";
	if (!result) {
		boost::system::error_code error = boost::asio::error::host_not_found;
		throw boost::system::system_error(error);
	}

	LOG(LOG_DEBUG) << "writing commands";
	write_text("CONNECT foo\n");
	read_until("\n");
	write_text("d\n");
	write_text("list\n");

	boost::system::error_code read_error;
	LOG(LOG_DEBUG) << "reading reply";
	string response_string = read_text();
	LOG(LOG_DEBUG) << "disconnecting";
	disconnect();
	LOG(LOG_DEBUG) << "disconnected";
	boost::cmatch matches;
	boost::regex new_line("\n");
	list<string> lines;
	boost::regex_split(back_inserter(lines), response_string, new_line);
	boost::regex expression("registration name (.+?) ip (.+?) port (.+?) type (.+?)");
	portMap.clear();
	while(lines.size())  {
		string current_string = *(lines.begin());
		lines.pop_front();

		if (boost::regex_match(current_string.c_str(), matches, expression)) {
			string portName(matches[1].first, matches[1].second);
			string ip(matches[2].first, matches[2].second);
			string portStr(matches[3].first, matches[3].second);
			string type(matches[4].first, matches[4].second);
			unsigned port = boost::lexical_cast<unsigned>(portStr.c_str());
			YarpPortDetails details(ip, port, type);
			portMap[portName] = details;
		}
	}

	LOG(LOG_DEBUG) << "YarpConnection: Received the following Yarp portmap:";
	for (map<string, YarpPortDetails>::iterator it=portMap.begin() ; it != portMap.end(); ++it )   {
		LOG(LOG_DEBUG) << (*it).first << " => " << (*it).second.getIp() << ":" << (*it).second.getPort();
	}
}


/** Destructor */
YarpConnection::~YarpConnection(){
}


/*------------------------------------------------------------------*/
/*---------               PUBLIC METHODS                     -------*/
/*------------------------------------------------------------------*/

/** Write text contents to a previously opened socket
* @param message The message to be written
* @return something */
int YarpConnection::write_text(string message){
	boost::system::error_code connection_error;
	boost::asio::write( *this->connectionSocket, boost::asio::buffer(message),
						boost::asio::transfer_all(), connection_error);
	if (connection_error) throw boost::system::system_error(connection_error);
	return(true);
}


string YarpConnection::getSocketString(){
	unsigned int timeoutCtr = 0;
	unsigned int timeout = 10000;
	while(read_char() != 'o' && timeoutCtr < timeout){
		timeoutCtr++;
	}
	if(timeoutCtr >= timeout){
		throw ISpikeException("Timeout exceeded in getSocketString");
	}
	timeoutCtr = 0;
	ostringstream strStream (ostringstream::out);
	unsigned char newChar = this->read_char();
	while(newChar != 'd' && timeoutCtr != timeout)	{
		strStream << newChar;
		newChar = this->read_char();
		++timeoutCtr;
	}
	if(timeoutCtr >= timeout){
		throw ISpikeException("Timeout exceeded in getSocketString");
	}
	return strStream.str();
}


/** Read text until a given string is received
* @param until The string where we stop
* @return The text that has been read including the stop characters */
string YarpConnection::read_until(string until){
	boost::asio::streambuf response;
	boost::asio::read_until(*this->connectionSocket, response, until);
	istream response_stream(&response);
	string response_string((istreambuf_iterator<char>(response_stream)), istreambuf_iterator<char>());
	return(response_string);
}


/** Connect to a given YARP port
* @param ip The IP address of the Yarp server
* @param port The port number to connect to
* @return true if connected, false otherwise  */
int YarpConnection::connect_to_port(string ip, unsigned port){
	boost::asio::io_service io_service;
	boost::asio::ip::tcp::endpoint endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(ip.c_str()), port);
	boost::system::error_code error = boost::asio::error::host_not_found;

	this->connectionSocket->close();
	this->connectionSocket->connect(endpoint, error);

	if (error || !this->connectionSocket->is_open()){
		LOG(LOG_DEBUG) << "Exception thrown connecting to port: "<<error.message();
		throw ISpikeException(error.message());
	}
	return true;
}


/** Reverses the connection to enable text to be read
* It will no longer be possible to write to this
* port  */
void YarpConnection::prepare_to_read_text(){
	write_text("CONNECT foo\n");
	boost::asio::streambuf response;
	boost::asio::read_until(*this->connectionSocket, response, "\n");
	write_text("r\n");
}


/** Reverses the connection to enable binary contents to be
* read. It will no longer be possible to write to this
* port  */
void YarpConnection::prepare_to_read_binary(){
	// Send header to select connection type.
	// this header is for fast_tcp, so we don't have to deal with flow control
	unsigned char hdr[8] = {'Y','A',0x64, 0x1E, 0, 0,'R','P'};
	write_binary(hdr,8);

	// Send name of our port - there is none, so send a name that
	// does not start with "/"
	unsigned char port[8] = {4,0,0,0,'m','i','n',0};
	write_binary(port,8);
	hdr[7] = '\0';

	// Check for acknowledgement
	read_binary(hdr,8);
	if (hdr[7]!='P') {
		throw ISpikeException("Cannot make connection handshake");
	}

	// Send header for payload (a command to reverse connection)
	unsigned char load_hdr[8] = {'Y','A',10, 0, 0, 0,'R','P'};
	write_binary(load_hdr,8);
	unsigned char load_hdr2[10] = { 1, 1, 255, 255, 255, 255, 255, 255, 255, 255 };
	write_binary(load_hdr2,10);
	unsigned char load_len[4] = { 8, 0, 0, 0 };
	write_binary(load_len,4);
	unsigned char reply_len[4] = { 0, 0, 0, 0 };
	write_binary(reply_len,4);

	// send data - this is a command to reverse the connection for reading
	unsigned char command_reverse[8] = { 0, 0, 0, 0, '~', 'r', 0, 1 };
	write_binary(command_reverse,8);
}


/** Reads image data into the supplied bitmap, resizing it if necessary
	Returns true if data has been successfully read, false otherwise. */
bool YarpConnection::read_image(Bitmap& bitmap){
	int result = read_data_header();
	if (result>=0) {
		unsigned char header[4*15];
		int image_len = result - sizeof(header);
		result = read_binary((unsigned char*)header,sizeof(header));
		if (result<0) {
			throw ISpikeException("Failed to read image header");
		}

		//Extract the width, height and depth of the image
		int depth = read_int((unsigned char*)(header+4*8)); // header.depth
		int width = read_int((unsigned char*)(header+4*11)); // header.width
		int height = read_int((unsigned char*)(header+4*12)); // header.height

		//Create new bitmap contents if width, height and depth do not match
		if (bitmap.getWidth() != width || bitmap.getHeight() != height || bitmap.getDepth() != depth) {
			bitmap.reset(width, height, depth);
		}

		//Read into bitmap
		unsigned char* contents = bitmap.getContents();
		read_binary(contents, bitmap.size());
		return true;
	}
	return false;
}


/*------------------------------------------------------------------*/
/*---------              PRIVATE METHODS                     -------*/
/*------------------------------------------------------------------*/

/** Disconnect the given socket */
void YarpConnection::disconnect(){
	write_text("q\n");
	this->connectionSocket->close();
}


/** Read binary contents from a previously opened socket as per http://eris.liralab.it/yarpdoc/yarp_protocol.html
* @param buffer Contents will go here
* @param length How much of the contents to read    */
int YarpConnection::read_binary(unsigned char* buffer, int length){
	return boost::asio::read(*this->connectionSocket, boost::asio::buffer(buffer, length));
}


unsigned char YarpConnection::read_char(){
	vector<unsigned char> buf(1);
	try	{
		size_t len = read(*this->connectionSocket, boost::asio::buffer(buf));
		assert(len == 1);
		//cout << buf[0];
		return buf[0];
		// process the 4 bytes in buf
	}
	catch (...)	{
		throw new ISpikeException("Error in read_char!" );
	}
}


/**  Reads a Yarp binary data header */
int YarpConnection::read_data_header(){
	int i;
	unsigned char load_hdr_ref[8] = {'Y','A',10, 0, 0, 0,'R','P'};
	unsigned char load_hdr[8] = {0,0,0,0,0,0,0,0};
	read_binary(load_hdr,8);
	for (i=0; i<8; i++) {
		if (load_hdr[i]!=load_hdr_ref[i]) {
			throw ISpikeException("Unexpected data received");
		}
	}
	unsigned char load_hdr2[10];
	load_hdr2[1] = 0;
	read_binary(load_hdr2,10);
	if (load_hdr2[1]!=1) {
		throw ISpikeException("Corrupt data received");
	}
	int blocks = load_hdr2[0];
	int len = 0;
	unsigned char load_len[4];
	unsigned int port_message_len = 0;
	for (i=0; i<blocks; i++) {
		read_binary((unsigned char *)load_len,4);
		len += load_len[0] + (load_len[1]<<8) + (load_len[2]<<16) +
				(load_len[3]<<24);
		if (i==0) {
			port_message_len = len;
		}
	}
	read_binary((unsigned char *)load_len,4);
	for (i=0; i<4; i++) {
		if (load_len[i]!=0)
			throw ISpikeException("Unexpected lengths received");
	}

	// extract the port header part
	unsigned char command_header[8];
	command_header[4] = '\0';
	read_binary(command_header,8);
	len -= 8;
	port_message_len -= 8;
	if (command_header[4]!='~') {
		throw ISpikeException("Unexpected port command received");
	}
	unsigned char cmd[256] = "?";
	cmd[0] = command_header[5];
	if (cmd[0]=='\0') {
		if (port_message_len>sizeof(cmd)) {
			throw ISpikeException("Port command too big\n");
		}
		read_binary(cmd,port_message_len);
		len -= port_message_len;
	}
	if (cmd[0]!='d') {
		throw ISpikeException("Unexpected port command!");
	}

	return len;
}


/** Reads an integer value from the Yarp connection */
int YarpConnection::read_int(unsigned char* buf){
	unsigned char *ubuf = (unsigned char *)buf;
	// this could be optimized away on little-endian machines!
	return ubuf[0] + (ubuf[1]<<8) + (ubuf[2]<<16) + (ubuf[3]<<24);
}


/** Read text from a previously connected socket
* @return The text that has been read   */
string YarpConnection::read_text(){
	return read_until("*** end of message");
}


/** Write binary contents to a previously opened socket as per http://eris.liralab.it/yarpdoc/yarp_protocol.html */
int YarpConnection::write_binary(unsigned char* buffer, int length){
	boost::system::error_code connection_error;
	boost::asio::write( *this->connectionSocket, boost::asio::buffer(buffer, length),
						boost::asio::transfer_all(), connection_error);
	if (connection_error) throw boost::system::system_error(connection_error);
	return(true);
}
