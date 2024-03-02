/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
    //YOUR CODE HERE
    Color *pixel = (Color *)malloc(sizeof(Color));
    if (pixel == NULL) {
        return NULL;
    }
    uint8_t b_value = image->image[row][col].B;
    if (b_value & 1) {
        pixel->R = 255;
        pixel->G = 255;
        pixel->B = 255;
    } else {
        pixel->R = 0;
        pixel->G = 0;
        pixel->B = 0;
    }
    return pixel;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
    //YOUR CODE HERE
    // allocate memory for new_image
    Image *new_image = (Image *)malloc(sizeof(Image));
    if (new_image == NULL) {
        return NULL;
    }
    new_image->rows = image->rows;
    new_image->cols = image->cols;
    new_image->image = (Color **)malloc(image->rows * sizeof(Color *));
    if (new_image->image == NULL) {
        free(new_image);
        return NULL;
    }
    for (uint8_t i = 0; i < image->rows; i++) {
        new_image->image[i] = (Color *)malloc(image->cols * sizeof(Color));
        if (new_image->image[i] == NULL) {
            for (uint8_t j = 0; j < i; j++) {
                free(new_image->image[j]);
            }
            free(new_image->image);
            free(new_image);
            return NULL;
        }
    }

    // initialize new_image
    for (uint8_t i = 0; i < new_image->rows; i++) {
        for (uint8_t j = 0; j < new_image->cols; j++) {
            Color *pixel = evaluateOnePixel(image, i, j);
            new_image->image[i][j].R = pixel->R;
            new_image->image[i][j].G = pixel->G;
            new_image->image[i][j].G = pixel->G;
            free(pixel);
        }
    }
    return new_image;
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image,
where each pixel is black if the LSB of the B channel is 0,
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
    //YOUR CODE HERE
    Image *original_image = readData(argv[1]);
    if (original_image == NULL) {
        return -1;
    }
    Image *new_image = steganography(original_image);
    if (new_image == NULL) {
        return -1;
    }
    writeData(new_image);
    freeImage(original_image);
    freeImage(new_image);
    return 0;
}
