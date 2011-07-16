#include <iSpike/Reader/ReaderDescription.hpp>


ReaderDescription::ReaderDescription(std::string readerName,
		std::string readerDescription,
		std::string readerType,
		const property_map& readerProperties) :
	readerName(readerName),
	readerDescription(readerDescription),
	readerType(readerType),
	readerProperties(readerProperties)
{
	;
}
