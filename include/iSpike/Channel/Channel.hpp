#ifndef CHANNEL_H_
#define CHANNEL_H_

#include <string>
#include <map>
#include <iSpike/Property.hpp>

/**
 * @class Channel
 * @brief Channel class
 *
 * This class represents a Channel
 *
 * @author Edgars Lazdins
 *
 */
class Channel {
protected:

  ///Channel Identifier
  int id;

  ///Neuron Width
  int width;

  ///Neuron Height
  int height;

  ///Channel Description
  std::string description;

public:

  virtual ~Channel() = 0;

  /**
   * Retrieves the Channel Identifier
   * @return Channel Identifier
   */
  int getId()
  {
    return this->id;
  }

  /**
   * Sets the identifier of the Channel
   * @param id The new identifier value
   */
  void setId(int id)
  {
    this->id = id;
  };

  /**
   * Retrieves the width of the Channel
   */
  int getWidth(){
    return this->width;
  }

  /**
   * Sets the width of the Channel
   */
  void setWidth(int width){
    this->width = width;
  }

  /**
   * Retrieves the height of the Channel
   */
  int getHeight(){
    return this->height;
  }

  /**
   * Sets the height of the Channel
   */
  void setHeight(int height){
    this->height = height;
  }

  /**
   * Retrieves the description of a Channel
   */
  std::string getDescription(){
    return this->description;
  }

  /**
   * Sets the description of a Channel
   */
  void setDescription(std::string description){
    this->description = description;
  }

  virtual bool isInitialised() = 0;

  virtual void step() = 0;
  //ChannelType getChannelType();
  //void setChannelType(ChannelType channelType);
};

#endif /* CHANNEL_H_ */
