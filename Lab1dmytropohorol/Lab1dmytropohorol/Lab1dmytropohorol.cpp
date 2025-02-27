#include "Lab1dmytropohorol.h"
#include <cstdio>
#include <cstring>
#pragma warning( disable : 4996)

int main()
{
	StackNode* StackTop = nullptr;
	bool bExitMenu = false;

	while (!bExitMenu)
	{
		std::printf("--- MENU ---\n"
					"1. Part 1: Read and print file.\n"
					"2. Part 2: Read file into stack, then display.\n"
					"3. Renumber lines in the current stack.\n"
					"4. Clear the stack.\n"
					"5. Exit\n"
					"Select an option: ");

		char inputLine[INPUT_BUFFER_SIZE];
		int SelectedMenuIndex = -1;
		if (std::fgets(inputLine, INPUT_BUFFER_SIZE, stdin))
		{
			std::sscanf(inputLine, "%d", &SelectedMenuIndex);
		}

		switch (SelectedMenuIndex)
		{
		case 1:
			std::printf("\n-- Part 1: Reading file line by line --\n");
			ReadFileAndPrint("file.txt");
			break;
		case 2:
			std::printf("\n-- Part 2: Loading file into stack with line numbers --\n");

			// Purge any old data, if present
			PurgeStack(&StackTop); 
			LoadFileToStack("file.txt", &StackTop);

			std::printf("\nStack contents: \n");
			PrintAndClearStack(&StackTop);

			break;
		case 3:
			if (!StackTop)
			{
				std::printf("\nStack is empty, nothing to renumber!\n");
			}
			else
			{
				std::printf("\nRenumbering lines in the stack...\n");
				RenumberStack(StackTop);
				std::printf("\nDone. Current stack contents:\n");
				PrintStack(StackTop);
			}
			break;
		case 4:
			std::printf("\nPurging the entire stack...\n");
			PurgeStack(&StackTop);
			std::printf("Stack is now empty.\n");
			break;
		case 5:
			std::printf("\nExiting...\n");
			bExitMenu = true;
			break;
		default:
			std::printf("\nInvalid choice!\n");
			break;
		}
	}

	// Make sure stack is empty before exit
	PurgeStack(&StackTop);
	return 0;
}

void ReadFileAndPrint(const char* Filename)
{
	FILE* FilePtr = std::fopen(Filename, "r");
	if (!FilePtr)
	{
		std::fprintf(stderr, "Couldnt open file: %s.\n", Filename);
		return;
	}

	char Buffer[MAX_LINE_LEN];
	while (std::fgets(Buffer, MAX_LINE_LEN, FilePtr))
	{
		if (NormalizeLine(Buffer))
		{
			std::printf("%s", Buffer);
		}
	}

	std::printf("\n--- End of file ---\n\n");
	std::fclose(FilePtr);
}

void LoadFileToStack(const char* Filename, StackNode** TopNodePtr)
{
	FILE* FilePtr = std::fopen(Filename, "r");
	if (!FilePtr)
	{
		std::fprintf(stderr, "Couldnt open file: %s.\n", Filename);
		return;
	}

	char Buffer[MAX_LINE_LEN];
	int LineNumber = 1;

	while (std::fgets(Buffer, MAX_LINE_LEN, FilePtr))
	{
		if (NormalizeLine(Buffer))
		{
			char ModifiedLine[MAX_LINE_LEN];
			std::sprintf(ModifiedLine, "%d: %s", LineNumber, Buffer);
			PushOntoStack(TopNodePtr, ModifiedLine);
			LineNumber++;
		}
	}
	std::fclose(FilePtr);
}

void PushOntoStack(StackNode** TopNodePtr, const char* Text)
{
	StackNode* NewNode = new StackNode;
	std::strcpy(NewNode->Line, Text);

	NewNode->Next = *TopNodePtr;
	*TopNodePtr = NewNode;
}

bool PopOfStack(StackNode** TopNodePtr, char* Buffer)
{
	if (!*TopNodePtr)
	{
		return false; // Stack is empty
	}

	if (Buffer)
	{
		std::strcpy(Buffer, (*TopNodePtr)->Line);
	}
	
	StackNode* TempNode = *TopNodePtr;
	*TopNodePtr = (*TopNodePtr)->Next;

	delete TempNode;
	return true;
}

const void PrintStack(const StackNode* TopNode)
{
	while (const StackNode* CurrentNode = TopNode)
	{
		std::printf(CurrentNode->Line);
		CurrentNode = CurrentNode->Next;
	}
	std::printf("\n");
}

void PurgeStack(StackNode** TopNodePtr)
{
	while (PopOfStack(TopNodePtr));
}

void PrintAndClearStack(StackNode** TopNodePtr)
{
	char PoppedLine[MAX_LINE_LEN];
	while (PopOfStack(TopNodePtr, PoppedLine))
	{
		std::printf("%s", PoppedLine);
	}
	std::printf("\n--- End of file ---\n\n");
}

void RenumberStack(StackNode* TopNode)
{
	StackNode* CurrentNode = TopNode;
	for (int CurrentNumber = 1; CurrentNode; CurrentNumber++)
	{
		char TempText[MAX_LINE_LEN];
		std::strcpy(TempText, CurrentNode->Line);
		std::snprintf(CurrentNode->Line, MAX_LINE_LEN, "%d: %s", CurrentNumber, TempText);
		CurrentNode = CurrentNode->Next;
	}
}

bool NormalizeLine(char* Buffer)
{
	size_t Length = std::strlen(Buffer);

	// TODO: validate this part of code with professor
	if (!Length || (Length == 1 && (Buffer[0] == '\n' || Buffer[0] == '\r')))
	{
		return false;
	}
	
	if (Length < (MAX_LINE_LEN - 1) && Buffer[Length - 1] != '\n')
	{
		Buffer[Length] = '\n';
		Buffer[Length + 1] = '\0';
	}
	return true;
}