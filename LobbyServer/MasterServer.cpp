#include "MasterServer.h"
#include "RakPeerInterface.h"
#include "BitStream.h"
#include "RakNetworkFactory.h"
#include "PacketEnumerations.h"
#include "StringCompressor.h"
#include "GetTime.h"

// Uncomment this define for debugging printfs
//#define _SHOW_MASTER_SERVER_PRINTF
#ifdef _SHOW_MASTER_SERVER_PRINTF
#include <stdio.h>
#endif

MasterServer::MasterServer()
{
}

MasterServer::~MasterServer()
{
	ClearServerList();
	RakNetworkFactory::DestroyRakPeerInterface(rakPeer);
}

bool MasterServer::Initialize(unsigned short maximumConnections, unsigned short localPort,int _threadSleepTimer)
{
	if (rakPeer->Initialize(maximumConnections, localPort, _threadSleepTimer)==false)
		return false;
	rakPeer->SetOccasionalPing(true);
	//rakPeer->StartSynchronizedRandomInteger(); // Keepalive
	rakPeer->SetMaximumIncomingConnections(maximumConnections);
	return true;
}

void MasterServer::Disconnect(void)
{
	rakPeer->Disconnect(200);
}

void MasterServer::Update(void)
{
	Packet *packet;
	unsigned serverIndex;
	unsigned long time;
	RakNetStatisticsStruct *rss;
	unsigned long connectionTime;
	packet = rakPeer->Receive();

	// TODO - should have multiple listing security

	time = RakNetGetTime();

	while (packet)
	{
		// Quick and dirty flood attack security:
		// If a client has been connected for more than 5 seconds,
		// and has sent more than 1000 bytes per second on average then ban them
		rss=rakPeer->GetStatistics(packet->playerId);
		if (rss)
		{
			connectionTime=time-rss->connectionStartTime;
			if (connectionTime > FLOOD_ATTACK_CHECK_DELAY &&
				(float)(rss->bitsReceived/8) / (float) connectionTime > FLOOD_ATTACK_BYTES_PER_MS)
			{
				rakPeer->CloseConnection(packet->playerId, true,0);
#ifdef _SHOW_MASTER_SERVER_PRINTF
				printf("%s banned for session due to for flood attack\n", (char*)rakPeer->PlayerIDToDottedIP(packet->playerId));
#endif
				rakPeer->AddToBanList(rakPeer->PlayerIDToDottedIP(packet->playerId));

				// Find all servers with this IP and kill them.
				serverIndex=0;
				while (serverIndex < gameServerList.serverList.size())
				{
					if (gameServerList.serverList[serverIndex]->connectionIdentifier.binaryAddress==packet->playerId.binaryAddress)
					{
						delete gameServerList.serverList[serverIndex];
						gameServerList.serverList.del(serverIndex);
					}
					else
						serverIndex++;
				}
				break;
			}

		}
		
		switch(packet->data[0])
		{
		case ID_NEW_INCOMING_CONNECTION:
			break;
		case ID_DISCONNECTION_NOTIFICATION:
			break;
		case ID_CONNECTION_LOST:
			break;
		case ID_RECEIVED_STATIC_DATA:
			break;
		case ID_MODIFIED_PACKET:
			OnModifiedPacket();
			break;
		case ID_REMOTE_PORT_REFUSED:
			break;
		case ID_QUERY_MASTER_SERVER:
			HandleQuery(packet);
			break;
		case ID_MASTER_SERVER_DELIST_SERVER:
			HandleDelistServer(packet);
			break;
		case ID_MASTER_SERVER_SET_SERVER:
			HandleUpdateServer(packet);
			break;
		case ID_PONG:
			HandlePong(packet);
			break;
		case ID_RELAYED_CONNECTION_NOTIFICATION:
			HandleRelayedConnectionNotification(packet);
			break;
		case 48: //ID_CHAT_MESSAGE:
			HandleChatMessage(packet);
			break;
		default:
			#ifdef _SHOW_MASTER_SERVER_PRINTF
			printf("MasterServer Unknown type %i\n", packet->data[0]);
			#endif
			break;
		}
		rakPeer->DeallocatePacket(packet);
		packet = rakPeer->Receive();
	}

	//make sure all the games are still alive
	serverIndex=0;
	while (serverIndex < gameServerList.serverList.size())
	{
		//is it time to ping?
		if (time >= gameServerList.serverList[serverIndex]->nextPingTime)
		{
			if (gameServerList.serverList[serverIndex]->failedPingResponses>=NUMBER_OF_MISSED_PINGS_TO_DROP)
			{//failed too many ping responses so get rid of it.
				#ifdef _SHOW_MASTER_SERVER_PRINTF
				printf("Deleting %s for lack of ping response.\n", (char*)rakPeer->PlayerIDToDottedIP(gameServerList.serverList[serverIndex]->connectionIdentifier));
				#endif
				gameServerList.serverList[serverIndex]->Clear();
				delete gameServerList.serverList[serverIndex];
				gameServerList.serverList.del(serverIndex);
			}
			else
			{//failed this ping so log it
				rakPeer->Ping((char*)rakPeer->PlayerIDToDottedIP(gameServerList.serverList[serverIndex]->connectionIdentifier),
					gameServerList.serverList[serverIndex]->connectionIdentifier.port, false);

				gameServerList.serverList[serverIndex]->nextPingTime = time + KEEP_ALIVE_PING_FREQUENCY;
				gameServerList.serverList[serverIndex]->failedPingResponses++;
				#ifdef _SHOW_MASTER_SERVER_PRINTF
				printf("Pinging %s. %i failed responses inclusive\n", (char*)rakPeer->PlayerIDToDottedIP(gameServerList.serverList[serverIndex]->connectionIdentifier),gameServerList.serverList[serverIndex]->failedPingResponses);
				#endif
				
				serverIndex++;
			}
		}
		else
			serverIndex++;
	}
}

void MasterServer::HandleDelistServer(Packet *packet)
{
	PlayerID serverPlayerID;
	int existingServerIndex;
	BitStream bitStream((const char*)packet->data, packet->length, false);

	bitStream.IgnoreBits(sizeof(unsigned char)*8); // Ignore the packet type enum
	bitStream.Read(serverPlayerID.port);
	serverPlayerID.binaryAddress=packet->playerId.binaryAddress;

	existingServerIndex=gameServerList.GetIndexByPlayerID(serverPlayerID);
	if (existingServerIndex>=0)
	{
		gameServerList.serverList[existingServerIndex]->Clear();
		delete gameServerList.serverList[existingServerIndex];
		gameServerList.serverList.del(existingServerIndex);
	}
	//else
		// Server does not already exist

	#ifdef _SHOW_MASTER_SERVER_PRINTF
	printf("%i servers on the list\n", gameServerList.serverList.size());
	#endif
}

void MasterServer::HandleQuery(Packet *packet)
{
	BasicDataStructures::List<GameServer*> serversWithKeysList;
	char ruleIdentifier[256];
	unsigned index, serverIndex;
	int key;
	bool queryAll;
	BitStream outputBitStream;
	BitStream compressedString((const char*)packet->data, packet->length, false);
	compressedString.IgnoreBits(8*sizeof(unsigned char));

	queryAll=true;

	while (compressedString.GetNumberOfUnreadBits()>0)
	{
		// Generate a list of the indices of the servers that have one or more of the specified keys.
		stringCompressor->DecodeString(ruleIdentifier, 256, &compressedString);
		if (ruleIdentifier[0]==0)
			// If we fail to read the first string, queryAll remains true.
			break;
		
		queryAll=false;

		if (IsReservedRuleIdentifier(ruleIdentifier))
			continue;

		for (index=0; index < gameServerList.serverList.size(); index++)
		{
			if (gameServerList.serverList[index]->connectionIdentifier==UNASSIGNED_PLAYER_ID)
				continue;

			if (gameServerList.serverList[index]->FindKey(ruleIdentifier))
			{
				serverIndex=serversWithKeysList.getIndexOf(gameServerList.serverList[index]);
				if (serverIndex==MAX_UNSIGNED_LONG)
				{
					gameServerList.serverList[index]->numberOfKeysFound=1;
					serversWithKeysList.insert(gameServerList.serverList[index]);
				}
				else
				{
					serversWithKeysList[serverIndex]->numberOfKeysFound++;
				}
			}
		}
	}
	
	// Write the packet id
	if (queryAll)
		outputBitStream.Write((unsigned char) ID_MASTER_SERVER_SET_SERVER);
	else
		outputBitStream.Write((unsigned char) ID_MASTER_SERVER_UPDATE_SERVER);
	if (queryAll)
	{
		// Write the number of servers
		outputBitStream.WriteCompressed((unsigned short)gameServerList.serverList.size());

		for (index=0; index < gameServerList.serverList.size(); index++)
		{
			// Write the whole server
			SerializeServer(gameServerList.serverList[index], &outputBitStream);	
		}		
	}
	else
	{
		compressedString.ResetReadPointer();
		compressedString.IgnoreBits(8*sizeof(unsigned char));

		// Write the number of servers with requested keys
		outputBitStream.WriteCompressed((unsigned short)serversWithKeysList.size());

		// For each server, write the header which consists of the IP/PORT.
		// Then go through the list of requested keys and write those
		for (index=0; index < serversWithKeysList.size(); index++)
		{
			SerializePlayerID(&(serversWithKeysList[index]->connectionIdentifier), &outputBitStream);

			outputBitStream.WriteCompressed((unsigned short)serversWithKeysList[index]->numberOfKeysFound);
			while (compressedString.GetNumberOfUnreadBits()>0)
			{
				// Generate a list of the indices of the servers that have one or more of the specified keys.
				stringCompressor->DecodeString(ruleIdentifier, 256, &compressedString);
				if (ruleIdentifier[0]==0)
					break;
				if (IsReservedRuleIdentifier(ruleIdentifier))
					continue;

				serversWithKeysList[index]->FindKey(ruleIdentifier);
				key=serversWithKeysList[index]->keyIndex;
				if (key>=0)
					SerializeRule(serversWithKeysList[index]->serverRules[key], &outputBitStream);
			}
		}
	}

	rakPeer->Send(&outputBitStream, MEDIUM_PRIORITY, RELIABLE, 0, packet->playerId, false);
}

void MasterServer::HandleUpdateServer(Packet *packet)
{
	GameServer *gameServer;
	bool newServerAdded;
	BitStream incomingBitStream((const char*)packet->data, packet->length, false);
	incomingBitStream.IgnoreBits(8*sizeof(unsigned char));

	gameServer = DeserializeServer(&incomingBitStream);
	gameServer->connectionIdentifier.binaryAddress=packet->playerId.binaryAddress;

	UpdateServerList(gameServer, true, &newServerAdded);

	if (newServerAdded)
	{
		#ifdef _SHOW_MASTER_SERVER_PRINTF
		printf("Server added. %i servers on the list\n", gameServerList.serverList.size());
		#endif
		gameServer->originationId=packet->playerId;
	}
	#ifdef _SHOW_MASTER_SERVER_PRINTF
	else
		printf("Server updated. %i servers on the list\n", gameServerList.serverList.size());
	#endif
}

void MasterServer::OnModifiedPacket(void)
{
#ifdef _SHOW_MASTER_SERVER_PRINTF
	printf("Modified packet.\n");
#endif
}

void MasterServer::HandleRelayedConnectionNotification(Packet *packet)
{
	char str[22];
	unsigned short clientGamePort, serverGamePort;
	BitStream incomingBitStream((const char*)packet->data, packet->length, false);
	incomingBitStream.IgnoreBits(8*sizeof(unsigned char));
	incomingBitStream.Read(clientGamePort);
	incomingBitStream.Read(serverGamePort);
	stringCompressor->DecodeString(str, 22, &incomingBitStream);

	BitStream outgoingBitStream;
	outgoingBitStream.Write((unsigned char)ID_RELAYED_CONNECTION_NOTIFICATION);
	// Assumes the game client is on the same computer as the master client
	outgoingBitStream.Write(packet->playerId.binaryAddress);
	outgoingBitStream.Write(clientGamePort);

	PlayerID targetID;
	rakPeer->IPToPlayerID(str, serverGamePort, &targetID);
        
	// Given the IP and port of the game system, give me the index into the game server list
	int serverIndex = gameServerList.GetIndexByPlayerID(targetID);

	if (serverIndex>=0)
	{
		rakPeer->Send(&outgoingBitStream, HIGH_PRIORITY, RELIABLE, 0, gameServerList.serverList[serverIndex]->originationId, false);
	}

	
}


/*************************************************************************
TransmitChatMessage: Send this message to the server
Param: message - The text message that is to be sent
*************************************************************************/
void MasterServer::HandleChatMessage(Packet *packet)
{
	char message[2048];
	char* mypacket;

	packet->data+sizeof(ChatMessageStruct);
 	sprintf(message, "%s\n", packet->data+sizeof(ChatMessageStruct));
			

	ChatMessageStruct s;//See MPlayerStructures.h
	//Set the ID so RakNet knows what type of packet it is
	s.typeId=ID_CHAT_MESSAGE;//See PacketEnumerations.h
	
	//Create the packet and size it to the ChatMessageStruct
	mypacket = new char[strlen(message) + 1 + sizeof(ChatMessageStruct)]; // 1 for the null terminator
	//Copy our ChatMessageStruct into the packet 
	memcpy(mypacket, &s, sizeof(ChatMessageStruct));
	//Copy the message into the packet
	memcpy(mypacket + sizeof(ChatMessageStruct), message, strlen(message) + 1);
   //Tell RakNet to send the packet to the server
	rakPeer->Send(mypacket, strlen(message) + 1 + sizeof(ChatMessageStruct), LOW_PRIORITY, RELIABLE_ORDERED, 1, UNASSIGNED_PLAYER_ID, true);
	delete [] mypacket;
	delete [] message;
};