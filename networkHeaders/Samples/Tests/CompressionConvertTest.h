#pragma once

#include "TestInterface.h"


#include "RakString.h"
#include "RakNetworkFactory.h"
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "RakPeer.h"
#include "RakSleep.h"
#include "RakNetTime.h"
#include "GetTime.h"
#include "DebugTools.h"






class CompressionConvertTest : public TestInterface
{
public:
	CompressionConvertTest(void);
	~CompressionConvertTest(void);
	int RunTest(DataStructures::List<RakNet::RakString> params,bool isVerbose,bool noPauses);//should return 0 if no error, or the error number
	RakNet::RakString GetTestName();
	RakNet::RakString ErrorCodeToString(int errorCode);
	void DestroyPeers();

protected:
	int RunAndSave(bool isVerbose,bool noPauses,FILE *serverToClientFrequencyTableFilePointer,FILE *clientToServerFrequencyTableFilePointer,bool destructionTest);

private:
	
	RakPeerInterface *rakClient;
	RakPeerInterface *rakServer;
};
