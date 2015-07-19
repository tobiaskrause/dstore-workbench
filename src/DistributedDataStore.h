#pragma once

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <Node.h>
#include <SimpleBroker.h>

/*
 * Class to simulate and manage a network of nodes to distribute files.
 */
class DistributedDataStore
{
 private:
  /*
   * Variable to generate uniuqe node ids
   */
  unsigned _nodeNameIndex;

  /*
   * List of nodes
   */
  std::vector<std::shared_ptr<Node>> _nodes;
  
  /*
   * The broker is used to distribute files over serveral nodes
   */
  Broker& _broker;

 public:

  /*
   * Removed C'tor 
   */
  DistributedDataStore() = delete;

  /*
   * C'tor
   *
   * @param broker Determine which broker mechanismn is used 
   */
  DistributedDataStore(Broker& broker): _nodeNameIndex(0), _broker(broker){};
  
  /*
   * D'tor
   */
  ~DistributedDataStore(){};

  /*
   * Add a node to the network
   *
   * @return The new node.
   */
  std::shared_ptr<Node> addNode();
  
  /*
   * Removes a particular node from the network.
   *
   * @param name Name of the node
   */
  void removeNode(const std::string name);

  /*
   * Removes all node from the network
   */
  void removeAllNodes();

  /*
   * Returns all node names of the network.
   *
   * @return A list of node names
   */
  std::vector<std::string> listNodeNames(); 

  /*
   * Method to store a file on the network. 
   *
   * @param name Name of the file
   * @param data Content of the file
   *
   * @return true if file could be successfully stored on the network. Otherwise false
   */
  bool store(const std::string& name, std::istream& data);

  /*
   * Method to retieve a file from the network.
   * @param name Name of the file
   *
   * @return A data stream of the content of the file. In case
   *          the file doesn't exists in the file store an empty 
   *          stream will be returned.
   */
  std::unique_ptr<std::istream> load(const std::string& name);

  /*
   * Display information about the status of the network
   */
  void showStatus();
};

