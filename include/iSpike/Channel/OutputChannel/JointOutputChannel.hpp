#ifndef JOINTOUTPUTCHANNEL_HPP_
#define JOINTOUTPUTCHANNEL_HPP_

#include <iSpike/Channel/OutputChannel/OutputChannel.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include <queue>
#include <string>
#include <vector>
#include <iSpike/Writer/AngleWriter.hpp>
#include <iSpike/Log/Log.hpp>

namespace ispike {

	/** Encodes joint angles into spikes */
	class JointOutputChannel : public OutputChannel {
		private:
			AngleWriter* writer;
			double minAngle;
			double maxAngle;
			double rateOfDecay;
			double currentAngle;

			/** Array holding the current variables */
			double* variables;

			/*! Number of current variables */
			int numVariables;

			/** The amount by which a current variable is incremented with each spike */
			double currentIncrement;

		protected:
			void updateProperties(std::map<std::string,Property*> properties, bool updateReadOnly);

	public:

		double getCurrentAngle()
		{
			return currentAngle;
		}

		JointOutputChannel();
		~JointOutputChannel();


		void setFiring(std::vector<int>& buffer);

		/**
	   * Initialises the channel
	   */
		void start();

		AngleWriter* getWriter() const
		{
			return writer;
		}

		void setWriter(AngleWriter *writer)
		{
			this->writer = writer;
		}

		void initialise(AngleWriter* writer)
		{
			initialise(writer, channelDescription->getChannelProperties());
		}

		void initialise(AngleWriter* writer, std::map<std::string,Property*> properties);

		boost::shared_ptr<boost::thread> getThreadPointer()
		{
			return threadPointer;
		}

		void setThreadPointer(boost::shared_ptr<boost::thread> threadPointer)
		{
			this->threadPointer = threadPointer;
		}

		bool isInitialised()
		{
			return this->initialised;
		}

		void step();

		void updateProperties(std::map<std::string,Property*> properties)
		{
			if(this->initialised)
			{
				this->stopRequested = true;
				{
					this->wait_condition.notify_all();
				}
				this->threadPointer->join();
				this->threadPointer.reset();
				this->stopRequested = false;
				this->updateProperties(properties, false);
				this->setThreadPointer(boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&JointOutputChannel::workerFunction, this))));
			}
		}

	};

}

#endif /* JOINTOUTPUTCHANNEL_HPP_ */
