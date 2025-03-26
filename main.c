#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


// BMP file header structure
typedef struct BmpFileHeader {
   uint16_t bfType;
   //cant read only 2 chars
   //char bfType[2];
   unsigned int bfSize;
   unsigned short int __bfReserved1;
   unsigned short int __bfReserved2;
   unsigned long int bfOffBits;
} __attribute__ ((packed)) BmpFileHeader; 

typedef struct BmpImageHeader {
   unsigned int biSize;
   int biWidth;
   int biHeight;
   unsigned short int biPlanes;
   unsigned short int biBitCount;
   unsigned int biCompression;
   unsigned int biSizeImage;
   int biXPelsPerMeter;
   int biYPelPerMeter;
   unsigned int biClrUsed;
   unsigned int biClrImportant;  
} __attribute__ ((packed)) BmpImageHeader;


// Function declaration
extern void fade_top(void* img, int width, int height, int dist);

int main(int argc, char* argv[]) {
    if(argc != 3){
        printf("Incorect number of arguments");
        return 1;
    }
    
    char* filename = argv[1];
    int dist = atoi(argv[2]);


    FILE *file_ptr = fopen(filename, "rb");
    if (file_ptr == NULL) {
        printf("Error opening file");
        return 1;
    }

    // Reading the BMP file headers
    BmpFileHeader fileHeader;
    fread(&fileHeader, sizeof(BmpFileHeader), 1, file_ptr);


    //checking if the file is of correct type 4D is M 42 is B (inverted beacause of little-endian)
    if (fileHeader.bfType != 0x4D42) {
        printf("Not a valid BMP file\n");
        fclose(file_ptr);
        return 1;
    }

    BmpImageHeader imageHeader;
    fread(&imageHeader, sizeof(BmpImageHeader), 1, file_ptr);

    int width = imageHeader.biWidth;
    int height = imageHeader.biHeight;

    printf("Width: %d, Height: %d\n", width, height);

    // Allocate memory for the image data
    uint32_t imageSize = imageHeader.biSizeImage;
    if (imageSize == 0) {
        imageSize = width * height * (imageHeader.biBitCount / 8);
    }
    //unit8_t because the color bit the size of 8
    uint8_t *imageData = (uint8_t *)malloc(imageSize);

    // Move the file pointer to the start of the image data
    fseek(file_ptr, fileHeader.bfOffBits, SEEK_SET);
    // Read the pointer to the data
    fread(imageData, 1, imageSize, file_ptr);
    fclose(file_ptr);

    if(dist <= height){
    // Call the x86 function
        fade_top(imageData, width, height, dist);
    }
    // Write the modified image data back to the file
    //file_ptr = fopen(filename, "wb");
    file_ptr = fopen("resault.bmp", "wb");
    if (file_ptr == NULL) {
        perror("Error opening file for writing");
        free(imageData);
        return 1;
    }

    fwrite(&fileHeader, sizeof(BmpFileHeader), 1, file_ptr);
    fwrite(&imageHeader, sizeof(BmpImageHeader), 1, file_ptr);
    fseek(file_ptr, fileHeader.bfOffBits, SEEK_SET);
    fwrite(imageData, 1, imageSize, file_ptr);
    fclose(file_ptr);

    free(imageData);
    return 0;
}
