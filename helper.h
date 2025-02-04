#ifndef HELPER_H
#define HELPER_H
#define S21_TMP_STR_SIZE 512
#include <stdarg.h>

// sprintf
char *s21_process_specifier(char *p, va_list args, char spec, int minus,
                            int plus, int space, int sharp, int zero_padding,
                            int width, int precision, int is_short, int is_long,
                            int is_long_double);
void s21_parse_length(const char **fmt, int *is_short, int *is_long,
                      int *is_long_double);
int s21_parse_precision(const char **fmt, va_list args);
int s21_parse_width(const char **fmt, va_list args);
void s21_parse_flags(const char **fmt, int *minus, int *plus, int *space,
                     int *sharp, int *zero_padding);
char *s21_handle_format(char **p, const char **fmt, va_list args);
char *s21_copy_char(char *p, const char **fmt);
char *s21_process_signed_int(char *p, va_list args, int minus, int plus,
                             int space, int zero_padding, int width,
                             int is_short, int is_long);
char *s21_process_unsigned_int(char *p, va_list args, int minus,
                               int zero_padding, int width, int is_short,
                               int is_long);
char *s21_process_float(char *p, va_list args, int minus, int zero_padding,
                        int width, int precision, int is_long_double);
char *s21_process_exp_float(char *p, va_list args, int minus, int zero_padding,
                            int width, int precision, int is_long_double,
                            char spec);
char *s21_process_base_int(char *p, va_list args, int minus, int zero_padding,
                           int width, char spec, int sharp, int is_short,
                           int is_long);
char *s21_process_pointer(char *p, va_list args, int minus, int zero_padding,
                          int width);
char *s21_process_char(char *p, va_list args, int minus, int zero_padding,
                       int width);
char *s21_process_string(char *p, va_list args, int minus, int zero_padding,
                         int width, int precision);
char *s21_process_percent(char *p, int minus, int zero_padding, int width);
void reverse(char *str, int len);
int int_to_str(long val, char *str, int base);
int int_to_str_unsigned(unsigned long val, char *str, int base);
int double_to_str(double val, char *str, int precision);
int double_to_str_exp(double val, char *str, int precision, char exp_char);

char *s21_print_padding(char *p, int minus, int zero_padding, int width,
                        int total_len, int pad_after);
char *s21_print_string(char *p, const char *val, int minus, int zero_padding,
                       int width, int precision);
char *s21_print_char(char *p, char val, int minus, int zero_padding, int width);
char *s21_print_signed_int(char *p, long val, int minus, int plus, int space,
                           int zero_padding, int width);
char *s21_print_unsigned_int(char *p, unsigned long val, int minus,
                             int zero_padding, int width, int base, char spec,
                             int sharp);
char *s21_print_double(char *p, long double val, int minus, int zero_padding,
                       int width, int precision);

char *s21_print_double_exp(char *p, long double val, int minus,
                           int zero_padding, int width, int precision,
                           char spec);

//

int s21_parse_percent(const char **str);
int s21_parse_float(const char **str, void *arg, int width,
                    char length_modifier, int assign);
int s21_process_specifier2(const char **str, char spec, void *arg, int width,
                           char length_modifier, int assign);
//
int s21_is_space(char c);
void s21_skip_spaces(const char **str);
int s21_return_failure(const char *str, int fields_matched);
const char *s21_parse_sign(const char *str, int *sign, int *width);
int s21_parse_integer(const char **str_ptr, void *dest, int width, int base,
                      int sign_flag, char length_modifier, int assign);
int s21_parse_float_number(const char **str_ptr, void *dest, int width,
                           char length_modifier, int assign);
int s21_parse_string(const char **str_ptr, char *dest, int width, int assign);
int s21_parse_chars(const char **str_ptr, char *dest, int width, int assign);
int s21_parse_pointer(const char **str_ptr, void **dest, int width, int assign);

#endif
