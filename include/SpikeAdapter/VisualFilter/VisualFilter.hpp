/*
 * VisualFilter.hpp
 *
 *  Created on: 5 Feb 2011
 *      Author: cembo
 */

#ifndef VISUALFILTER_HPP_
#define VISUALFILTER_HPP_

#include <SpikeAdapter/Bitmap.hpp>

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

  virtual Bitmap getRPlusGMinus() = 0;
  virtual Bitmap getGPlusRMinus() = 0;
  virtual Bitmap getBPlusYMinus() = 0;

};

#endif /* VISUALFILTER_HPP_ */
