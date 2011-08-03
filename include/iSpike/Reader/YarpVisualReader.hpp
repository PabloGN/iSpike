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
		virtual ~YarpVisualReader();
		Bitmap& getBitmap();
		void initialize(map<string, Property>& properties);
		void start();

	private:
		//=============================  VARIABLES  =========================
		/** Image for loading from YARP and returning */
		Bitmap* bitmap1;

		/** Image for loading from YARP and returning */
		Bitmap* bitmap2;

		/** Controls which bitmap is returned to the user */
		bool returnBitmap1;

		YarpConnection* yarpConnection;

		map<string, YarpPortDetails> portMap;


		//=============================  METHODS  ============================
		void swapBitmap();
		void workerFunction();

	};

}

#endif /* YARPVISUALREADER_HPP_ */
