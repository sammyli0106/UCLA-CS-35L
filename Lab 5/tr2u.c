#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void inputError()
{
  if (ferror(stdin))
    {
      fprintf(stderr, "read() failed");
      exit(1);
    }
}

void outputError()
{
  if (ferror(stdout))
    {
      fprintf(stderr, "write() failed");
      exit(1);
    }
}

int main(int argc, char *argv[])
{
  //check if the command line has more than 3 arguments, include the name
  //of the program
  if (argc < 0 || argc > 3)
    {
      fprintf(stderr, "Incorrect number of command line arguments, Please provide total of three");
      exit(1);
    }

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

  //create a buffer to read data from
  //the buffer only
  char buffer[1];
  
  //file descriptor number
  int fdRead;
  fdRead = 0;
  int fdWrite;
  fdWrite = 1;
  
  //flag for read() fail
  int rdCheck = read(fdRead, buffer, 1);

  //flag for write() fail
  int wrCheck;
  
  //flag for matching found
  int flag;
  flag = 0;

  //read one byte at a time
  while(rdCheck > 0)
    {
      
      if (rdCheck == -1)
	{
	  //read() fails if return -1
	  inputError();
	}

      //search through from and look for matching to replace with to
      int k;
      for (k = 0; k < length; k++)
	{
	  if (buffer[0] == input1[k])
	    {
	      flag = 1;
	      buffer[0] = input2[k];
	      wrCheck = write(fdWrite, buffer, 1);
	      if(wrCheck == -1)
		{
	          outputError();
		}
	    }
	}
      if (flag != 1)
      {
	  wrCheck = write(fdWrite, buffer, 1);
	  if (wrCheck ==  -1)
	    {
	      outputError();
	    }
       }
      
      //reset the flag and ready to read again
      flag = 0;
      rdCheck = read(fdRead, buffer, 1);
      if (rdCheck == -1)
	{
	  inputError();
	}
    }

  
  return 0;
}