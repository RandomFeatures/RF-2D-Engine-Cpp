// ----------------------------------------------------------------------
// RakNet version 1.0
// Filename ChatExample.cpp
// Created by Rakkar Software (rakkar@rakkarsoftware.com) January 24, 2003
// Very basic chat engine example
// ----------------------------------------------------------------------

#include "..\rf2dEngine\source\PacketEnumerations.h"
#include "RakNetworkFactory.h"
#include "RakServerInterface.h"
#include "NetworkTypes.h"
#include "BitStream.h"
#include "..\rf2dEngine\source\MPlayerStructures.h"
#include <assert.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h> // Sleep
#else
#include <unistd.h> // usleep
#endif
// We copy this from Multiplayer.cpp to keep things all in one file for this example
unsigned char GetPacketIdentifier(Packet *p);
void TransmitChatMessage(const char *message);
void TransmitMoveCharacter(char *charID, int x, int y);

RakServerInterface *server;
// Holds enumeration data
//const int MOTD_MAXIMUM_LENGTH=50; // Characters allocated for the message of the day, used for enumerations
//const int SERVER_NAME_MAXIMUM_LENGTH=40; // Characters allocated for the server name



const int SCDS_MAXIMUM_NAME_LENGTH=40;


int main(void)
{
	// Pointers to the interfaces of our server and client.
	// Note we can easily have both in the same program
	server=RakNetworkFactory::GetRakServerInterface();
	RakNetStatisticsStruct *rss;
	//server->InitializeSecurity(0,0);

	int i = server->GetNumberOfAddresses();

	// Holds packets
	Packet* p;

	// GetPacketIdentifier returns this
	unsigned char packetIdentifier;

	// Record the first client that connects to us so we can pass it to the ping function
	PlayerID clientID=UNASSIGNED_PLAYER_ID;

	// Holds user data
	char portstring[30];
	EnumerationDataStruct enumerationDataStruct;

	// A server
//	puts("Enter the server port to listen on");
//	gets(portstring);
//	if (portstring[0]==0)
		strcpy(portstring, "60005");
//	puts("Enter the server name  (up to 40 characters)");
//	gets(enumerationDataStruct.serverName);
//	if (enumerationDataStruct.serverName[0]==0)
		strcpy(enumerationDataStruct.serverName, "System");
	puts("Enter the server message of the day  (up to 50 characters)");
	gets(enumerationDataStruct.MOTD);
	if (enumerationDataStruct.MOTD[0]==0)
		strcpy(enumerationDataStruct.MOTD, "System: Welcome to the Random Features Inc. Chat Server");
	// Note this passes by value, because otherwise people could get access
	// to and damage our internal data
	server->SetStaticServerData((char*)&enumerationDataStruct, sizeof(EnumerationDataStruct));
	puts("Starting server.\n");
	// Starting the server is very simple.  2 players allowed.
	// 0 means we don't care about a connectionValidationInteger, and false
	// for low priority threads
	bool b = server->Start(32, 0, 30, atoi(portstring));
	if (b)
		puts("Server started, waiting for connections.\n");
	else
	{ 
		puts("Server failed to start.  Terminating.\n");
		exit(1);
	}


	puts("Numbers '1' - '9' to adjust simulation speed for the client, '10' - real time.");
	puts("'quit' to quit.\n'stat' to show stats.\n'ping' to ping.\n'ban' to ban an IP from connecting.\n Type to talk.");

	char message[2048];

	// Loop for input
	while (1)
	{

// This sleep keeps RakNet responsive
#ifdef _WIN32
		Sleep(30);
#else
		usleep(30 * 1000);
#endif


		if (kbhit())
		{
			// Notice what is not here: something to keep our network running.  It's
			// fine to block on gets or anything we want
			// Because the network engine was painstakingly written using threads.
			gets(message);

			// Clear remaining input
			while (kbhit()) getch();

			if (strcmp(message, "quit")==0)
			{
				puts("Quitting.");
				break;
			}

			if (strcmp(message, "stat")==0)
			{
				rss=server->GetStatistics(server->GetPlayerIDFromIndex(0));
				StatisticsToString(rss, message, 2);
				printf("%s", message);
				printf("Ping %i\n", server->GetAveragePing(server->GetPlayerIDFromIndex(0)));
		
				continue;
			}

			if (strcmp(message, "ping")==0)
			{
					if (server->GetConnectedPlayers()>0)
					server->PingPlayer(clientID);

				continue;
			}

			if (strcmp(message, "ban")==0)
			{
				printf("Enter IP to ban.  You can use * as a wildcard\n");
				gets(message);
				server->AddToBanList(message);
				printf("IP %s added to ban list.\n", message);

				continue;
			}

			// Message now holds what we want to broadcast
			char message2[420];
			// Append Server: to the message so clients know that it ORIGINATED from the server
			// All messages to all clients come from the server either directly or by being
			// relayed from other clients
			message2[0]=0;
			strcpy(message2, "[Server]: ");
			strcat(message2, message);
		
  			TransmitChatMessage(message2);
			//server->Send(message2, strlen(message2)+1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_PLAYER_ID, true);
		}

		// Get a packet from either the server or the client

		p = server->Receive();

		
		if (p==0)
			continue; // Didn't get any packets

		// We got a packet, get the identifier with our handy function
		packetIdentifier = GetPacketIdentifier(p);

		//printf("%i\n",packetIdentifier);

		// Check if this is a network message packet
		switch (packetIdentifier)
		{
			case ID_DISCONNECTION_NOTIFICATION:
				  // Connection lost normally
				printf("ID_DISCONNECTION_NOTIFICATION\n");
				break;

		
			case ID_NEW_INCOMING_CONNECTION:
				 // Somebody connected.  We have their IP now
				printf("ID_NEW_INCOMING_CONNECTION\n");
				clientID=p->playerId; // Record the player ID of the client
				break;

			case ID_RECEIVED_STATIC_DATA:
				 // Got static data
				printf("ID_RECEIVED_STATIC_DATA\n");
				printf("%s has connected\n", ((StaticClientDataStruct*)(server->GetStaticClientData(p->playerId))->GetData())->name);
				//TODO Send ClientList to all!!
				//sprintf(message, "System: %s", enumerationDataStruct.MOTD);
				//server->Send(message, strlen(message)+1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p->playerId, false);

				break;

			case ID_MODIFIED_PACKET:
				// Cheater!
				printf("ID_MODIFIED_PACKET\n");
				break;

			case ID_CONNECTION_LOST:
				// Couldn't deliver a reliable packet - i.e. the other system was abnormally
				// terminated
				printf("ID_CONNECTION_LOST\n");
				break;
		
			case ID_PATH_NPC:
				{
					MoveCharacterStruct s1 = *((MoveCharacterStruct *) p->data);
					TransmitMoveCharacter(s1.charId,s1.destX,s1.destY);
					printf("Path NPC\n");
				}
				break;
		
			case 48: //ID_CHAT_MESSAGE:
				//Append the client name
 				sprintf(message, "[%s]: %s\n", ((StaticClientDataStruct*)(server->GetStaticClientData(p->playerId))->GetData())->name, p->data+sizeof(ChatMessageStruct));
				//Broadcast to all clients
				TransmitChatMessage(message);
				//Display for the server
				printf(message);
				break;
				//server->Send((char*)p, sizeof(p)+1, LOW_PRIORITY, RELIABLE_ORDERED, 1, UNASSIGNED_PLAYER_ID, true);
			//default:
				 //Tell RakNet to send the packet to the server
			//	server->Send((char*)p, sizeof(p)+1, LOW_PRIORITY, RELIABLE_ORDERED, 1, UNASSIGNED_PLAYER_ID, true);
			//	printf("Path NPC\n");
				// The server knows the static data of all clients, so we can prefix the message
				// With the name data
				//printf("%s: %s\n", ((StaticClientDataStruct*)(server->GetStaticClientData(p->playerId))->GetData())->name, p->data);

				// Relay the message.  We prefix the name for other clients.  This demonstrates
				// That messages can be changed on the server before being broadcast
				// Sending is the same as before
				//sprintf(message, "%s: %s", ((StaticClientDataStruct*)(server->GetStaticClientData(p->playerId))->GetData())->name, p->data);
				//server->Send(message, strlen(message)+1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p->playerId, true);
				//server->Send(message, strlen(message)+1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_PLAYER_ID, true);
			
			//	break;
		}

		// We're done with the packet
		server->DeallocatePacket(p);
	}

	server->Disconnect(300);
	// We're done with the network
	RakNetworkFactory::DestroyRakServerInterface(server);

	return 0;
}

// Copied from Multiplayer.cpp
// If the first byte is ID_TIMESTAMP, then we want the 5th byte
// Otherwise we want the 1st byte
unsigned char GetPacketIdentifier(Packet *p)
{
	if (p==0)
		return 255;

	if ((unsigned char)p->data[0] == ID_TIMESTAMP)
	{
		assert(p->length > sizeof(unsigned char) + sizeof(unsigned long));
		return (unsigned char) p->data[sizeof(unsigned char) + sizeof(unsigned long)];
	}
	else
		return (unsigned char) p->data[0];
}


/*************************************************************************
TransmitChatMessage: Send this message to the server
Param: message - The text message that is to be sent
*************************************************************************/
void TransmitChatMessage(const char *message)
{
	if (message==0)
		return;
	
	ChatMessageStruct s;//See MPlayerStructures.h
	//Set the ID so RakNet knows what type of packet it is
	s.typeId=ID_CHAT_MESSAGE;//See PacketEnumerations.h
	
	//Create the packet and size it to the ChatMessageStruct
	char* packet;
	packet = new char[strlen(message) + 1 + sizeof(ChatMessageStruct)]; // 1 for the null terminator
	//Copy our ChatMessageStruct into the packet 
	memcpy(packet, &s, sizeof(ChatMessageStruct));
	//Copy the message into the packet
	memcpy(packet + sizeof(ChatMessageStruct), message, strlen(message) + 1);
   //Tell RakNet to send the packet to the server
	server->Send(packet, strlen(message) + 1 + sizeof(ChatMessageStruct), LOW_PRIORITY, RELIABLE_ORDERED, 1, UNASSIGNED_PLAYER_ID, true);
	delete [] packet;
};

void TransmitMoveCharacter(char *charID, int x, int y)
{
	MoveCharacterStruct s;//See MPlayerStructures.h
	//Set the ID so RakNet knows what type of packet it is
	s.typeId=ID_PATH_NPC;//See PacketEnumerations.h
	//The struct has a property for the data we want so fill it in
	
	strcpy(s.charId, charID);
	s.destX = x;
	s.destY = y;
	//Tell RakNet to send the packet to the server
	server->Send((char*)&s,sizeof(MoveCharacterStruct), HIGH_PRIORITY, RELIABLE_ORDERED,0, UNASSIGNED_PLAYER_ID, true);
};

