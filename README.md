# Image-Steganography-using-C

## Description

Image Steganography is a CLI-based C project used to hide secret data inside BMP images using the Least Significant Bit (LSB) technique. The project performs secure data hiding and retrieval through encoding and decoding operations without visibly affecting image quality.

## Features


• Hide secret files inside BMP images

• Extract hidden data from stego images

• Uses LSB encoding technique

• Supports .txt, .c, and .cpp secret files

• Maintains original image quality

• Automatic recovery of secret file extension

• Preserves original image quality

• Command-line based implementation

## 🛠️ Technologies Used


• C Programming

• File Handling

• Bitwise Operators

• Command Line Interface (CLI)

• BMP Image Processing

• Structures and pointers

## Applications


• Secure communication

• Digital watermarking

• Copyright protection

• Military data transfer

• Authentication systems

• Confidential data sharing

## Project Structure

Steganography-Project/

├── main.c
├── common.h
├── encode.c
├── encode.h
├── decode.c
├── decode.h
├── beautiful.bmp
├── secret.txt
└── output.bmp

## 🖼️ Sample BMP Image

Sample BMP image used for hiding and retrieving secret data using the Least Significant Bit (LSB) steganography technique.

![beautiful.bmp](beautiful.bmp)


## Working Principle

### 🔒 Encoding Process


▸ Validate input files

▸ Check BMP image capacity

▸ Copy BMP header

▸ Encode magic string

▸ Encode secret file extension

▸ Encode secret file size

▸ Encode secret file data

▸ Generate stego image

### 🔓 Decoding Process


▸ Open stego image

▸ Decode magic string

▸ Decode file extension

▸ Decode secret file size

▸ Decode secret file data

▸ Reconstruct original file

## LSB Technique

The Least Significant Bit (LSB) technique is a data hiding method used in steganography to embed secret information inside image pixels. In this project, the secret data bits are stored in the least significant bits of BMP image bytes. Since only the last bit is modified, the changes are not visible to the human eye, which helps maintain the original image quality.

Example
Original image byte is:
10110110

To hide bit 1, the last bit is changed:

Modified Bit : 10110111

Only the least significant bit changes, so the image appears visually unchanged while carrying hidden data.

## ▶️ Compilation
gcc *.c

## Encoding Command

./a.out -e beautiful.bmp secret.txt output.bmp

Parameters :

./a.out → Executable file

-e → Encode operation

beautiful.bmp → Source BMP image

secret.txt → Secret file to hide

output.bmp → Generated stego image

### Decoding Command

./a.out -d output.bmp decoded.txt

Parameters :

./a.out → Executable file

-d → Decode operation

output.bmp → Stego image containing hidden data

decoded.txt → Extracted secret output file

## ✅ Advantages


• Secure data hiding technique

• Minimal image distortion

• Easy encoding and decoding process

• Efficient bit-level implementation

• Supports multiple secret file formats

## ⚠️ Limitations


• Supports only BMP image format

• Limited by image storage capacity

• Not suitable for very large secret files

## 📌 Learning Outcomes


• Learned image steganography concepts

• Improved bitwise programming skills

• Understood BMP image structure

• Practiced binary file operations

• Gained knowledge of secure data hiding

## 🎯 Conclusion

This project successfully implements image steganography using the LSB technique in C programming. It demonstrates practical implementation of bit manipulation, binary file processing, and secure data hiding while preserving image quality.


### -Pravallika Thirukkovalluru
