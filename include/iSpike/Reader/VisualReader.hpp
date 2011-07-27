#ifndef VISUALREADER_HPP_
#define VISUALREADER_HPP_

//iSpike includes
#include <iSpike/Reader/Reader.hpp>
#include <iSpike/Bitmap.hpp>

//Other includes
#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>

namespace ispike {

	/** Continuously fetches and retrieves an image upon request */
	class VisualReader : public Reader {
		public:
			VisualReader() { imageID = 0; }

			/** Returns a reference to the image in the visual reader  */
			virtual Bitmap& getBitmap() = 0;
			unsigned getImageID() { return imageID; }


		protected:
			/** Records an ID of the bitmap so that classes connecting to this reader only update
			when image changes */
			unsigned imageID;

	};

}

#endif /* VISUALREADER_HPP_ */
