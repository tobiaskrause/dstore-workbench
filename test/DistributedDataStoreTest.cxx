#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <test/helper.h>
#include <DistributedDataStore.h>
#include <SimpleBroker.h>

BOOST_AUTO_TEST_SUITE(DistributedDataStoreTest)

BOOST_AUTO_TEST_CASE( add_node )
{
  SimpleBroker simpleBroker;
  DistributedDataStore dds(simpleBroker);
  dds.addNode();
  std::vector<std::string> nodeNames = dds.listNodeNames();
  BOOST_CHECK_EQUAL(nodeNames.size(), (size_t)1);
  dds.removeAllNodes();     
}

BOOST_AUTO_TEST_CASE( remove_node )
{
  SimpleBroker simpleBroker;
  DistributedDataStore dds(simpleBroker);
  dds.addNode();
  std::vector<std::string> nodeNames = dds.listNodeNames();
  BOOST_CHECK_EQUAL(nodeNames.size(), (size_t)1);
  dds.removeNode(nodeNames.front());
  nodeNames = dds.listNodeNames();
  BOOST_CHECK_EQUAL(nodeNames.size(), (size_t)0);
  dds.removeAllNodes();     
}

BOOST_AUTO_TEST_CASE( store_and_load_object )
{
  SimpleBroker simpleBroker;
  DistributedDataStore dds(simpleBroker);
  dds.addNode();
  std::string key = "file01";
  std::string testData = "Hello World!!!";
  std::unique_ptr<std::istream> pTestDataStream = getStreamFromString(testData);
  dds.store(key, *pTestDataStream);
  auto resultRawData = dds.load(key);
  BOOST_CHECK(testData.compare(getStringFromStream(resultRawData)) == 0); 
  dds.removeAllNodes();     
}

BOOST_AUTO_TEST_CASE( load_not_existing_object )
{
  SimpleBroker simpleBroker;
  DistributedDataStore dds(simpleBroker);
  dds.addNode();
  std::string key = "file01";
  auto resultRawData = dds.load(key);
  BOOST_CHECK(getStringFromStream(resultRawData).size() == 0); 
  dds.removeAllNodes();     
}


BOOST_AUTO_TEST_SUITE_END()
