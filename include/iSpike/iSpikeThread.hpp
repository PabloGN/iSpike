#ifndef ISPIKETHREAD_HPP
#define ISPIKETHREAD_HPP

#include <iSpike/Log/Log.hpp>

namespace ispike {

	/*! Methods common to all threads in iSpike along with error setting and retrieval */
	class iSpikeThread {

		public:
			/** Constructor */
			iSpikeThread(){
				errorFlag = false;
				running = false;
				stopRequested = false;
				errorMessage = "";
			}

			/*! Clears the error in the thread */
			virtual void clearError(){ errorFlag = false; errorMessage = ""; }

			/*! Returns the message associated with an error */
			virtual string getErrorMessage() { return errorMessage; }

			/*! Returns true if an error has occurred */
			virtual bool isError() { return errorFlag; }

			/*! Returns true if thread is running */
			virtual bool isRunning() { return running; }

			/*! Returns true if thread has been requested to stop. */
			virtual bool isStopRequested() { return stopRequested; }

			/*! Asks thread to stop running */
			virtual void requestStop() { stopRequested = true; }

			/*! Sets an error in the thread */
			virtual void setError(string errorMsg) { errorFlag = true; errorMessage = errorMsg; LOG(LOG_CRITICAL)<<"Thread error: "<< errorMsg; }

			/*! Sets running state to true */
			void setRunning(bool running) { this->running = running; stopRequested = !running; }

			/*!  Initialises the channel and starts the conversion thread  */
			virtual void start() = 0;


		private:
			/*! Records if an error has taken place in the thread.  */
			bool errorFlag;

			/*! Holds a message associated with an error in the thread */
			string errorMessage;

			/*! Flag to record whether thread is running */
			bool running;

			/*! Controls running of thread */
			bool stopRequested;

	};



}

#endif//ISPIKETHREAD_HPP