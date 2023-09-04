/* Program to perform multi-digit integer arithmetic.

   Skeleton program written by Alistair Moffat, ammoffat@unimelb.edu.au,
   August 2023, with the intention that it be modified by students
   to add functionality, as required by the assignment specification.
   All included code is (c) Copyright University of Melbourne, 2023

   Student Authorship Declaration:

   (1) I certify that except for the code provided in the initial skeleton
   file, the  program contained in this submission is completely my own
   individual work, except where explicitly noted by further comments that
   provide details otherwise.  I understand that work that has been developed
   by another student, or by me in collaboration with other students, or by
   non-students as a result of request, solicitation, or payment, may not be
   submitted for assessment in this subject.  I understand that submitting for
   assessment work developed by or in collaboration with other students or
   non-students constitutes Academic Misconduct, and may be penalized by mark
   deductions, or by other penalties determined via the University of
   Melbourne Academic Honesty Policy, as described at
   https://academicintegrity.unimelb.edu.au.

   (2) I also certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor will I
   do so until after the marks are released. I understand that providing my
   work to other students, regardless of my intention or any undertakings made
   to me by that other student, is also Academic Misconduct.

   (3) I further understand that providing a copy of the assignment
   specification to any form of code authoring or assignment tutoring service,
   or drawing the attention of others to such services and code that may have
   been made available via such a service, may be regarded as Student General
   Misconduct (interfering with the teaching activities of the University
   and/or inciting others to commit Academic Misconduct).  I understand that
   an allegation of Student General Misconduct may arise regardless of whether
   or not I personally make use of such solutions or sought benefit from such
   actions.

   Signed by: Julian Lewis 1444859
   Dated:     30/08/2023

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <assert.h>
#include <unistd.h>
#include <math.h>

/* All necessary #defines provided as part of the initial skeleton */

#define INTSIZE	500	/* max number of digits per integer value */
#define LINELEN	999	/* maximum length of any input line */
#define NVARS	26	/* number of different variables */

#define CH_A     'a'    /* character 'a', first variable name */

#define ERROR	(-1)	/* error return value from some functions */
#define PROMPT	"> "	/* the prompt string for interactive input */

#define PRINT	'?'	/* the print operator */
#define ASSIGN	'='	/* the assignment operator */
#define PLUS	'+'	/* the addition operator */
#define MULT	'*'	/* the multiplication operator */
#define POWR	'^'	/* the power-of operator */
#define DIVS	'/'	/* the division operator */
#define ALLOPS  "?=+*^/"

#define CH_ZERO  '0'    /* character zero */
#define CH_ONE   '1'    /* character one */
#define CH_NINE  '9'    /* character nine */

#define CH_COM   ','    /* character ',' */
#define PUT_COMMAS 3    /* interval between commas in output values */

#define INT_ZERO 0	/* integer 0 */
#define INT_TEN  10	/* integer 10 */

/* Crude fileno fix to allow compilation on Windows */
#define FILENO_STDIN 0
#define FILENO_STDOUT 1 


/* Placeholder typedef for skeleton code
*/

typedef struct {
    int length;
    int digits[INTSIZE];
} longint_t;

#define LONG_ZERO 0
#define LONG_ONE  1


/****************************************************************/

/* A "magic" additional function needing explicit declaration 
int fileno(FILE *); */

/* Skeleton program function prototypes */

void print_prompt(void);
void print_tadaa();
void print_error(char *message);
int  read_line(char *line, int maxlen);
void process_line(longint_t vars[], char *line);
int  get_second_value(longint_t vars[], char *rhsarg,
	longint_t *second_value);
int  to_varnum(char ident);
void do_print(int varnum, longint_t *var);
void do_assign(longint_t *var1, longint_t *var2);
void do_plus(longint_t *var1, longint_t *var2);
void zero_vars(longint_t vars[]);
longint_t parse_num(char *rhs);
int get_longint_length(char *num);

int max_2_ints(int num1, int num2);
void do_product(longint_t *var1, longint_t *var2);
void print_register_info(longint_t *var1);
void digit_shift(longint_t *var1, int shift_width);
void do_exponent(longint_t *var1, longint_t *var2);
int longint_to_integer(longint_t *var);
void do_divide(longint_t *var1, longint_t *var2);
int larger_num(longint_t *var1, longint_t *var2);
void assign_subset(longint_t *subset, longint_t *var, int index, int length);


/****************************************************************/

/* Main program controls all the action
*/
int
main(int argc, char *argv[]) {

    /* Read up to 1000 digit char array */
	char line[LINELEN+1] = {0};
	longint_t vars[NVARS];

    /* Initialise vars as a 0 longint array and prompt user for input */
	zero_vars(vars);
	print_prompt();

    /* Read input from user into line array */
	while (read_line(line, LINELEN)) {
		if (strlen(line) > 0) {
			/* non empty line, so process it */
			process_line(vars, line);
		}
        /* Prompt user for input again */
		print_prompt();
	}

    /* All done! finish up :) */
	print_tadaa();
	return 0;
}

/****************************************************************/

/* Prints the prompt indicating ready for input, but only if it
   can be confirmed that the input is coming from a terminal.
   Plus, output might be going to a file, that's why the prompt,
   if required, is written to stderr and not stdout
*/
void
print_prompt(void) {
	if (isatty(FILENO_STDIN)) {
		fprintf(stderr, "> ");
		fflush(stderr);
	}
}

void
print_tadaa() {
	/* all done, so pack up bat and ball and head home,
	   getting the exact final lines right is a bit tedious,
	   because input might be coming from a file and output
	   might be going to a file */
	if (isatty(FILENO_STDIN) && isatty(FILENO_STDOUT)) {
		printf("\n");
	}
	printf("ta daa!!!\n");
	if (isatty(FILENO_STDIN) && !isatty(FILENO_STDOUT)) {
		fprintf(stderr, "\n");
	}
}

void
print_error(char *message) {
	/* need to write an error message to the right place(s)
	*/
	if (isatty(FILENO_STDIN) || isatty(FILENO_STDOUT)) {
		fprintf(stderr, "%s\n", message);
		fflush(stderr);
	}
	if (!isatty(FILENO_STDOUT)) {
		printf("%s\n", message);
	}
}

/****************************************************************/

/* Reads a line of input into the array passed as argument,
   returns false if there is no input available.
   All whitespace characters are removed on the way through.
*/
int
read_line(char *line, int maxlen) {
	int i=0, c;
	while (((c=getchar())!=EOF) && (c!='\n')) {
		if (i<maxlen && !isspace(c)) {
			line[i++] = c;
		}
	}
	line[i] = '\0';
	/* then, if the input is coming from a file or the output
	   is going to a file, it is helpful to echo the input line
	   and record what the command was */
	if (!isatty(FILENO_STDIN) || !isatty(FILENO_STDOUT)) {
		printf("%s%s\n", PROMPT, line);
	}
	return ((i>0) || (c!=EOF));
}

/****************************************************************/

/* Process a command by parsing the input line into parts
*/
void
process_line(longint_t vars[], char *line) {
	int varnum, optype, status;
	longint_t second_value;

	/* determine the LHS variable, it
	   must be first character in compacted line
	*/
	varnum = to_varnum(line[0]);
	if (varnum==ERROR) {
		print_error("invalid LHS variable");
		return;
	}

	/* more testing for validity 
	*/
	if (strlen(line)<2) {
		print_error("no operator supplied");
		return;
	}

	/* determine the operation to be performed, it
	   must be second character of compacted line
	*/
	optype = line[1];
	if (strchr(ALLOPS, optype) == NULL) {
		print_error("unknown operator");
		return;
	}

	/* determine the RHS argument (if one is required),
	   it must start in the third character of compacted line
	*/
	if (optype != PRINT) {
		if (strlen(line)<3) {
			print_error("no RHS supplied");
			return;
		}
		status = get_second_value(vars, line+2, &second_value);
		if (status==ERROR) {
			print_error("RHS argument is invalid");
			return;
		}
	}

	/* finally, do the actual operation
	*/
	if (optype == PRINT) {
		do_print(varnum, vars+varnum);
	} else if (optype == ASSIGN) {
		do_assign(vars+varnum, &second_value);
	} else if (optype == PLUS) {
		do_plus(vars+varnum, &second_value);
	} else if (optype == MULT) {
        do_product(vars+varnum, &second_value);
    } else if (optype == POWR) {
        do_exponent(vars+varnum, &second_value);
    } else {
		print_error("operation not available yet");
		return;
	}
	return;
}

/****************************************************************/

/* Convert a character variable identifier to a variable number
*/
int
to_varnum(char ident) {
	int varnum;
	varnum = ident - CH_A;
	if (0<=varnum && varnum<NVARS) {
		return varnum;
	} else {
		return ERROR;
	}
}

/****************************************************************/

/* Process the input line to extract the RHS argument, which
   should start at the pointer that is passed
*/
int
get_second_value(longint_t vars[], char *rhsarg,
			longint_t *second_value) {
	char *p;
	int varnum2;
	if (isdigit(*rhsarg)) {
		/* first character is a digit, so RHS is a number
		   now check the rest of RHS for validity */
		for (p=rhsarg+1; *p; p++) {
			if (!isdigit(*p)) {
				/* nope, found an illegal character */
				return ERROR;
			}
		}
		/* nothing wrong, ok to convert */
		*second_value = parse_num(rhsarg);
		return !ERROR;
	} else {
		/* argument is not a number, so should be a variable */
		varnum2 = to_varnum(*rhsarg);
		if (varnum2==ERROR || strlen(rhsarg)!=1) {
			/* nope, not a variable either */
			return ERROR;
		}
		/* and finally, get that variable's value */
		do_assign(second_value, vars+varnum2);
		return !ERROR;
	}
	return ERROR;
}

/* Set the vars array to all zero values
*/
void
zero_vars(longint_t vars[]) {
	int i;
    /* Initialise a zero longint_t */
	longint_t zero;
    for(i=0; i<INTSIZE; i++) {
        zero.digits[i] = 0;
    }
    zero.length = 0;

    /* Iterate and assign all registers to longint_t zero */
	for (i=0; i<NVARS; i++) {
		do_assign(vars+i, &zero);
	}
	return;
}

/*****************************************************************
******************************************************************

Your answer to the assignment should start here, using your new
typedef defined at the top of the program. The next few functions
will require modifications because of the change of structure
used for a long_int, and then you'll need to start adding whole
new functions after you get these first ones working properly.
Try and make the new functions fit the style and naming pattern
of the existing ones, ok?

******************************************************************
*****************************************************************/

/* Create an internal-format number out of a string
*/
longint_t
parse_num(char *rhs) {
	int i, rhs_len = strlen(rhs), first_non_zero = 0, leading_zero_count = 0;;
	int parsed_digit;
    longint_t parsed_num;
    

    /* Read number backwards into parsed_num digits buffer */
    for(i=0; i<rhs_len; i++) {
        
        /* Offset characters by the code point of '0' */
        
		parsed_digit = rhs[rhs_len - 1 - i] - CH_ZERO;
		/* Check if the digit being read in is non-zero */
		if(rhs[i] != '0') {
			first_non_zero = 1;
		}
		/* Overwrite leading zeros */
		if(!first_non_zero) {
			leading_zero_count++;
		}
		
		parsed_num.digits[i] = (int)parsed_digit;
    }
	parsed_num.length = rhs_len - leading_zero_count;

    if(parsed_num.length > INTSIZE) {
        print_error("longint_t overflow has occurred.");
    }

    return parsed_num;
}

/****************************************************************/

/* Print out a longint value
*/
void
do_print(int varnum, longint_t *var) {

	printf("register %c: ", varnum+CH_A);
    int i;

    if(var->length == 0) {
        printf("0");
    } else {
        for(i=var->length-1; i>=0; i--) {
            /* Print `,` every 3rd digit from the end */
            if(i < var->length - 1 && (i + 1) % 3 == 0) {
                printf(",");
            }
            printf("%d", var->digits[i]);
        }
    }
    printf("\n");
	
}

/****************************************************************/

/* Assign a longint value, could do this with just an assignment
   statement, because structs can be assigned, but this is more
   elegant, and only copies over the array elements (digits) that
   are currently in use: var1 = var2

*/
void
do_assign(longint_t *var1, longint_t *var2) {
	
	*var1 = *var2;
}

/****************************************************************/

/* Update the indicated variable var1 by doing an addition
   using var2 to compute var1 = var1 + var2
*/
void
do_plus(longint_t *var1, longint_t *var2) {

	/*printf("\n\nAddition:\n");
	print_register_info(var1);
	print_register_info(var2);
	printf("End addition\n\n");*/

    int i, len_increase = 0, var1_len = var1->length, 
		var2_len = var2->length, carry = 0;

    int longest_len = max_2_ints(var1_len, var2_len);

	/* Iterate through all digits involved in the sum */
    for(i=0; i < longest_len + carry; i++) {

        /* Check if the longint_t digits buffer is going to be overflowed  */
        if(longest_len + 1 > INTSIZE) {
            print_error("longint_t overflow has occurred.");
            exit(EXIT_FAILURE);
        }

		/* If the current digit is in-bounds for both numbers, sum them 
		and update corresponding var1 digit */
        if(i < var2_len && i < var1_len) {
        	var1->digits[i] += var2->digits[i];
		} else if (i >= var1_len) {
			/* var1 is out of bounds, so just assign it the digit of var2 */
			var1->digits[i] = var2->digits[i];
		}
        /* Add previous carry to the current digit and reset carry */
        var1->digits[i] += carry;
        carry = 0;

        /* Handle digit carries */
		if(var1->digits[i] >= INT_TEN) {
			
            /* Check if carry occurs in final digit */
            if(i == longest_len - 1) {
                len_increase = 1;
            }
			carry = 1;
			var1->digits[i] -= INT_TEN;
		}

    }
    
    /* Check if the longint_t digits buffer has been overflowed 
    if (longest_len + len_increase > INTSIZE) {
        print_error("longint_t overflow has occurred.");
    } */

    /* Update the length of var1 */
	var1->length = longest_len + len_increase;
}

/*****************************************************************
******************************************************************

Put your new functions below this line. Make sure you add suitable
prototypes at the top of the program.

******************************************************************
*****************************************************************/

/* Return the largest of 2 input integers
*/
int
max_2_ints(int num1, int num2) {
	int max_int = (num1 > num2) ?  num1 : num2;
	return max_int;
}

/* This is a debugging function for printing
   the length and value of a longint_t register
*/
void
print_register_info(longint_t *var1) {
	int i = 0, var_len = var1->length;
	printf("Length: %d, Value: ", var_len);
	for(i=var_len - 1; i>=0; i--) {
		printf("%d", var1->digits[i]);
	}
	printf("\n");
}

/* Shift a longint_t register over by `shift_width` digits and increment
   the length by one. This is the same effect as multiplying by
   a power of 10, where `shift_width` is the exponent.
*/
void
digit_shift(longint_t *var1, int shift_width) {
	int i, var1_len = var1->length, 
		starting_pos = (var1_len + shift_width - 1);

	for(i=starting_pos; i>=0; i--) {
		if(i-shift_width >= 0) {
			var1->digits[i] = var1->digits[i-shift_width];
		} else {
			var1->digits[i] = 0;
		}
	}

	var1->length += shift_width;
}

/* Update the indicated variable var1 by doing a multiplication
   using var2 to compute var1 = var1 * var2
*/
void
do_product(longint_t *var1, longint_t *var2) {
    int i, j, curr_digit_product, var1_len = var1->length, 
        var2_len = var2->length;

    /* Initialise empty longint_t structs to store intermediate products */
    longint_t curr_total_product, final_product;

	char int_str[3];

	/* Initialise a zero longint_t */
	longint_t zero;
    for(i=0; i<INTSIZE; i++) {
        zero.digits[i] = 0;
    }
    zero.length = 0;

	do_assign(&curr_total_product, &zero);
	do_assign(&final_product, &zero);

	/* Iterate through var2 (second number) */
	for(i = 0; i < var2_len; i++) {

		/* Iterate through var1 (first number) */
		for(j = 0; j < var1_len; j++) {
			/* Calculate product of corresponding base10 digits */
			curr_digit_product = var1->digits[j] * var2->digits[i];
			
			/* Assign value to a longint_t */
			sprintf(int_str, "%d", curr_digit_product);
			curr_total_product = parse_num(int_str);

            digit_shift(&curr_total_product, i + j);
            do_plus(&final_product, &curr_total_product);
		}
		
	}
	do_assign(var1, &final_product);
}

/* Converts a longint_t variable to an integer and returns the result.
   Note: this isn't intended to be used for longint arithmetic, only
   for loop control for exponentiation.
*/
int
longint_to_integer(longint_t *var) {
    int i, converted_int = 0, var_len = var->length;

	/* Iterate through each digit of `var` */
    for(i = 0; i < var_len; i++) {
		/* Add digit to the integer output, accounting for position */
        converted_int += var->digits[i] * pow(10, i);
    }

    return converted_int;
}

/* Update the indicated variable var1 by raising var1 to the power
   of var2, var1 = var1 ^ var2
*/
void
do_exponent(longint_t *var1, longint_t *var2) {
    int i, exponent = longint_to_integer(var2);
    longint_t result = *var1;

	if(exponent == 0) {
		result.digits[0] = 1;
		result.length = 1;
	}

	/* Iteratively perform multiplication of var1 to itself */
    for(i=0; i < exponent - 1; i++) {
        do_product(&result, var1);
    }

	/* Finally assign result */
    *var1 = result;
}

/* Returns 1 if var1 is greater or equal to var2
*/
int
larger_num(longint_t *var1, longint_t *var2) {

	/* We immediately know the inequality based on a difference in lengths */
	if(var1->length > var2->length) {
		return 1;
	} else if (var1->length < var2->length) {
		return 0;
	}

	/* Define loop variable */
	int i = var1->length - 1;

	/* Loop to check character by character until a tiebreaker is found */
	while(i >= 0) {
		/* If any inequality is present, return value of inequality */
		if(var1->digits[i] > var2->digits[i]) {
			return 1;
		} else if(var1->digits[i] < var2->digits[i]){
			return 0;
		}
	} 
	/* var1 is equal to var2, return 1 */
	return 1;
}

/* Assign to a longint_t `subset` the value and length of a 'slice' or
   subset of `var` described by its starting index and length.
   Note: This function takes a subset slice in the regular order of a
   number - from largest digit to smallest (aka in reverse for a longint_t)
*/
void
assign_subset(longint_t *subset, longint_t *var, int index, int length) {
	int i, j = 0;

	subset->length = length;

	/* Iterate through all elements of the subset slice */
	while(i >= (index - length)) {

		subset->digits[j] = var->digits[i];

		j++;
		i--;
	}
}

/* Update the indicated variable var1 by doing an integer division
   using var2 to compute var1 = var1 / var2
*/
void
do_divide(longint_t *var1, longint_t *var2) {

	int i, var1_len = var1->length, selection_width = 0;
	longint_t result, curr_subset;

	/* Assign initial value to the current subset */
	assign_subset(&curr_subset, var1, var1_len-1, 1);

	/* Loop through all digits in the first number */
	for(i=var1_len-1; i>=0; i--) {

		/* Check if var2 is greater than the current subset of var1 */

		if(larger_num(var2, ))

		/* Check if var2 is divisible by current subset of var1 */

	}

}

/*
Algorithms are fun!!!
*/