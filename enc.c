#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// define some constants
#define MAX_FILENAME	15
#define MIN_PASSWORD	8

void make_new_name(char *new_name, char *original_name);
int length_of_password(char *password);
int is_alpha(char c);
int is_digit(char c);
int is_valid_password(char *password);
void perform_XOR(char *input_filename, char *output_filename, char *password);
void print_first_five(char *filename);

/*
*/


void make_new_reverse_name(char *new_name, char *original_name)
{
	strcpy(new_name, "r");
	strcat(new_name, original_name);
}


/*	append 'new-' in front of filename
*/
void make_new_name(char *new_name, char *original_name)
{
	strcpy(new_name, "n-");
	strcat(new_name, original_name);
}

/*
*/
int length_of_password(char *password)
{
	return strlen(password);
}

/*
*/
int is_alpha(char c)
{
	return (('A'<=c && c<='Z')
	     || ('a'<=c && c<='z')) ? 1 : 0;
							// return 1 if is character
							// 0 otherwise
}

/*
*/
int is_digit(char c)
{
	return ('0'<=c && c<='9') ? 1 : 0;
							// return 1 if is digit
							// 0 otherwise
}

/*	verify if the password meets requirements
	- at least 8 characters long
	- has an alphabet character
	- has a digit
*/
int is_valid_password(char *password)
{
	int has_char=0, has_digit=0;
	int len = strlen(password);

	// check if contains character and digit
	for (int i = 0; i < len; i++) {
		if (is_alpha(password[i]))
			has_char  = 1;
		if (is_digit(password[i]))
			has_digit = 1;
	}

	// messages
	if (len < 8)
		printf("The password needs to have at least 8 characters.\n");
	if (!has_char)
		printf("The password needs to contain at least 1 alphabetical character.\n");
	if (!has_digit)
		printf("The password needs to contain at least 1 digit.\n");


	return ((len < 8) || !has_char || !has_digit) ? 0 : 1;
										// return 0 if is conditions are not met
										// 1 otherwise
}

/*	encrypt|decrypt files with XOR operation
	with a password
*/
void perform_XOR(char *input_filename, char *output_filename, char *password)
{
	FILE *input_file, *output_file; 
	int block_size = strlen(password);
	unsigned char block[block_size];
	int num_bytes;

	// files
	input_file  = fopen( input_filename, "rb");
	output_file = fopen(output_filename, "wb");

	// encryption|decryption
	do {
		num_bytes = fread(block, 1, block_size, input_file);

		// XOR byte by byte
		for (int i = 0; i < num_bytes; i++) 
			block[i] ^= password[i];

		fwrite(block, 1, num_bytes, output_file);
	} while (num_bytes == block_size);

	// clean up
	fclose( input_file);
	fclose(output_file);
}

/*	output first five bytes of file
// 	in 2-digit hex
// */
// void print_first_five(char *filename)
// {
// 	FILE *file = fopen(filename, "rb");
// 	unsigned char block[5];

// 	fread(block, 1, 5, file);

// 	for (int i = 0; i < 5; i++)
// 		printf("%02x\n", block[i]);

// 	// clean up
// 	fclose(file);
// }







void reverse(char *input_filename, char *output_filename, int block_size)
{
	FILE *input_file, *output_file;
	unsigned char block[block_size];
	int num_bytes;

	// files
	input_file  = fopen( input_filename, "rb");
	output_file = fopen(output_filename, "wb");

	// reverse
	do {
		num_bytes = fread(block, 1, block_size, input_file);

		// reverse byte by byte
		for (int i = 0; i < num_bytes/2; i++) {
			unsigned char temp = block[i];
			block[i] = block[num_bytes-1-i];
			block[num_bytes-1-i] = temp;
		}

		fwrite(block, 1, num_bytes, output_file);
	} while (num_bytes == block_size);

	// clean up
	fclose( input_file);
	fclose(output_file);
}







int main(int argc, char *argv[])
{
	// check valid arguments //
	if (argc != 4) {
		printf("usage:   %s <filename> <password> <reversing block size[<16]>\n", argv[0]);
		printf("example: %s auckland.jpg password1 12\n", argv[0]);
		return 1;
	}


	if (atoi(argv[3]) > 16){
        printf("Block size must be less than 16\n");
        return 0;
    }
    if (atoi(argv[3]) < 1){
        printf("Block size must be greater than 0\n");
        return 0;
    }
    if (atoi(argv[3]) == 0){
        printf("Block size must be an integer\n");
        return 0;
    }



	/******** PART 1 ********/
	char filename_new[20];
	char rev_filename_new[30];

	make_new_name(filename_new, argv[1]);
	make_new_reverse_name(rev_filename_new, argv[1]);

	printf("New filename = %s\n", rev_filename_new);
	printf("Password length = %d\n", length_of_password(argv[2]));

	/******** PART 2 ********/
	if (!is_valid_password(argv[2])) return 1;
									// quit if invalid password

	/******** PART 3 ********/
	perform_XOR(argv[1], filename_new, argv[2]);

	/******** PART 4 ********/
	reverse(filename_new, rev_filename_new, atoi(argv[3]));
	remove(filename_new);

	// /******** PART 5 ********/
	// print_first_five(filename_new);


	return 0;
}