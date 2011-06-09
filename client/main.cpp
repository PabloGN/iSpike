/**
 * iSpike test client, allows creating a single input or output channel and then runs it indefinitely
 */

#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#include <iSpike/Reader/ReaderFactory.hpp>
#include <iSpike/Reader/FileAngleReader.hpp>
#include <iSpike/Reader/ReaderDescription.hpp>
#include <iSpike/Property.hpp>
#include <iSpike/Channel/InputChannel/InputChannelFactory.hpp>
#include <iSpike/Channel/InputChannel/JointInputChannel.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <fstream>
#include <iSpike/Channel/OutputChannel/OutputChannelDescription.hpp>
#include <iSpike/Channel/OutputChannel/OutputChannelFactory.hpp>
#include <iSpike/Channel/OutputChannel/OutputChannel.hpp>
#include <iSpike/Writer/WriterFactory.hpp>
#include <iSpike/Writer/WriterDescription.hpp>
#include <iSpike/Writer/Writer.hpp>
#include <iSpike/Common.hpp>
#include <iSpike/Log/Log.hpp>

int main(int argc, char* argv[])
{
    Log::ReportingLevel() = LOG_DEBUG;

    std::cout << "Select the type of Channel you want to create" << std::endl;
    std::cout << "(1)Input Channel (2)Output Channel" << std::endl;
    int typeOfChannel;
    LOG(LOG_DEBUG) << "here";
    std::cin >> typeOfChannel;
    std::cin.ignore();
    Channel* createdChannel;
    if(typeOfChannel == 1)
    {
      LOG(LOG_DEBUG) << "and here";
      InputChannelFactory channelFactory;
      LOG(LOG_DEBUG) << "then here";
      //get all input channels and output them to the console
      std::vector<InputChannelDescription> inputChannelDescriptions = channelFactory.getAllChannels();
      std::cout << "Available Input Channels:" << std::endl;
      for(int i = 0; i < inputChannelDescriptions.size(); i++)
      {
        std::cout << i << ": " << inputChannelDescriptions[i].getChannelName() << ": " << inputChannelDescriptions[i].getChannelDescription() << std::endl;
      }

      //let the user pick a channel
      std::cout << "Please select a channel:" << std::endl;
      int selectedChannel = -1;
      while(selectedChannel < 0 || selectedChannel > (inputChannelDescriptions.size() - 1))
      {
        std::cin >> selectedChannel;
      }
      std::cin.ignore();

      std::cout << "Would you like to connect to a Yarp server? (y/n)" << std::endl;
      std::string useYarp;
      std::cin >> useYarp;
      ReaderFactory readerFactory;
      if (useYarp.compare("y") == 0)
      {
        std::string ip;
        std::string port;
        std::cout << "IP Address:" << std::endl;
        std::cin >> ip;
        std::cout << "Port:" << std::endl;
        std::cin >> port;
        readerFactory = ReaderFactory(ip, port);
      } else {
        readerFactory =  ReaderFactory();
      }
      LOG(LOG_DEBUG) << "readerFactory created";

      //get the properties for that channel and let the user provide values
      std::map<std::string,Property*> channelProperties = inputChannelDescriptions[selectedChannel].getChannelProperties();
      std::map<std::string, Property*> constructedChannelProperties = Common::getProperties(channelProperties);

      //now find all Readers that are supported by that channel
      std::string readerType = inputChannelDescriptions[selectedChannel].getReaderType();
      std::vector<ReaderDescription> readerDescriptions = readerFactory.getReadersOfType(readerType);
      std::cout << "Available Readers for this Channel:" << std::endl;
      for(int i = 0; i < readerDescriptions.size(); i++)
      {
        std::cout << i << ": " << readerDescriptions[i].getReaderName() << ": " << readerDescriptions[i].getReaderDescription() << std::endl;
      }

      //let the user pick a reader
      std::cout << "Please select a reader:" << std::endl;
      int selectedReader = -1;
      while(selectedReader < 0 || selectedReader > (readerDescriptions.size() - 1))
      {
        std::cin >> selectedReader;
      }
      std::cin.ignore();

      //get the properties for that reader and let the user provide values
      std::map<std::string,Property*> readerProperties = readerDescriptions[selectedReader].getReaderProperties();
      std::map<std::string, Property*> constructedReaderProperties = Common::getProperties(readerProperties);
      LOG(LOG_DEBUG) << "Creating the reader";
      Reader* reader = readerFactory.create(readerDescriptions[selectedReader].getReaderName(), constructedReaderProperties);
      LOG(LOG_DEBUG) << "Creating the channel";
      InputChannel* channel = channelFactory.create(inputChannelDescriptions[selectedChannel].getChannelName(), reader, constructedChannelProperties);
      LOG(LOG_DEBUG) << "Starting the channel";
      channel->start();
      LOG(LOG_DEBUG) << "Started";
      createdChannel = channel;
    } else if(typeOfChannel == 2)
    {
      LOG(LOG_DEBUG) << "and here";
      OutputChannelFactory outputFactory;
      WriterFactory writerFactory;
      LOG(LOG_DEBUG) << "then here";
      //get all output channels and output them to the console
      std::vector<OutputChannelDescription> outputChannelDescriptions = outputFactory.getAllChannels();
      std::cout << "Available Output Channels:" << std::endl;
      for(int i = 0; i < outputChannelDescriptions.size(); i++)
      {
        std::cout << i << ": " << outputChannelDescriptions[i].getChannelName() << ": " << outputChannelDescriptions[i].getChannelDescription() << std::endl;
      }

      //let the user pick a channel
      std::cout << "Please select a channel:" << std::endl;
      int selectedChannel = -1;
      while(selectedChannel < 0 || selectedChannel > (outputChannelDescriptions.size() - 1))
      {
        std::cin >> selectedChannel;
      }
      std::cin.ignore();

      //get the properties for that channel and let the user provide values
      std::map<std::string,Property*> channelProperties = outputChannelDescriptions[selectedChannel].getChannelProperties();
      std::map<std::string, Property*> constructedChannelProperties = Common::getProperties(channelProperties);

      //now find all Writers that are supported by that channel
      std::string writerType = outputChannelDescriptions[selectedChannel].getWriterType();
      std::vector<WriterDescription> writerDescriptions = writerFactory.getWritersOfType(writerType);
      std::cout << "Available Writers for this Channel:" << std::endl;
      for(int i = 0; i < writerDescriptions.size(); i++)
      {
        std::cout << i << ": " << writerDescriptions[i].getWriterName() << ": " << writerDescriptions[i].getWriterDescription() << std::endl;
      }

      //let the user pick a writer
      std::cout << "Please select a writer:" << std::endl;
      int selectedWriter = -1;
      while(selectedWriter < 0 || selectedWriter > (writerDescriptions.size() - 1))
      {
        std::cin >> selectedWriter;
      }
      std::cin.ignore();

      //get the properties for that reader and let the user provide values
      std::map<std::string,Property*> writerProperties = writerDescriptions[selectedWriter].getWriterProperties();
      std::map<std::string, Property*> constructedWriterProperties = Common::getProperties(writerProperties);

      Writer* writer = writerFactory.create(writerDescriptions[selectedWriter].getWriterName(), constructedWriterProperties);
      OutputChannel* channel = outputFactory.create(outputChannelDescriptions[selectedChannel].getChannelName(), writer, constructedChannelProperties);
      channel->start();
      createdChannel = channel;
    }
    int numOfNeurons = 5;
    while(true)
    {
      boost::this_thread::sleep(boost::posix_time::milliseconds(200));
      createdChannel->step();
      if(typeOfChannel == 1)
      {
        std::vector<std::vector<int> > firings = ((InputChannel*)createdChannel)->getFiring();
        std::ostringstream spikedNeurons;
        spikedNeurons << "Spiked Neurons: [";
         if (!firings.empty())
         {
       for(unsigned int i = 0; i < firings.front().size(); i++)
       {
         spikedNeurons << firings.front().at(i) << ",";
       }
       spikedNeurons << "]";
       LOG(LOG_DEBUG) << spikedNeurons.str();
       Common::writePatternToFile("spikes.txt", firings.front(), numOfNeurons);
       std::vector<int> something = *(firings.begin());
       //outputChannel->setFiring(&something);
         }
      }
      else
      {

      }
    }
    return 1;
}
