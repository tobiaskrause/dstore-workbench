#pragma once

#include <string>
#include <memory>
#include <iostream>
#include <vector>
#include <exception>
#include <FileStore.h>


class NodeNotActiveException : public std::exception
{  
};
/*
 * A logical unit which can holds files or parts of files.
 * It is used to simulate distribution of files over several 
 * nodes. 
 */
class Node
{
 private:
  /*
   * List of files on this node
   */
  std::vector<std::string> _storedData;
  
  /*
   * Name of the storage
   */
  std::string _storeName;

  /*
   * Storage of files
   */
  FileStore _store;

  /*
   * Name of the node
   */
  std::string _nodeName; 

  /*
   * Indicates that this node is currently active
   */
  bool _isActive;
  /*
   * Remove the file storage of the node
   *
   * @return true if storage was successfully removed, otherwise false.
   */
  bool removeFileStore();
  
 public:

  /*
   * Destroy the given node and deletes all data on it. 
   *
   * @param node A reference to the target node
   *
   * @return true if storage was successfully removed, otherwise false.
   */
  static bool Remove(Node& node);

  /*
   * C'tor 
   *
   * @param nodeName name of the node
   */
  Node(const std::string& nodeName);

  /*
   * Copy C'tor 
   * 
   * The class is not copyable
   */
  Node(const Node& node) = delete;

  /*
   * Move C'tor 
   * 
   * The class is not moveable
   */
  Node(const Node&& node) = delete;

  /*
   * D'tor 
   */
  ~Node();

  /*
   * Method to store a file on a node.
   *
   * @param name Name of the file
   * @param data Content of the file
   */
  void put(const std::string& name, std::istream& data);

  /*
   * Method to retieve a file from the node.
   * If the file exists in the node the content of the
   * file is retrieved for the file store.
   *
   * @param name Name of the file
   *
   * @return A data stream of the content of the file. In case
   *         the file doesn't exists in the file store an empty 
   *         stream will be returned.
   */
  std::unique_ptr<std::istream> get(const std::string& name);

  /*
   * Method to get the occupancy of the node
   *
   * @return The number of files stored on this node
   */
  size_t getUsage();

  /*
   * Method to get the name of the node.
   * 
   * @return The name of the node.
   */
  std::string getName();

  /*
   * Indicates that this node is currently active. Once
   * a node becomes inactive it is not possible to become
   * active again.
   * 
   * @return true if actice, false if not
   */
  bool isActive();

  /*
   * Checks if a file with the given file name is stored on 
   * this node. 
   *
   * @param filename Name of the file
   * 
   * @return true if file is stored on the node, otherwise false
   */
  bool containsFile(std::string filename);
};
