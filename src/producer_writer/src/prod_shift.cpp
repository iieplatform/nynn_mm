#include<nynn_mm_config.h>
#include<ProducerRPC.h>
using namespace nynn::mm::rpc;
int main(int argc, char**argv)
{
	string prodHost=argv[1];
	uint32_t prodPort=strtoul(argv[2],NULL,0);
	uint32_t vtxno=strtoul(argv[3],NULL,0);
	uint32_t n=strtoul(argv[4],NULL,0);

	std::shared_ptr<ProducerRPC> prod(new ProducerRPC(prodHost,prodPort));

	RawBlock rblk;
	vector<int8_t>& xblk=rblk;
	Block* blk=rblk;
	CharContent *content=*blk;
	int i=0;
	while(i++<n && prod->shift(vtxno)!=INVALID_BLOCKNO);
}

