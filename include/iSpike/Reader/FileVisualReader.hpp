#ifndef FILEVISUALREADER_HPP_
#define FILEVISUALREADER_HPP_

#include <iSpike/Reader/VisualReader.hpp>
#include <iSpike/Bitmap.hpp>

namespace ispike {

	/** Retrieves an image from a file and outputs it on request */
	class FileVisualReader : public VisualReader {

		public:

			FileVisualReader();
			~FileVisualReader();
			Bitmap& getBitmap();
			void initialize(map<string, Property>& properties);
			void setProperties(map<string, Property>& properties);
			void start() {} //Required by iSpikeThread, but not used by this class

		private:

			//====================  VARIABLES  ======================
			/** Bitmap that holds data from file */
			Bitmap bitmap;

			//====================  METHODS  ========================
			void readPPMImage(string& fileName);
			void workerFunction() {} //Required by iSpikeThread, but not used by this class

	};

}

#endif /* FILEVISUALREADER_HPP_ */
