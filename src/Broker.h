#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <functional>
#include <Node.h>
#include <Color.h>

/*
 * This structure holds the information of a spectific file.
 * A Broker algorithm should enhanced this class with addional 
 * information.  
 */
class DataMap
{
  protected:
   /*
    * Name of file
    */
   std::string _filename;

   /*
    * Protected C'tor 
    *
    * @param filename Name of file
    */
   DataMap(std::string filename) : _filename(filename) {};
 
  public:
   /*
    * C'tor
    */
   DataMap() = default;

   /*
    * Copy C'tor
    */
   DataMap(const DataMap& that)
   {
     _filename = that.filename();
   };

   /*
    * D'tor
    */ 
   virtual ~DataMap() = default; 
   
   /*
    * Retrieve the filename
    *
    * @return The name of the file
    */
   std::string filename() const
   {
     return _filename;
   };
 
   /*
    * Retrieve a node where the file exists
    *
    * @returns A shared Pointer of the node where the file is located
    */
   virtual std::shared_ptr<Node> node() const = 0;
};

/*
 * A Vector of DataMap pointers
 */
typedef std::vector<std::shared_ptr<DataMap>> DataMapVector;
  
/*
 * A Vector of Node pointers
 */
typedef std::vector<std::shared_ptr<Node>> NodeVector;


/*
 * Abstract Broker class to perform the distribution of a file to nodes 
 */
class Broker
{
 public:
  Broker() = default;
  virtual ~Broker()= default;
  
  /*
   * Method to distribute the file on nodes. Depending on how
   * the algorithm distribute the file it could be possible
   * that a file is splitted into parts and/or replicated.
   *
   * @param nodes A list of nodes for distribute the given file 
   * @param name The file name
   * @param data The content of the file
   * @return A list of the location of file parts
   */
  virtual const DataMapVector storeOnNodes(const NodeVector& nodes,
				   const std::string name, 
				   std::istream& data) = 0;

  /*
   * Find a complete set of file parts to retore the requested file.
   *
   * @param name The file name
   * @return A list of the location of file parts
   */
  virtual const DataMapVector lookupFileParts(const std::string name) = 0;


  /*
   * Writes details to the file - node mapping to a provided stream
   *
   * @param stream The stream to writes to
   */
  virtual void printFileNodeMappingsToStream(std::ostream &stream) = 0;
};

