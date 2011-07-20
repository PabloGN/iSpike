#ifndef YARPVISUALREADER_HPP_
#define YARPVISUALREADER_HPP_

#include <iSpike/Reader/VisualReader.hpp>
#include <iSpike/YarpConnection.hpp>
#include <iSpike/Bitmap.hpp>
#include <boost/thread.hpp>
#include <map>
#include <iSpike/YarpPortDetails.hpp>

namespace ispike{

/** Retrieves an image from a given yarp port and makes it available upon request */
class YarpVisualReader : public VisualReader {
	public:
		YarpVisualReader(string nameserverIP, unsigned nameserverPort);
		~YarpVisualReader();
		Bitmap getData();
		void initialise(map<string, Property>& properties);
		void start();

	private:
		//=============================  VARIABLES  =========================
		Bitmap* buffer;
		string portName;
		YarpConnection* yarpConnection;
		map<string, YarpPortDetails> portMap;


		//=============================  METHODS  ============================
		void workerFunction();

	};

}

#endif /* YARPVISUALREADER_HPP_ */
