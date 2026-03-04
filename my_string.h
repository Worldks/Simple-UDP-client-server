#ifndef MY_STRING_H_SENTRY
#define MY_STRING_H_SENTRY
#include <stdlib.h>
#include <stdio.h>

enum { max_number_length = 12 };
enum compare { less, equal, greather };	/* equal - 1 => may use in if() */

struct string {
	char *str;
	int length;
	int occupied_size; /* <= length - 1 */
};

void string_init(struct string **strpp);
void string_add_ch(struct string *strp, const char ch);
void string_delete(struct string **strpp);
void string_add_str(struct string *strp, const char *str);
int string_delete_part(struct string *strp, int start_position, int len);

int str_length(const char *str);
int is_str_equal(const char *str_1, const char *str_2);
void str_copy(char *dest, const char *src);
void number_to_str(int number, char *str);
int str_to_number(const char *str); /* only positive numbers */
enum compare compare_strings(const char *str_1, const char *str_2);

#endif
