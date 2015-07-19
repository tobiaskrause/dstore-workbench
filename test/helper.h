#pragma once

#include <sstream>
#include <string>
#include <memory>

/*
 * Helper to convert a stream into a string
 */
static std::string getStringFromStream(std::unique_ptr<std::istream>& stream)
{
  std::stringstream stringStream;
  stringStream << stream->rdbuf();
  return stringStream.str();
};

/*
 * Helper to convert a string into a stream
 */
static std::unique_ptr<std::istream> getStreamFromString(const std::string& string)
{
  return std::unique_ptr<std::istream>(new std::stringstream(string));
};

