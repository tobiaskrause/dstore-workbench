#include <iostream>
#include <sstream>
#include <memory>
#include <boost/filesystem.hpp>
#include <FileStore.h>

bool FileStore::Create(const std::string name)
{
  createFileIfNotExists(name);
  return boost::filesystem::exists(name);
}

bool FileStore::Remove(const std::string name)
{
  removeFile(name);
  return !boost::filesystem::exists(name);
}


FileStore::FileStore(const std::string name): 
_name(name)
{
  createFileIfNotExists(name);
  _stream.open(name, std::fstream::in | std::fstream::out |
	       std::fstream::binary);
}

FileStore::~FileStore()
{
  _stream.close();
}

void FileStore::createFileIfNotExists(std::string name)
{
  if(!boost::filesystem::exists(name))
  {
    std::ofstream f(name);
    f.flush();
    f.close();
  }
}

void FileStore::removeFile(std::string name)
{
  boost::filesystem::remove(name);
}

bool FileStore::hasEntry(std::string name)
{
  auto got = _entries.find(name);
  return got != _entries.end();
}

void FileStore::put(const std::string name, std::istream& data)
{
  if (!hasEntry(name))
  {
    // Find the end of the file
    _stream.seekg(0, _stream.end);
    size_t pos = _stream.tellg();
    
    // Read Content data
    data.seekg(0, data.end);
    size_t lenghtOfData = data.tellg();
    data.seekg(0, data.beg);
    char content[lenghtOfData];    
    data.read(content, lenghtOfData);

    // Write content to file store
    _stream.write(content, lenghtOfData);
    
    // Create FileEntry
    FileEntry entry(name, pos, lenghtOfData);
    _entries[name] = entry;
  }
}

std::unique_ptr<std::istream> FileStore::get(const std::string name)
{
  using namespace std;
  string data;
  if (hasEntry(name))
  {
    FileEntry entry = _entries[name];
    char rawData[entry.size()];
    _stream.seekg(entry.pos());
    _stream.read(rawData, entry.size());
    data = std::string(rawData, entry.size());
  }
  return unique_ptr<stringstream> (new stringstream(data));
}
