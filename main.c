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

