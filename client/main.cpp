/**
 * a utility to test angle writing capabilities to Yarp
 */

#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#include <iSpike/Reader/ReaderFactory.hpp>
#include <iSpike/Reader/ConsoleAngleReader.hpp>
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
    std::string ip_address;
    std::string port;
    std::string port_name;
    /*
     * Get IP and Port
     */
    std::cout << "Please provide a YARP server ip address:" << std::endl;
    std::cin >> ip_address;
    std::cout << "Please provide the port:" << std::endl;
    std::cin >> port;

    /*
     * Get port name
     */
    std::cout << "Please provide the YARP port name:" << std::endl;
    std::cin >> port_name;

    /*
     * Create writer
     */

    YarpAngleWriter* writer = new YarpAngleWriter(ip_address, port);
    std::map<std::string,Property*> default_properties = writer->getWriterDescription().getWriterProperties();
    StringProperty* portName = new StringProperty(
        default_properties["Port Name"]->getName(),
        port_name,
        default_properties["Port Name"]->getDescription(),
        true
    );
    default_properties["Port Name"] = portName;
    writer->initialise(default_properties);

    /*
     * Create a Joint Output Channel
     */

    JointOutputChannel* outputChannel = new JointOutputChannel();
    outputChannel->initialise(writer);
    outputChannel->start();

    /*
     * Create a Reader
     */

    ConsoleAngleReader* reader = new ConsoleAngleReader();
    reader->initialise();

    /*
     * Create a Joint Input Channel
     */

    JointInputChannel* inputChannel = new JointInputChannel();
    inputChannel->initialise(reader);
    inputChannel->start();

    /*
     * Loop
     */

    while(true)
    {
      inputChannel->step();
      std::vector< std::vector<int> > spikes = inputChannel->getFiring();
      if(!spikes.empty())
        outputChannel->setFiring(&(spikes[0]));
      outputChannel->step();
    }

    return 1;
}
