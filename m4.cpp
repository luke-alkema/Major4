/*
*	Flename:		m4.cpp
*   Project:		Major4Assignment
*   By:				Luke Alkema
*   Date:			December 2, 2023
*   Description:	This program opens a text file called teams.txt and uses that to 
*					open many other team files, and then parse data from them and display
*					the data for each team for the season after all the data is read from 
*					the individual teams text file.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#pragma warning(disable: 4996)
#define LINE_LENGTH 41
const int success = 1;
const int fail = 0;
const char emptyArray[LINE_LENGTH] = "";


//function prototypes
void deleteNewLineCharacter(char string[]);
int processGames(char filename[]);
void getHomeTeamName(char filename[], char homeTeamName[]);
int parseLine(char gameResult[], char opponentTeamName[], int* primaryTeamScore, int* OpponentTeamScore);


int main(void)	
{

	char teamsArray[LINE_LENGTH] = "";
	int status = success;
	FILE* pTeamsList = NULL;


	if ((pTeamsList = fopen("teams.txt", "r")) == NULL)
	{
		printf("Error: Can't open teams.txt\n");
		status = fail;
	}

	if (status == success)
	{
		while ((fgets(teamsArray, LINE_LENGTH, pTeamsList)) != NULL)  	//while not end of file or no errors
		{
			deleteNewLineCharacter(teamsArray);
			if (teamsArray[0] == '\0')
			{
				continue;  //empty line in teams.txt file, skip it
			}
			status = processGames(teamsArray);
			strcpy(teamsArray, emptyArray); //clears out previous name
		}

		if ((fclose(pTeamsList)) != fail)
		{
			printf("Error: Can't close teams.txt\n");
			status = fail;
		}
	}

	return status;

}//end main




// Function:	processGames()
// Parameters:	char: filename[]: An array of characters that specifies the file for processGames() to open		
// Returns:		int: Status: 1 if success and no errors, 0 if there were errors			
// Description: This function gets a filename from main, then opens the file and processes the data by checking which
//				team won (by calling parseLine()) and then displaying the calculations it does to the user. It will
//				skip over blank lines. 
int processGames(char filename[])
{
	FILE* pGameResults = NULL;
	char gameResults[LINE_LENGTH] = "";
	char homeTeamName[LINE_LENGTH] = "";
	int status = success;
	int primaryTeamScore = 0;
	int opponentTeamScore = 0;
	double wins = 0.0;
	double losses = 0.0;
	double ties = 0.0;
	double winningPercentage = 0.0;

	


	if ((pGameResults = fopen(filename, "r")) == NULL)
	{
		printf("Warning: Can't open \"%s\". Skipping team.\n", filename);
		status = fail;
	}

	if (status == success)
	{
		getHomeTeamName(filename, homeTeamName); // this function cannot fail if fopen was successful, so I put it here

		printf("Processing %s:\n", filename);
		while ((fgets(gameResults, LINE_LENGTH, pGameResults)) != NULL)
		{

			deleteNewLineCharacter(gameResults);

			if (gameResults[0] == '\0')
			{
				continue;  //empty line in game score file, skip it

			}

			char opponentTeamName[LINE_LENGTH] = ""; // I redeclare in loop to remove old team names completly from the memory position I am in. Sorry stack.
			if ((parseLine(gameResults, opponentTeamName, &primaryTeamScore, &opponentTeamScore)) == fail)
			{
				printf("Warning: malformed line. Skipping game.\n");
				continue;
			}


			if (primaryTeamScore > opponentTeamScore)
			{
				wins++;
				printf("\t%s beat %s %d-%d\n", homeTeamName, opponentTeamName, primaryTeamScore, opponentTeamScore);
			}
			if (primaryTeamScore == opponentTeamScore)
			{
				ties++;
				printf("\t%s and %s tied at %d\n", homeTeamName, opponentTeamName, primaryTeamScore);
			}
			if (primaryTeamScore < opponentTeamScore)
			{
				losses++;
				printf("\t%s lost to %s %d-%d\n", homeTeamName, opponentTeamName, opponentTeamScore, primaryTeamScore);
			}


			opponentTeamScore = 0; //reseting them for each new game in the file
			primaryTeamScore = 0;

		}


		if (wins != 0 || losses != 0 || ties != 0)
		{
			winningPercentage = (2 * wins + ties) / (2 * (wins + losses + ties));
			printf("Season result for %s: %.3f (%.0f-%.0f-%.0f)\n", homeTeamName, winningPercentage, wins, losses, ties);
		}

		if ((fclose(pGameResults)) != fail)
		{
			printf("Error: Can't close %s.\n", filename);
			status = fail;
		}
		printf("\n");
	}

	return status;
}//end proccessGames




// Function:	parseLine()
// Parameters:	char gameResult[], char opponentTeamName[], int* primaryTeamScore, int* opponentTeamScore		
// Returns:		int status: Will return one if success and 0 if failure			
// Description: This function gets the game result from processGames() and will sort through the line, separating the 
//				opponentTeamName from the file scores, and it also parses the score each team got into actual integers.
int parseLine(char gameResult[], char opponentTeamName[], int* primaryTeamScore, int* opponentTeamScore)
{
	int status = success;

	do
	{
		char* pComma = strchr(gameResult, ',');
		if (pComma == NULL)
		{
			status = fail;
			break;
		}

		int i = 0;
		while (gameResult != pComma)
		{
			opponentTeamName[i] = gameResult[0];

			gameResult++;
			i++;
		}
		opponentTeamName[i] = '\0'; //forces end of name to clear old team names

		char* pDash = strchr(gameResult, '-');
		if (pDash == NULL)
		{
			status = fail;
			break;
		}

		gameResult = pComma + 1;  //go one spot in memory past the comma to get the score for the primary team

		*primaryTeamScore = atoi(gameResult);

		gameResult = pDash + 1; // go one spot in memory past the dash to get the score for the opponent team

		*opponentTeamScore = atoi(gameResult);

	} while (false);


	return status;
}//end parseLine



// Function:	deleteNewLineCharacter()
// Parameters:	char: string[]: Any array of characters			
// Returns:		void			
// Description: This function simply replaces the '\n' character from the end of a C-string with null (if it is there) (normally used because fgets() adds a \n automatically to the end of a string) 

void deleteNewLineCharacter(char string[])
{
	int i = 0;

	while ((string[i] != '\0') && (string[i] != '\n'))
	{
		++i;
	}

	if (string[i] == '\n')
	{
		string[i] = '\0';
	}
}// credits: Segro, C. (2015, Oct 21). strequal.c. eConestoga,  https://conestoga.desire2learn.com/d2l/le/content/879640/viewContent/17227039/View




// Function:	getHomeTeamName()
// Parameters:	char filename[], char homeTeamName[]		
// Returns:		void			
// Description: This function is for any file in a C-string, it will simply take of the extension from the name of the file. 
//				The file must have a '.' to work properly, as the dot indicates there is an extension on the filename. 
void getHomeTeamName(char filename[], char homeTeamName[])
{

	size_t i = strlen(filename);
	strcpy(homeTeamName, filename);

	while (true)
	{
		homeTeamName[i] = '\0';
		i--;
		if (homeTeamName[i] == '.')
		{
			homeTeamName[i] = '\0';
			break;
		}
		if (homeTeamName[0] == '\0')
		{
			break;
		}
	}
}//end getHomeTeamName()