#ifndef CHANNEL_H_
#define CHANNEL_H_

#include <string>
#include <map>
#include <iSpike/Property.hpp>
namespace ispike {

	/** Abstract class holding properties common to all input and output channels */
	class Channel {
		public:
			Channel(){ initialized = false; }
			virtual ~Channel() {}

			/** Retrieves the Channel Identifier  */
			int getId()	{ return this->id; }

			/** Sets the identifier of the Channel */
			void setId(int id){ this->id = id; }

			/** Retrieves the width of the Channel */
			int getWidth(){ return this->width;	}

			/** Sets the width of the Channel */
			void setWidth(int width){ this->width = width; }

			/** Retrieves the height of the Channel  */
			int getHeight(){ return this->height; }

			/** Sets the height of the Channel */
			void setHeight(int height){	this->height = height;	}

			/** Retrieves the description of a Channel  */
			std::string getDescription(){ return this->description;	}

			/** Sets the description of a Channel */
			void setDescription(std::string description){ this->description = description; }

			/** Returns a map containing the current properties */
			virtual map<string, Property> getProperties(){ return propertyMap; }

			/** Sets the properties */
			virtual setProperties(map<string, Property>& properties);

			/** Advances the channel by one time step */
			void step() = 0;


		protected:
			//============================  VARIABLES  =========================
			/** Channel Identifier */
			int id;

			/** Neuron Width */
			int width;

			/** Neuron Height */
			int height;

			/** Channel Description */
			std::string description;

			/** Map holding the current properties */
			map<string, Property> propertyMap;

			/** Map holding new properties, for updating when thread has finished processing the current time step*/
			map<string, Property> newPropertyMap;

			/** Flag to indicate that properties should be updated */
			bool copyProperties;

			/** Records if we are in the middle of a step */
			bool isStepping;

			//=============================  METHODS  ===========================
			bool isInitialised() {return initialized; }

			/** Enables updating channel properties during run-time  */
			virtual void updateProperties(std::map<std::string,Property*> properties) = 0;

			void setInitialized(bool initialized) { this->initialized = initialized; }


		private:
			/** Flag recording whether channel is initialized or not */
			bool initialized;

	};

}

#endif /* CHANNEL_H_ */
