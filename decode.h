#ifndef DECODE_H
#define DECODE_H

#include "common.h"

typedef struct DecodeInfo
{
    char *stego_image_fname;
    char secret_fname[100];

    FILE *stego_image_fptr;
    FILE *secret_fptr;
    
    char secret_extn[10];      
    int secret_extn_size;
    int secret_file_size;
} DecodeInfo;

Status validate_decode_args(char *argv[], DecodeInfo *decInfo);
Status do_decoding(char *argv[], DecodeInfo *decInfo);
Status open_img_file(DecodeInfo *decInfo);
Status decode_magic_string(DecodeInfo *decInfo);
Status decode_secret_file_extn(char *argv[],DecodeInfo *decInfo);
Status decode_secret_file_size(DecodeInfo *decInfo);
Status decode_secret_file_data(DecodeInfo *decInfo);
char decode_1byte_from_lsb(char *buffer_8);
int  decode_4bytes_from_lsb(char *buffer_32);

#endif