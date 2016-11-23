// TOPOLOGY
// Always running
// Active game servers == active clients

#ifndef __MASTER_SERVER_H
#define __MASTER_SERVER_H

#include "MasterCommon.h"
#include "NetworkTypes.h"
#include "..\rf2dEngine\source\MPlayerStructures.h"
#include "..\rf2dEngine\source\PacketEnumerations.h"
#include <assert.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>


class MasterServer : public MasterCommon
{
public:
	MasterServer();
	~MasterServer();

	// Starts the master server
	bool Initialize(unsigned short maximumConnections, unsigned short localPort,int _threadSleepTimer);
	
	// Stops the master server
	void Disconnect(void);

	// Call once per frame.
	void Update(void);

	// Event when a packet was tampered with mid-stream.  Override.
	void OnModifiedPacket(void);
protected:
	char message[2048];
	void HandleQuery(Packet *packet);
	void HandleDelistServer(Packet *packet);
	void HandleUpdateServer(Packet *packet);
	void HandleRelayedConnectionNotification(Packet *packet);
	void HandleChatMessage(Packet *packet);
};

#endif