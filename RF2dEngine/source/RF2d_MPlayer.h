
//RakNet Files
#include "PacketEnumerations.h"
#include "RakNetworkFactory.h"
#include "RakServerInterface.h"
#include "RakClientInterface.h"
#include "NetworkTypes.h"
#include "BitStream.h"
#include <string>


using namespace RakNet;

#ifndef RF2D_MPLAYER_H
#define RF2D_MPLAYER_H


#define CLIENT_PORT 60004
#define SERVER_PORT 60005
//#define GAME_CLIENT_PORT 60006

enum CONNECTMODE { SERVER, PEER };
enum CLIENTTYPE { HOST, CLIENT };

class cApp;

class cMPlayerControl
{
public:

	cMPlayerControl(cApp* pParent);
	~cMPlayerControl();
	//void ConnectServer(char* ip, char* name);
	void JoinGame(const char* ip);
	void HostGame();
	void Disconnect();
	void Update();
	void ProcessPacket(Packet* packet);
	void TransmitChatMessage(const char *message);
	void TransmitMoveCharacter(char *charID, int x, int y);
	void TransmitCastSpell(int spellID, char *casterID, char *targetID);
	void TransmitMeleeAttack(char *attackerID, char *targetID);
	void TransmitChangeMap(char *MapName);
	void TransmitStartMap(char *MapName);
	void TransmitPlayerName(char *PName);
	void TransmitJoinGame();
	void TransmitServerReady();
	void TransmitClientReady();
	void TransmitAddNPC(int dir, int action, int x, int y, char *guid, char *fileName, char *name);
	void TransmitCharTurn(char *charID);
	void TransmitTurnDone();
	bool IsGameHost();
	bool InGame();
	bool IsJoining();
	bool IsConnected();
	bool IsGameReady();
	bool UseDefaultArmy();
	void UseDefaultArmy(bool def);
	const char* GetOPlayerName();
	const char* GetMapName();
	void SetMapName(std::string map);
	const char* GetHostIP();
	const char* GetPeerIP();
	const char* GetPlayerName();
	int	GetSide();
	void SetSide(int side);
	void SetPlayerData(std::string pname, std::string pass);
	// We copy this from Multiplayer.cpp to keep things all in one file for this example
	unsigned char GetPacketIdentifier(Packet *p);
	// Event if we lose the connection to the master server
	void OnLostConnection(void);
	// Event if the master server is full when we try to connect
	void OnMasterServerFull(void);
	// Event if a packet was tampered with mid-steram
	void OnModifiedPacket(void);
	// Event when we complete a query
	void OnGameServerListQueryComplete(void);
	// Event when a game client wants to connect to our server
	// You should call AdvertiseSystem to the passed IP and port from your game instance
	void OnConnectionRequest(const char *clientIP, unsigned short clientPort);

private:
	cApp				*m_pParent;
	RakServerInterface	*m_ServerClient;
	RakClientInterface	*m_GameClient;

	CONNECTMODE			 m_ConnectMode;
	CLIENTTYPE			 m_ClientType;
	//RakClientInterface  *m_Client;
	std::string			 m_HostIP;
	std::string			 m_PeerIP;
	std::string			 m_PName;
	std::string			 m_MapName;
	std::string			 m_Pass;
	std::string			 m_OName;
	bool				 m_DefaultArmy;
	bool				 m_Connected;
	bool				 m_GameHost;
	bool				 m_InGame;
	bool				 m_Joining;
	bool				 m_ImReady;
	bool				 m_TheyReady;
	int					 m_MySide;

protected:
	void HandleServerListResponse(Packet *packet, bool overwriteExisting);
	void HandleRelayedConnectionNotification(Packet *packet);
};


#endif