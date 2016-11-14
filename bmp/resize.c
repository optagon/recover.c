/**
 * resize.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Resizes given images by given index of n.
 */

#include <string.h>       
#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
#include "bmp.h"


int main(int argc, char* argv[])
{
		// ensure proper usage
		if (argc != 4)
		{
				printf("Usage: ./copy infile outfile\n");
				return 1;
		}

		// remember filenames
		int n = atoi(argv[1]);
		if(n < 0 || n >= 100)
		{
				printf("Please provide a number between 0 and 100.\n");
				return 2;
		}
		char* infile = argv[2];
		char* outfile = argv[3];

		// open input file 
		FILE* inptr = fopen(infile, "r");
		if (inptr == NULL)
		{
				printf("Could not open %s.\n", infile);
				return 3;
		}

		// open output file
		FILE* outptr = fopen(outfile, "w");
		if (outptr == NULL)
		{
				fclose(inptr);
				fprintf(stderr, "Could not create %s.\n", outfile);
				return 4;
		}

		// read infile's BITMAPFILEHEADER
		BITMAPFILEHEADER bf;
		fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

		// read infile's BITMAPINFOHEADER
		BITMAPINFOHEADER bi;
		fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

		// ensure infile is (likely) a 24-bit uncompressed BMP 4.0
		if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
						bi.biBitCount != 24 || bi.biCompression != 0)
		{
				fclose(outptr);
				fclose(inptr);
				fprintf(stderr, "Unsupported file format.\n");
				return 4;
		}

		
		BITMAPFILEHEADER bf_out = bf;
		BITMAPINFOHEADER bi_out = bi;
		bi_out.biWidth = bi.biWidth * n;
		bi_out.biHeight = bi.biHeight * n;

		int padding_in =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
		int padding_out =  (4 - (bi_out.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

		bf_out.bfSize = 54 + bi_out.biWidth * abs(bi_out.biHeight) * 3 + abs(bi_out.biHeight) *  padding_out;
		bi_out.biSizeImage = ((((bi_out.biWidth * bi_out.biBitCount) + 31) & ~31) / 8) * abs(bi_out.biHeight);

		// write outfile's BITMAPFILEHEADER
		fwrite(&bf_out, sizeof(BITMAPFILEHEADER), 1, outptr);

		// write outfile's BITMAPINFOHEADER
		fwrite(&bi_out, sizeof(BITMAPINFOHEADER), 1, outptr);
		
        // iterate over infile's scanlines
		for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
		{
				for(int f = 0; f < n; f++)
				{
						// iterate over pixels in scanline
						for (int j = 0; j < bi.biWidth; j++)
						{
								// temporary storage
								RGBTRIPLE triple;

								// read RGB triple from infile
								fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

								
								for(int m = 0; m < n; m++) 
								{
								        // write RGB triple to outfile
										fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
								}
						}

						fseek(inptr, padding_in, SEEK_CUR);

						// then add it back (to demonstrate how)
						for (int k = 0; k < padding_out; k++)
								fputc(0x00, outptr);

						fseek(inptr, -(bi.biWidth * 3 + padding_in), SEEK_CUR);

				}
				fseek(inptr, (bi.biWidth * 3) + padding_in, SEEK_CUR);
		}
        // close infile
		fclose(inptr);
        // close outfile
		fclose(outptr);
        // that's all folks
		return 0;
}