/************************************************************************
University of Leeds
School of Computing
COMP2932- Compiler Design and Construction
Lexer Module

I confirm that the following code has been developed and written by me and it is entirely the result of my own work.
I also confirm that I have not copied any parts of this program from another person or any other source or facilitated someone to copy this program from me.
I confirm that I will not publish the program online or share it with anyone without permission of the module leader.

Student Name:Teon Green
Student ID:201734612
Email:sc23tg@leeds.ac.uk
Date Work Commenced:25/2/2025
*************************************************************************/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "lexer.h"


// YOU CAN ADD YOUR OWN FUNCTIONS, DECLARATIONS AND VARIABLES HERE
#define NumKeywords 21
FILE *input;
int LineCount;
bool TokenReady;
Token t;
const char* keywords[NumKeywords] = {"class", "constructor", "method", "function", "int", "boolean", "char", "void", "var", "static", "field", "let", "do", "if", "else", "while", "return", "true", "false", "null", "this"};

// IMPLEMENT THE FOLLOWING functions
//***********************************

// Initialise the lexer to read from source file
// file_name is the name of the source file
// This requires opening the file and making any necessary initialisations of the lexer
// If an error occurs, the function should return 0
// if everything goes well the function should return 1

int InitLexer (char* file_name)
{
  input = fopen(file_name, "r");
  if (input == NULL){
    return 0;
  }
  TokenReady = false;
  LineCount = 1;
  return 1;
}

bool IsKeyWord(char* str){
  for(int i = 0; i < NumKeywords; i++){
    if(strcmp(str, keywords[i]) == 0){
      return true;
    }
    return false;
  }
}


int EatWC(){
  //Consume white space and comments then return char
  int c = getc(input);
  while (c != -1){
    if (c == '\n'){
      LineCount++;
    }
    if (c == '/'){
      c = getc(input);
      if (c == '/'){
        while (c != '\n' && c != EOF){
          c = getc(input);
          
        }
        LineCount++;
      } else if (c == '*'){
        while (1==1){
          c = getc(input);
          if (c == EOF){
            return 0;
          } else if (c == '*'){
            c = getc(input);
            if (c == '/'){
              break;
            }
          }
        }
      } else {
        return c;
      }
    } else if (!isspace(c)){
      return c;
    }
    c = getc(input);
  }
  return EOF;
}

char* ResetBuffer(char* buffer){
  // Reset the buffer to empty
  for (int i = 0; i < 100; i++){
    buffer[i] = '\0';
  }
  return buffer;
}
void GatherTokens(){
  // Gather tokens from the source file and store them in the token stream
  // This function should be called after the lexer has been initialised and the source file opened
  
  char buffer[100];
  char wordbuffer[100][100];
  int i, j = 0;
  char c = EatWC();
  while (c != EOF){
    buffer[i++] = c;
    c = getc(input);
    while (isalnum(c) || c == '_'){
      buffer[i++] = c;
      c = getc(input);
    }
    buffer[i] = '\0';
    strcpy(wordbuffer[j], buffer);
    j++; strcpy(buffer, ResetBuffer(buffer)); i = 0;
    c = EatWC();
  }
  for (int k = 0; k < j; k++){
    printf("%dth Word: %s\n", k, wordbuffer[k]);
  }
  

  TokenReady = true;
}

// Get the next token from the source file
Token GetNextToken ()
{
	Token t;
  t.tp = ERR;
  return t;
}

// peek (look) at the next token in the source file without removing it from the stream
Token PeekNextToken ()
{
  if (TokenReady){
    return t;
  }
  t.tp = ERR;
  return t;
}

// clean out at end, e.g. close files, free memory, ... etc
int StopLexer ()
{
	return 0;
}

// do not remove the next line
//#ifndef TEST
int main ()
{
	// implement your main function here
  // NOTE: the autograder will not use your main function
  if (IsKeyWord("class")){
    printf("True\n");
  }
  printf("Working\n");
  if (InitLexer("Main.jack")){
    printf("File opened successfully\n");
  } else {
    printf("File not opened successfully\n");
  }
  GatherTokens();
	return 0;
}
// do not remove the next line
//#endif
