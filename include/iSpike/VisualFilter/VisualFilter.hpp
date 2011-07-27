#ifndef VISUALFILTER_HPP_
#define VISUALFILTER_HPP_

#include <iSpike/Bitmap.hpp>

namespace ispike {
	/** This class represents a Visual Filter */
	class VisualFilter {
		public:
		  virtual Bitmap& getOpponencyMap() = 0;
		  virtual ~VisualFilter();
	};
}

#endif /* VISUALFILTER_HPP_ */
