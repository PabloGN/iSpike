//iSpike includes
#include <iSpike/Log/Log.hpp>
#include <iSpike/NeuronSim/IzhikevichNeuronSim.hpp>

//Boost includes
#include <boost/test/unit_test.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

//Other includes
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
using namespace ispike;

BOOST_AUTO_TEST_CASE(ComponentPerformance){
	//Enable disable test
	//return;

	//Reporting level
	//Log::ReportingLevel() = LOG_ERROR;
	LOG(LOG_DEBUG)<<"Component Performance Test";

	//Results file
	ofstream resultsFileStream;
	resultsFileStream.open("ComponentPerformanceResults.txt", fstream::out);

	//Number of time steps to run each test
	//int numTestTimeSteps = 100000;
	int numTestTimeSteps = 1000;

	//Number of independent test runs
	int numTestRuns = 3;

	// First test: encode angle into spikes using different resolutions
	LOG(LOG_DEBUG)<<"Testing neural simulator";

	//Measure performance for different numbers of neurons, logarithmic scale
	for(int numNeur = 100; numNeur<=100000; numNeur*=5 ){
		resultsFileStream<<"Measuring performance averaged over "<<numTestTimeSteps<<" time steps with "<<numNeur<<" neurons"<<endl;

		//Test runs
		for(int testNum=0; testNum<=numTestRuns; ++testNum){

			//Create neural simulator with appropriate number of neurons
			IzhikevichNeuronSim neuralSim;
			neuralSim.initialize(numNeur);

			//Time the required number of time steps
			boost::posix_time::ptime startTime(boost::posix_time::microsec_clock::universal_time());
			for(int i=0; i<numTestTimeSteps; ++i){
				neuralSim.step();
			}
			boost::posix_time::ptime endTime(boost::posix_time::microsec_clock::universal_time());
			boost::posix_time::time_duration timeDur = endTime - startTime;
			resultsFileStream<<( (double)timeDur.total_microseconds() / (1000.0 * (double)numTestTimeSteps) )<<endl;
			LOG(LOG_DEBUG)<<"Test "<<testNum<<"; "<<numTestTimeSteps<<" time steps took "<<(timeDur.total_microseconds()/1000.0)<<" ms";
		}
		resultsFileStream<<endl;
	}

	resultsFileStream<<endl;
	resultsFileStream.flush();

	//Close results file
	resultsFileStream.close();

}
