#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//free the allocated memory
void freeMem(char **ptr, int rowLen)
{
  int i;
  //free every word before free the prt
  for (i = 0; i < rowLen; i++)
    {
      free(ptr[i]);
    }
  free(ptr);
}

//XOR function translate pass in arguments
char decrypt(const char c)
{
  return (char) (c^42);
}

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

}

int main()
{
  char input;
  char *array = NULL;
  char **array2 = NULL;
  int length = 0;
  int length2 = 0;
  char *temp = NULL;
  char **temp2 = NULL;

  
  input = getchar();
  if (ferror(stdin))
    {
      fprintf(stderr, "getchar() failed");
      exit(1);
    }
  

  while(!feof(stdin))
    {
      //input = getc(stdin);

      //allocate the first space for array that is storing one word
      if (array == NULL)
	{
	  temp = array;
	  array = (char*) malloc(sizeof(char));
	  if (array == NULL)
	    {
	      fprintf(stderr, "Memory allocation fail");
	      free(temp);
	      exit(1);
	    }
	}
      else
	{
	  temp = array;
	  array = (char*) realloc(array, (length + 1)*sizeof(char));
	  if (array == NULL)
            {
              fprintf(stderr, "Memory allocation fail");
	      free(temp);
              exit(1);
            }
	}
      array[length] = input;
      length++;

      //finish reading one word and put it in the next row
      if (input == ' ' && array != NULL)
	{
	  if (array2 == NULL)
	    {
	      temp2 = array2;
	      array2 = (char**) malloc(sizeof(char*));
	      if (array2 == NULL)
              {
                fprintf(stderr, "Memory allocation fail");
		freeMem(temp2, length2);
                exit(1);
              }
	    }
	  else
	    {
	      temp2 = array2;
	      array2 = (char**) realloc(array2, (length2 + 1)*sizeof(char*));
	      if (array2 == NULL)
              {
                fprintf(stderr, "Memory allocation fail");
		freeMem(temp2, length2);
                exit(1);
              }
	    }
           array2[length2] = array;
           length2++;
           //clear up the first word, and ready to store the next one                                         
           array = NULL;
	   //last = length;
           length = 0;
	}
      
      input = getc(stdin);
      //check for input error
      if (ferror(stdin))
       {
         fprintf(stderr, "getc() failed");
	 freeMem(array2, length2);
         exit(1);
       }
      
    }
  
  array = (char*) realloc(array, (length + 1)*sizeof(char));
  array[length] = ' ';
              
  array2 = (char**) realloc(array2, (length2 + 1)*sizeof(char*));
  array2[length2] = array;
  length2++;
  
  qsort(array2, length2, sizeof(char*), frobcmp);
  
  for (int i = 0; i < length2; i++)
    {
      int counter = 0;
      while(array2[i][counter] != ' ')
	{
	printf("%c", array2[i][counter]);
	
	if (ferror(stdout))
	  {
	    fprintf(stderr, "printf () failed");
	    freeMem(array2, length2);
	    exit(1);
	  }
	counter++;
	}
      //print out the spaces after each chararacter
      printf("%c", array2[i][counter]);
      if (ferror(stdout))
          {
            fprintf(stderr, "printf () failed");
	    freeMem(array2, length2);
	    exit(1);
          }
    }
  
  return 0;
}
