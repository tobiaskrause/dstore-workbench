#include <string>
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <boost/shared_array.hpp>
#include <test/helper.h>
#include <FileStore.h>

namespace fs = boost::filesystem;

BOOST_AUTO_TEST_SUITE(FileStoreTest)

BOOST_AUTO_TEST_CASE( create_file_store )
{
  std::string name = "create_file_store_test";
  BOOST_CHECK(FileStore::Create(name));
  BOOST_CHECK(FileStore::Remove(name));
}

BOOST_AUTO_TEST_CASE( use_file_store_without_create )
{
  std::string name = "create_file_store_test";
  FileStore fstore(name);
  BOOST_CHECK(fs::exists(name));
  BOOST_CHECK(FileStore::Remove(name));
}


BOOST_AUTO_TEST_CASE( store_object )
{
  using namespace std;
  string name = "create_file_store_test";
  BOOST_CHECK(FileStore::Create(name));
  FileStore fstore(name);
  string key = "key01";
  string testData = "Hello World!!!";
  std::unique_ptr<std::istream> pTestDataStream = getStreamFromString(testData);
  fstore.put(key, *pTestDataStream);
  auto resultRawData = fstore.get(key);
  BOOST_CHECK(testData.compare(getStringFromStream(resultRawData)) == 0); 
  BOOST_CHECK(FileStore::Remove(name));
}


BOOST_AUTO_TEST_SUITE_END()

