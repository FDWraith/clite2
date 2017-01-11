#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "process.h"

int main(){
  printf("Beginning Test for Reading From File\n");

  printf("=========================\n");
  
  printf("Testing findStringPair\n\n");
  
  char * fullString = (char *)malloc(sizeof(char *));
  strcpy(fullString, "<TABLE:TABLENAME>Blah Blah<TABLE_END:TABLENAME>SomethingElse");
  char denom1[256];
  strcpy(denom1, "<TABLE:TABLENAME>");
  char denom2[256];
  strcpy(denom2, "<TABLE_END:TABLENAME>");
  printf("\tFullString:[%s]\n", fullString);
  printf("\tdenom1:[%s]\n", denom1);
  printf("\tdenom2:[%s]\n", denom2);
  printf("\nfindStringPair output:[%s]\n", findStringPair(&fullString, denom1, denom2));
  printf("Result of FullString after OP:[%s]\n", fullString);

  printf("\nTesting Complete, Moving on\n");

  printf("=========================\n");

  

  //int fd = openFileAttempt("testfile");
  
}
