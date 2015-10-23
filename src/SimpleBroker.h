#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <Broker.h>

/*
 * Structure to map a file to a specific node
 */
class SimpleDataMap : public DataMap
{
  protected:  
   /*
    * Location of the file
    */
   std::shared_ptr<Node> _pNode;

   /*
    * Protected C'tor 
    *
    * @param filename Name of file
    */
   SimpleDataMap(std::string filename) : DataMap(filename) {};
 
  public:

   SimpleDataMap() = delete;
  
   /*
    * C'tor 
    *
    * @param filename Name of file
    * @param pNode Location of the file
    */
   SimpleDataMap(std::string filename, std::shared_ptr<Node> pNode) :
   DataMap(filename), _pNode(pNode)
   {
   };

   SimpleDataMap(const SimpleDataMap& that) : DataMap(that)
   {
     _pNode = that.node();
   };

   virtual ~SimpleDataMap() = default;

   /*
    * Retrieve the node of the file
    *
    * @returns A shared Pointer of the node where the file is located
    */
   virtual std::shared_ptr<Node> node() const
   {
     return _pNode;
   };
};

/*
 * A vector of SimpleDataMap pointers
 */
typedef std::vector<std::shared_ptr<SimpleDataMap>> SimpleDataMapVector;

/*
 * Map of file names and the corresponding nodes where the file parts resides.
 */
typedef std::unordered_map<std::string, DataMapVector> DataDistributionMap; 

/*
 * Implementation of the abstract Broker class. This implementation takes
 * the whole file and stores it on a single node with the smallest occupancy. 
 */
class SimpleBroker : public Broker
{
 private:
  void printNodeWithStatusToStream(std::ostream &stream, std::shared_ptr<Node> node);
  NodeVector sortNodeVectorByUsage(const NodeVector& nodes);


 protected:
  /*
   * Information about the distribution of files
   */
  DataDistributionMap _dataDistributionMap;

  std::vector<std::string> getFileNames();

  /*
   * Stores the file content on the node with the lowest occupancy.
   * File names are unique on each node. If a file name exists on a node
   * the broker tries to store the file on another node.
   * 
   * @param nodes Available nodes
   * @param name  File name
   * @param data  Content of the file
   *
   * @return The node on which the file was stored. The shared pointer points to NULL if the file 
   *         wasn't stored on any node.
   */
  std::shared_ptr<Node> storeOnMinUsageNode(const NodeVector& nodes, 
					    const std::string name,
					    std::istream& data);
 public:
  /*
   * C'tor
   */
  SimpleBroker(): Broker() {};

  /*
   * Copy C'tor
   */
  SimpleBroker(const SimpleBroker& that)
  {
    _dataDistributionMap = that._dataDistributionMap;
  };

  /* 
   * D'tor
   */
  virtual ~SimpleBroker() = default;

  /*
   * Method to distribute the file on the node with the smallest occupancy.
   *
   * @param nodes A list of nodes for distribute the given file 
   * @param name The file name
   * @param data The content of the file
   * @returns A list of the location of file parts
   */
  virtual const DataMapVector storeOnNodes(const NodeVector& nodes, 
					 const std::string name,
					 std::istream& data);

  /*
   * Finds the node of the file and returns it.
   *
   * @param name The file name
   * @returns A list of the location of file parts
   */
  virtual const DataMapVector lookupFileParts(const std::string name);

  /*
   * Writes details to the file - node mapping to a provided stream.
   * Node names in ...
   *               ... green means that the node is active.
   *               ... red means that the node is inactive and no more availible
   *                   all data on such are node are lost.
   *
   * @param stream The stream to writes to
   */
  virtual void printFileNodeMappingsToStream(std::ostream &stream);
};
