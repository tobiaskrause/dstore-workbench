CXX ?= gcc
CXXFLAGS ?= -Wall -std=c++0x -Ilibs/boost_1_58_0 -I. -Isrc
BOOST_LIB_DIR=libs/boost_1_58_0/stage/lib

LDFLAGS ?= -L$(BOOST_LIB_DIR) -lboost_filesystem -lboost_system
LDFLAGS_TEST ?= $(LDFLAGS) -lboost_unit_test_framework

DSTORE_OBJS = src/FileStore.cxx\
              src/Node.cxx\
              src/DistributedDataStore.cxx \
              src/SimpleBroker.cxx

DSTORE_MAIN = src/dstore.cxx


TEST_OBJS = test/main.cxx\
            test/NodeTest.cxx\
            test/FileStoreTest.cxx\
            test/SimpleBrokerTest.cxx\
            test/DistributedDataStoreTest.cxx\
            test/ReplicateBrokerTest.cxx

TEST_NAME = testdriver
DSTORE_NAME = dstore
BIN_DIR = bin

bin: 
	mkdir $@

build_dstore: $(DSTORE_OBJS)
	$(CXX) $(CXXFLAGS) -o $(addprefix $(BIN_DIR)/, $(DSTORE_NAME)) $(DSTORE_OBJS) $(DSTORE_MAIN) $(LDFLAGS)

run_dstore : bin build_dstore
	cd $(BIN_DIR) && LD_LIBRARY_PATH=../$(BOOST_LIB_DIR) ./$(DSTORE_NAME) && cd -

run_test: bin build_test
	cd $(BIN_DIR) && LD_LIBRARY_PATH=../$(BOOST_LIB_DIR) ./$(TEST_NAME) --log_level=test_suite && cd -

build_test: $(TEST_OBJS) $(DSTORE_OBJS)
	$(CXX) $(CXXFLAGS) -DBOOST_TEST_DYN_LINK $^ -o $(addprefix $(BIN_DIR)/, $(TEST_NAME)) $(LDFLAGS_TEST)
 
clean:
	rm -f $(BIN_DIR)/*


