/*
 * VisualFilter.hpp
 *
 *  Created on: 5 Feb 2011
 *      Author: Edgars Lazdins
 */

#ifndef VISUALFILTER_HPP_
#define VISUALFILTER_HPP_

#include <iSpike/Bitmap.hpp>

/**
 * @class VisualFilter
 * @brief Visual Filter class
 *
 * This class represents a Visual Filter
 *
 * @author Edgars Lazdins
 *
 */
class VisualFilter {
private:

public:

  virtual Bitmap getOpponencyMap() = 0;

};

#endif /* VISUALFILTER_HPP_ */
