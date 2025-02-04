#ifndef S21_STRING_H
#define S21_STRING_H

#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define s21_NULL ((void *)0)
#define S21_SSCANF_MAX_FIELD_WIDTH 4096
typedef long unsigned s21_size_t;

void *s21_memchr(const void *str, int symbol, s21_size_t size);
int s21_memcmp(const void *mem_1, const void *mem_2, s21_size_t size);
void *s21_memcpy(void *dest, const void *src, s21_size_t size);
void *s21_memset(void *str, int symbol, s21_size_t size);
char *s21_strncat(char *result, const char *str, s21_size_t size);
char *s21_strchr(const char *str, int symbol);
int s21_strncmp(const char *str_1, const char *str_2, s21_size_t size);
char *s21_strncpy(char *destptr, const char *srcptr, s21_size_t size);
s21_size_t s21_strcspn(const char *str, const char *keys);
char *s21_strerror(int errorNumber);
s21_size_t s21_strlen(const char *str);
char *s21_strpbrk(const char *str, const char *keys);
char *s21_strrchr(const char *str, int symbol);
char *s21_strstr(const char *destptr, const char *srcptr);
char *s21_strtok(char *str, const char *delim);

int s21_sprintf(char *str, const char *format, ...);
int s21_sscanf(const char *str, const char *format, ...);

void *s21_to_upper(const char *str);
void *s21_to_lower(const char *str);
void *s21_insert(const char *src, const char *str, size_t start_index);
void *s21_trim(const char *src, const char *trim_chars);

#endif