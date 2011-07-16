#include <iSpike/Reader/ReaderDescription.hpp>


ReaderDescription&
ReaderDescription::operator=(const ReaderDescription & copy_from_me)
{
	if (this != &copy_from_me) {// protect against invalid self-assignment

		LOG(LOG_DEBUG) << "Assigning reader description for " << copy_from_me.readerName;
		this->readerName = copy_from_me.readerName;
		this->readerDescription = copy_from_me.readerDescription;
		this->readerType = copy_from_me.readerType;
		this->readerProperties = std::map<std::string,Property*>();
		for(std::map<std::string,Property*>::const_iterator iter = copy_from_me.readerProperties.begin();
				iter != copy_from_me.readerProperties.end(); ++iter) {
			Property* newProperty;
			switch(iter->second->getType()) {
				case Property::Integer:
					newProperty = new IntegerProperty(iter->second->getName(), ((IntegerProperty*)(iter->second))->getValue(), iter->second->getDescription(), iter->second->isReadOnly());
					break;
				case Property::Double:
					newProperty = new DoubleProperty(iter->second->getName(), ((DoubleProperty*)(iter->second))->getValue(), iter->second->getDescription(), iter->second->isReadOnly());
					break;
				case Property::String:
					newProperty = new StringProperty(iter->second->getName(), ((StringProperty*)(iter->second))->getValue(), iter->second->getDescription(), iter->second->isReadOnly());
					break;
				case Property::Combo:
					newProperty = new ComboProperty(iter->second->getName(), ((ComboProperty*)(iter->second))->getValue(), iter->second->getDescription(), ((ComboProperty*)(iter->second))->getOptions(), iter->second->isReadOnly());
					break;
			}
			this->readerProperties[iter->first] = newProperty;
		}
	}
	// by convention, always return *this
	return *this;
}



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



ReaderDescription::ReaderDescription(const ReaderDescription& copy_from_me)
{
	LOG(LOG_DEBUG) << "Copying reader description for " << copy_from_me.readerName;
	this->readerName = copy_from_me.readerName;
	this->readerDescription = copy_from_me.readerDescription;
	this->readerType = copy_from_me.readerType;
	this->readerProperties = std::map<std::string,Property*>();
	for(std::map<std::string,Property*>::const_iterator iter = copy_from_me.readerProperties.begin(); iter != copy_from_me.readerProperties.end(); ++iter)
	{
		Property* newProperty;
		switch(iter->second->getType())
		{
			case Property::Integer:
				newProperty = new IntegerProperty(iter->second->getName(), ((IntegerProperty*)(iter->second))->getValue(), iter->second->getDescription(), iter->second->isReadOnly());
				break;
			case Property::Double:
				newProperty = new DoubleProperty(iter->second->getName(), ((DoubleProperty*)(iter->second))->getValue(), iter->second->getDescription(), iter->second->isReadOnly());
				break;
			case Property::String:
				newProperty = new StringProperty(iter->second->getName(), ((StringProperty*)(iter->second))->getValue(), iter->second->getDescription(), iter->second->isReadOnly());
				break;
			case Property::Combo:
				newProperty = new ComboProperty(iter->second->getName(), ((ComboProperty*)(iter->second))->getValue(), iter->second->getDescription(), ((ComboProperty*)(iter->second))->getOptions(), iter->second->isReadOnly());
				break;
		}
		this->readerProperties[iter->first] = newProperty;
	}
}



ReaderDescription::~ReaderDescription()
{
	LOG(LOG_DEBUG) << "Description " << this->getReaderName() << " destroyed!";
	for(std::map<std::string,Property*>::iterator iter = readerProperties.begin(); iter != readerProperties.end(); ++iter)
	{
		delete iter->second;
	}
	readerProperties.clear();
}


