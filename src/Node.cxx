#include <Node.h>

bool Node::Remove(Node& node)
{
  return node.removeFileStore();
}

Node::Node(const std::string& nodeName):
_storeName("fs_" + nodeName),
_store(_storeName),
_nodeName(nodeName),
_isActive(true)
{
}

Node::~Node()
{
}

bool Node::removeFileStore()
{
  if (_isActive)
  {
    bool isRemoved = FileStore::Remove(_storeName);
    if (isRemoved);
    {
      _isActive = false;
    }
    return isRemoved; 
  } 
  else
  {
    return true;
  }

}

void Node::put(const std::string& name, std::istream& data)
{
  if (!_isActive)
  {
    throw new NodeNotActiveException();
  }
  _store.put(name, data);
  _storedData.push_back(name);
}

std::unique_ptr<std::istream> Node::get(const std::string& name)
{
  if (!_isActive)
  {
    throw new NodeNotActiveException();
  }
  return _store.get(name);
}

size_t Node::getUsage()
{
  if (!_isActive)
  {
    throw new NodeNotActiveException();
  }
  return _storedData.size();
}

std::string Node::getName()
{
  return _nodeName;
}

bool Node::isActive()
{
  return _isActive;
}

bool Node::containsFile(std::string filename)
{
  return std::find(_storedData.begin(), _storedData.end(), filename) != _storedData.end();
}
