#ifndef VISUALDATAREDUCER_HPP_
#define VISUALDATAREDUCER_HPP_

#include <iSpike/Bitmap.hpp>
#include <iSpike/Reader/VisualReader.hpp>

namespace ispike{

	/** This class represents a Visual Data Reducer */
	class VisualDataReducer {
		public:
			virtual ~VisualDataReducer();
			virtual Bitmap& getReducedImage() = 0;

	};

}

#endif /* VISUALDATAREDUCER_HPP_ */
