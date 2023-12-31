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

#define MAX_DIGIT_PRODUCT_LEN 3 /* The maximum possible length (+1) of the
                                    product of two 1-digit numbers */

/* Crude fileno fix to allow compilation on Windows */
#define FILENO_STDIN 0
#define FILENO_STDOUT 1 


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
void do_product(longint_t *var1, longint_t *var2);
void digit_shift(longint_t *var1, int shift_width);
void do_exponent(longint_t *var1, longint_t *var2);
int longint_to_integer(longint_t *var);
int larger_num(longint_t *var1, longint_t *var2);
int do_minus(longint_t *var1, longint_t *var2);
int small_divide(longint_t *subset, longint_t *divisor, longint_t *remainder);
void do_divide(longint_t *var1, longint_t *var2);

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
    } else if (optype == DIVS) {
		do_divide(vars+varnum, &second_value);
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
    zero.length = 1;

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

    if(!first_non_zero) {
        parsed_num.digits[0] = 0;
        parsed_num.length = 1;
    }

    if(parsed_num.length > INTSIZE) {
        print_error("longint_t overflow has occurred.");
        exit(EXIT_FAILURE);
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

    
    for(i=var->length-1; i>=0; i--) {
        /* Print `,` every 3rd digit from the end */
        if(i < var->length - 1 && (i + 1) % 3 == 0) {
            printf(",");
        }
        printf("%d", var->digits[i]);
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

	int i, digit_sum, digit1, digit2, 
        var1_len = var1->length, var2_len = var2->length;


    /* Initialise a sum variable to longint_t zero */
    longint_t sum;
    sum.length = 1;
    for(i=0; i<INTSIZE; i++) {
        sum.digits[i] = 0;
    }

    i = 0;
    /* Loop while in bounds of the var1 and var2 */
    while(i < INTSIZE && (i < var1_len || i < var2_len)) {
        
        /* Do longint_t bounds detection */
        digit1 = var1->digits[i];
        digit2 = var2->digits[i];
        if(i >= var1_len) {
            digit1 = 0;
        }
        if(i >= var2_len) {
            digit2 = 0;
        }

        /* Add the corresponding digits */
		digit_sum = digit1 + digit2 + sum.digits[i];
        sum.digits[i] = digit_sum % INT_TEN;
        sum.digits[i+1] = digit_sum / INT_TEN;
        i++;
    }
    sum.length = i + digit_sum / INT_TEN;

    /* Check for overflows before final assignment */
    if(sum.length > INTSIZE) {
        print_error("longint_t overflow has occurred.");
        exit(EXIT_FAILURE);
    }

    do_assign(var1, &sum);

}

/*****************************************************************
******************************************************************

Put your new functions below this line. Make sure you add suitable
prototypes at the top of the program.

******************************************************************
*****************************************************************/

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
    /* Handle multiplying by 0 */
    if(var1->digits[var1->length - 1] == 0 || 
        var2->digits[var2->length - 1] == 0) {
        var1->digits[0] = 0;
        var1->length = 1;
        return;
    }
	char int_str[MAX_DIGIT_PRODUCT_LEN];

	/* Initialise a zero longint_t */
	longint_t zero;
    for(i=0; i<INTSIZE; i++) {
        zero.digits[i] = 0;
    }
    zero.length = 1;

	do_assign(&curr_total_product, &zero);
	do_assign(&final_product, &zero);

	/* Iterate through var2 (second number) */
	for(i = 0; i < var2_len; i++) {

		/* Iterate through var1 (first number) */
		for(j = 0; j < var1_len; j++) {
			/* Calculate product of corresponding base10 digits */
			curr_digit_product = var1->digits[j] * var2->digits[i];
			
			/* Assign value to a longint_t, accounting for digit position */
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
    /* EDGE CASE HANDLING */
    if(var1->digits[var1->length - 1] <= 1) {
        /* Exponential base is 0 or 1, don't modify vakyes */
        return;
    }
	if(exponent == 0) {
		var1->digits[0] = 1;
		var1->length = 1;
        return;
	}
    /* END EDGE CASE HANDLING */

	/* Iteratively perform multiplication of var1 to itself */
    for(i=0; i < exponent - 1; i++) {
        do_product(&result, var1);
    }
    
	/* Finally assign result */
    *var1 = result;
}

/* Returns 1 if `var1` is greater than `var2`, 0 if `var1` is equal to `var2`
   and -1 if `var1` is less than `var2`
*/
int
larger_num(longint_t *var1, longint_t *var2) {
	/* We immediately know the inequality based on a difference in lengths */
	if(var1->length > var2->length) {
		return 1;
	} else if (var1->length < var2->length) {
		return -1;
	}
	/* Define loop variable */
	int i = var1->length - 1;
	/* Loop to check character by character until a tiebreaker is found */
	while(i >= 0) {
		/* If any inequality is present, return value of inequality */
		if(var1->digits[i] > var2->digits[i]) {
			return 1;
		} else if(var1->digits[i] < var2->digits[i]){
			return -1;
		}
		i--;
	} 
	/* var1 is equal to var2, return 0 */
	return 0;
}


/* Assign to `var1` the result var1 = var1 - var2, returns -1 if
   the result is negative (does not modify var1 or var2 in this case)
*/
int
do_minus(longint_t *var1, longint_t *var2) {

	int i, digit_diff, carry = 0, var1_len = var1->length, 
    var2_len = var2->length, leading_zero_count = 0;
    longint_t difference;
    
    /* Check if the result is going to be negative */
    if(larger_num(var1, var2) < 0) {
        return -1;
    }

    i = 0;
    /* Loop while in bounds of the var1 and var2 */
    while(i < INTSIZE && (i < var1_len || i < var2_len)) {
    
        /* Do initial subtraction */
        if(i < var2_len) {
            digit_diff = var1->digits[i] - var2->digits[i] - carry;
        } else {
            digit_diff = var1->digits[i] - carry;
        }
        if(digit_diff < 0) {

            /* Handle necessary 'borrows' from next digit */
            digit_diff += 10;
            carry = 1;
        } else {
            carry = 0;
        }
        difference.digits[i] = digit_diff;
        
        /* Increment `leading_zero_count if the current digit is zero, 
        resetting if non_zero is found */
        if(digit_diff == 0) {
            leading_zero_count++;
        } else {
            leading_zero_count = 0;
        }
        i++;
    }
    difference.length = i - leading_zero_count;
	if(difference.length == 0) {
		difference.length = 1;
	}
    do_assign(var1, &difference);
    return 1;
}

/* Returns an integer that is the result of the integer division 
   `subset` / `divisor` where `subset` is a slice of a larger longint_t.
   Modifies `remainder` to be the remainder of the integer division.
*/
int
small_divide(longint_t *subset, longint_t *divisor, longint_t *remainder) {
	longint_t quotient;
	int result = 0;
	quotient = *subset;
	/* Use iterative subtraction to obtain quotient and remainder */
	while(do_minus(&quotient, divisor) > 0) {
		result++;
	}
	/* Assign remainder and return result */
	*remainder = quotient;
	return result;
}

/* Update the indicated variable var1 by doing an integer division
   using var2 to compute var1 = var1 / var2
*/
void
do_divide(longint_t *var1, longint_t *var2) {
    int i, subset_index, curr_digit = 0, index = var1->length - 1, 
    selection_width = 1;
	longint_t subset, result, remainder, remainder_prefix;
	remainder.length = 1;
	remainder.digits[0] = 0;
	/* HANDLE EDGE CASES */
	if(var2->digits[var2->length - 1] == 0) {
		print_error("Division by 0 error.");
		exit(EXIT_FAILURE);
	}
	if(larger_num(var2, var1) > 0) {
		var1->digits[0] = 0;
		var1->length = 1;
		return;
	}
	/* Initialise result to all zeros */
	for(i=0;i<INTSIZE;i++) {
		result.digits[i] = 0;
	}
    /* Iterate through var1 backwards */
    for(i=index; i>=0; i--) {
		/* Choose the current subset to divide by */
		subset.length = selection_width;
		for(subset_index=0; subset_index < selection_width; subset_index++) {
			subset.digits[subset_index] = var1->digits[i + subset_index];
		}
		/* If necessary, add previous remainders */
		if(remainder.length > 1 || remainder.digits[0] > 0) {
			do_assign(&remainder_prefix, &remainder);
			digit_shift(&remainder_prefix, selection_width);
			do_plus(&subset, &remainder_prefix);
		}
		/* If the subset is greater than or equal to the divisor, divide */
        if(larger_num(&subset, var2) >= 0 && 
			subset.digits[subset.length - 1] != 0) {
			curr_digit = small_divide(&subset, var2, &remainder);
			selection_width = 1;
		} else {
			/* If not, increase selection width if the first digits ISN'T 0 */
			curr_digit = 0;
			if(subset.digits[subset.length - 1] != 0 )
			{
				selection_width++;
			}
		}
		/* Write division result to the result longint_t */
		result.digits[i] = curr_digit;
    }
	/* Remove leading zeros, modify var1 and return */
	for(i=INTSIZE-1; i>=0; i--) {
		if(result.digits[i] != 0) {
			result.length = i + 1;
			*var1 = result;
			return;
		}
	}
}

/*
Algorithms are fun!!!
*/