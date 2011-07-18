#include <iSpike/Channel/Channel.hpp>




/** Sets the properties. This will be done immediately if we are not stepping or deferred until the end of the step */
void Channel::setProperties(std::map<std::string,Property> properties){
	if(isStepping){
		newPropertyMap = properties;
		copyProperties = true;
	}
	else{
		if(isInitialized())
			updateProperties(properties, true);
		else
			updateProperties(properties, false);
	}
}

