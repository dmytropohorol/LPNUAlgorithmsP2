#pragma once

#define MAX_LINE_LEN 256
#define INPUT_BUFFER_SIZE 256
#define MAX_FILES         32 // Max number of .txt files

struct StackNode {
    char Line[MAX_LINE_LEN];
    StackNode* Next;
};

//---------------------------------------------------------------------
// LAB1INTERFACE
//---------------------------------------------------------------------

// Read and print the file line by line (Part 1)
void ReadFileAndPrint(const char* Filename);

// Read file and push each line on the stack with line-number prefix (Part 2)
void LoadFileToStack(const char* Filename, StackNode** TopNodePtr);

// Push a new line onto the stack
void PushOntoStack(StackNode** TopNodePtr, const char* Text);

// Pop the top line from the stack and returning it in buffer
bool PopOfStack(StackNode** TopNodePtr, char* Buffer = nullptr);

// Print the entire stack without popping
const void PrintStack(const StackNode* TopNode);

// Free the stack
void PurgeStack(StackNode** TopNodePtr);

// Print the stacks contents while popping everything out (LIFO order)
void PrintAndClearStack(StackNode** TopNodePtr);

// Renumber stack in reverse order of how they were pushed
void RenumberStack(StackNode* TopNode);

// Normalize the line by adding a newline character at the end if its missing
bool NormalizeLine(char* Buffer);

//---------------------------------------------------------------------
// LAB2INTERFACE
//---------------------------------------------------------------------

// Directory scanning function
bool ChooseTextFileFromCurrentDirectory(char* OutChosenFile);

// Check if a file name ends with ".txt"
bool IsTextFile(const char* Filename);

// Convert wide (wchar_t*) to ASCII char* using WideCharToMultiByte
// Returns true on success, false if conversion fails or out of space.
bool WideToChar(const wchar_t* WideInput, char* OutBuffer, int OutBufferSize);