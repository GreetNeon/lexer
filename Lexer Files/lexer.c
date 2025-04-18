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

//Current Errors: Symbols Skipped: Make them symbol tokens, Errors not handled correctly

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
char current_file[100];

// IMPLEMENT THE FOLLOWING functions
//***********************************

// Initialise the lexer to read from source file
// file_name is the name of the source file
// This requires opening the file and making any necessary initialisations of the lexer
// If an error occurs, the function should return 0
// if everything goes well the function should return 1

int InitLexer (char* file_name)
{
  strcpy(current_file, file_name);
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
  }
  return false;
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
          //Eat Comment
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
        // If not a comment, return the char
        ungetc(c, input);
        printf("Char: %c\n", c);
        return '/';
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
void BuildToken(){
  // Gather tokens from the source file and store them in the token stream
  // This function should be called after the lexer has been initialised and the source file opened
  
  char buffer[100];
  char word_buffer[100][100];
  char string_buffer[100][100];
  int chr, wrd, str = 0;

  char c = EatWC();
  //Loop through the file until EOF or a token is found
  if (c == EOF){
    t.tp = EOFile;
    strcpy(t.lx, "End of file");
    t.ln = LineCount;
    TokenReady = true;
    return;
  }
  while(c != EOF){
    if (c == '"'){
      //String Literal
      c = getc(input);
      while(c != '"'){
        buffer[chr++] = c;
        c = getc(input);
        if (c == EOF){
          // End of file in string literal
          // Set error code and message in token
          printf("Error: End of file in string literal\n");
          t.tp = ERR;
          t.ec = EofInStr;
          t.ln = LineCount;
          strcpy(t.lx, "End of file in string literal");
          strcpy(t.fl, current_file);
          TokenReady = true;
          return;
        }
        if (c == '\n'){
          // New line in string literal
          // Set error code and message in token
          printf("Error: New line in string literal\n");
          t.tp = ERR;
          t.ec = NewLnInStr;
          t.ln = LineCount;
          strcpy(t.lx, "New line in string literal");
          strcpy(t.fl, current_file);
          TokenReady = true;
          return;
        }
      }
      buffer[chr] = '\0';
      strcpy(string_buffer[str], buffer);
      str++; strcpy(buffer, ResetBuffer(buffer)); chr = 0;
      t.tp = STRING;
      strcpy(t.lx, string_buffer[str-1]);
      t.ln = LineCount;
      TokenReady = true;
      return;
      
    }
    else if (isalnum(c) || c == '_'){
      while(isalnum(c) || c == '_'){
        buffer[chr++] = c;
        c = getc(input);
      }
      buffer[chr] = '\0';
      strcpy(word_buffer[wrd], buffer);
      if (IsKeyWord(buffer)){
        t.tp = RESWORD;
      } else {
        t.tp = ID;
      }
      strcpy(t.lx, word_buffer[wrd]);
      t.ln = LineCount;
      TokenReady = true;
      wrd++; strcpy(buffer, ResetBuffer(buffer)); chr = 0;
      return;
    }
    else if(isspace(c)){
      c = EatWC();
    }
    else{
      //Must Be a symbol or illegal character
      if (c == '{' || c == '}' || c == '(' || c == ')' || c == '[' || c == ']' || c == '.' || c == ',' || c == ';' || c == '+' || c == '-' || c == '*' || c == '/' || c == '&' || c == '|' || c == '<' || c == '>' || c == '=' || c == '~'){
        t.tp = SYMBOL;
        t.lx[0] = c;
        t.lx[1] = '\0';
        t.ln = LineCount;
        TokenReady = true;
        return;
      } else {
        // Illegal symbol in source file
        // Set error code and message in token
        printf("Error: Illegal symbol in source file\n");
        return;
      }
    }
  }
}
  // Print out the tokens gathered
  // for (int i = 0; i < wrd; i++){
  //   printf("Word: %s\n", word_buffer[i]);
  // }
  // for (int i = 0; i < str; i++){
  //   printf("String: %s\n", string_buffer[i]);
  // }


  // while (c != EOF){
  //   buffer[i++] = c;
  //   c = getc(input);
  //   printf("Current char: %c\n", c);
  //   if (c == '"'){
  //     printf("String Literal\n");
  //     while (c != '"'){
  //       buffer[i++] = c;
  //       c = getc(input);
  //     }
  //     buffer[i++] = c;
  //     buffer[i] = '\0';
  //     strcpy(wordbuffer[j], buffer);
  //     // Create a string token
  //     j++; strcpy(buffer, ResetBuffer(buffer)); i = 0;
  //     c = EatWC();
      
  //   }
  //   while (isalnum(c) || c == '_'){
  //     buffer[i++] = c;
  //     c = getc(input);
  //   }
  //   buffer[i] = '\0';
  //   strcpy(wordbuffer[j], buffer);
  //   j++; strcpy(buffer, ResetBuffer(buffer)); i = 0;
  //   c = EatWC();
  // }
  // for (int k = 0; k < j; k++){
  //   //printf("%dth Word: %s\n", k, wordbuffer[k]);
  // }
// Get the next token from the source file
Token GetNextToken ()
{
	Token t;
  TokenReady = false;
  t.tp = ERR;
  BuildToken();
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
  if (input != NULL){
    fclose(input);
  }
  TokenReady = false;
  t.tp = ERR;
  t.ln = -1;
  strcpy(t.lx, "");
  strcpy(t.fl, "");
  strcpy(current_file, "");
  LineCount = 0;

  return 1;
	return 0;
}

// do not remove the next line
//#ifndef TEST
int main ()
{
	// implement your main function here
  // NOTE: the autograder will not use your main function
  if (InitLexer("Main.jack")){
    printf("File opened successfully\n");
  } else {
    printf("File not opened successfully\n");
  }
  for (int i = 0; i < 100; i++){
    if (t.tp == EOFile){
      printf("\n!!End of file reached!!\n");
      break;
    }
    Token lst[2];
    lst[0] = t;
    GetNextToken();
    lst[1] = t;
    printf("\nToken: %s\n", t.lx);
    printf("Token Type: %d\n", t.tp);
    printf("Line Number: %d\n", t.ln);
  }
  StopLexer();
	return 0;
}
// do not remove the next line
//#endif
