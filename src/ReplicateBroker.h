#pragma once

#include <string>
#include <memory>
#include <Broker.h>
#include <SimpleBroker.h>

/*
 * Structure to map a file to several nodes
 */
class ReplicateDataMap : public DataMap
{
 private:
  /*
   * Vector of all node which stores the file
   */
  std::vector<std::shared_ptr<Node>> _nodes;

 public:

  ReplicateDataMap() = delete;

  /*
   * C'tor 
   *
   * @param filename Name of file
   * @param node Location of the file
   */
  ReplicateDataMap(std::string name, std::shared_ptr<Node> node): DataMap(name) 
  {
   _nodes.push_back(node);
  };

  ReplicateDataMap(const ReplicateDataMap& that) : DataMap(that)
  {
    _nodes = that._nodes;  
  }

  virtual ~ReplicateDataMap() = default;

  /*
   * Tries to return an active node where the file
   * is stored. If not possible a inactive node is
   * returned. 
   *
   * @return If possible the a active node is returned
             where the file can be loaded from. Oterwise
             a inactive node is returned
   */
  virtual std::shared_ptr<Node> node() const
  {
    for (auto iter = _nodes.begin(); iter != _nodes.end(); iter++)
    {
      if ((*iter)->isActive())
      {
	return (*iter);
      }
    }
    return _nodes.front();
  };

  /*
   * Returns all location of the file 
   *
   * @return A vector of Node pointers
   */
  std::vector<std::shared_ptr<Node>> nodes() const
  {
    return _nodes;
  }

  /*
   * Returns all location of the file 
   *
   * @param node Node pointer which is should 
   *             be added to the DataMap
   */
  void addNode(std::shared_ptr<Node> node)
  {
    _nodes.push_back(node);
  }
};

/*
 * Struture to compare file names of DataMaps
 */
struct DataMapNameComparator
{
private:
  std::string _name;
public:
  explicit DataMapNameComparator(std::string& name): _name(name){};
  inline bool operator()(const std::shared_ptr<DataMap> dataMap){ return dataMap->filename() == _name;};
};

/*
 * Enhanced the DataMapVector to add to a DataMap more than one node
 */
class ReplicateDataMapVector : public DataMapVector
{
 public:
  ReplicateDataMapVector(): DataMapVector(){};
   ~ReplicateDataMapVector() = default;
 
   /*
    * This mehod tries to find a existing ReplicateDataMap within
    * the DataMapVector and adds a addional Node to it.
    * Should no entries found for the file its created a new
    * ReplicateDataMap entry.
    *
    * @param name Name of the file
    * @param node Node where the file is stored
    */ 
  void add(std::string name, std::shared_ptr<Node> node)
  {
    auto dataMapIter = std::find_if(begin(), end(), DataMapNameComparator(name));
    if (dataMapIter != end())
    {
      std::shared_ptr<ReplicateDataMap> dataMap = std::static_pointer_cast<ReplicateDataMap>(*dataMapIter);
      dataMap->addNode(node);
    }
    else
    {
      std::shared_ptr<DataMap> repDataMap(new ReplicateDataMap(name, node));
      push_back(repDataMap);
    }
  }
};

/*
 * This class is an enhancement of the SimpleBroker class. This implementation takes
 * the whole file and stores it on two nodes with the smallest occupancy. 
 */
class ReplicateBroker : public SimpleBroker
{
 private:
  /*
   * Number of replicas for each file 
   */
  unsigned _replicas;

  void printNodeWithStatusToStream(std::ostream &stream, std::shared_ptr<Node> node)
  {
    stream << (node->isActive() ? Color::Green : Color::Red);
    stream << node->getName();
    stream << Color::Default;
  };

 public:
  ReplicateBroker() : SimpleBroker(), _replicas(1) {};
  virtual ~ReplicateBroker()= default;
  
  /*
   * Method to distribute the file on nodes. Considering the number
   * of replicas member variable the file is stored multiple times on
   * different nodes.
   *
   * @param nodes A list of nodes for distrute the given file 
   * @param name The file name
   * @param data The content of the file
   * @returns A list of the location of file parts and replicas
   */
  virtual const DataMapVector storeOnNodes(const std::vector<std::shared_ptr<Node>>& nodes,
				   const std::string name, 
				   std::istream& data)
  {
    ReplicateDataMapVector dataMapVector;
    for (unsigned i = 0; i <= _replicas; i++)
    {
      std::shared_ptr<Node> node = storeOnMinUsageNode(nodes, name, data);
      if (node.get() != NULL)
      {
	dataMapVector.add(name,node);
      }
    }
    if (!dataMapVector.empty())
    {
      _dataDistributionMap[name] = dataMapVector;
    }
    return dataMapVector;
  };

  /*
   * Writes details to the file - node mapping to a provided stream.
   * Node names in ...
   *               ... green means that the node is active.
   *               ... red menas that the node is inactive and no more availible
   *                   all data on such are node are lost.
   *
   * @param stream The stream to writes to
   */
  virtual void printFileNodeMappingsToStream(std::ostream &stream)
  {
    auto fileNames = getFileNames();
 
    // Iterate through all files
    for (auto iterFileNames = fileNames.begin(); iterFileNames != fileNames.end(); iterFileNames++)
    {
      auto &filename = *iterFileNames;
      stream << filename << " stored on {";
      auto dataMapList = _dataDistributionMap[filename];

      // Per design of this broker we have only one entry pro file.
      // So we don't need to iterate through the vector
      auto dataMap = std::static_pointer_cast<ReplicateDataMap>(dataMapList.front());
      auto nodeVector = dataMap->nodes();
      // The ReplicateDataMap should have multiple Node entries
      for (auto nodeIter = nodeVector.begin(); nodeIter != nodeVector.end(); nodeIter++)
      {
        printNodeWithStatusToStream(stream, *nodeIter);
	if (nodeIter + 1 != nodeVector.end())
	{
	  stream << " , ";
	}
      } 
        stream << "}\n";
    }
  };
};
