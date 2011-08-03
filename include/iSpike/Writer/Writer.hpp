#ifndef WRITER_HPP_
#define WRITER_HPP_

//iSpike includes
#include "iSpike/Property.hpp"
#include <iSpike/Writer/WriterDescription.hpp>
#include <iSpike/Log/Log.hpp>
#include "iSpike/iSpikeThread.hpp"

//Other includes
#include <map>
#include <string>
using namespace std;

namespace ispike {

	/** This class represents a Writer, capable of writing analogue information
		to a file, socket or other destination. */
	class Writer : public PropertyHolder, public iSpikeThread {
		public:
			Writer(){ initialized = false; }
			virtual ~Writer(){}
			WriterDescription getWriterDescription() const { return writerDescription; }
			virtual void initialize(map<string, Property>& properties) = 0;
			bool isInitialized() { return initialized; }
			void setInitialized(bool initialized) { this->initialized = initialized; }

		protected:
			/** Description of the Writer */
			WriterDescription writerDescripton;


		private:
			/** Records whether Writer has been initialized */
			bool initialized;

	};

}

#endif /* WRITER_HPP_ */
