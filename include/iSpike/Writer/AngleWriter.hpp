/*
 * AngleWriter.hpp
 *
 *  Created on: 9 Mar 2011
 *      Author: Edgars Lazdins
 */

#ifndef ANGLEWRITER_HPP_
#define ANGLEWRITER_HPP_

#include <iSpike/Writer/Writer.hpp>

/**
 * Angle Writer, receives an angle as input and writes it to the designated output
 */
class AngleWriter : public Writer {

public:

  /**
   * Adds an angle to the processing queue
   */
  virtual void addAngle(double angle) = 0;

  /**
   * Initialises the writer and starts the main thread
   */
  virtual void start() = 0;

  virtual bool getInitialised() = 0;

  virtual void setInitialised(bool initialised) = 0;

};

#endif /* ANGLEWRITER_HPP_ */
