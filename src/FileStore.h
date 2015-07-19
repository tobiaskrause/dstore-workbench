#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <boost/shared_array.hpp>


/*
 * The class FileEntry holds information of a perticular element inside of a
 * file store. With this information the file can be retrieved from the file store.
 */
class FileEntry
{
 private:
  /*
   * Name of the file entry
   */
  std::string _name;

  /*
   * Position of the file inside the file store steam
   */
  size_t _pos;

  /*
   * The actual size of the file
   */
  size_t _size;

 public:

  /* Default c'tor
   * 
   */
  FileEntry() :
   _name(""),
   _pos(0),
   _size(0) {};

  /*
   * C'tor 
   */
  FileEntry(std::string name, size_t pos, size_t size) :
   _name(name),
   _pos(pos),
   _size(size) {};


  /*
   * Copy c'tor
   */
  FileEntry(const FileEntry& fileEntry)
  {
    _name = fileEntry._name;
    _pos = fileEntry._pos;
    _size = fileEntry._size;
  };

  /*
   * Move c'tor
   */ 
  FileEntry(const FileEntry&& fileEntry)
  {
    _name = std::move(fileEntry._name);
    _pos = fileEntry._pos;
    _size = fileEntry._size;
  };
 
  /*
   * D'tor
   */
  ~FileEntry(){};

 /*
  * Returns the name of the file entry
  *
  * @return Name of the file
  */
 std::string name()
 {
   return _name;
 }

 /*
  * Returns the starting position of the file
  * entry inside the file store stream 
  *
  * @return Position of the file
  */
 size_t pos()
 {
   return _pos;
 }

 /*
  * Returns the size of the file entry
  * inside the file store stream 
  *
  * @return Position of the file
  */
  size_t size()
 {
   return _size;
 }
};

/*
 * Manages the data silo to store file entries.
 * The class FileStore uses a single file to store
 * all file entires.
 * The file store doesn't persistently store the file entry
 * information of the files. This leads to a data lost after the
 * process ends.    
 */
class FileStore
{
 private:
  /*
   * Name of the file store
   */
  std::string _name;

  /*
   * Internal represntation of the file stream.
   * This file stream hold all file entires
   */
  std::fstream _stream;

  /*
   * Manages all file names stored in the store. 
   * Each file name is associated with an instance of FileEntry.
   */
  std::unordered_map<std::string, FileEntry> _entries;

  /*
   * Static method to create a data silo 
   *
   * @param name The name of the file 
   */
  static void createFileIfNotExists(std::string name);
  
  /*
   * Static method to remove the data silo 
   *
   * @param name The name of the file 
   */  
  static void removeFile(std::string name);
  
  /*
   * Checks whether a file is stored in the file store or not. 
   *
   * @param name Name of the file
   *
   * @return true if file exists in file store, otherwise false
   */
  bool hasEntry(std::string name);
  
 public:
  /*
   * Static method to create a data silo of the file store 
   *
   * @param name The name of the file 
   *
   * @return true if data silo was created successfully, otherwise false 
   */
  static bool Create(const std::string name);

  /*
   * Static method to remove a data silo 
   *
   * @param name The name of the file 
   *
   * @return true if data silo was removed successfully, otherwise false 
   */
  static bool Remove(const std::string name);

  /*
   * C'tor 
   * 
   * @param name Name of the file store
   */
  FileStore(const std::string name);

  /*
   * Copy C'tor 
   * 
   * The class is not copyable
   */
  FileStore(const FileStore& fileStore) = delete;
  
  /*
   * Move C'tor 
   * 
   * The class is not moveable
   */
  FileStore(const FileStore&& fileStore) = delete;

  /*
   * D'tor
   */
  ~FileStore();

  /*
   * Method to store a file inside the file store.
   * If the file doesn't exists in the file store the file
   * will be written to the end of data silo. A new
   * FileEntry holds the detail of the new entry. 
   *
   * @param name Name of the file
   * @param data Content of the file
   */
  void put(const std::string name, std::istream& data);

  /*
   * Method to retieve a file from the file store.
   * If the file exists in the file store the content of the
   * file is retrieved for the data silo by the information 
   * of the corresponding FileEntry.
   *
   * @param name Name of the file
   *
   * @return A data stream of the content of the file. In case
   *          the file doesn't exists in the file store an empty 
   *          stream will be returned.
   */
  std::unique_ptr<std::istream> get(const std::string name);
};
