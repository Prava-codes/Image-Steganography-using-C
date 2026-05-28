#include <stdio.h>
#include<string.h>
#include "encode.h"
//#include "types.h"


Status validate_encode_args(char *argv[],EncodeInfo *encInfo)
{
    //source file validation
    char *bmpExtensionCheck = strstr(argv[2],".bmp");
    if(bmpExtensionCheck == NULL)
    {
       printf("\033[1;31mERROR\033[0m : Source image file must be in .bmp format\n");
        return failure;
    }

    FILE *sourceFilePtr = fopen(argv[2],"r");
    if(sourceFilePtr == NULL)
    {
        printf("\033[1;31mERROR\033[0m : Source image file does not exist\n");
        return failure;
    }
    encInfo->src_image_fname=argv[2];     


    //secret file validation
    char *allowedExtensions[] = {".txt",".c",".cpp"};
    int extensioncount = 3;
    int isvalidExtension = 0;
    for(int i = 0; i < extensioncount; i++)
    {
        if (strstr(argv[3], allowedExtensions[i]) != NULL)  
        {
            strcpy(encInfo->secret_extn, allowedExtensions[i]); 
            isvalidExtension = 1;
            break;
        }
    }
    if(!isvalidExtension)
    {
        printf("\033[1;31mERROR\033[0m : Secret file name not provided\n");
        printf("secret file must be .txt, .c, .cpp\n");
        return failure;
    }
    encInfo->secret_fname = argv[3];
    FILE *secretFilePtr = fopen(argv[3],"r");
    if(secretFilePtr == NULL)
    {
        printf("\033[1;31mERROR\033[0m : Secret file does not exist\n");
        return failure;
    }

    //output file validation
    if(argv[4]!=NULL)
    {
        char *outputExtensionCheck = strstr(argv[4],".bmp");
        {
            if(outputExtensionCheck == NULL)
            {
               printf("\033[1;31mERROR\033[0m : Output image file must be in .bmp format\n");
                return failure;
            }
        }
        FILE *outputFilePtr = fopen(argv[4],"r");
        if(outputFilePtr == NULL)
        {
            printf("\033[1;31mERROR\033[0m : Output image file does not exist\n");
            return failure;
        }
        encInfo->output_image_fname=argv[4];
        return success;
    }
    encInfo->output_image_fname="output.bmp";  
    return success;
}


Status do_encoding(char *argv[], EncodeInfo *encInfo)
{
    printf("Opening required files\n");
    if (open_files(encInfo) != success)
    {
        printf("Opening required files...\n");
        return failure;
    }
    printf("Files opened Successfully\n");
    printf("Checking image capacity\n");
    if (check_capacity(encInfo) != success)
    {
        printf("\033[1;31mERROR\033[0m : Image does not have enough capacity\n");
        return failure;
    }
    printf("Copying BMP header Successfully\n");
    if (copy_bmp_header(encInfo) != success)
    {
        printf("\033[1;31mERROR\033[0m : Copying BMP header failed\n");
        return failure;
    }
    printf("Encoding Magic String Successfully\n");
    if (encode_magic_string(MAGIC_STRING, encInfo) != success)
    {
        printf("\033[1;31mERROR\033[0m : Encoding magic string failed\n");
        return failure;
    }
    printf("Encoding Secret File Extension Successfully\n");
    if (encode_secret_file_extn(encInfo) != success)
    {
        printf("\033[1;31mERROR\033[0m : Encoding extension failed\n");
        return failure;
    }
    printf("Encoding Secret File Size Successfully\n");
    if (encode_secret_file_size(encInfo) != success)
    {
        printf("\033[1;31mERROR\033[0m : Encoding secret file size failed\n");
        return failure;
    }
    printf("Encoding Secret File Data Successfully\n");
    if (encode_secret_file_data(encInfo) != success)
    {
        printf("\033[1;31mERROR\033[0m : Encoding secret file data failed\n");
        return failure;
    }
    printf("Copying remaining image data Successfully\n");
    if (copy_remaining_img_data(encInfo) != success)
    {
        printf("\033[1;31mERROR\033[0m : Copying remaining data failed\n");
        return failure;
    }
    printf("Everything is copied\n");
    printf("\033[1;32mSuccess\033[0m : Encoding completed Successfully\n");
    return success;
}


/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: success or failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    encInfo->src_image_fptr = fopen(encInfo->src_image_fname, "r");

    if (encInfo->src_image_fptr == NULL)
    {
     fprintf(stderr, "\033[1;31mERROR\033[0m : Unable to open file %s\n", encInfo->src_image_fname);

     return failure;
    }

    encInfo->secret_fptr = fopen(encInfo->secret_fname, "r");

    if (encInfo->secret_fptr == NULL)
    {
     fprintf(stderr, "\033[1;31mERROR\033[0m : Unable to open file %s\n", encInfo->secret_fname);

     return failure;
    }

    encInfo->output_image_fptr = fopen(encInfo->output_image_fname, "w");

    if (encInfo->output_image_fptr == NULL)
    {
     fprintf(stderr, "\033[1;31mERROR\033[0m : Unable to open file %s\n", encInfo->output_image_fname);

     return failure;
    }

    return success;
}



//check the capacity of source file
Status check_capacity(EncodeInfo *encInfo)
{
    unsigned int img_capacity = get_image_size_for_bmp(encInfo->src_image_fptr);
    encInfo->secret_file_size = get_file_size(encInfo->secret_fptr);
    unsigned long long int encoding_things = (strlen(MAGIC_STRING)+ 4 +strlen(encInfo->secret_extn) + 4 + encInfo->secret_file_size)*8;

    if(img_capacity > encoding_things)
    {
        return success;
    }
    else
    {
         return failure;
    }
}


/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
unsigned int get_image_size_for_bmp(FILE *fptr_image)
{
    unsigned int width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("Image width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("Image height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}


//get secret file size
unsigned int get_file_size(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END);
    unsigned int size = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);
    return size;
}


//copy 54 bytes from source file to destination file
Status copy_bmp_header(EncodeInfo *encInfo)
{
    unsigned char header[54];
    fseek(encInfo->src_image_fptr,0,SEEK_SET);
    fread(header,54,sizeof(unsigned char),encInfo->src_image_fptr);
    fwrite(header,54,sizeof(unsigned char),encInfo->output_image_fptr);
    return success;
}


//store magic string
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    int length = strlen(magic_string);
    
    for(int i = 0; i < length; i++)
    {
        char buffer[8];
        fread(buffer,sizeof(unsigned char),8,encInfo->src_image_fptr);
        encode_1byte_to_lsb(magic_string[i],buffer);
        fwrite(buffer,sizeof(unsigned char),8,encInfo->output_image_fptr);
    }
    return success;
}



//encode secret file extension
Status encode_secret_file_extn(EncodeInfo *encInfo)
{
    //take extension which is already stored in validation
    char *fileExtension = encInfo->secret_extn;
    int extensionLength = strlen(fileExtension);  //find length of extn
    encInfo->secret_extn_size = extensionLength; //store length in file


     //encode extension(any extension,it is integer only..so 4*8=32 bytes)
    char buffer_32[32];
    fread(buffer_32,sizeof(unsigned char),32,encInfo->src_image_fptr);
    encode_4byte_to_lsb(extensionLength, buffer_32);
    fwrite(buffer_32, sizeof(unsigned char), 32, encInfo->output_image_fptr);

for (int i = 0; i < extensionLength; i++)
    {
        //encode extension characters
        char buffer_8[8];
        fread(buffer_8,sizeof(unsigned char),8,encInfo->src_image_fptr);
        encode_1byte_to_lsb(encInfo->secret_extn[i], buffer_8);
        fwrite(buffer_8,sizeof(unsigned char),8,encInfo->output_image_fptr);
    }

    return success;
}


Status encode_secret_file_size(EncodeInfo *encInfo)
{
    char buffer_32[32];

    // Read 32 bytes from source image
    fread(buffer_32, 1, 32, encInfo->src_image_fptr);

    // Encode the file size
    encode_4byte_to_lsb(encInfo->secret_file_size, buffer_32);

    // Write 32 bytes to output image
    fwrite(buffer_32, 1, 32, encInfo->output_image_fptr);

    return success;
}


//to encode secret file data
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    
    for(int i = 0; i < encInfo->secret_file_size; i++)
    {
        char buffer_8[8];    //we need to encode 8 bytes of image for 1 byte so use array
        char secret_1;     //from secret file,we need to encode byte by byte..so use character
        fread(buffer_8,sizeof(unsigned char),8,encInfo->src_image_fptr);   //buffer itself pointer so no need of &
        fread(&secret_1,sizeof(unsigned char),1,encInfo->secret_fptr);    //fread 1st arfument must be pointer..so use &
        encode_1byte_to_lsb(secret_1, buffer_8);
        fwrite(buffer_8,sizeof(unsigned char),8,encInfo->output_image_fptr);
    }
    return success;
}


//Encode 1 byte into LSB of image data array
Status encode_1byte_to_lsb(char data, char *buffer_8)
{
    for(int i = 0; i < 8; i++)
    {
        buffer_8[i]=buffer_8[i] & ~(1<<0);  //to clear lsb bit
        int bit = (data >> (7 - i)) & 1;
        buffer_8[i]=buffer_8[i] | bit;  //to set bit
    }
    return success;
}


//Encode 4byte(int) into LSB of image data array
Status encode_4byte_to_lsb(int data, char *buffer_32)
{
    for(int i = 0; i < 32; i++)
    {
        buffer_32[i]=buffer_32[i] & ~(1<<0);  //to clear lsb bit
        int bit = (data >> (31 - i)) & 1;  //to go through each bit
        buffer_32[i]=buffer_32[i] | bit;  //to set bit
    }
    return success;
}


//copy the remaining bytes into output
Status copy_remaining_img_data(EncodeInfo * encInfo)
{
    char imageByte;
    while(fread(&imageByte,sizeof(unsigned char),1,encInfo->src_image_fptr) == 1)
    {
        fwrite(&imageByte,sizeof(unsigned char),1,encInfo->output_image_fptr);
    }
    return success;
}




