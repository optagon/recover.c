#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

typedef uint8_t byte;

int main (int argc, char **argv)
{
    
    FILE *outfile;        //pointer which stores output file
    byte buffer[512];    //buffer to read from the card
    char newfile[8];    //stores the name of a new file
    int count1 = 0; //keep track of number of photos found
    int bol = 0;    //works as a bool (0 or 1) to check if jpeg found
    
    //beginning of jpeg
    byte beginning1[4] = {0xff, 0xd8, 0xff, 0xe0};
    byte beginning2[4] = {0xff, 0xd8, 0xff, 0xe1};

    //open memory card file 
    FILE * file = fopen("card.raw", "r");
    if (card == NULL)
    {   
       fclose(file);    
       return 1;
    }
       //detect the end of the file - rewrite it into a for loop
    while (!feof(file))
    {
          int i;
          int match = 0;    //variable to track matching signatures

          fread(buffer, sizeof(buffer[0]), BLOCK_SIZE, card);    //reading

          //writes 512 bytes until new jpeg is found
          for (i = 0; i < 4; i++)
              if (buffer[i] == signature1[i] || buffer[i] == signature2[i])
                 match++;
        
            //if all matches, a jpeg is found
            //it then copies every byte till a new jpeg is found
          if (match == 4)
          {
                    //closes a file if opened 
                    if (bol == 1)
                        fclose(outfile);
                    
                    //opens a new jpeg
                    sprintf(newfile, "%.3d.jpeg", count1);
                    outfile = fopen(newfile, "w");
                            if (outfile == NULL)
                            {
                                        printf("Not Enough Space\n");
                                        return 2;
                            }
                    count1++; //increases num of files found
                    
                    printf("Progressing.......\n");
                    
                    //writes value from the buffer to the output file
                    fwrite(buffer, sizeof(buffer[0]), BLOCK_SIZE, outfile);
            bol = 1;    //sets bol to 1 (true)
          }

          // if a file is already opened but dosent matches the signature
          //indicates that it is a part of the previous jpeg
          // if bol=1 (true) a file is opened
          else if (bol == 1)
                    fwrite(buffer, sizeof(buffer[0]), BLOCK_SIZE, outfile);

          // if neither signature matches nor file is opened, set bol to (false) 0
          else bol = 0;
    }
    
    fclose(outfile);
    fclose(card);
    printf("Completed\n");
    getchar();
    return 0;
}