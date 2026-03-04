#include "my_string.h"

enum compare compare_strings(const char *str_1, const char *str_2)
{
	int len_1, len_2, i;
	len_1 = str_length(str_1);
	len_2 = str_length(str_2);
	if(len_1 == len_2) {
		for(i = 0; (str_1[i] == str_2[i]) && (i < len_1); i++)
			{}
		if(i == len_1)
			return equal;
		goto compare;
	}
	if(len_1 < len_2)
		len_1 = len_2;
	for(i = 0; (str_1[i] == str_2[i]) && (i < len_1); i++) {
		if(i == len_1 - 1)
			return less;
	}

compare:
	if(str_1[i] < str_2[i])
		return less;
	else
		return greather;
}

int str_length(const char *str)
{
	int i;
	for(i = 0; str[i] != '\0'; i++)
		{}
	return i;
}

int is_str_equal(const char *str_1, const char *str_2)
{
	int len_1, len_2, i;
	len_1 = str_length(str_1);
	len_2 = str_length(str_2);
	if(len_1 != len_2)
		return 0;
	for(i = 0; (str_1[i] == str_2[i]) && (i < len_1); i++)
		{}
	if(i == len_1)
		return 1;
	else
		return 0;
}

void str_copy(char *dest, const char *src)
{
	int len, i;
	len = str_length(src);
	for(i = 0; i <= len; dest[i] = src[i], i++)
		{}
}

void number_to_str(int number, char *str)
{
	int i, j;
	char tmp_str[max_number_length] = { 0 };
	char tmp;
	i = 0;
	j = 0;
	if(number == 0) {
		*str = 0x30;
		*(str+1) = '\0';
		return;
	}
	while(number != 0) {
		tmp = number % 10;
		number = number/10;
		tmp = tmp + 0x30;
		*(tmp_str + i) = tmp;
		i++;
	}
	i--;
	while(j <= i) {
		*(str + j) = *(tmp_str + i -j);
		j++;
	}
	*(str + j) = '\0';
}

int str_to_number(const char *str) /* FIX (wi str contain negative number?) */
{
	int number = 0, i = 0;
	char ch;
	ch = *str;
	while(ch != '\0') {	/* or something else (as terminated symbol) */
		ch = ch - 0x30;
		number = number*10 + ch;
		i++;
		ch = *(str+i);
	}
	return number;
}

static void increas_string_size(struct string *strp)
{
	char *new_str;
	void *mem;
	int i;
	mem = malloc(sizeof(strp->length) * 2);
	if(mem)
		new_str = mem;
	else
		perror("increas_string_size");
	for(i = 0; i <= strp->occupied_size; i++)
		new_str[i] = (strp->str)[i];
	free(strp->str);
	strp->str = new_str;
	strp->length *= 2;
}

void string_init(struct string **strpp)
{
	void *mem = malloc(sizeof(struct string));
	if(mem)
		(*strpp) = (struct string *)mem;
	else
		perror("string_init");

	mem = malloc(sizeof(char) * 512);
	if(mem) {
		(*strpp)->str = (char *)mem;
		(*strpp)->str[0] = '\0';
	} else
		perror("string_init");
	(*strpp)->occupied_size = 0;
	(*strpp)->length = 512;
}

void string_delete(struct string **strpp)
{
	free((*strpp)->str);
	free(*strpp);
	*strpp = NULL;
}

void string_add_ch(struct string *strp, char ch)
{
	if(strp->occupied_size == strp->length - 1)
		increas_string_size(strp);
	(strp->str)[strp->occupied_size] = ch;
	strp->occupied_size++;
	strp->str[(strp->occupied_size)] = '\0';
}

void string_add_str(struct string *strp, const char *str)
{
	int strlen, i;
	strlen = str_length(str);
	if(strp->occupied_size + strlen > strp->length - 1)
		increas_string_size(strp);
	for(i = 0; i < strlen; i++) {
		strp->str[(strp->occupied_size)] = str[i];
		strp->occupied_size++;
	}
	strp->str[(strp->occupied_size)] = '\0';
}

int string_delete_part(struct string *strp, int start_position, int len)
{
	if(start_position >= strp->occupied_size ||
		strp->occupied_size - start_position < 0)
		return 1;	/* error */
	(strp->str)[start_position] = '\0';
	strp->occupied_size = start_position;
	return 0;
}
