#include "Lab2dmytropohorol.h"
#include <windows.h>
#include <iostream>
#include <fstream>

#pragma warning( disable : 4996)

int main()
{
	StackNode* StackTop = nullptr;
	bool bExitMenu = false;

	while (!bExitMenu)
	{
		std::cout << "--- MENU ---\n"
				  << "1. Part 1: Read and print file.\n"
				  << "2. Part 2: Read file into stack, then display.\n"
				  << "3. Renumber lines in the current stack.\n"
				  << "4. Clear the stack.\n"
				  << "5. Exit\n"
				  << "Select an option: ";

		char inputLine[INPUT_BUFFER_SIZE];
		std::cin.getline(inputLine, INPUT_BUFFER_SIZE);
		int SelectedMenuIndex = std::atoi(inputLine);
		char ChosenFile[MAX_PATH] = { 0 };

		switch (SelectedMenuIndex)
		{
		case 1:
			std::cout << "\n-- Part 1: Reading and printing a chosen text file --\n";
			if (ChooseTextFileFromCurrentDirectory(ChosenFile))
			{
				ReadFileAndPrint(ChosenFile);
			}
			break;
		case 2:
			std::cout << "\n-- Part 2: Load chosen text file into stack, then display and clear --\n";
			if (ChooseTextFileFromCurrentDirectory(ChosenFile))
			{
				PurgeStack(&StackTop);
				LoadFileToStack(ChosenFile, &StackTop);
				std::cout << "\nStack content (LIFO order):\n";
				PrintAndClearStack(&StackTop);
			}
			break;
		case 3:
			if (!StackTop)
			{
				std::cout << "\nStack is empty, nothing to renumber!\n";
			}
			else
			{
				std::cout << "\nRenumbering lines in the existing stack...\n";
				RenumberStack(StackTop);
				std::cout << "\nDone. Current stack contents:\n";
				PrintStack(StackTop);
			}
			break;
		case 4:
			std::cout << "\nClearing the entire stack...\n";
			PurgeStack(&StackTop);
			std::cout << "Stack is now empty.\n";
			break;
		case 5:
			std::cout << "\nExiting...\n";
			bExitMenu = true;
			break;
		default:
			std::cout << "\nInvalid choice!\n";
			break;
		}
	}

	// Make sure stack is empty before exit
	PurgeStack(&StackTop);
	return 0;
}

void ReadFileAndPrint(const char* Filename)
{
	std::ifstream FileStream(Filename);
	if (!FileStream)
	{
		std::cout << "Could not open file: " << Filename << "\n";
		return;
	}

	std::cout << "\n--- Contents of " << Filename << " ---\n\n";

	while (FileStream)
	{
		char Buffer[MAX_LINE_LEN];
		FileStream.getline(Buffer, MAX_LINE_LEN);
		if (NormalizeLine(Buffer))
		{
			std::cout << Buffer;
		}
	}

	std::cout << "\n--- End of file ---\n\n";
	FileStream.close();
}

void LoadFileToStack(const char* Filename, StackNode** TopNodePtr)
{
	std::ifstream FileStream(Filename);
	if (!FileStream)
	{
		std::cout << "Could not open file: " << Filename << "\n";
		return;
	}

	int LineNumber = 1;
	while (FileStream)
	{
		char Buffer[MAX_LINE_LEN];
		FileStream.getline(Buffer, MAX_LINE_LEN);
		if (NormalizeLine(Buffer))
		{
			char ModifiedLine[MAX_LINE_LEN];
			std::snprintf(ModifiedLine, MAX_LINE_LEN, "%d: %s", LineNumber, Buffer);
			PushOntoStack(TopNodePtr, ModifiedLine);
			LineNumber++;
		}
	}

	std::cout << "\n--- End of file ---\n\n";
	FileStream.close();
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

bool NormalizeLine(char* Buffer)
{
	size_t Length = std::strlen(Buffer);
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

const void PrintStack(const StackNode* TopNode)
{
	const StackNode* CurrentNode = TopNode;
	while (CurrentNode)
	{
		std::cout << CurrentNode->Line;
		CurrentNode = CurrentNode->Next;
	}
	std::cout << "\n";
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
		std::cout << PoppedLine;
	}
	std::cout << "\n";
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

bool ChooseTextFileFromCurrentDirectory(char* OutChosenFile)
{
	if (!OutChosenFile)
	{
		return false;
	}

	WIN32_FIND_DATAW FindFileData;
	HANDLE FindHandle = FindFirstFileW(L"*", &FindFileData);
	if (FindHandle == INVALID_HANDLE_VALUE)
	{
		std::cout << "Error: could not open current directory.\n";
		return false;
	}

	char Filenames[MAX_FILES][MAX_PATH];
	int NumOfTextFiles = 0;

	for (int i = 0; i < MAX_FILES; i++)
	{
		Filenames[i][0] = '\0';
	}

	bool Found = false;
	while (!Found && NumOfTextFiles < MAX_FILES)
	{
		if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{ 
			// Its not a directory, than check if .txt
			char TempName[MAX_PATH];
			if (WideToChar(FindFileData.cFileName, TempName, MAX_PATH)
				&& IsTextFile(TempName))
			{
				std::strcpy(Filenames[NumOfTextFiles], TempName);
				NumOfTextFiles++;
			}
		}
		Found = !FindNextFileW(FindHandle, &FindFileData);
	}
	FindClose(FindHandle);

	if (NumOfTextFiles == 0)
	{
		std::cout << "No .txt files found in the current directory.\n";
		return false;
	}

	std::cout << "\nList of all .txt files in current directory:\n";
	for (int i = 0; i < NumOfTextFiles; i++)
	{
		std::cout << (i + 1) << ". " << Filenames[i] << "\n";
	}

	std::cout << "\nEnter the number of the file you want: ";
	char InputLine[INPUT_BUFFER_SIZE];
	std::cin.getline(InputLine, INPUT_BUFFER_SIZE);
	int Choice = std::atoi(InputLine);

	if (Choice < 1 || Choice > NumOfTextFiles)
	{
		std::cout << "Invalid selection!\n";
		return false;
	}

	std::strcpy(OutChosenFile, Filenames[Choice - 1]);
	return true;
}

bool IsTextFile(const char* Filename)
{
	int Length = (int)std::strlen(Filename);
	if (Length < 4)
	{
		return false;
	}

	const char* Extension = Filename + (Length - 4);

	// Convert last 4 chars to lowercase and compare with ".txt" 
	char LowerExtension[5];
	for (int i = 0; i < 4; i++)
	{
		LowerExtension[i] = std::tolower(Extension[i]);
	}
	LowerExtension[4] = '\0';

	return std::strcmp(LowerExtension, ".txt") == 0;
}

bool WideToChar(const wchar_t* WideInput, char* OutBuffer, int OutBufferSize)
{
	if (WideCharToMultiByte(CP_ACP, 0, WideInput, -1, NULL, 0, NULL, NULL) > OutBufferSize)
	{
		return false; // not enough space
	}
	return WideCharToMultiByte(CP_ACP, 0, WideInput, -1, OutBuffer, OutBufferSize, NULL, NULL) > 0;
}