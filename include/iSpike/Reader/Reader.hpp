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
	class Reader : public PropertyHolder, public iSpikeThread {
		public:
			Reader(){ initialized = false; }
			virtual ~Reader(){}
			ReaderDescription getReaderDescription() const { return readerDescripton; }
			virtual void initialize(map<string, Property>& properties) = 0;
			bool isInitialized() { return initialized; }
			void setInitialized(bool initialized) { this->initialized = initialized; }

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
