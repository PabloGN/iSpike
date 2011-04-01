/*
 * ISpikeException.hpp
 *
 *  Created on: 24 Mar 2011
 *      Author: Edgars
 */

#ifndef ISPIKEEXCEPTION_HPP_
#define ISPIKEEXCEPTION_HPP_

#include <string>
class ISpikeException : public std::exception
{
private:
    std::string description;
public:
	ISpikeException(std::string description) throw()
	{
		this->description = description;
	}

	const std::string& Msg() const {
	    return description;
	}

	const char* what() const throw() {
	    return description.c_str();
	}


    ~ISpikeException() throw() {}
};

#endif /* ISPIKEEXCEPTION_HPP_ */
