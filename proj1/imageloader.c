/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object.
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename)
{
    //YOUR CODE HERE
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Error opening file");
        return NULL;
    }

    uint32_t rows, cols;
    fscanf(fp, "%*s %u %u %*d", &cols, &rows);

    // allocate memory for new_image
    Image *new_image = (Image *)malloc(sizeof(Image));
    if (new_image == NULL) {
        return NULL;
    }

    new_image->image = (Color **)malloc(rows * sizeof(Color *));
    if (new_image->image == NULL) {
        free(new_image);
        return NULL;
    }

    for (uint32_t i = 0; i < rows; i++) {
        new_image->image[i] = (Color *)malloc(cols * sizeof(Color));
        if (new_image->image[i] == NULL) {
            for (uint32_t j = 0; j < i; j++) {
                free(new_image->image[j]);
            }
            free(new_image->image);
            free(new_image);
            return NULL;
        }
    }

    // initialize new_image
    new_image->rows = rows;
    new_image->cols = cols;
    for (uint32_t i = 0; i < rows; i++) {
        for (uint32_t j = 0; j < cols; j++) {
            fscanf(fp, "%hhu %hhu %hhu", &new_image->image[i][j].R, &new_image->image[i][j].G, &new_image->image[i][j].B);
        }
    }

    fclose(fp);

    return new_image;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
    //YOUR CODE HERE
    printf("P3\n");
    printf("%u %u\n", image->cols, image->rows);
    printf("255\n");

     for (uint32_t i = 0; i < image->rows; i++) {
     	for (uint32_t j = 0; j < image->cols - 1; j++) {
     		Color current_pixel = image->image[i][j];
     		printf("%3hhu %3hhu %3hhu   ", current_pixel.R, current_pixel.G, current_pixel.B);
     	}
     	Color current_pixel = image->image[i][image->cols - 1];
     	printf("%3hhu %3hhu %3hhu\n", current_pixel.R, current_pixel.G, current_pixel.B);
     }
}

//Frees an image
void freeImage(Image *image)
{
    //YOUR CODE HERE
    for (uint32_t i = 0; i < image->rows; i++) {
        free(image->image[i]);
    }
    free(image->image);
    free(image);
}