/*
 * ReaderFactory.cpp
 *
 *  Created on: 13 Mar 2011
 *      Author: cembo
 */
#include <iSpike/Reader/ReaderFactory.hpp>

std::vector<ReaderDescription> ReaderFactory::readerList = ReaderFactory::initialiseReaderList();
