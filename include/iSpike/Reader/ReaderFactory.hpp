/*
 * ReaderFactory.hpp
 *
 *  Created on: 13 Mar 2011
 *      Author: Edgars Lazdins
 */

#ifndef READERFACTORY_HPP_
#define READERFACTORY_HPP_

#include <vector>
#include <iSpike/Reader/Reader.hpp>
#include <iSpike/Reader/AngleReader.hpp>
#include <iSpike/Reader/FileAngleReader.hpp>
#include <iSpike/Reader/FileVisualReader.hpp>
#include <iSpike/Reader/YarpAngleReader.hpp>
#include <iSpike/Reader/YarpVisualReader.hpp>
#include <iSpike/Reader/ReaderDescription.hpp>

/**
 * @class ReaderFactory
 * @brief A Factory of Readers
 *
 * The Reader Factory can list all Readers available in the system and can produce a particular type of a Reader
 */
class ReaderFactory {

	private:
		/// A list of available readers
		std::vector<ReaderDescription> readerList;
		std::string ip;
		std::string port;

	public:
		ReaderFactory();
		ReaderFactory(std::string ip, std::string port);
		std::vector<ReaderDescription> getReadersOfType(std::string readerType);
		Reader* create(std::string readerName, std::map<std::string,Property*> readerProperties );
};


#endif /* READERFACTORY_HPP_ */
