#include <stdio.h>
#include <string.h>
#include "decode.h"

Status validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    // Validate output image
    char *bmpExtensionCheck = strstr(argv[2], ".bmp");
    if (bmpExtensionCheck == NULL)
    {
        printf("\033[1;31mERROR\033[0m : Stego image file must be in .bmp format\n");
        return failure;
    }
    FILE *stegoFilePtr = fopen(argv[2], "r");
    if (stegoFilePtr == NULL)
    {
       printf("\033[1;31mERROR\033[0m : Stego image file does not exist\n");
        return failure;
    }
    fclose(stegoFilePtr);
    decInfo->stego_image_fname = argv[2];   // store in structure

     // Validate secret file
    if (argv[3] != NULL)
    {
        strcpy(decInfo->secret_fname, argv[3]);
        return success;
    }
    else
    {
        // Default secret file name
        strcpy(decInfo->secret_fname,"decoded_secret");
    }
    return success;
}


Status do_decoding(char *argv[], DecodeInfo *decInfo)
{
    printf("\nOpening required files...\n");
    if (open_img_file(decInfo) != success)
    {
        printf("\033[1;31mERROR\033[0m : Opening files failed\n");
        return failure;
    }
    printf("Files opened successfully\n");

    printf("Decoding magic string...\n");
    if (decode_magic_string(decInfo) != success)
    {
        printf("\033[1;31mERROR\033[0m : Failed to decode magic string\n");
        return failure;
    }
    printf("Magic string Decoded Successfully \n");

    printf("Decoding secret file extension...\n");
    if (decode_secret_file_extn(argv,decInfo) != success)
    {
       printf("\033[1;31mERROR\033[0m : Failed to decode file extension\n");
        return failure;
    }
    printf("secret File Extension Decoded successfully\n");

   printf("Decoding secret file size...\n");
    if (decode_secret_file_size(decInfo) != success)
    {
        printf("\033[1;31mERROR\033[0m : Failed to decode secret file size\n");
        return failure;
    }
    printf("Secret File Size Decoded Successfully\n");

    printf("Decoding secret file data...\n");
    if (decode_secret_file_data(decInfo) != success)
    {
        printf("\033[1;31mERROR\033[0m : Failed to decode secret file data\n");
        return failure;
    }
    printf("Secret File Data Decoded Successfully\n");

    printf("\033[1;32mSuccess\033[0m : Decoding completed successfully\n");
    return success;
}


Status open_img_file(DecodeInfo *decInfo)
{
    decInfo->stego_image_fptr=fopen(decInfo->stego_image_fname,"r");
    if(decInfo->stego_image_fptr==NULL)
    {
        fprintf(stderr,"\033[1;31mERROR\033[0m : unable to open file %s\n",decInfo->stego_image_fname);
        return failure;
    }

    decInfo->secret_fptr=fopen(decInfo->secret_fname,"w");
    if(decInfo->secret_fptr==NULL)
    {
        fprintf(stderr,"\033[1;31mERROR\033[0m : Unable to open file %s\n",decInfo->secret_fname);
        return failure;
    }
    return success;
}


Status decode_magic_string(DecodeInfo *decInfo)
{
    int magicStringLength = strlen(MAGIC_STRING);   // length of "#*" = 2
    char decoded_magic[magicStringLength + 1];
    char buffer_8[8];

    // Move file pointer to pixel data
    fseek(decInfo->stego_image_fptr, 54,SEEK_SET);

    // Decode each character
    for (int i = 0; i < magicStringLength; i++)
    {
        fread(buffer_8, sizeof(unsigned char), 8, decInfo->stego_image_fptr);
        decoded_magic[i] = decode_1byte_from_lsb(buffer_8);
    }

    decoded_magic[magicStringLength] = '\0';   // null terminate

    // Compare with MAGIC_STRING
    if (strcmp(decoded_magic,MAGIC_STRING) == 0)
    {
        printf("Magic string decoded successfully\n");
        return success;
    }
    else
    {
        printf("\033[1;31mERROR\033[0m : Magic string mismatch. Found: %s\n", decoded_magic);
        return failure;
    }

}



Status decode_secret_file_extn(char *argv[],DecodeInfo *decInfo)
{
    char buffer_32[32];
    fread(buffer_32, sizeof(unsigned char), 32, decInfo->stego_image_fptr);
    int extn_size = decode_4bytes_from_lsb(buffer_32);
    char decoded_extension[extn_size + 1];
    
    // Decode extension characters
    for (int i = 0; i < extn_size; i++)
    {
        char buffer_8[8];
        fread(buffer_8, sizeof(unsigned char), 8, decInfo->stego_image_fptr);

        decoded_extension[i] = decode_1byte_from_lsb(buffer_8);
    }

    decoded_extension[extn_size] = '\0';   
    strcpy(decInfo->secret_extn, decoded_extension);
    // Build final output filename using strcat   
    int i = 0;
    while (decInfo->secret_fname[i] != '\0')
    {
        if (decInfo->secret_fname[i] == '.')
        {
            decInfo->secret_fname[i] = '\0';  // cut at dot
            break;
        }
        i++;
    }
    strcat(decInfo->secret_fname, decInfo->secret_extn);   // append extension

    printf("Decoded output file: %s\n", decInfo->secret_fname);

    // Open secret file for writing
    decInfo->secret_fptr = fopen(decInfo->secret_fname, "w");
    if (decInfo->secret_fptr == NULL)
    {
        printf("\033[1;31mERROR\033[0m : Unable to create output file\n");
        return failure;
    }
    return success;
}


Status decode_secret_file_size(DecodeInfo *decInfo)
{
    char buffer_32[32];

    // Read 32 bytes from stego image
    fread(buffer_32, sizeof(char), 32, decInfo->stego_image_fptr);

    // Decode integer from LSBs
    decInfo->secret_file_size = decode_4bytes_from_lsb(buffer_32);

    printf("Secret file size: %d bytes\n",decInfo->secret_file_size);

    return success;
}


Status decode_secret_file_data(DecodeInfo *decInfo)
{
    for(int i = 0; i < decInfo->secret_file_size; i++)
    {
        char buffer_8[8];   // 8 bytes from image
        char secret_1;      // decoded character

        // Read 8 bytes from stego image
        fread(buffer_8, sizeof(unsigned char), 8,
              decInfo->stego_image_fptr);

        // Decode 1 byte from LSBs
        secret_1 = decode_1byte_from_lsb(buffer_8);

        // Write decoded byte into secret file
        fwrite(&secret_1, sizeof(unsigned char), 1,
               decInfo->secret_fptr);
    }

    return success;
}



int  decode_4bytes_from_lsb(char *buffer_32)
{

    int data = 0;

    for (int i = 0; i < 32; i++)
    {
        data = data << 1;              // make space for next bit
        int bit = buffer_32[i] & 1;    // extract LSB
        data = data | bit;             // set that bit
    }
    return data;
}



char decode_1byte_from_lsb(char *buffer_8)
{
    char decodedcharacter = 0;
     for(int i = 0; i < 8; i++)
    {
        int bit=buffer_8[i] & 1;
        decodedcharacter = decodedcharacter << 1;
        decodedcharacter = decodedcharacter | bit;
    }
    return decodedcharacter;
}







