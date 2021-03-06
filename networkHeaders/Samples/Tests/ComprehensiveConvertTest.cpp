#include "ComprehensiveConvertTest.h"





/*
Description: Does a little bit of everything forever. This is an internal sample just to see if RakNet crashes or leaks memory over a long period of time.

Todo:
RPC  replacement tests when RPC3 includes work.

Success conditions:
No failures.



Failure conditions:
Connect fails without pending ops or current connection.

*/






int ComprehensiveConvertTest::RunTest(DataStructures::List<RakNet::RakString> params,bool isVerbose,bool noPauses)
{

	static const int CONNECTIONS_PER_SYSTEM =4;

	SystemAddress currentSystem;

	//	DebugTools::ShowError("Note: The conversion of this is on hold until the original sample's problem is known.",!noPauses && isVerbose,__LINE__,__FILE__);

	//	return 55;


	//	AutoRPC autoRpcs[NUM_PEERS];




	//AutoRPC autoRpcs[NUM_PEERS];

	
	int peerIndex;
	float nextAction;
	int i;
	int portAdd;


	char data[8096];

	int seed = 12345;
	if (isVerbose)
		printf("Using seed %i\n", seed);
	seedMT(seed);

	for (i=0; i < NUM_PEERS; i++)
	{

		//autoRpcs[i].RegisterFunction("RPC1", RPC1, false);
		//autoRpcs[i].RegisterFunction("RPC2", RPC2, false);
		//autoRpcs[i].RegisterFunction("RPC3", RPC3, false);
		//autoRpcs[i].RegisterFunction("RPC4", RPC4, false);
		peers[i]=RakNetworkFactory::GetRakPeerInterface();
		peers[i]->SetMaximumIncomingConnections(CONNECTIONS_PER_SYSTEM);
		SocketDescriptor socketDescriptor(60000+i, 0);
		peers[i]->Startup(NUM_PEERS, 0, &socketDescriptor, 1);
		peers[i]->SetOfflinePingResponse("Offline Ping Data", (int)strlen("Offline Ping Data")+1);
		peers[i]->ApplyNetworkSimulator(500,50,50);



		//		peers[i]->AttachPlugin(&autoRpc[i]);



	}

	for (i=0; i < NUM_PEERS; i++)
	{

		portAdd=randomMT()%NUM_PEERS;

		currentSystem.SetBinaryAddress("127.0.0.1");
		currentSystem.port=60000+portAdd;
		if(!peers[i]->IsConnected (currentSystem,true,true) )//Are we connected or is there a pending operation ?
		{
			if (!peers[i]->Connect("127.0.0.1", 60000+portAdd, 0, 0))
			{
				DebugTools::ShowError("Problem while calling connect.\n",!noPauses && isVerbose,__LINE__,__FILE__);
				return 1;

			}

		}

	}

	RakNetTime endTime = RakNet::GetTime()+10000;
	while (RakNet::GetTime()<endTime)
	{
		nextAction = frandomMT();

		if (nextAction < .04f)
		{
			// Initialize
			peerIndex=randomMT()%NUM_PEERS;
			SocketDescriptor socketDescriptor(60000+peerIndex, 0);
			peers[peerIndex]->Startup(NUM_PEERS, randomMT()%30, &socketDescriptor, 1);
			portAdd=randomMT()%NUM_PEERS;

			currentSystem.SetBinaryAddress("127.0.0.1");
			currentSystem.port=60000+portAdd;
			if(!peers[peerIndex]->IsConnected (currentSystem,true,true) )//Are we connected or is there a pending operation ?
			{
				if(!peers[peerIndex]->Connect("127.0.0.1", 60000+portAdd, 0, 0))
				{
					DebugTools::ShowError("Problem while calling connect.\n",!noPauses && isVerbose,__LINE__,__FILE__);
					return 1;

				}

			}
		}
		else if (nextAction < .09f)
		{
			// Connect
			peerIndex=randomMT()%NUM_PEERS;
			portAdd=randomMT()%NUM_PEERS;

			currentSystem.SetBinaryAddress("127.0.0.1");
			currentSystem.port=60000+portAdd;
			if(!peers[peerIndex]->IsConnected (currentSystem,true,true) )//Are we connected or is there a pending operation ?
			{
				if (!peers[peerIndex]->Connect("127.0.0.1", 60000+portAdd, 0, 0))
				{
					DebugTools::ShowError("Problem while calling connect.\n",!noPauses && isVerbose,__LINE__,__FILE__);
					return 1;

				}
			}
		}
		else if (nextAction < .10f)
		{
			// Disconnect
			peerIndex=randomMT()%NUM_PEERS;
			//	peers[peerIndex]->Shutdown(randomMT() % 100);
		}
		else if (nextAction < .12f)
		{
			// GetConnectionList
			peerIndex=randomMT()%NUM_PEERS;
			SystemAddress remoteSystems[NUM_PEERS];
			unsigned short numSystems=NUM_PEERS;
			peers[peerIndex]->GetConnectionList(remoteSystems, &numSystems);
			if (numSystems>0)
			{
				if (isVerbose){
					printf("%i: ", 60000+numSystems);
					for (i=0; i < numSystems; i++)
					{
						printf("%i: ", remoteSystems[i].port);
					}
					printf("\n");
				}
			}			
		}
		else if (nextAction < .14f)
		{
			// Send
			int dataLength;
			PacketPriority priority;
			PacketReliability reliability;
			unsigned char orderingChannel;
			SystemAddress target;
			bool broadcast;

			//	data[0]=ID_RESERVED1+(randomMT()%10);
			data[0]=ID_USER_PACKET_ENUM;
			dataLength=3+(randomMT()%8000);
			//			dataLength=600+(randomMT()%7000);
			priority=(PacketPriority)(randomMT()%(int)NUMBER_OF_PRIORITIES);
			reliability=(PacketReliability)(randomMT()%((int)RELIABLE_SEQUENCED+1));
			orderingChannel=randomMT()%32;
			if ((randomMT()%NUM_PEERS)==0)
				target=UNASSIGNED_SYSTEM_ADDRESS;
			else
				target=peers[peerIndex]->GetSystemAddressFromIndex(randomMT()%NUM_PEERS);

			broadcast=(bool)(randomMT()%2);
#ifdef _VERIFY_RECIPIENTS
			broadcast=false; // Temporarily in so I can check recipients
#endif

			peerIndex=randomMT()%NUM_PEERS;
			sprintf(data+3, "dataLength=%i priority=%i reliability=%i orderingChannel=%i target=%i broadcast=%i\n", dataLength, priority, reliability, orderingChannel, target.port, broadcast);
			//unsigned short localPort=60000+i;
#ifdef _VERIFY_RECIPIENTS
			memcpy((char*)data+1, (char*)&target.port, sizeof(unsigned short));
#endif
			data[dataLength-1]=0;
			peers[peerIndex]->Send(data, dataLength, priority, reliability, orderingChannel, target, broadcast);
		}
		else if (nextAction < .18f)
		{
			// RPC
			int dataLength;
			PacketPriority priority;
			PacketReliability reliability;
			unsigned char orderingChannel;
			SystemAddress target;
			bool broadcast;
			char RPCName[10];

			data[0]=ID_USER_PACKET_ENUM+(randomMT()%10);
			dataLength=3+(randomMT()%8000);
			//			dataLength=600+(randomMT()%7000);
			priority=(PacketPriority)(randomMT()%(int)NUMBER_OF_PRIORITIES);
			reliability=(PacketReliability)(randomMT()%((int)RELIABLE_SEQUENCED+1));
			orderingChannel=randomMT()%32;
			peerIndex=randomMT()%NUM_PEERS;
			if ((randomMT()%NUM_PEERS)==0)
				target=UNASSIGNED_SYSTEM_ADDRESS;
			else
				target=peers[peerIndex]->GetSystemAddressFromIndex(randomMT()%NUM_PEERS);
			broadcast=(bool)(randomMT()%2);
#ifdef _VERIFY_RECIPIENTS
			broadcast=false; // Temporarily in so I can check recipients
#endif

			sprintf(data+3, "dataLength=%i priority=%i reliability=%i orderingChannel=%i target=%i broadcast=%i\n", dataLength, priority, reliability, orderingChannel, target.port, broadcast);
#ifdef _VERIFY_RECIPIENTS
			memcpy((char*)data, (char*)&target.port, sizeof(unsigned short));
#endif
			data[dataLength-1]=0;
			sprintf(RPCName, "RPC%i", (randomMT()%4)+1);
			//				autoRpc[i]->Call(RPCName);
			//peers[peerIndex]->RPC(RPCName, data, dataLength*8, priority, reliability, orderingChannel, target, broadcast, 0, UNASSIGNED_NETWORK_ID,0);
		}
		else if (nextAction < .181f)
		{
			// CloseConnection
			SystemAddress target;
			peerIndex=randomMT()%NUM_PEERS;
			target=peers[peerIndex]->GetSystemAddressFromIndex(randomMT()%NUM_PEERS);
			peers[peerIndex]->CloseConnection(target, (bool)(randomMT()%2), 0);
		}
		else if (nextAction < .20f)
		{
			// Offline Ping
			peerIndex=randomMT()%NUM_PEERS;
			peers[peerIndex]->Ping("127.0.0.1", 60000+(randomMT()%NUM_PEERS), (bool)(randomMT()%2));
		}
		else if (nextAction < .21f)
		{
			// Online Ping
			SystemAddress target;
			target=peers[peerIndex]->GetSystemAddressFromIndex(randomMT()%NUM_PEERS);
			peerIndex=randomMT()%NUM_PEERS;
			peers[peerIndex]->Ping(target);
		}
		else if (nextAction < .24f)
		{
			// SetCompileFrequencyTable
			peerIndex=randomMT()%NUM_PEERS;
			peers[peerIndex]->SetCompileFrequencyTable(randomMT()%2);
		}
		else if (nextAction < .25f)
		{
			// GetStatistics
			SystemAddress target, mySystemAddress;
			RakNetStatistics *rss;
			mySystemAddress=peers[peerIndex]->GetInternalID();
			target=peers[peerIndex]->GetSystemAddressFromIndex(randomMT()%NUM_PEERS);
			peerIndex=randomMT()%NUM_PEERS;
			rss=peers[peerIndex]->GetStatistics(mySystemAddress);
			if (rss)
			{
				StatisticsToString(rss, data, 0);
				if (isVerbose)
					printf("Statistics for local system %i:\n%s", mySystemAddress.port, data);

			}

			rss=peers[peerIndex]->GetStatistics(target);
			if (rss)
			{
				StatisticsToString(rss, data, 0);
				if (isVerbose)
					printf("Statistics for target system %i:\n%s", target.port, data);

			}			
		}

		for (i=0; i < NUM_PEERS; i++)
			peers[i]->DeallocatePacket(peers[i]->Receive());


		RakSleep(0);

	}


	

	return 0;

}

void ComprehensiveConvertTest::DestroyPeers()
{

for (int i=0; i < NUM_PEERS; i++)
		RakNetworkFactory::DestroyRakPeerInterface(peers[i]);

}



RakNet::RakString ComprehensiveConvertTest::GetTestName()
{

	return "ComprehensiveConvertTest";

}

RakNet::RakString ComprehensiveConvertTest::ErrorCodeToString(int errorCode)
{

	switch (errorCode)
	{

	case 0:
		return "No error";
		break;

	case 1: 
		return "Connect function failed";
		break;

	default:
		return "Undefined Error";
	}


}



ComprehensiveConvertTest::ComprehensiveConvertTest(void)
{
}

ComprehensiveConvertTest::~ComprehensiveConvertTest(void)
{
}