/*
 * WriterFactory.hpp
 *
 *  Created on: 13 Mar 2011
 *      Author: Edgars Lazdins
 */

#ifndef WRITERFACTORY_HPP_
#define WRITERFACTORY_HPP_

#include <vector>
#include <iSpike/Writer/Writer.hpp>
#include <iSpike/Writer/AngleWriter.hpp>
#include <iSpike/Writer/YarpAngleWriter.hpp>
#include <iSpike/Writer/FileAngleWriter.hpp>
#include <iSpike/Writer/WriterDescription.hpp>
#include <iSpike/Property.hpp>

/**
 * @class WriterFactory
 * @brief A Factory of Writers
 *
 * The Writer Factory can list all Writers available in the system and can produce a particular type of a Writer
 */
class WriterFactory {
	private:
		/// A list of available writers
		std::vector<WriterDescription> writerList;
		std::string ip;
		std::string port;

	public:
		WriterFactory();
		WriterFactory(std::string ip, std::string port);
		std::vector<WriterDescription> getWritersOfType(std::string writerType);
		Writer* create(std::string writerName, std::map<std::string,Property*> writerProperties );

};


#endif /* WRITERFACTORY_HPP_ */
