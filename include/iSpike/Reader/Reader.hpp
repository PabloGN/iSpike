#ifndef READER_HPP_
#define READER_HPP_

//iSpike includes
#include "iSpike/iSpikeThread.hpp"
#include "iSpike/Property.hpp"
#include <iSpike/Reader/ReaderDescription.hpp>
#include <iSpike/Log/Log.hpp>

//Other includes
#include <map>
#include <string>
using namespace std;

namespace ispike {

	/** This class represents a Reader, capable of retrieving analogue information
	  * from a predefined source and serving it upon request */
	class Reader : public PropertyHolder {
		public:
			Reader(){ initialized = false; }
			ReaderDescription getReaderDescription() const { return readerDescription; }
			map<string, Property> getProperties() { return propertyMap; }
			void initialize(map<string, Property>& properties) = 0;
			bool isInitialized() { return initialized; }
			void setInitialized(bool initialized) { this->initialized = initialized; }
			void setProperties(map<string, Property>& properties) = 0;

		protected:
			//=======================  VARIABLES  =========================
			/** Description of the reader */
			ReaderDescription readerDescripton;



		private:
			/** Records whether reader has been initialized */
			bool initialized;

	};

}

#endif /* READER_HPP_ */
