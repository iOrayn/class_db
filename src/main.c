#include <stdio.h>
#include <stdlib.h>
#include "repl.h"
#include <stdbool.h>
#include <string.h>


int main(int argc, char* argv[], char* envp[]){
  (void)argc;
  (void)argv;
  (void)envp;
  repl();
  return 0;
  
}
