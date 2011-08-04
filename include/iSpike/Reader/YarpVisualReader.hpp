#ifndef YARPVISUALREADER_HPP_
#define YARPVISUALREADER_HPP_

//iSpike includes
#include <iSpike/Bitmap.hpp>
#include <iSpike/Reader/VisualReader.hpp>
#include <iSpike/YarpConnection.hpp>
#include <iSpike/YarpPortDetails.hpp>

//Other includes
#include <map>

namespace ispike{

/** Retrieves an image from a given yarp port and makes it available upon request */
class YarpVisualReader : public VisualReader {
	public:
		YarpVisualReader(string nameserverIP, unsigned nameserverPort);
		virtual ~YarpVisualReader();
		Bitmap& getBitmap();
		void initialize(map<string, Property>& properties);
		void setProperties(map<string, Property>& properties);
		void start();

	private:
		//=============================  VARIABLES  =========================
		/** Image for loading from YARP and returning */
		Bitmap* bitmap1;

		/** Image for loading from YARP and returning */
		Bitmap* bitmap2;

		/** Controls which bitmap is returned to the user */
		bool returnBitmap1;

		/** Handles connection to YARP  */
		YarpConnection* yarpConnection;

		/** Name of the port details that we connect to */
		string portName;


		//=============================  METHODS  ============================
		void swapBitmap();
		void updateProperties(map<string, Property>& properties);
		void workerFunction();

	};

}

#endif /* YARPVISUALREADER_HPP_ */
