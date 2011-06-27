#ifndef ANGLEREADER_HPP_
#define ANGLEREADER_HPP_

#include <iSpike/Reader/Reader.hpp>
#include <vector>
#include <string>
using namespace std;

/**
 * @class AngleReader
 * @brief AngleReader class
 *
 * AngleReader continuously fetches and retrieves joint angles upon request
 *
 * @author Edgars Lazdins
 *
 */
class AngleReader : public Reader {

	public:
		virtual ~AngleReader(){}

		/**
		* Retrieves the angles
		*/
		virtual std::vector<double> getData() = 0;

		/**
		* Initialises the reader and starts the main thread
		*/
		virtual void start() = 0;

		virtual bool getInitialised() const = 0;

		virtual void setInitialised(bool initialised) = 0;

};

#endif /* ANGLEREADER_HPP_ */
