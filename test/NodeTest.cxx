#include <boost/test/unit_test.hpp>
#include <test/helper.h>
#include <Node.h>

BOOST_AUTO_TEST_SUITE(NodeTest)

BOOST_AUTO_TEST_CASE( store_object_on_node )
{
  std::string name = "Node_1";
  Node node(name);
  std::string key = "key01";
  std::string testData = "Hello World!!!";
  std::unique_ptr<std::istream> pTestDataStream = getStreamFromString(testData);
  node.put(key, *pTestDataStream);
  auto resultRawData = node.get(key);
  BOOST_CHECK(testData.compare(getStringFromStream(resultRawData)) == 0);
  BOOST_CHECK(node.containsFile(key));
  BOOST_CHECK(Node::Remove(node));
}

BOOST_AUTO_TEST_SUITE_END()
