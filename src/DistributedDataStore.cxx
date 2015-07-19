#include <iostream>
#include <DistributedDataStore.h>
#include <Color.h>

std::shared_ptr<Node> DistributedDataStore::addNode()
{
  std::stringstream nodeName;
  nodeName << "Node_" << _nodeNameIndex++;
  std::shared_ptr<Node> node(new Node(nodeName.str()));
  _nodes.push_back(node);
  return node;
}

void DistributedDataStore::removeNode(const std::string name)
{
  for (auto iter = _nodes.begin(); iter != _nodes.end(); iter++)
  {
    if((*iter)->getName().compare(name) == 0)
    {
      Node::Remove(*(*iter).get());
      _nodes.erase(iter);
      break;
    }
  }
}

void DistributedDataStore::removeAllNodes()
{
  for (auto iter = _nodes.begin(); iter != _nodes.end(); iter++)
  {
    Node::Remove(*(*iter).get());
  }
  _nodes.clear();
}

std::vector<std::string> DistributedDataStore::listNodeNames()
{
  std::vector<std::string> nodeNames;
  for (auto iter = _nodes.begin(); iter != _nodes.end(); iter++)
  {
    nodeNames.push_back((*iter)->getName());
  }
  return nodeNames;
} 

bool DistributedDataStore::store(const std::string& name, std::istream& data)
{
  auto dataMapList = _broker.storeOnNodes(_nodes, name, data);
  return dataMapList.empty() ? false : true;
}

std::unique_ptr<std::istream> DistributedDataStore::load(const std::string& name)
{
  std::unique_ptr<std::stringstream> resultStream(new std::stringstream);
  auto dataMapVector = _broker.lookupFileParts(name);
  for (auto iter = dataMapVector.begin(); iter != dataMapVector.end(); iter++)
  {
    std::shared_ptr<DataMap> dataMap = (*iter);
    std::unique_ptr<std::istream> stream = dataMap->node()->get(dataMap->filename());
    (*resultStream) << stream->rdbuf();
  }  
  return std::move(resultStream);
}

void DistributedDataStore::showStatus()
{
  using namespace std;

  cout << "========== Status Information ============\n";
  cout << "List of nodes:\n";
  cout << "-- Name --\t|-- Usage --\n";
  for (auto iter = _nodes.begin(); iter != _nodes.end(); iter++)
  {
    shared_ptr<Node> node = *iter;
    cout << (node->isActive() ? Color::Green : Color::Red);
    cout << node->getName() << "\t\t| " << node->getUsage();    
    cout << Color::Default;
    cout << "\n";
  }
  cout << "\n";
  cout << "List of files:\n";
  _broker.printFileNodeMappingsToStream(cout);
  cout << "==========================================\n";
}
