// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "Provider.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/concurrency/Thread.h>
#include <thrift/concurrency/PosixThreadFactory.h>
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/server/TNonblockingServer.h>

#include <nynn_mm_config.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace ::apache::thrift::concurrency;

using boost::shared_ptr;

using namespace  ::nynn::mm;

static boost::shared_ptr<SubgraphSet> subgraphSet;

class ProviderHandler : virtual public ProviderIf {
 public:
  ProviderHandler() {
    // Your initialization goes here
  }

  bool createSubgraph(const int32_t subgraphKey) {
	  try{
		  subgraphSet->createSubgraph(subgraphKey);
		  return true;
	  }catch(NynnException &ex){
		  return false;
	  }
  }

  bool destroySubgraph(const int32_t subgraphKey) {
	  try{
		  subgraphSet->destroySubgraph(subgraphKey);
		  return true;
	  }catch(NynnException &ex){
		  return false;
	  }
  }

  bool attachSubgraph(const int32_t subgraphKey) {
	  try{
		  subgraphSet->attachSubgraph(subgraphKey);
		  return true;
	  }catch(NynnException &ex){
		  return false;
	  }
  }

  bool detachSubgraph(const int32_t subgraphKey) {
	  try{
		  subgraphSet->detachSubgraph(subgraphKey);
		  return true;
	  }catch(NynnException &ex){
		  return false;
	  }
  }

  void getSubgraphKeys(std::vector<int32_t> & keys) {
	  subgraphSet->getSubgraphKeys(keys);
  }

  int32_t getWidthOfVertexInterval() {
	  return subgraphSet->getWidthOfVertexInterval();
  }

  bool lock(const int32_t vtxno, const int32_t flag) {
	  return subgraphSet->lock(vtxno,flag);
  }

  bool unlock(const int32_t vtxno) {
	  try{
		  subgraphSet->unlock(vtxno);
		  return true;
	  }catch(NynnException &ex){
		  return false;
	  }
  }

  int32_t getSize(const int32_t vtxno) {
	  return subgraphSet->getSize(vtxno);
  }

  int32_t getHeadBlkno(const int32_t vtxno) {
	  return subgraphSet->getHeadBlkno(vtxno);
  }

  int32_t getTailBlkno(const int32_t vtxno) {
	  return subgraphSet->getTailBlkno(vtxno);
  }

  void readAllBlknos(std::vector<int32_t> & blknos,const int32_t vtxno) {
	  subgraphSet->readAllBlknos(vtxno,blknos); 
  }

  void read(std::vector<int8_t> & xblk, const int32_t vtxno, const int32_t blkno) {
	  xblk.resize(sizeof(Block));
	  Block* blk=reinterpret_cast<Block*>(const_cast<int8_t*>(xblk.data()));
	  subgraphSet->read(vtxno,blkno,blk);
  }

  void readn(std::vector<int8_t> & xblks, const int32_t vtxno, const int32_t blkno, const int32_t n) {
	  subgraphSet->readn(vtxno,blkno,n,xblks);
  }

  int32_t insertPrev(const int32_t vtxno, const int32_t nextBlkno, const std::vector<int8_t> & xblk) {
	  Block *blk=reinterpret_cast<Block*>(const_cast<int8_t*>(xblk.data()));
	  return subgraphSet->insertPrev(vtxno,nextBlkno,blk);
  }

  int32_t insertNext(const int32_t vtxno, const int32_t prevBlkno, const std::vector<int8_t> & xblk) {
	  Block *blk=reinterpret_cast<Block*>(const_cast<int8_t*>(xblk.data()));
	  return subgraphSet->insertNext(vtxno,prevBlkno,blk);
  }

  int32_t remove(const int32_t vtxno, const int32_t blkno) {
	  try{
		  return subgraphSet->remove(vtxno,blkno);
	  }catch(NynnException &ex){
		  return INVALID_BLOCKNO;
	  }
  }

  int32_t unshift(const int32_t vtxno, const std::vector<int8_t> & newHeadXBlk) {
	  Block *newHeadBlk=reinterpret_cast<Block*>(const_cast<int8_t*>(newHeadXBlk.data()));
	  return subgraphSet->unshift(vtxno,newHeadBlk);
  }

  int32_t shift(const int32_t vtxno) {
	  try{
		  return subgraphSet->shift(vtxno);
	  }catch(NynnException &ex){
		  return INVALID_BLOCKNO;
	  }
  }

  int32_t push(const int32_t vtxno, const std::vector<int8_t> & newTailXBlk) {
	  Block* newTailBlk=reinterpret_cast<Block*>(const_cast<int8_t*>(newTailXBlk.data()));
	  return subgraphSet->push(vtxno,newTailBlk); 
  }

  int32_t pop(const int32_t vtxno) {
	  try{
		  return subgraphSet->pop(vtxno);
	  }catch(NynnException &ex){
		  return INVALID_BLOCKNO;
	  }
  }

};

int main(int argc, char **argv) {
	int port =strtoul(getenv("PROVIDER_PORT"),NULL,0);
	subgraphSet.reset(new SubgraphSet(getenv("SUBGRAPHSET_BASEDIR")));

	boost::shared_ptr<ProviderHandler> handler(new ProviderHandler());
	boost::shared_ptr<TProcessor> processor(new ProviderProcessor(handler));
	//boost::shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
	boost::shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
	boost::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
	boost::shared_ptr<ThreadFactory> pthreadFactory(new PosixThreadFactory());

	boost::shared_ptr<ThreadManager> threadManager=ThreadManager::newSimpleThreadManager(10,4);
	threadManager->threadFactory(pthreadFactory);

	TNonblockingServer server(processor,transportFactory,transportFactory,protocolFactory,protocolFactory,port,threadManager);
	threadManager->start();
	server.serve();
	return 0;
}

