Instructions
============
This project is a workbench to try out different strategies to distribute files on nodes.

Structure
=========
```
 |
 |- bin  ( Target for binaries )
 |
 |- src  ( Source files )
 |
 |- test ( Test source files)
 |
 |- lib  ( Needed boost libraries )
```
How to build
============
In order to compile and link this project the build system should have boost (e.g 1.58) installed.
In case the boost libraries are not installed system wide you can symlink the lib folder to your boost installation.  

Tested with gcc 4.4.7

To build & run tests

make run_test

To build & run dstore application

make run_dstore

Distribution Strategies
=======================
To try out different strategies one can implement the Broker interface (Broker.h).

Simple
------
This strategy is implemented in SimpleBroker(.cxx/.h). It stores files on available nodes in a round robin fashion to balanced the occupancy of the nodes.

In case a node gets unavailable all data on this node are lost. 

Replicate
---------
The implementation of this strategy can be found in ReplicateBroker(.h). It uses the same round robin mechanism to balance the occupancy between the nodes. But it additional add a replicate of the file to the network to minimize the chance of data lost after a node became unavailable.
