#include <iostream>
#include <sstream>
#include <DistributedDataStore.h>
#include <SimpleBroker.h>
#include <ReplicateBroker.h>

void store(DistributedDataStore& dataStore, std::string name, std::string data)
{
  std::stringstream stream;
  stream << data;
  dataStore.store(name, stream); 
}

void runTestScenario(Broker& broker)
{
  //Create a distributed data store
  DistributedDataStore dataStore(broker);
  //Add some nodes
  std::cout << "Add nodes:\n";
  auto node1 = dataStore.addNode();
  dataStore.addNode();
  dataStore.addNode();

  // Show status
  dataStore.showStatus();

  //Store some data;
  std::cout << "Store data:\n";
  for (int i = 0; i < 4; i++)
  {
    std::stringstream fileName;
    fileName << "File" << i;
    store(dataStore, fileName.str(), "myData");
    // Show status
    dataStore.showStatus();
  
  }
  // Remove a node
  std::cout << "Remove a node:\n";
  dataStore.removeNode(node1->getName());

  // Show status
  dataStore.showStatus();

  // Cleanup
  dataStore.removeAllNodes();
}

int main(int argc, char** argv)
{ 
  std::cout << "+++++ Test Scenarion <SimpleBroker> ++++++\n";
  std::cout << "++++++++++++++++++++++++++++++++++++++++++\n\n";
  SimpleBroker simpleBroker;
  runTestScenario(simpleBroker);
  std::cout << "++++++++++++++++++ END +++++++++++++++++++\n\n\n";
  std::cout << "+++++ Test Scenarion <ReplciateBroker> ++++++\n";
  std::cout << "++++++++++++++++++++++++++++++++++++++++++\n\n";
  ReplicateBroker replicateBroker;
  runTestScenario(replicateBroker);
  std::cout << "++++++++++++++++++ END +++++++++++++++++++\n\n\n";

  return 0;
}



