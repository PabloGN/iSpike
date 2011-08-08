//iSpike includes
#include "iSpike/ISpikeException.hpp"
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


/** Checks property exists and returns its value if it does */
int PropertyHolder::updateIntegerProperty(Property& property){
	checkProperty(property);
	propertyMap[property.getName()].setInt(property.getInt());
	++updatePropertyCount;
	return property.getInt();
}

/** Checks property exists, updates the property in the map with the value in the supplied property and returns its value if it does */
double PropertyHolder::updateDoubleProperty(Property& property){
	checkProperty(property);
	propertyMap[property.getName()].setDouble(property.getDouble());
	++updatePropertyCount;
	return property.getDouble();
}

/** Checks property exists, updates the property in the map with the value in the supplied property and returns its value if it does */
string PropertyHolder::updateStringProperty(Property& property){
	checkProperty(property);
	propertyMap[property.getName()].setString(property.getString());
	++updatePropertyCount;
	return property.getString();
}

/** Checks property exists, updates the property in the map with the value in the supplied property and returns its value if it does */
string PropertyHolder::updateComboProperty(Property& property){
	checkProperty(property);
	propertyMap[property.getName()].setString(property.getString());
	propertyMap[property.getName()].setOptions(property.getOptions());
	++updatePropertyCount;
	return property.getString();
}


/*--------------------------------------------------------------------*/
/*---------                PROTECTED METHODS                     -------*/
/*--------------------------------------------------------------------*/

/** Checks that property exists and that its name in the map matches its internal name */
void PropertyHolder::checkProperty(Property& property){
	if(propertyMap.count(property.getName()) == 0){
		LOG(LOG_CRITICAL) << "Property does not exist in property map: " << property.getName() <<endl;
		throw ISpikeException("Property not recognized");
	}
}


/** Prints out the properties */
void PropertyHolder::printProperties(){
	for(map<string, Property>::iterator iter = propertyMap.begin(); iter !=propertyMap.end(); ++iter ){
		Property& tmpProp = iter->second;
		cout<<"Name: "<<tmpProp.getName()<<", ";
		if(tmpProp.getType() == Property::Integer)
			cout<<"Type: Integer; value="<<tmpProp.getInt()<<endl;
		else if(tmpProp.getType() == Property::Double)
			cout<<"Type: Double; value="<<tmpProp.getDouble()<<endl;
		else if(tmpProp.getType() == Property::String)
			cout<<"Type: String; value="<<tmpProp.getString()<<endl;
		else if(tmpProp.getType() == Property::Combo){
			cout<<"Type: Combo; value="<<tmpProp.getString()<<"; Options=";
			vector<string> tmpOptions = tmpProp.getOptions();
			for(int i=0; i<tmpOptions.size(); ++i)
				cout<<tmpOptions[i]<<", ";
			cout<<endl;
		}
	}
}
