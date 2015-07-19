#include <boost/test/unit_test.hpp>
#include <test/BrokerTest.h>
#include <ReplicateBroker.h>

BOOST_AUTO_TEST_SUITE(ReplicateBrokerTest)

BOOST_AUTO_TEST_CASE( get_file_that_not_exists )
{
  ReplicateBroker broker;
  _get_file_that_not_exists(broker);
}

BOOST_AUTO_TEST_CASE( insert_duplicate )
{
  ReplicateBroker broker;
  _insert_duplicate(broker);
}

BOOST_AUTO_TEST_CASE( distribute_with_one_node )
{
  ReplicateBroker broker;
  _distribute_with_one_node(broker);
}

BOOST_AUTO_TEST_CASE( distribute_with_two_nodes )
{
  ReplicateBroker broker;
  _distribute_with_two_nodes(broker);
}

BOOST_AUTO_TEST_SUITE_END()

