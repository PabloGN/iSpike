//iSpike includes
#include <iSpike/Log/Log.hpp>
#include <iSpike/NeuronSim/IzhikevichNeuronSim.hpp>
#include <iSpike/Reader/FileVisualReader.hpp>
#include <iSpike/VisualDataReducer/LogPolarVisualDataReducer.hpp>
#include <iSpike/VisualFilter/DOGVisualFilter.hpp>

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
	int numTestRuns = 10;

	// FIRST TEST: Neural Simulator
	if(false){//Switch test on or off
		LOG(LOG_DEBUG)<<"Testing neural simulator";
		resultsFileStream<<"----------------------------------"<<endl<<"Neural Simulator Test"<<endl;

		//Measure performance for different numbers of neurons, logarithmic scale
		for(int numNeur = 100; numNeur<=1000000; numNeur*=10 ){
			resultsFileStream<<"Time (ms) per time step averaged over "<<numTestTimeSteps<<" time steps with "<<numNeur<<" neurons"<<endl;

			//Test runs
			for(int testNum=0; testNum<numTestRuns; ++testNum){

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
				LOG(LOG_DEBUG)<<"Simulator Test "<<testNum<<"; "<<numTestTimeSteps<<" time steps took "<<(timeDur.total_microseconds()/1000.0)<<" ms";
			}
			resultsFileStream<<endl;
		}
		resultsFileStream<<endl;
		resultsFileStream.flush();
	}


	// SECOND TEST: Foveation Performance
	if(false){//Switch test on or off
		LOG(LOG_DEBUG)<<"Testing foveation performance";
		resultsFileStream<<"----------------------------------"<<endl<<"Foveation Performance Test"<<endl;

		numTestTimeSteps = 1000;
		numTestRuns = 10;

		//Measure performance for different numbers of neurons, logarithmic scale
		for(int res = 50; res<=1000; res+=50){

			//Generate file name
			ostringstream imgStr;
			imgStr<<"images/image"<<res<<"x"<<res<<".ppm";
			resultsFileStream<<"Time (ms) per time step averaged over "<<numTestTimeSteps<<" time steps with image file: "<<imgStr.str()<<endl;

			//Create reader, read in image and get bitmap
			FileVisualReader visualReader;
			std::map<std::string, Property> fileVisualReaderProperties = visualReader.getProperties();
			fileVisualReaderProperties.at("File Name") = Property(imgStr.str(), "File Name", "description", true);
			visualReader.initialize(fileVisualReaderProperties);
			Bitmap& bitmap = visualReader.getBitmap();

			//Create log polar visual data reducer
			LogPolarVisualDataReducer visualDataReducer;
			visualDataReducer.setOutputHeight(res);
			visualDataReducer.setOutputWidth(res);
			visualDataReducer.setFoveaRadius(res/4);
			visualDataReducer.setBitmap(bitmap);//First call to this method initializes the class and takes much longer

			//Test runs
			for(int testNum=0; testNum<numTestRuns; ++testNum){

				//Time the required number of time steps
				boost::posix_time::ptime startTime(boost::posix_time::microsec_clock::universal_time());
				for(int i=0; i<numTestTimeSteps; ++i){
					visualDataReducer.setBitmap(bitmap);
					visualDataReducer.getReducedImage();
				}
				boost::posix_time::ptime endTime(boost::posix_time::microsec_clock::universal_time());
				boost::posix_time::time_duration timeDur = endTime - startTime;
				resultsFileStream<<( (double)timeDur.total_microseconds() / (1000.0 * (double)numTestTimeSteps) )<<endl;
				LOG(LOG_DEBUG)<<"Foveation Test "<<testNum<<"; "<<numTestTimeSteps<<" time steps took "<<(timeDur.total_microseconds()/1000.0)<<" ms";
			}
			resultsFileStream<<endl;
		}
		resultsFileStream<<endl;
		resultsFileStream.flush();
	}


	// THIRD TEST: Difference of Gaussians Performance
	if(true){//Switch test on and off
		LOG(LOG_DEBUG)<<"Testing difference of Gaussian performance";
		resultsFileStream<<"----------------------------------"<<endl<<"Difference of Gaussians Test"<<endl;

		numTestTimeSteps = 1000;
		numTestRuns = 10;

		//Measure performance for different numbers of neurons, logarithmic scale
		for(int res = 50; res<=1000; res+=50){

			//Generate file name
			ostringstream imgStr;
			imgStr<<"images/image"<<res<<"x"<<res<<".ppm";
			resultsFileStream<<"Time (ms) per time step averaged over "<<numTestTimeSteps<<" time steps with image file: "<<imgStr.str()<<endl;

			//Create reader, read in image and get bitmap
			FileVisualReader visualReader;
			std::map<std::string, Property> fileVisualReaderProperties = visualReader.getProperties();
			fileVisualReaderProperties.at("File Name") = Property(imgStr.str(), "File Name", "description", true);
			visualReader.initialize(fileVisualReaderProperties);
			Bitmap& bitmap = visualReader.getBitmap();

			//Create log polar visual data reducer - needed by the DOGVisualFilter
			LogPolarVisualDataReducer visualDataReducer;
			visualDataReducer.setOutputHeight(res);
			visualDataReducer.setOutputWidth(res);
			visualDataReducer.setFoveaRadius(res/4);
			visualDataReducer.setBitmap(bitmap);

			//Create the DOG visual filter
			DOGVisualFilter dogVisualFilter(&visualDataReducer);
			dogVisualFilter.setPositiveSigma(2.0);
			dogVisualFilter.setPositiveFactor(4.0);
			dogVisualFilter.setNegativeSigma(4.0);
			dogVisualFilter.setNegativeFactor(2.0);
			dogVisualFilter.setOpponencyTypeID(0);
			dogVisualFilter.update();//First call to this method initializes the class and takes much longer

			//Test runs
			for(int testNum=0; testNum<numTestRuns; ++testNum){

				//Time the required number of time steps
				boost::posix_time::ptime startTime(boost::posix_time::microsec_clock::universal_time());
				for(int i=0; i<numTestTimeSteps; ++i){
					dogVisualFilter.update();
					dogVisualFilter.getBitmap();
				}
				boost::posix_time::ptime endTime(boost::posix_time::microsec_clock::universal_time());
				boost::posix_time::time_duration timeDur = endTime - startTime;
				resultsFileStream<<( (double)timeDur.total_microseconds() / (1000.0 * (double)numTestTimeSteps) )<<endl;
				LOG(LOG_DEBUG)<<"Visual Filter Test ("<<testNum<<") "<<imgStr.str()<<"; "<<numTestTimeSteps<<" time steps took "<<(timeDur.total_microseconds()/1000.0)<<" ms";
			}
			resultsFileStream<<endl;
		}
		resultsFileStream<<endl;
		resultsFileStream.flush();
	}

	//Close results file
	resultsFileStream.close();

}
