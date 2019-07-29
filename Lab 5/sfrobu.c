#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

//free memory function
void freeMem(char **ptr, int rowLen)
{
  int i;
  for (i = 0; i < rowLen; i++)
    {
      free(ptr[i]);
    }
}

//flag whether given f option to ignore case
//change all byte to upper case
int fOption = 0;

char decrypt(const char c)
{
  if (fOption == 1)
    {
      //toupper's argument is either EOF or
      //non negative number
      return toupper((unsigned char) c^42);
    }
  else
  {  
      //no f option here
      return (char) (c^42);
    }
}

//keep the frobcmp function the same

int frobcmp(const void* a, const void* b)
{
  
  //typecasts to char pointer from void pointer
  const char* input1 = *(const char**)a;
  const char* input2 = *(const char**)b;
  int index = 0;
 
  while (input1[index] != ' ' && input2[index] != ' ')
    {
      //both input are the same
      if (input1[index] == ' ' && input2[index] == ' ')
	return 0;
      //input 1 is larger than input 2
      if (((decrypt(input1[index])) > (decrypt(input2[index])))
	  || (input2[index] == ' ' && input1[index] != ' '))
	return 1;
      //input 1 is smaller than input 2
      if (((decrypt(input1[index])) < (decrypt(input2[index])))
          || (input1[index] == ' ' && input2[index] != ' '))
	return -1;
      index++;
    }
  
  return 0;
}

int main(int argc, char *argv[])
{
  //free memory pointers
  char *temp = NULL;
  char **temp2 = NULL;

  //check if the command line has the correct number input arguments
  //should be only 2 argument, the name of the program and the option f
  
  if (argc < 0 || argc > 2)
    {
      fprintf(stderr, "Incorrect number of command line arguments, only option -f available");
      exit(1);
    }
  //create a variable to store the return value from strncmp
  int number;

  if (argc == 2)
    {
      number = strncmp(argv[1], "-f", 2); 
      if ((argc == 2 && number < 0) || (argc == 2 && number > 0))
	{
	  fprintf(stderr, "Incorrect command line option, only option -f available");
	  exit(1);
	}
      else if (argc == 2 && number == 0)
	{
	  //check if option f is indicate in the command line arguments  
	  fOption = 1;
	}
    }
  
  //use the fstat func here
  struct stat file;
  //file descriptor would be zero here due to read
  int fdRead;
  fdRead = 0;
  size_t status = fstat(fdRead, &file);
  //check if the fstat fail, will return -1
  if (status < 0)
    {
      fprintf(stderr, "fstat() fail");
      exit(1);
    }

  //Declare some variables
  char **list = NULL;
  //create a pointer for the buffer file pass in read()
  char *buffer = NULL;
  //give one more space for the ending space character
  //size_t bufSize = file.st_size + 1;
  size_t bufSize = file.st_size;
  //variable for read error check
  int rdCheck;
  //varaible for write error check
  int wrCheck;
   
  //check if the standard input is a regular file to begin with
  if (S_ISREG(file.st_mode))
    {
      
      //handle regular files
      temp = buffer;
      buffer = (char*) malloc(sizeof(char) * bufSize);
      
      //check if malloc fail
      if (buffer == NULL)
	{
	  fprintf(stderr, "Memory allocation fail");
	  free(temp);
	  exit(1);
	}
      //start read into the buffer file
      //read in only with regular file size
      rdCheck = read(0, buffer, file.st_size);
      if (rdCheck == -1)
	{
	  fprintf(stderr, "read() failed");
	  free(temp);
	  exit(1);
	}

      //put a while loop to keep reading and reallocating one space at a time
      //increase the size of the buffer if the file is growing
      //until it rdCheck equal 0, which is end of file
      //here rdCheck return the number of bytes I read from the fixed file successfully

      //handle growing files, not check is it works    

      //looping through the buffer file to find out the number of rows
      int row = 0;
      //first append the last space to the buffer file
      //check if the first char is empty
      if (buffer[row] != ' ')
	{
	  row++;
	}
      for (size_t i = 0; i < bufSize; i++)
	{
	  //increment when there is space appears
	  if (buffer[i] == ' ')
	    {
	      row++;
	    }
	}

      buffer[bufSize - 1] = ' ';
      
       //allocate memory for my wordList to store the address of the words
       temp2 = list;
       list = (char**) malloc(row * sizeof(char*));
       if (list == NULL)
	 {
	   fprintf(stderr, "Memory allocation fail");
	   freeMem(temp2, row);
	   exit(1);
	 }
       //create a flag to indicate still working on the same row
       int rowFlag = 0;
       //create a index for moving along the **list
       size_t index = 0;
       //loop through the bufFile to put stuffs inside this time
       for (size_t j = 0; j < bufSize; j++)
	 {
	   if (rowFlag == 0)
	     {
	       //start having the address of the first char of the word now
	       rowFlag = 1;
	       list[index] = &buffer[j];
	       index++;
	       
	       if (buffer[j] == ' ')
		 {
		   --j;
		 }
	       
	     }
	   else if (rowFlag == 1 && buffer[j] == ' ')
	     {
	       //hit a space after passing a word
	       //ready to put into the address of the first char of next word
	       rowFlag = 0;
	     }
	   
	 }

       qsort(list, row, sizeof(char*), frobcmp);
       
       for (int i = 0; i < row; i++)
        {
          int counter = 0;
          while(list[i][counter] != ' ')
            {
              //use write() here
	      
	      wrCheck = write(1, &list[i][counter], 1);
              if (wrCheck == -1)
                {
                  fprintf(stderr, "write() failed");
		  freeMem(list, row);
                  exit(1);
                }
	      
              counter++;
            }
           //print put the spaces
	  
	  wrCheck = write(1, &list[i][counter], 1);
          if (wrCheck == -1)
            {
              fprintf(stderr, "write() failed");
	      freeMem(list, row);
              exit(1);
            }
	  
        }
      
       //no error, free everything
       free(buffer);
       free(list);
    }
    else
    { 
      //input is not a regular file
      //repeat the same thing in assignment 4, but use sys call
      //ptr to store my word
      char *string = NULL;
      int length = 0;
      int length2 = 0;
      //variable for read()
      char curChar[1];
      //variable for write()
      char wChar[1];
      //check for read()
      int check;
      //check for write()
      int check2;

      check = read(0, curChar, 1);
      if (check == -1)
	{
	  fprintf(stderr, "read() failed");
	  exit(1);
	}

      //continue until read() return zero which is end of file
      while (check > 0)
	{
	  if (string == NULL)
	    {
	      temp = string;
	      string = (char*) malloc(sizeof(char));
	      if (string == NULL)
	       {
		 fprintf(stderr, "Memory allocation fail");
		 free(temp);
		 exit(1);
	       }
	    }
	  else
	    {
	      temp = string;
	      string = (char*) realloc(string, (length + 1) * sizeof(char));
	      if (string == NULL)
		{
		  fprintf(stderr, "Memory allocation fail");
		  free(temp);
		  exit(1);
		}
	    }
	  string[length] = curChar[0];
	  length++;

	  //finish reading a word and put it in the next row
	  if (curChar[0] == ' ' && string != NULL)
	    {
	      if (list == NULL)
		{
		  temp2 = list;
		  list = (char**) malloc(sizeof(char*));
		  if (list == NULL)
		    {
		      fprintf(stderr, "Memory allocation fail");
		      freeMem(temp2, length2);
		      exit(1);
		    }
		}
	      else
		{
		  temp2 = list;
		  list = (char**) realloc(list, (length2 + 1)*sizeof(char*));
		  if (list == NULL)
		    {
		      fprintf(stderr, "Memory allocation fail");
		      freeMem(temp2, length2);
		      exit(1);
		    }
		}
	      list[length2] = string;
	      length2++;
	      string = NULL;
	      length = 0;
	    }

	  //read again for the next char of the word that is working on
	  check = read(0, curChar, 1);
	  if (check == -1)
	    {
	      fprintf(stderr, "read() fail");
	      freeMem(temp2, length2);
	      exit(1);
	    }
	  
	}
      //outside while loop, manually append sp to the last word
      //add it into the word list
      string = (char*) realloc(string, (length + 1)*sizeof(char));
      string[length] = ' ';

      list = (char**) realloc(list, (length2 + 1)*sizeof(char*));
      list[length2] = string;
      length2++;
      
      qsort(list, length2, sizeof(char*), frobcmp);
      
      //set up the print function first
      
      for (int i = 0; i < length2; i++)
	{
	  int counter = 0;
	  while(list[i][counter] != ' ')
	    {
	      //use write() here
	       check2 = write(1, &list[i][counter], 1);
	      if (check2 == -1)
		{
		  fprintf(stderr, "write() failed");
		  freeMem(list, length2);
		  exit(1);
		}
	      counter++;
	    }
	  
	   //print put the spaces                                                                    
          check2 = write(1, &list[i][counter], 1);
          if (check2 == -1)
            {
              fprintf(stderr, "write() failed");
	      freeMem(list, length2);
              exit(1);
            }
	  
	}
      //no error, free everything
      freeMem(list, length2);
    }
  
  return 0;
}