#ifndef PROPERTY_HPP_
#define PROPERTY_HPP_

#include <map>
#include <string>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <iSpike/Log/Log.hpp>
using namespace std;

namespace ispike {

	/** Describes a general Channel, Reader or Writer Property */
	class Property {
		public:
			enum ValueType {
				Integer,
				Double,
				String,
				Combo
			};

		protected:
			ValueType type;
			string name;
			string description;
			bool readOnly;

		public:
			Property(ValueType type, string name, string description, bool readOnly);
			Property(const Property& prop);
			virtual ~Property();
			virtual string toString() { return ""; }
			ValueType getType() { return this->type; }
			string getName() {	return this->name; }
			string getDescription() { return this->description; }
			bool isReadOnly() { return this->readOnly; }

	};


	/** Describer an Integer Property */
	class IntegerProperty : public Property{
		private:
			int value;

		public:
			IntegerProperty(int paramValue, string paramName, string paramDescription, bool readOnly);
			IntegerProperty(const IntegerProperty& intProp);
			string toString() { return boost::lexical_cast<string>(this->value); }
			int getValue() { return this->value; }
			void setValue(int value) { this->value = value;	}
			IntegerProperty& operator=(const IntegerProperty& rhs);
	};


	/** Describes a Double Property */
	class DoubleProperty : public Property {
		private:
			double value;

		public:
			DoubleProperty(double paramValue, string paramName, string paramDescription, bool readOnly);
			DoubleProperty(const DoubleProperty& doubProp);
			string toString() { return boost::lexical_cast<string>(this->value); }
			double getValue() { return this->value; }
			void setValue(double value) { this->value = value;	}
			DoubleProperty& operator=(const DoubleProperty& rhs);
	};



	/** Describes a String Property */
	class StringProperty : public Property {
		private:
			string value;

		public:
			StringProperty(string paramValue, string paramName, string paramDescription, bool readOnly);
			StringProperty(const StringProperty& doubProp);
			std::string toString() { return value; }
			string getValue() { return value; }
			void setValue(string value) { this->value = value;	}
			StringProperty& operator=(const StringProperty& rhs);
	};


	/** Describes a Combo Property */
	class ComboProperty : public Property {
		private:
			string value;
			vector<string> options;

		public:
			ComboProperty(vector<string> paramOptions, string paramValue, string paramName, string paramDescription, bool readOnly);
			ComboProperty(const ComboProperty& doubProp);
			std::string toString() { return boost::lexical_cast<std::string>(this->value); }
			string getValue() { return this->value; }
			void setValue(string value) { this->value = value;	}
			ComboProperty& operator=(const ComboProperty& rhs);
	};

}



#endif /* PROPERTY_HPP_ */
