#ifndef CHANNEL_H_
#define CHANNEL_H_

//iSpike includes
#include <iSpike/PropertyHolder.hpp>
#include <iSpike/Log/Log.hpp>

//Other includes
#include <string>
#include <map>
using namespace std;

namespace ispike {

	/** Abstract class holding properties common to all input and output channels */
	class Channel : public PropertyHolder {
		public:
			Channel(){ initialized = false; LOG(LOG_DEBUG) << "CHANNEL CONSTRUCTOR";}
			virtual ~Channel();

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

			/** Returns the size of the channel */
			int size() { return width*height; }

			/** Retrieves the description of a Channel  */
			string getDescription(){ return this->description;	}

			/** Sets the description of a Channel */
			void setDescription(std::string description){ this->description = description; }

			/** Advances the channel by one time step */
			virtual void step() = 0;


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


			//=============================  METHODS  ===========================
			bool isInitialized() {return initialized; }
			void setInitialized(bool initialized) { this->initialized = initialized; }


		private:
			/** Flag recording whether channel is initialized or not */
			bool initialized;

	};

}

#endif /* CHANNEL_H_ */
