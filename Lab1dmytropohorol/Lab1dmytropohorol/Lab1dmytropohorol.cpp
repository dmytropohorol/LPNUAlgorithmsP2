#include "Lab1dmytropohorol.h"
#include <cstdio>
#include <cstring>
#pragma warning( disable : 4996)

int main()
{
    StackNode* StackTop = NULL;
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
        std::strcpy(Buffer, (*TopNodePtr)->Line); // Copy the top node line into buffer
    }
    
    // Remove the top node from the stack
    StackNode* TempNode = *TopNodePtr;
    *TopNodePtr = (*TopNodePtr)->Next;

    // Deallocate the popped node
    delete TempNode;
    return true;
}

const void PrintStack(const StackNode* TopNode)
{
    const StackNode* CurrentNode = TopNode;
    while (CurrentNode != NULL)
    {
        std::printf(CurrentNode->Line);
        CurrentNode = CurrentNode->Next;
    }
}

void PurgeStack(StackNode** TopNodePtr)
{
    char DummyBuffer[MAX_LINE_LEN];
    // Pop until empty
    while (PopOfStack(TopNodePtr, DummyBuffer));
}

void PrintAndClearStack(StackNode** TopNodePtr)
{
    char PoppedLine[MAX_LINE_LEN];
    while (PopOfStack(TopNodePtr, PoppedLine))
    {
        std::printf("%s", PoppedLine);
    }
}

void RenumberStack(StackNode* TopNode)
{
    int CurrentNumber = 1;
    StackNode* CurrentStack = TopNode;
    while (CurrentStack != NULL)
    {
        // Move aside the original text
        char TempText[MAX_LINE_LEN];
        std::strcpy(TempText, CurrentStack->Line);

        // Rewrite the line with a new number prefix
        std::sprintf(CurrentStack->Line, "%d: %s", CurrentNumber, TempText);

        CurrentNumber++;
        CurrentStack = CurrentStack->Next;
    }
}

bool NormalizeLine(char* Buffer)
{
    size_t LineLength = std::strlen(Buffer);

    // TODO: validate this part of code with professor
    if (LineLength == 1)
    {
		return false; // Empty line
    }
    
    if (Buffer[LineLength - 1] != '\n'
        && LineLength < (MAX_LINE_LEN - 1)) // Check if we are not at buffer capacity
    {
        Buffer[LineLength] = '\n';
        Buffer[LineLength + 1] = '\0';
        LineLength++;
    }

    return true;
}