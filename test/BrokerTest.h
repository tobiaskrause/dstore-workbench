#pragma once

#include <test/helper.h>
#include <Broker.h>

static void _get_file_that_not_exists(Broker& broker)
{
  std::string nodeName = "Node_1";
  std::shared_ptr<Node> node(new Node(nodeName));
  std::vector<std::shared_ptr<Node>> nodes = {node};
  std::string filename = "file_1";
  auto resultData = node->get(filename);
  BOOST_CHECK(getStringFromStream(resultData).size() == 0); 
  BOOST_CHECK(Node::Remove(*(node.get())));
}

static void _insert_duplicate(Broker& broker)
{
  std::string nodeName = "Node_1";
  std::shared_ptr<Node> node(new Node(nodeName));
  std::vector<std::shared_ptr<Node>> nodes = {node};
  std::string filename = "file_1";
  std::string data_file_1 = "testdata1234567890";
  std::unique_ptr<std::istream> pTestData_1Stream = getStreamFromString(data_file_1);
  broker.storeOnNodes(nodes, filename, *pTestData_1Stream);
  auto resultData = node->get(filename);
  BOOST_CHECK(data_file_1.compare(getStringFromStream(resultData)) == 0); 
  //Insert again
  DataMapVector dataMapVector = broker.storeOnNodes(nodes, filename, *pTestData_1Stream);
  BOOST_CHECK(dataMapVector.size() == 0);
  BOOST_CHECK(Node::Remove(*(node.get())));
}

static void _distribute_with_one_node(Broker& broker)
{
  std::string nodeName = "Node_1";
  std::shared_ptr<Node> node(new Node(nodeName));
  std::vector<std::shared_ptr<Node>> nodes = {node};
  std::string filename = "file_1";
  std::string data_file_1 = "testdata1234567890";
  std::unique_ptr<std::istream> pTestData_1Stream = getStreamFromString(data_file_1);
  broker.storeOnNodes(nodes, filename, *pTestData_1Stream);
  auto resultData = node->get(filename);
  BOOST_CHECK(data_file_1.compare(getStringFromStream(resultData)) == 0); 
  BOOST_CHECK(Node::Remove(*(node.get())));
}

static void _distribute_with_two_nodes(Broker& broker)
{
  std::string nodeName1 = "Node_1";
  std::shared_ptr<Node> node1(new Node(nodeName1));
  std::string nodeName2 = "Node_2";
  std::shared_ptr<Node> node2(new Node(nodeName2));
  std::vector<std::shared_ptr<Node>> nodes = {node1, node2};
  std::string filename1 = "file_1";
  std::string data_file_1 = "1testdata1234567890";
  std::unique_ptr<std::istream> pTestData_1Stream = getStreamFromString(data_file_1);
  broker.storeOnNodes(nodes, filename1, *pTestData_1Stream);
  std::string filename2 = "file_2";
  std::string data_file_2 = "2testdata1234567890";
  std::unique_ptr<std::istream> pTestData_2Stream = getStreamFromString(data_file_2);
  broker.storeOnNodes(nodes, filename2, *pTestData_2Stream);
  auto resultData1 = node1->get(filename1);
  BOOST_CHECK(data_file_1.compare(getStringFromStream(resultData1)) == 0);
  auto resultData2 = node2->get(filename2);
  BOOST_CHECK(data_file_2.compare(getStringFromStream(resultData2)) == 0);
  BOOST_CHECK(Node::Remove(*(node1.get())));
  BOOST_CHECK(Node::Remove(*(node2.get())));
}
