#include<linuxcpp.hpp>
#include<nynn_common.hpp>
#include<nynn_zmqprot.hpp>
#include<nynn_mm_types.hpp>
#include<nynn_mm_config.hpp>
#include<nynn_mm_handler.hpp>

using namespace std;
using namespace nynn;
using namespace nynn::mm;
typedef uint32_t (Block::BlockHeader::*Next)();
int main(int argc,char**argv){
	string text_ip=argv[1];
	string text_port=argv[2];
	uint32_t vtxno=parse_int(argv[3],~0ul);
	uint32_t fstblkno=parse_int(argv[4],0);
	assert(vtxno!=~0ul);
	assert(fstblkno==0||fstblkno==1);
	log_i("vtxno=%u",vtxno);
	Next nexts[2]={&Block::BlockHeader::getNext,&Block::BlockHeader::getPrev};
	Next next=nexts[fstblkno];
	zmq::context_t ctx;
	zmq::socket_t sock(ctx,ZMQ_REQ);
	string serv_endpoint="tcp://"+text_ip+":"+text_port;
	sock.connect(serv_endpoint.c_str());
	log_i("connected to %s",serv_endpoint.c_str());
	prot::Requester req(sock);
	Block blk;
	CharContent *cctt=blk;
	uint32_t blkno=fstblkno;
	while(blkno!=INVALID_BLOCKNO){
		read(req,vtxno,blkno,&blk);
		blkno=(blk.getHeader()->*next)();
		string line(cctt->begin(),cctt->end());
		cout<<line<<endl;
	}
	sock.close();
}
