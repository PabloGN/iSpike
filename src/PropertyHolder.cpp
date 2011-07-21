#include "iSpike/PropertyHolder.hpp"
using namespace ispike;


/*--------------------------------------------------------------------*/
/*---------              PROTECTED METHODS                     -------*/
/*--------------------------------------------------------------------*/

/** Adds a property to the map */
void PropertyHolder::addProperty(Property property){
	if(propertyMap.count(property.getName()) != 0){
		LOG(LOG_CRITICAL) << "Property cannot be added because it already exists: "<< property.getName() <<endl;
		throw ISpikeException("Property cannot be added because it already exists.");
	}
	propertyMap[property.getName()] = property;
}


/*--------------------------------------------------------------------*/
/*---------              PROTECTED METHODS                     -------*/
/*--------------------------------------------------------------------*/

/** Checks property exists and returns its value if it does */
string PropertyHolder::getComboPropertyValue(string propertyName){
	checkProperty(propertyName);
	return getValue((ComboProperty)propertyMap[propertyName]);
}


/** Checks property exists and returns its value if it does */
double PropertyHolder::getDoublePropertyValue(string propertyName){
	checkProperty(propertyName);
	return getValue((DoubleProperty)propertyMap[propertyName]);
}


/** Checks property exists and returns its value if it does */
int PropertyHolder::getIntegerPropertyValue(string propertyName){
	checkProperty(propertyName);
	return getValue((IntegerProperty)propertyMap[propertyName]);
}


/** Checks property exists and returns its value if it does */
string PropertyHolder::getStringPropertyValue(string propertyName){
	checkProperty(propertyName);
	return getValue((StringProperty)propertyMap[propertyName]);
}

/** Checks property exists and returns its value if it does */
string PropertyHolder::getPropertyValue(ComboProperty& property){
	checkProperty(property);
	return property.getValue();
}

/** Checks property exists and returns its value if it does */
double PropertyHolder::getPropertyValue(DoubleProperty& property){
	checkProperty(property);
	return property.getValue();
}

/** Checks property exists and returns its value if it does */
int PropertyHolder::getPropertyValue(IntegerProperty& property){
	checkProperty(property);
	return property.getValue();
}

/** Checks property exists and returns its value if it does */
string PropertyHolder::getPropertyValue(StringProperty& property){
	checkProperty(property);
	return property.getValue();
}

/** Checks property exists and returns its value if it does */
string PropertyHolder::updatePropertyValue(ComboProperty& property){
	checkProperty(property);
	((ComboProperty)propertyMap[property.getName()]).setValue(property.getValue());
	return property.getValue();
}

/** Checks property exists, updates the property in the map with the value in the supplied property and returns its value if it does */
double PropertyHolder::updatePropertyValue(DoubleProperty& property){
	checkProperty(property);
	((DoubleProperty)propertyMap[property.getName()]).setValue(property.getValue());
	return property.getValue();
}

/** Checks property exists, updates the property in the map with the value in the supplied property and returns its value if it does */
int PropertyHolder::updatePropertyValue(IntegerProperty& property){
	checkProperty(property);
	((IntegerProperty)propertyMap[property.getName()]).setValue(property.getValue());
	return property.getValue();
}

/** Checks property exists, updates the property in the map with the value in the supplied property and returns its value if it does */
string PropertyHolder::updatePropertyValue(StringProperty& property){
	checkProperty(property);
	((StringProperty)propertyMap[property.getName()]).setValue(property.getValue());
	return property.getValue();
}


/*--------------------------------------------------------------------*/
/*---------                PRIVATE METHODS                     -------*/
/*--------------------------------------------------------------------*/

/** Checks that property exists and that its name in the map matches its internal name */
void PropertyHolder::checkName(string& propertyName){
	if(propertyMap.count(propertyName) == 0){
		LOG(LOG_CRITICAL) << "Property does not exist: " << propertyName <<endl;
		throw ISpikeException("Property not recognized");
	}

	//Check that property name matches the map name
	std::string paramName = iter->second.getName();
	if(propertyMap[propertyName].getName() != propertyName){
		LOG(LOG_CRITICAL) << "Property name mismatch: " <<propertyMap[propertyName].getName()<<", "<<paramName<<endl;
		throw ISpikeException("Property name mismatch");
	}
}


/** Checks that property exists and that its name in the map matches its internal name */
void PropertyHolder::checkProperty(Property& property){
	if(propertyMap.count(property.getName()) == 0){
		LOG(LOG_CRITICAL) << "Property does not exist: " << property.getName() <<endl;
		throw ISpikeException("Property not recognized");
	}
}
