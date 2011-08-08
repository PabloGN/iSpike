#ifndef ISPIKEEXCEPTION_HPP_
#define ISPIKEEXCEPTION_HPP_

#include <string>
#include <sstream>
using namespace std;

namespace ispike {

	class ISpikeException : public exception {
		private:
			string description;

		public:
			ISpikeException(string description) throw() {
				this->description = description;
			}
			ISpikeException(string description, int arg1) throw() {
				ostringstream oss;
				oss<<description<<arg1;
				this->description = oss.str();
			}
			~ISpikeException() throw() {}

			const string& msg() const {
				return description;
			}

			const char* what() const throw() {
				return description.c_str();
			}

		};

}

#endif /* ISPIKEEXCEPTION_HPP_ */
