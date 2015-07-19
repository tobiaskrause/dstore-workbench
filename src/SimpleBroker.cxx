#include <vector>
#include <string>
#include <memory>
#include <SimpleBroker.h>

/*
 * Sort function to sort a list (ascending) with respect on the node usage.
 */
bool sortByUsage(std::shared_ptr<Node> a, std::shared_ptr<Node> b)
{
  return a->getUsage() < b->getUsage();
};

NodeVector SimpleBroker::sortNodeVectorByUsage(const NodeVector& nodes)
{
  std::vector<std::shared_ptr<Node>> nodesByOccupancy = nodes;
  std::sort(nodesByOccupancy.begin(), nodesByOccupancy.end(), sortByUsage);
  return nodesByOccupancy;
}

std::shared_ptr<Node> SimpleBroker::storeOnMinUsageNode(const NodeVector& nodes, 
							const std::string name,
							std::istream& data)
{
  auto nodesByOccupancy = sortNodeVectorByUsage(nodes);
  std::shared_ptr<Node> usedNode;
  
  // Iterate through the vector and try to find a node where
  // the name doesn't exists
  while (!nodesByOccupancy.empty())
  {
    std::shared_ptr<Node> node = nodesByOccupancy.front();
    if (node->containsFile(name))
    {
      nodesByOccupancy.erase(nodesByOccupancy.begin());
    }
    else
    {
      node->put(name, data);
      usedNode = node;
      break;
    }
  }
  return usedNode;
}

const DataMapVector SimpleBroker::storeOnNodes(const NodeVector& nodes, 
					       const std::string name,
					       std::istream& data)
{
  DataMapVector dataMapVector;
  std::shared_ptr<Node> node = storeOnMinUsageNode(nodes, name, data);
  if (node.get() != NULL)
  {
    std::shared_ptr<DataMap> dataMap(new SimpleDataMap(name,node));
    dataMapVector.push_back(dataMap);
    _dataDistributionMap[name] = dataMapVector;
  }
  return dataMapVector;
}

const DataMapVector SimpleBroker::lookupFileParts(const std::string name)
{
  if (_dataDistributionMap.find(name) != _dataDistributionMap.end())
  {
    return _dataDistributionMap[name];
  }
  return DataMapVector();
};

void SimpleBroker::printFileNodeMappingsToStream(std::ostream &stream)
{
  auto fileNames = getFileNames();
  
  // Iterate through all files
  for (auto iterFileNames = fileNames.begin(); iterFileNames != fileNames.end(); iterFileNames++)
  {
    auto &filename = *iterFileNames;
    stream << filename << " stored on ";
    auto dataMapList = _dataDistributionMap[filename];

    // Per design of this broker we have only one entry pro file.
    // So we don't need to iterate through the vector
    auto dataMap = dataMapList.front();
    printNodeWithStatusToStream(stream, dataMap->node());
  }
}

std::vector<std::string> SimpleBroker::getFileNames()
{
  std::vector<std::string> fileNames;
  for (auto iter = _dataDistributionMap.begin(); iter != _dataDistributionMap.end(); iter++)
  {
    fileNames.push_back(iter->first);
  }
  return fileNames;
}

void SimpleBroker::printNodeWithStatusToStream(std::ostream &stream, std::shared_ptr<Node> node)
{
  stream << "{";
  stream << (node->isActive() ? Color::Green : Color::Red);
  stream << node->getName();
  stream << Color::Default;
  stream << "}\n";
}
