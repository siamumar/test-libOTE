#include <iostream>
#include <cryptoTools/Network/Channel.h>
#include <cryptoTools/Network/Session.h>
#include <cryptoTools/Network/IOService.h>
#include <cryptoTools/Common/Log.h>
#include <cryptoTools/Common/BitVector.h>
#include <cryptoTools/Crypto/PRNG.h>
#include <libOTe/Base/BaseOT.h>
#include <libOTe/TwoChooseOne/OTExtInterface.h>
#include <libOTe/TwoChooseOne/IknpOtExtReceiver.h>
#include <libOTe/TwoChooseOne/IknpOtExtSender.h>

using namespace std;
using namespace osuCrypto;

void Server_s(){
    cout << "in server" << endl;
	IOService ios;
	Session ep(ios, "localhost", 1212, SessionMode::Server);
	Channel chl = ep.addChannel();

    PRNG prng(_mm_set_epi32(4253465, 3434565, 234435, 23987045));

    IknpOtExtSender sender;
    DefaultBaseOT base;		
    uint64_t baseCount = 128;
    BitVector bv(baseCount);
    vector<block> baseMsg(baseCount);
    bv.randomize(prng);
    base.receive(bv, baseMsg, prng, chl, 1);	
    sender.setBaseOts(baseMsg, bv, chl);

    uint64_t num_OTs = 1024;
    vector<array<block,2>> sendMsgs(num_OTs);
    sender.send(sendMsgs, prng, chl);

    cout << "done OT" << endl;
}

void Client_s(){
    cout << "in client" << endl;
	IOService ios;
	Session ep(ios, "localhost", 1212, SessionMode::Client);
	Channel chl = ep.addChannel();

    PRNG prng(_mm_set_epi32(4253465, 3434565, 234435, 23987045));

    IknpOtExtReceiver receiver;
    DefaultBaseOT base;	
    uint64_t baseCount = 128;
    vector<array<block, 2>> baseMsg(baseCount);
    base.send(baseMsg, prng, chl, 1);
    receiver.setBaseOts(baseMsg, prng, chl);

    uint64_t num_OTs = 1024;
    vector<block> recvMsgs(num_OTs);
    BitVector bv(num_OTs);
    bv.randomize(prng);
    receiver.receive(bv, recvMsgs, prng, chl);

    cout << "done OT" << endl;
}

int main(int argc, char** argv){
    if ((argc > 1) && (!strcmp(argv[1], "-s"))) Server_s();
    else Client_s();
}