#include "RF2d_Application.h"
#include "RF2d_MPlayer.h"
#include "RF3d_Global.h"
//My Files
#include "MPlayerStructures.h"


//Constructor
cMPlayerControl::cMPlayerControl(cApp* pParent)
{
	m_pParent = pParent;
	m_ServerClient = NULL;
	m_GameClient = NULL;
	m_Connected = false;
	m_PName = "Default Client";
	m_InGame = false;
	m_GameHost = false;
	m_Joining = false;
	m_ImReady = false;
	m_TheyReady = false;
	m_DefaultArmy = false;
	m_ServerClient=RakNetworkFactory::GetRakServerInterface();
	m_GameClient=RakNetworkFactory::GetRakClientInterface();
};

//Destructor
cMPlayerControl::~cMPlayerControl()
{
	if(m_Connected == true)//should be disconnected before now
	{
		Disconnect();//but better to be safe
	}
};

int	cMPlayerControl::GetSide(){ return m_MySide; };
void cMPlayerControl::SetSide(int side) { m_MySide = side; };


void cMPlayerControl::JoinGame(const char* ip)
{
	// Connect to a host at this IP address
	char myIP[20];
	strcpy(myIP,ip);
	m_GameHost = false;
	m_MySide = CHALLENGER;

	//m_GameClient=RakNetworkFactory::GetRakClientInterface();
	m_PeerIP = ip;
	m_ClientType = CLIENT;
	m_ConnectMode = PEER;
	if (m_GameClient)
	{

		//m_GameClient->Initialize(2, CLIENT_PORT, 0);
		//m_GameClient->SetMaximumIncomingConnections(1);
		if (m_GameClient->Connect(myIP, SERVER_PORT,CLIENT_PORT, 0, 30))
		{
			m_Connected = true;
			hge->System_Log("Connection Successful");
		}
		else
		{
			hge->System_Log("Bad connection attempt.  Terminating.");
		}

	}
};

void cMPlayerControl::HostGame()
{
	//Im hosting so wait for a client to join
//	m_ServerClient=RakNetworkFactory::GetRakServerInterface();
	m_GameHost = true;
	m_MySide = HOMETEAM;
	m_ConnectMode = SERVER;
	m_ClientType = HOST;
	if (m_ServerClient)
	{
		if (m_ServerClient->Start(1, 0, 30, SERVER_PORT))
		{
			m_Connected = true;
			hge->System_Log("Server started, waiting for connections.");
		}
		else
			hge->System_Log("Server failed to start.  Terminating.");

		//m_ServerClient->Initialize(2, SERVER_PORT, 0);
		//m_ServerClient->SetMaximumIncomingConnections(1);
	}
};

//Object properties
const char* cMPlayerControl::GetHostIP(){return m_HostIP.c_str();};
const char* cMPlayerControl::GetPeerIP(){return m_PeerIP.c_str();};
const char* cMPlayerControl::GetPlayerName(){return m_PName.c_str();};
const char* cMPlayerControl::GetMapName(){return m_MapName.c_str();};;
const char* cMPlayerControl::GetOPlayerName(){return m_OName.c_str();};
bool cMPlayerControl::IsGameHost(){return m_GameHost;};
bool cMPlayerControl::InGame(){return m_InGame;};
bool cMPlayerControl::IsJoining(){return m_Joining;};
bool cMPlayerControl::UseDefaultArmy(){return m_DefaultArmy;};
void cMPlayerControl::UseDefaultArmy(bool def){m_DefaultArmy = def;};
void cMPlayerControl::SetMapName(std::string map){m_MapName = map;};
void cMPlayerControl::SetPlayerData(std::string pname, std::string pass){ m_PName = pname; m_Pass = pass; };
/*************************************************************************
ConnectServer: Create a new instance of the rakClient and connect to the server
Param: ip - text containing the IP Address of the server ex: '192.168.0.1'
*************************************************************************/
/*
void cMPlayerControl::ConnectServer(char* ip, char* name)
{
	m_HostIP = ip;
	m_PName = name;
	m_ConnectMode = SERVER;

	// Pointers to the interfaces of our server and client.
	// Note we can easily have both in the same program
	m_Client=RakNetworkFactory::GetRakClientInterface();

	// Crude interface
	StaticClientDataStruct staticClientData;

	// Get our input
	strcpy(staticClientData.name, m_PName);
	// Note this passes by value, because otherwise people could
	// get access to and damage our internal data
	// UNASSIGNED_PLAYER_ID is to specify changing our own data, rather than our copy of another clients data
	m_Client->SetStaticClientData(UNASSIGNED_PLAYER_ID, (char*)&staticClientData, sizeof(StaticClientDataStruct));

	hge->System_Log("Attempting connection to %s on port %i using local port %i",m_HostIP,SERVER_PORT,CLIENT_PORT);

	// Connecting the client is very simple.  0 means we don't care about
	// a connectionValidationInteger, and false for low priority threads
	//if (m_client->Connect("192.168.1.102", SERVER_PORT, CLIENT_PORT, 0, 30))
	if (m_Client->Connect(m_HostIP, SERVER_PORT, CLIENT_PORT, 0, 30))
	{
		m_Connected = true;
		hge->System_Log("Connection Successful");
	}
	else
	{
		hge->System_Log("Bad connection attempt.  Terminating.");
	}

};
*/

bool cMPlayerControl::IsConnected()
{
	bool tmp = false;
	switch (m_ConnectMode)
		{
		case SERVER:
			// We're done with the packet
			tmp = m_ServerClient->IsActive();
			break;
		case PEER:
			// We're done with the packet
			tmp = m_GameClient->IsConnected();
			break;
		}
	return tmp && m_Connected;
};

bool  cMPlayerControl::IsGameReady()
{
	return m_ImReady && m_TheyReady;
};

/*************************************************************************
Disconnect: Break the connection to the server and free the instance of the rakClient
Param: None
*************************************************************************/
void cMPlayerControl::Disconnect()
{
	if (m_Connected == true)
	{
		hge->System_Log("MPlayer Disconnect");
		// Be nice and let the server know we quit.
		switch (m_ConnectMode)
		{
		case SERVER:
			m_ServerClient->Disconnect(300);
			break;
		case PEER:
			m_GameClient->Disconnect(300);
			break;
		};
		// We're done with the network
		RakNetworkFactory::DestroyRakClientInterface(m_GameClient);
		RakNetworkFactory::DestroyRakServerInterface(m_ServerClient);
		m_GameClient = NULL;
		m_ServerClient = NULL;
		m_Connected = false;
	}
};

/*************************************************************************
Update: Polling cycle to watch for new packets from the server and
		process them.
Param: None
*************************************************************************/
void cMPlayerControl::Update()
{
	if (m_Connected == true)
	{
		// Holds packets
		Packet* packet;

		// This sleep keeps RakNet responsive
		Sleep(1);

		switch (m_ConnectMode)
		{
		case SERVER:
			// Get a packet from  the Server
			if (m_ServerClient) packet = m_ServerClient->Receive();
			break;
		case PEER:
				// Get a packet from  the Peer
			if (m_GameClient) packet = m_GameClient->Receive();
			break;
		}

		ProcessPacket(packet);

		switch (m_ConnectMode)
		{
		case SERVER:
			// We're done with the packet
			m_ServerClient->DeallocatePacket(packet);
			break;
		case PEER:
			// We're done with the packet
			m_GameClient->DeallocatePacket(packet);
			break;
		}
	}
};

void cMPlayerControl::ProcessPacket(Packet* packet)
{

	// GetPacketIdentifier returns this
	unsigned char packetIdentifier;

	if (packet==0)
		return; // Didn't get any packets

	// We got a packet, get the identifier with our handy function
	packetIdentifier = GetPacketIdentifier(packet);

	// Check if this is a network message packet
	switch (packetIdentifier)
	{
		case ID_DISCONNECTION_NOTIFICATION:
				// Connection lost normally
			hge->System_Log("ID_DISCONNECTION_NOTIFICATION");
			break;
		case ID_REMOTE_DISCONNECTION_NOTIFICATION: // Server telling the clients of another client disconnecting gracefully.  You can manually broadcast this in a peer to peer enviroment if you want.
			hge->System_Log("ID_REMOTE_DISCONNECTION_NOTIFICATION");
			break;
		case ID_REMOTE_CONNECTION_LOST: // Server telling the clients of another client disconnecting forcefully.  You can manually broadcast this in a peer to peer enviroment if you want.
			hge->System_Log("ID_REMOTE_CONNECTION_LOST");
			break;
		case ID_REMOTE_NEW_INCOMING_CONNECTION: // Server telling the clients of another client connecting.  You can manually broadcast this in a peer to peer enviroment if you want.
			hge->System_Log("ID_REMOTE_NEW_INCOMING_CONNECTION");
			break;
		case ID_REMOTE_EXISTING_CONNECTION: // Server telling you of an existing connection that was there before you connected
			hge->System_Log("ID_REMOTE_EXISTING_CONNECTION");
			break;
		case ID_CONNECTION_BANNED: // Banned from this server
			hge->System_Log("We are banned from this server.");
			break;
		case ID_RECEIVED_STATIC_DATA:
				// Got static data
			hge->System_Log("ID_RECEIVED_STATIC_DATA");
			//hge->System_Log("Server name: %s", ((EnumerationDataStruct*)(m_Client->GetStaticServerData())->GetData())->serverName);
			//hge->System_Log("Server MOTD: %s", ((EnumerationDataStruct*)(m_Client->GetStaticServerData())->GetData())->MOTD);
			//Send the MotD to the GUI for displaying to the user
				m_pParent->GetGUIManager()->ProcessPacket(packet,packetIdentifier);
			break;
		case ID_REMOTE_PORT_REFUSED:
			  //Could be a firewall issue
			hge->System_Log("ID_REMOTE_PORT_REFUSED");
			break;
		case ID_NEW_INCOMING_CONNECTION:
			hge->System_Log("ID_NEW_INCOMING_CONNECTION");
			break;
		case ID_NO_FREE_INCOMING_CONNECTIONS:
				// Sorry, the server is full.  I don't do anything here but
			// A real app should tell the user
			hge->System_Log("ID_NO_FREE_INCOMING_CONNECTIONS");
			break;
		case ID_MODIFIED_PACKET:
			// Cheater!
			hge->System_Log("ID_MODIFIED_PACKET");
			break;
		case ID_CONNECTION_LOST:
			// Couldn't deliver a reliable packet - i.e. the other system was abnormally
			// terminated
			hge->System_Log("ID_CONNECTION_LOST");
			break;
		case ID_CONNECTION_REQUEST_ACCEPTED:
			// This tells the client they have connected
			if (!m_GameHost)
				{
					hge->System_Log("ID_CONNECTION_REQUEST_ACCEPTED");
					TransmitJoinGame();
				}
			break;
		case ID_CHAT_MESSAGE:
			//This a chat message so send it to the GUI
			m_pParent->GetGUIManager()->ProcessPacket(packet,packetIdentifier);
			break;
		case ID_TURN_DONE:
		case ID_CHAR_TURN:
		case ID_ADD_NPC:
		case ID_PATH_NPC:
		case ID_CAST_SPELL:
		case ID_MELEE_ATTACK:
				m_pParent->GetGameObjMangr()->ProcessPacket(packet,packetIdentifier);
			break;
		case ID_CHANGE_MAP:
			{
				if (!m_GameHost)
				{
					hge->System_Log("Client told to change map");
					ChangeMapStruct s = *((ChangeMapStruct *) packet->data);
					if (m_DefaultArmy)
						m_pParent->ChangeMap(s.MapName, m_MySide);
					else
						m_pParent->ChangeMap(s.MapName, CUSTTEAM);
					m_pParent->GetGameObjMangr()->xmit_AllNPCs();
					m_pParent->ChangeGameState(MPGAME, CURRENT);
					//m_pParent->StartRound();
					m_InGame = true;
				}
			}
			break;
		case ID_START_MAP:
			{
				if (!m_InGame && !m_GameHost)
				{
					hge->System_Log("Client told to start map");
					ChangeMapStruct s = *((ChangeMapStruct *) packet->data);
					if (m_DefaultArmy)
					{
						m_pParent->ChangeMap(s.MapName, m_MySide);
						m_pParent->GetGameObjMangr()->xmit_AllNPCs();
						m_pParent->GetGUIManager()->ShowMPlayGameWindows();
						m_pParent->ChangeGameState(MPGAME, CURRENT);
					}
					else
					{
						m_pParent->ChangeMap(s.MapName, CUSTTEAM);
						m_pParent->GetGUIManager()->ShowCharSelectWindow();
						m_pParent->GetGUIManager()->ShowChatWindow();
						m_pParent->ChangeGameState(TEAM, ALL);
					}
					hge->System_Log("Send Client NPCs");

					m_InGame = true;
					m_ImReady = true;
					TransmitClientReady();
				}
			}
		case ID_JOIN_GAME:
			{

				if (m_GameHost)
				{
					hge->System_Log("Server Got Join Game");
					//send start map
					TransmitStartMap(m_pParent->GetMainMap()->MapName());
					//send NPCs
					m_pParent->GetGameObjMangr()->xmit_AllNPCs();
					//Tell the client that teh server is ready to play
					m_ImReady = true;
					TransmitServerReady();
				}
			}
			break;
		case ID_SERVER_READY:
			{
				hge->System_Log("rcv Server Ready");
				m_TheyReady = true;
			}
            break;
		case ID_CLIENT_READY:
			{
				hge->System_Log("rcv Client Ready");
				m_TheyReady = true;
			}
            break;
		case ID_MAP_LOADED:
			break;
		case ID_PLAYER_NAME:
			{
				PlayerNameStruct s = *((PlayerNameStruct *) packet->data);
				m_OName = s.PName;
			}
            break;
		default:
			hge->System_Log("Unknown packet type in cMPlayerControl::Update.  Type %i with length %i", packetIdentifier, packet->length);
			break;
	}
};

//Not Currently used but seemed like a good idea
//from the RakNet Samples
void cMPlayerControl::OnLostConnection(void)
{
#ifdef _SHOW_MASTER_SERVER_PRINTF
	hge->System_Log("Connection lost.\n");
#endif
}
void cMPlayerControl::OnMasterServerFull(void)
{
#ifdef _SHOW_MASTER_SERVER_PRINTF
	hge->System_Log("Server full.\n");
#endif
}
void cMPlayerControl::OnModifiedPacket(void)
{
#ifdef _SHOW_MASTER_SERVER_PRINTF
	hge->System_Log("Modified packet.\n");
#endif
}

//The following 4 function will be used when I switch to using a Lobby Server
void cMPlayerControl::OnGameServerListQueryComplete(void)
{
#ifdef _SHOW_MASTER_SERVER_PRINTF
	hge->System_Log("Query complete.\n");
#endif
}
// Event when a game client wants to connect to our server
// You should call AdvertiseSystem to the passed IP and port from your game instance
void cMPlayerControl::OnConnectionRequest(const char *clientIP, unsigned short clientPort)
{
#ifdef _SHOW_MASTER_SERVER_PRINTF
	hge->System_Log("Master server indicates a connection request from %s:%i.\n", clientIP, clientPort);
#endif
	switch (m_ConnectMode)
	{
	case SERVER:
			// We're done with the packet
			m_ServerClient->AdvertiseSystem((char*)clientIP, clientPort);
			break;
	case PEER:
			// We're done with the packet
			m_GameClient->AdvertiseSystem((char*)clientIP, clientPort);
			break;
	}
};
void cMPlayerControl::HandleServerListResponse(Packet *packet, bool overwriteExisting)
{
	return;
};
void cMPlayerControl::HandleRelayedConnectionNotification(Packet *packet)
{
	return;
};

// Copied from Multiplayer.cpp
// If the first byte is ID_TIMESTAMP, then we want the 5th byte
// Otherwise we want the 1st byte
unsigned char cMPlayerControl::GetPacketIdentifier(Packet *p)
{
	if (p==0)
		return 255;
	if ((unsigned char)p->data[0] == ID_TIMESTAMP)
	{
//		assert(p->length > sizeof(unsigned char) + sizeof(unsigned long));
		return (unsigned char) p->data[sizeof(unsigned char) + sizeof(unsigned long)];
	}
	else
		return (unsigned char) p->data[0];

}

/*************************************************************************
TransmitChatMessage: Send this message to the server
Param: message - The text message that is to be sent
*************************************************************************/
void cMPlayerControl::TransmitChatMessage(const char *message)
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
	switch (m_ConnectMode)
	{
	case SERVER:
		m_ServerClient->Send(packet, strlen(message) + 1 + sizeof(ChatMessageStruct), LOW_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_PLAYER_ID, true);
		break;
	case PEER:
		m_GameClient->Send(packet, strlen(message) + 1 + sizeof(ChatMessageStruct), LOW_PRIORITY, RELIABLE_ORDERED, 1);
		break;
	}
	delete [] packet;
};


void cMPlayerControl::TransmitMoveCharacter(char *charID, int x, int y)
{
	MoveCharacterStruct s;//See MPlayerStructures.h
	//Set the ID so RakNet knows what type of packet it is
	s.typeId=ID_PATH_NPC;//See PacketEnumerations.h
	//The struct has a property for the data we want so fill it in

	strcpy(s.charId, charID);
	s.destX = x;
	s.destY = y;
	switch (m_ConnectMode)
	{
	case SERVER:
			// We're done with the packet
			m_ServerClient->Send((char*)&s,sizeof(MoveCharacterStruct), LOW_PRIORITY, RELIABLE_ORDERED, 1, UNASSIGNED_PLAYER_ID, true);
			break;
	case PEER:
			// We're done with 6the packet
			m_GameClient->Send((char*)&s,sizeof(MoveCharacterStruct), LOW_PRIORITY, RELIABLE_ORDERED, 1);
			break;
	}
};


void cMPlayerControl::TransmitStartMap(char *MapName)
{
	hge->System_Log("TransmitStartMap");
	ChangeMapStruct s;//See MPlayerStructures.h
	//Set the ID so RakNet knows what type of packet it is
	s.typeId=ID_START_MAP;//See PacketEnumerations.h
	//The struct has a property for the data we want so fill it in

	strcpy(s.MapName, MapName);
	//Tell RakNet to send the packet to the server
	switch (m_ConnectMode)
	{
	case SERVER:
			// We're done with the packet
			m_ServerClient->Send((char*)&s,sizeof(ChangeMapStruct), LOW_PRIORITY, RELIABLE_ORDERED, 1, UNASSIGNED_PLAYER_ID, true);
			break;
	case PEER:
			// We're done with 6the packet
			m_GameClient->Send((char*)&s,sizeof(ChangeMapStruct), LOW_PRIORITY, RELIABLE_ORDERED, 1);
			break;
	}

};

void cMPlayerControl::TransmitPlayerName(char *PName)
{
	hge->System_Log("TransmitPlayerName");
	PlayerNameStruct s;//See MPlayerStructures.h
	//Set the ID so RakNet knows what type of packet it is
	s.typeId=ID_PLAYER_NAME;//See PacketEnumerations.h
	//The struct has a property for the data we want so fill it in

	strcpy(s.PName, PName);
	//Tell RakNet to send the packet to the server
	switch (m_ConnectMode)
	{
	case SERVER:
			// We're done with the packet
			m_ServerClient->Send((char*)&s,sizeof(PlayerNameStruct), LOW_PRIORITY, RELIABLE_ORDERED, 1, UNASSIGNED_PLAYER_ID, true);
			break;
	case PEER:
			// We're done with 6the packet
			m_GameClient->Send((char*)&s,sizeof(PlayerNameStruct), LOW_PRIORITY, RELIABLE_ORDERED, 1);
			break;
	}

};

void cMPlayerControl::TransmitChangeMap(char *MapName)
{
	hge->System_Log("TransmitChangeMap");
	ChangeMapStruct s;//See MPlayerStructures.h
	//Set the ID so RakNet knows what type of packet it is
	s.typeId=ID_CHANGE_MAP;//See PacketEnumerations.h
	//The struct has a property for the data we want so fill it in

	strcpy(s.MapName, MapName);
	//Tell RakNet to send the packet to the server
	switch (m_ConnectMode)
	{
	case SERVER:
			// We're done with the packet
			m_ServerClient->Send((char*)&s,sizeof(ChangeMapStruct), LOW_PRIORITY, RELIABLE_ORDERED, 1, UNASSIGNED_PLAYER_ID, true);
			break;
	case PEER:
			// We're done with 6the packet
			m_GameClient->Send((char*)&s,sizeof(ChangeMapStruct), LOW_PRIORITY, RELIABLE_ORDERED, 1);
			break;
	}
};


void cMPlayerControl::TransmitCastSpell(int spellID, char *casterID, char *targetID)
{
	CastSpellStruct s;//See MPlayerStructures.h
	//Set the ID so RakNet knows what type of packet it is
	s.typeId=ID_CAST_SPELL;//See PacketEnumerations.h
	//The struct has a property for the data we want so fill it in

	strcpy(s.sourcID, casterID);
	strcpy(s.trgtID, targetID);
	s.spellId = spellID;
	//Tell RakNet to send the packet to the server
	switch (m_ConnectMode)
	{
	case SERVER:
			// We're done with the packet
			m_ServerClient->Send((char*)&s,sizeof(CastSpellStruct), LOW_PRIORITY, RELIABLE_ORDERED, 1, UNASSIGNED_PLAYER_ID, true);
			break;
	case PEER:
			// We're done with 6the packet
			m_GameClient->Send((char*)&s,sizeof(CastSpellStruct), LOW_PRIORITY, RELIABLE_ORDERED, 1);
			break;
	}
};

void cMPlayerControl::TransmitMeleeAttack(char *attackerID, char *targetID)
{
	MeleeAttackStruct s;//See MPlayerStructures.h
	//Set the ID so RakNet knows what type of packet it is
	s.typeId=ID_MELEE_ATTACK;//See PacketEnumerations.h
	//The struct has a property for the data we want so fill it in

	strcpy(s.sourcID, attackerID);
	strcpy(s.trgtID, targetID);

	//Tell RakNet to send the packet to the server
	switch (m_ConnectMode)
	{
	case SERVER:
			// We're done with the packet
			m_ServerClient->Send((char*)&s,sizeof(MeleeAttackStruct), LOW_PRIORITY, RELIABLE_ORDERED, 1, UNASSIGNED_PLAYER_ID, true);
			break;
	case PEER:
			// We're done with 6the packet
			m_GameClient->Send((char*)&s,sizeof(MeleeAttackStruct), LOW_PRIORITY, RELIABLE_ORDERED, 1);
			break;
	}
};

void cMPlayerControl::TransmitAddNPC(int dir, int action, int x, int y, char *guid, char *fileName, char *name)
{
	hge->System_Log("TransmitAddNPC");
	AddNPCStruct s;//See MPlayerStructures.h
	//Set the ID so RakNet knows what type of packet it is
	s.typeId=ID_ADD_NPC;//See PacketEnumerations.h
	//The struct has a property for the data we want so fill it in
	strcpy(s.guid, guid);
	strcpy(s.fileName, fileName);
	strcpy(s.name, name);
	s.action = action;
	s.dir = dir;
	s.x = x;
	s.y = y;
	//Tell RakNet to send the packet to the server
	switch (m_ConnectMode)
	{
	case SERVER:
			// We're done with the packet
			m_ServerClient->Send((char*)&s,sizeof(AddNPCStruct), LOW_PRIORITY, RELIABLE_ORDERED, 1, UNASSIGNED_PLAYER_ID, true);
			break;
	case PEER:
			// We're done with 6the packet
			m_GameClient->Send((char*)&s,sizeof(AddNPCStruct), LOW_PRIORITY, RELIABLE_ORDERED, 1);
			break;
	}

};

void cMPlayerControl::TransmitJoinGame()
{
	hge->System_Log("TransmitJoinGame");
	JoinGameStruct s;//See MPlayerStructures.h
	//Set the ID so RakNet knows what type of packet it is
	s.typeId=ID_JOIN_GAME;//See PacketEnumerations.h
	//The struct has a property for the data we want so fill it in

	//Tell RakNet to send the packet to the server
	switch (m_ConnectMode)
	{
	case SERVER:
			// We're done with the packet
			m_ServerClient->Send((char*)&s,sizeof(JoinGameStruct), LOW_PRIORITY, RELIABLE_ORDERED, 1, UNASSIGNED_PLAYER_ID, true);
			break;
	case PEER:
			// We're done with 6the packet
			m_GameClient->Send((char*)&s,sizeof(JoinGameStruct), LOW_PRIORITY, RELIABLE_ORDERED, 1);
			break;
	}
	m_Joining = true;
};

void cMPlayerControl::TransmitServerReady()
{
	hge->System_Log("TransmitServerReady");
	ServerReadyStruct s;//See MPlayerStructures.h
	//Set the ID so RakNet knows what type of packet it is
	s.typeId=ID_SERVER_READY;//See PacketEnumerations.h
	//The struct has a property for the data we want so fill it in

	//Tell RakNet to send the packet to the server
	// We're done with the packet
	m_ServerClient->Send((char*)&s,sizeof(ServerReadyStruct), LOW_PRIORITY, RELIABLE_ORDERED, 1, UNASSIGNED_PLAYER_ID, true);

};
void cMPlayerControl::TransmitClientReady()
{
	hge->System_Log("TransmitClientReady");
	ClientReadyStruct s;//See MPlayerStructures.h
	//Set the ID so RakNet knows what type of packet it is
	s.typeId=ID_CLIENT_READY;//See PacketEnumerations.h
	//The struct has a property for the data we want so fill it in

	// We're done with 6the packet
	m_GameClient->Send((char*)&s,sizeof(ClientReadyStruct), LOW_PRIORITY, RELIABLE_ORDERED, 1);
};


void cMPlayerControl::TransmitCharTurn(char *charID)
{
	CharTurnStruct s;//See MPlayerStructures.h
	//Set the ID so RakNet knows what type of packet it is
	s.typeId=ID_CHAR_TURN;//See PacketEnumerations.h
	//The struct has a property for the data we want so fill it in

	strcpy(s.charID, charID);
	switch (m_ConnectMode)
	{
	case SERVER:
			// We're done with the packet
			m_ServerClient->Send((char*)&s,sizeof(CharTurnStruct), LOW_PRIORITY, RELIABLE_ORDERED, 1, UNASSIGNED_PLAYER_ID, true);
			break;
	case PEER:
			// We're done with 6the packet
			m_GameClient->Send((char*)&s,sizeof(CharTurnStruct), LOW_PRIORITY, RELIABLE_ORDERED, 1);
			break;
	}

};

void cMPlayerControl::TransmitTurnDone()
{
	TurnDoneStruct s;//See MPlayerStructures.h
	//Set the ID so RakNet knows what type of packet it is
	s.typeId=ID_TURN_DONE;//See PacketEnumerations.h
	//The struct has a property for the data we want so fill it in

	switch (m_ConnectMode)
	{
	case SERVER:
			// We're done with the packet
			m_ServerClient->Send((char*)&s,sizeof(TurnDoneStruct), LOW_PRIORITY, RELIABLE_ORDERED, 1, UNASSIGNED_PLAYER_ID, true);
			break;
	case PEER:
			// We're done with 6the packet
			m_GameClient->Send((char*)&s,sizeof(TurnDoneStruct), LOW_PRIORITY, RELIABLE_ORDERED, 1);
			break;
	}

};

