/*
 * This is an example unit test that doesn't really do anything useful.
 * It is here as a reference for you when creating additional unit tests.
 * For additional reference information, see the "test.h" header.
 */
 
#include "test.h" // Brings in the UnitTest++ framework
#include <iSpike/Reader/FileAngleReader.hpp>
#include <iSpike/Writer/FileAngleWriter.hpp>
#include <iSpike/Channel/InputChannel/JointInputChannel.hpp>
#include <iSpike/Channel/OutputChannel/JointOutputChannel.hpp>
 
 TEST(angleEncodeDecode) // Declares a test named "sanity_check"
 {
	 //Create the Reader
	 FileAngleReader* reader = new FileAngleReader();


	 FileAngleWriter* writer = new FileAngleWriter();
     writer->initialise();
     JointOutputChannel* outputChannel = new JointOutputChannel();
     outputChannel->initialise(writer);
     outputChannel->start();
}
