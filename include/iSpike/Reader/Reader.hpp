#ifndef READER_HPP_
#define READER_HPP_

//iSpike includes
#include "iSpike/ISpikeThread.hpp"
#include "iSpike/PropertyHolder.hpp"
#include <iSpike/Description.hpp>

//Other includes
#include <map>
#include <string>
using namespace std;

namespace ispike {

	/** This class represents a Reader, capable of retrieving analogue information
	  * from a predefined source and serving it upon request */
	class Reader : public PropertyHolder, public ISpikeThread {
		public:
			Reader() : initialized(false) { }
			virtual ~Reader(){}
			Description getReaderDescription() const { return readerDescription; }
			virtual void initialize(map<string, Property>& properties) = 0;
			bool isInitialized() { return initialized; }
			void setInitialized(bool initialized) { this->initialized = initialized; }

		protected:
			//=======================  VARIABLES  =========================
			/** Description of the reader */
			Description readerDescription;


		private:
			/** Records whether reader has been initialized */
			bool initialized;

	};

}

#endif /* READER_HPP_ */
