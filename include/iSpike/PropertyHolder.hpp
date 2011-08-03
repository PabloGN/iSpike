#ifndef PROPERTYHOLDER_H
#define PROPERTYHOLDER_H

#include "iSpike/Property.hpp"

#include <map>
#include <string>
using namespace std;

namespace ispike {

	/** Abstract class with functionality to get and set properties */
	class PropertyHolder {
		public:
			/** Returns a map containing the current properties */
			virtual map<string, Property> getProperties(){ return propertyMap; }

			/** Sets the properties */
			virtual void setProperties(map<string, Property>& properties) = 0;

		protected:
			//=======================  VARIABLES  ========================
			void addProperty(Property property);

			//=======================  METHODS  ===========================
			string getComboPropertyValue(string propertyName);
			double getDoublePropertyValue(string propertyName);
			int getIntegerPropertyValue(string propertyName);
			string getStringPropertyValue(string propertyName);
			string getPropertyValue(ComboProperty& property);
			double getPropertyValue(DoubleProperty& property);
			int getPropertyValue(IntegerProperty& property);
			string getPropertyValue(StringProperty& property);
			string updatePropertyValue(ComboProperty& property);
			double updatePropertyValue(DoubleProperty& property);
			int updatePropertyValue(IntegerProperty& property);
			string updatePropertyValue(StringProperty& property);

		protected:
			//=======================  VARIABLES  ========================
			/** Properties */
			map<string, Property> propertyMap;

			//======================  METHODS  ===========================
			void checkName(string& propertyName);
			void checkProperty(Property& property);

	};


}

#endif//PROPERTYHOLDER_H
