#ifndef ISPIKEEXCEPTION_HPP_
#define ISPIKEEXCEPTION_HPP_

#include <string>
using namespace std;

namespace ispike {

	class ISpikeException : public exception {
		private:
			string description;

		public:
			ISpikeException(string description) throw() {
				this->description = description;
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
