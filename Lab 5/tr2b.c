#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void inputError()
{
  if (ferror(stdin))
    {
      fprintf(stderr, "getchar() failed");
      exit(1);
    }
}

void outputError()
{
  if (ferror(stdout))
    {
      fprintf(stderr, "putchar() failed");
      exit(1);
    }
}

int main(int argc, char *argv[])
{  
  //check if the command line has more than 3 arguments
  //the correct number of arguments should be just 3
  //One for the name of the program, one for from, one for to
  if (argc < 0 || argc > 3)
    {
      fprintf(stderr, "Incorrect number of command line arguments, Please provide total of three");
      exit(1);
    }

  //create the pointer to char array to store the two pass in arguments
  //this is our from
  char *input1 = argv[1];
  //this is our to
  char *input2 = argv[2];
  
  //report error for from and to if they are not the same length
  if (strlen(input1) != strlen(input2))
    {
      fprintf(stderr, "From and To are not the same length");
      exit(1);
    }

  int length = strlen(input1);
  
  //check if from has duplicates input and output error
  //Find the two repeating elements in a given array
  int i, j;
  for (i = 0; i < length; i++)
    {
      for (j = i+1; j < length; j++)
	{
	  if (input1[i] == input1[j])
	    {
	      fprintf(stderr, "From has duplicates bytes");
	      exit(1);
	    }
	}
    }
  
  //report input and output error for using the getchar and putchar
  //transliterating every byte in from to the corresponding byte in to
  //apply this transliterating to the standard input and apply to the
  //standard output
  char read;
  int flag;
  flag = 0;
  
  //look up the ferror things and add on
  //while ((read=getchar()) != EOF)
  read = getchar();
  inputError();
  
  while (!feof(stdin))
    {
      //call to check the input error
      //inputError();

      //search through from and try to look for the char
      //before we replace with the one in to
      int k;
      for (k = 0; k < length; k++)
	{
	  if (read == input1[k])
	    {
	      //find a match in from and output the corresponding word in to
	      flag = 1;
	      putchar(input2[k]);
	      outputError();
	    }
	}
      //outside the loop which means didnt find anything
      if (flag != 1)
	{
          putchar(read);
          outputError();
	}
      //reset the flag
      flag = 0;
      read = getchar();
    }
  
  return 0;
}