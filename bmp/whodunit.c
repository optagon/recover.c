#include<cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"


int main(int argc, char* argv[])
{
    
    if (argc != 3)
    {
        printf("Invalid input, please provide all data \n");
        return 1;
    }
   
    FILE* clue_picture = fopen(argv[1], "r");
    if (clue_picture == NULL)
    {
    printf("Sorry... I am not able to open the file \n");
    return 1;
    }
    
    FILE* solution = fopen(argv[2], "w");
    if (solution == NULL)
    {
    fclose(clue_picture);
    printf("Sorry... I am not able to create the file \n");
    return 1;
    }
    
    int fh_size = sizeof(BITMAPFILEHEADER);
    int ih_size = sizeof(BITMAPINFOHEADER);
    
    //fread(&data, size, number, inptr)
    BITMAPFILEHEADER fh;
    fread(&fh, fh_size, 1, clue_picture);
    BITMAPINFOHEADER ih;
    fread(&ih, ih_size, 1, clue_picture);

    if ((ih.biSize != 40) || (ih.biCompression != 0) || (ih.biBitCount != 24) || (fh.bfOffBits != 54))
    {
        fclose(solution);
        fclose(clue_picture);
        printf("Invalid format \n");
        
    }
    
   
   //fwrite(&data, size, number, outputr);
    fwrite(&fh, sizeof(BITMAPFILEHEADER), 1, solution);
    fwrite(&ih, sizeof(BITMAPINFOHEADER), 1, solution);
    
     
    //calculate padding
    int padding =  (4 - (ih.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    for (int i = 0, biHeight = abs(ih.biHeight); i < biHeight; i++)
    {
        for (int j = 0; j < ih.biWidth; j++)
        {
            RGBTRIPLE triple;
            
            fread(&triple, sizeof(RGBTRIPLE), 1, clue_picture);

            if(triple.rgbtRed == 255 && triple.rgbtGreen == 0 && triple.rgbtBlue == 0)
                triple.rgbtRed = 0;
            
            if(triple.rgbtRed == 255 && triple.rgbtGreen == 255 && triple.rgbtBlue == 255)
            {
                triple.rgbtRed = 0;
                triple.rgbtGreen = 255;
                triple.rgbtBlue = 255;
            }
            
            if(triple.rgbtRed == 000 && triple.rgbtGreen == 000 && triple.rgbtBlue == 000)
            {
                triple.rgbtRed = 255;
                triple.rgbtGreen = 255;
                triple.rgbtBlue = 255;
            }
           fwrite(&triple, sizeof(RGBTRIPLE), 1, solution);
        }
        // fsee(inptr, amount, from)
        fseek(clue_picture, padding, SEEK_CUR);

        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, solution);
        }
    }
    
    fclose(clue_picture);
    fclose(solution);

    printf("Thank you for using my program, check out the solution file in your folder \n");
    printf("%ld", sizeof(RGBTRIPLE));
    return 0;
}