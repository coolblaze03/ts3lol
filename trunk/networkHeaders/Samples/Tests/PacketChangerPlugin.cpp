#include "PacketChangerPlugin.h"





PacketChangerPlugin::PacketChangerPlugin(void)
{
}

PacketChangerPlugin::~PacketChangerPlugin(void)
{
}




void PacketChangerPlugin::OnInternalPacket(InternalPacket *internalPacket, unsigned frameNumber, SystemAddress remoteSystemAddress, RakNetTime time, bool isSend)
{



	internalPacket->data[0]=ID_USER_PACKET_ENUM+2;






	//(void) frameNumber; (void) remoteSystemAddress; (void) time; (void) isSend;





}