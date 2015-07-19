#include <boost/test/unit_test.hpp>
#include <SimpleBroker.h>
#include <test/BrokerTest.h>

BOOST_AUTO_TEST_SUITE(SimpleBrokerTest)

BOOST_AUTO_TEST_CASE( get_file_that_not_exists )
{
  SimpleBroker broker;
  _get_file_that_not_exists(broker);
}

BOOST_AUTO_TEST_CASE( insert_duplicate )
{
  SimpleBroker broker;
  _insert_duplicate(broker);
}

BOOST_AUTO_TEST_CASE( distribute_with_one_node )
{
  SimpleBroker broker;
  _distribute_with_one_node(broker);
}

BOOST_AUTO_TEST_CASE( distribute_with_two_nodes )
{
  SimpleBroker broker;
  _distribute_with_two_nodes(broker);
}

BOOST_AUTO_TEST_SUITE_END()
