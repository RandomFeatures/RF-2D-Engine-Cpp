#ifndef MULTIPLAYER_TYPES
#define MULTIPLAYER_TYPES

// Holds enumeration data
const int MOTD_MAXIMUM_LENGTH=50; // Characters allocated for the message of the day, used for enumerations
const int SERVER_NAME_MAXIMUM_LENGTH=40; // Characters allocated for the server name

#pragma pack(1)
struct EnumerationDataStruct
{
	char MOTD[MOTD_MAXIMUM_LENGTH];  // (Message of the day) Optional, replace with whatever and/or add more fields
	char serverName[SERVER_NAME_MAXIMUM_LENGTH];

};

#pragma pack(1)
struct StaticClientDataStruct
{
	unsigned char typeId;  // ID_SET_CLIENT_DATA
	char name[40];
};

#pragma pack(1)
struct ChatMessageStruct
{
	unsigned char typeId; // ID_CHAT_MESSAGE
	// You could append data manually.  You could also put an array such as
	// char myMessage[50]; // Very inefficient and unsafe!  Hackers could overflow the array
	// Or you could use a BitStream (the best solution) as in
	// BitStream bitStream;
	// bitStream.Write((unsigned char)ID_CHAT_MESSAGE); bitStream.Write("Hello World", strlen("HelloWorld")+1);
	// With the case of the BitStream, you wouldn't need a struct at all.  See the documentation for more details.
};


//The following structs are not used by this project but 
//are part of the "What about making a real game?" 
//section from the guide
#pragma pack(1)
struct MoveCharacterStruct 
{
	unsigned char typeId;  // ID_MOVE_NPC
	char charId[20]; //which character is moving
	int destX; //x location
	int destY; //y location
};

#pragma pack(1)
struct ChangeMapStruct 
{
	unsigned char typeId;  // ID_CHANGE_MAP
	char MapName[20]; //New Map Name
	int npcCount; //# of Add NPC messages to follow
};

#pragma pack(1)
struct PlayerNameStruct 
{
	unsigned char typeId;  // ID_PLAYER_NAME
	char PName[20]; //New Map Name
};

#pragma pack(1)
struct CastSpellStruct 
{
	unsigned char typeId;  // ID_CAST_SPELL
	int spellId; //which spell is being cast
	char sourcID[20]; //who is doing the casting
	char trgtID[20]; //who is being cast at
};

#pragma pack(1)
struct MeleeAttackStruct 
{
	unsigned char typeId;  // ID_CAST_SPELL
	char sourcID[20]; //who is doing the attacking
	char trgtID[20]; //who is being attacked
};

#pragma pack(1)
struct AddNPCStruct 
{
	unsigned char typeId;  // ID_ADD_NPC
	int dir;
	int side;
	int action;
	int x;
	int y;
	char guid[30];
	char fileName[100];
	char name[20];
};

#pragma pack(1)
struct JoinGameStruct 
{
	unsigned char typeId;  // ID_JOIN_GAME
};

#pragma pack(1)
struct ServerReadyStruct 
{
	unsigned char typeId;  // ID_SERVER_READY
};
#pragma pack(1)
struct ClientReadyStruct 
{
	unsigned char typeId;  // ID_CLIENT_READY
};
#pragma pack(1)
struct MapLoadedStruct 
{
	unsigned char typeId;  // ID_MAP_LOADED
};


#pragma pack(1)
struct CharTurnStruct 
{
	unsigned char typeId;  // ID_CHAR_TURN
	char charID[20]; //who is being attacked
};


#pragma pack(1)
struct TurnDoneStruct 
{
	unsigned char typeId;  // ID_TURN_DONE
};



#endif