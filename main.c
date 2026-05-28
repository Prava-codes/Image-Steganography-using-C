/*
Name: T.PRAVALLIKA
Roll_no: 25048_038
Submission Date: 22/05/2026
Project name : Steaganography
Title : Image Steganography using LSB Technique in BMP Images
*/



#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "decode.h"
#include "common.h"


Opr_type check_operation(char *option)
{
    if (strcmp(option, "-e") == 0)
        return encode;

    else if (strcmp(option, "-d") == 0)
        return decode;

    else
        return unsupported;
}
int main(int argc, char *argv[])
{
    EncodeInfo encodeInfo;

    if( argc == 1 )
    {
 // print -> Error + usage msg
        printf("\nEncoding: ./a.out -e <.bmp_file> <.text_file> [output file]\nDecoding: ./a.out -d <.bmp_file> [output file]\n");
        printf("\nMinimum arguments should between FOUR to FIVE \n\n");
 return 0;
    }

    int opr = check_operation(argv[1]);

    if( opr == encode )
    {
        if( argc < 4 | argc > 5 )
        {
            // print -> Error + usage msg
            printf("Invalid command line arguments\n");
            printf("Usage: ./a.out -e <source_image.bmp> <secret_file> [output_image.bmp]\n");
        }
        else
        {
            printf("Encode operation selected\n");
            if (validate_encode_args(argv, &encodeInfo) == failure)
            {
                printf("Validation failed for input files\n");
                return 0;
            }
            else    
            {
                printf("Validation completed successfully\n");
                printf("source image file: %s\n", encodeInfo.src_image_fname);
                printf("Secret file: %s\n",encodeInfo.secret_fname);
                printf("output file: %s\n",encodeInfo.output_image_fname);
                do_encoding(argv, &encodeInfo);
            }
        }

    }
    else if( opr == decode )
    {
        DecodeInfo decInfo;
        if( argc < 3 | argc > 4 ) 
        {
            // print -> Error + usage msg
            printf("\033[1;31mERROR\033[0m : Invalid command line arguments\n");
            printf("The arguments should be ./a.out -d <stego.bmp> <secret>\n");
            
            return 0;
        }
        else
        {
           printf("Decode operation selected\n");
            if(validate_decode_args(argv, &decInfo) == failure)
            {
                printf("Validation failed\n");
                return 0;
            }
            else
            {
                do_decoding(argv,&decInfo);
            }
        }
    }
    else
    {
        // Print -> Error + usage msg
        printf("\033[1;31mERROR\033[0m : Invalid operation selected\n");
        printf("Use -e for encoding or -d for decoding\n");
    }

return 0;

}

/*
1. PROJECT DESCRIPTION:

*This project implements Image Steganography using the C programming language, where secret information is hidden inside a BMP image using the Least Significant Bit (LSB) technique.
*The main objective of the project is to provide secure communication by concealing secret data within an image file without causing visible changes to the image.

*The application performs two operations:

Encoding :

*Hides secret data into a BMP image.
*Stores a magic string, file extension, file size, and file content.
*Generates a new encoded image.

Decoding :

*Extracts hidden information from the encoded image.
*Verifies the magic string.
*Retrieves the hidden file extension, size, and data.
*Reconstructs the original secret file.

*The project uses bit manipulation techniques, file handling operations, structures, pointers, and command-line arguments to implement data hiding and retrieval.


2. OBJECTIVE :
The objectives of this project are:

*To understand the concept of steganography.
*To hide secret data securely within an image.
*To implement encoding and decoding using the LSB technique.
*To understand file handling and bitwise operations in C.
*To provide secure data transmission.

3. PROBLEM STATEMENT :

*Data transferred over networks may be accessed by unauthorized users.
*Encryption protects data but reveals the existence of secret information.
*Steganography overcomes this issue by hiding information inside multimedia files such as images.

*This project hides secret data within a BMP image so that the presence of hidden information remains unnoticed.

4. CONCEPTS USED :
*Structures
*File Handling
*Pointers
*Arrays and Strings
*Command Line Arguments
*Bitwise Operators
*Functions
*Modular Programming

5. WORKING PRINCIPLE  :

ENCODING PROCESS(encode.c & encode.h) 
-> Read command line arguments.
-> Validate source image and secret file.
-> Open source image, secret file, and output image.
-> Check image capacity.
-> Copy BMP header.
-> Encode magic string.
-> Encode secret file extension.
-> Encode secret file size.
-> Encode secret file data.
-> Copy remaining image data.
-> Generate encoded image.


DECODING PROCESS(decode.c & decode.h)
-> Read command line arguments.
-> Open encoded image.
-> Decode magic string.
-> Decode secret file extension.
-> Decode secret file size.
-> Decode secret file data.
-> Reconstruct original file.


6. ADVANTAGES :
*Provides additional security.
*Hidden data is difficult to detect.
*Image quality remains almost unchanged.
*Supports different secret file formats.
*Simple and efficient implementation.


7. APPLICATIONS :
*Secure communication
*Military information transfer
*Digital watermarking
*Authentication systems
*Medical data security
*Copyright protection


8. CONCLUSION

*The Image Steganography project successfully hides and retrieves secret information inside a BMP image using the Least Significant Bit technique.
*The project demonstrates the practical implementation of file handling, bit manipulation, and data security concepts in C programming.
*while maintaining image quality and ensuring secure information transmission.

*/
