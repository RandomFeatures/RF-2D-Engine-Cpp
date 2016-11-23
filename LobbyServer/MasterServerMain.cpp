// This is my own internal test program for the master server but serves as a good example.

#include "MasterCommon.h"
#include "MasterServer.h"
#include <stdio.h>
#include <string.h>
#include <conio.h>


int main(void)
{
	MasterServer masterServer;
	int serverListSize;
	const char *outputString;
	int outputInt;
	bool identiferFound;
	int index;
	char ch;

	EnumerationDataStruct enumerationDataStruct;
	puts("Enter the server name  (up to 40 characters)");
	gets(enumerationDataStruct.serverName);
	if (enumerationDataStruct.serverName[0]==0)
		strcpy(enumerationDataStruct.serverName, "Default server name");
	puts("Enter the server message of the day  (up to 50 characters)");
	gets(enumerationDataStruct.MOTD);
	if (enumerationDataStruct.MOTD[0]==0)
		strcpy(enumerationDataStruct.MOTD, "Default MOTD");
	


	if (masterServer.Initialize(10, 60000, 0))
		printf("Lobby Server started, waiting for connections.\n");
	else
		printf("Lobby server failed to start.\n");

	printf("(p)rint\n(q)uit\n");

	while (1)
	{
		if (kbhit())
		{
			ch=getch();
			if (ch=='q')
				break;
			else if (ch=='p')
			{
				serverListSize=masterServer.GetServerListSize();
				if (serverListSize==0)
				{
					printf("No servers in list\n");
				}
				else
				{
					for (index=0; index < serverListSize; index++)
					{
						printf("%i. ", index);
						outputString=masterServer.GetServerListRuleAsString(index, "IP", &identiferFound);
						if (identiferFound)
							printf("%s:", outputString);
						else
							printf("NO_IP:");
						outputInt=masterServer.GetServerListRuleAsInt(index, "Port", &identiferFound);
						if (identiferFound)
							printf("%i ", outputInt);
						else
							printf("NO_PORT ");
						outputString=masterServer.GetServerListRuleAsString(index, "Game type", &identiferFound);
						if (identiferFound)
							printf("%s ", outputString);
						else
							printf("NIL_GT ");
						outputString=masterServer.GetServerListRuleAsString(index, "Game name", &identiferFound);
						if (identiferFound)
							printf("%s ", outputString);
						else
							printf("NIL_GN ");
						outputInt=masterServer.GetServerListRuleAsInt(index, "Score", &identiferFound);
						if (identiferFound)
							printf("%i\n", outputInt);
						else
							printf("NO_SCORE\n");
					}
				}
			}
			ch=0;
		}
		masterServer.Update();
	}

	return 0;
}