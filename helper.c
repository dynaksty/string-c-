#include "helper.h"

#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
// #include <string.h>

#include "s21_string.h"

// Копирование обычных символов
char *s21_copy_char(char *p, const char **fmt) {
  *p++ = **fmt;
  (*fmt)++;
  return p;
}

// Обработка форматирования
char *s21_handle_format(char **p, const char **fmt, va_list args) {
  int minus = 0, plus = 0, space = 0, sharp = 0, zero_padding = 0;
  int width = 0, precision = -1;
  int is_short = 0, is_long = 0, is_long_double = 0;

  // Парсинг флагов
  s21_parse_flags(fmt, &minus, &plus, &space, &sharp, &zero_padding);
  // Парсинг ширины
  width = s21_parse_width(fmt, args);
  // Парсинг точности
  precision = s21_parse_precision(fmt, args);
  // Парсинг модификаторов длины
  s21_parse_length(fmt, &is_short, &is_long, &is_long_double);
  // Получение спецификатора
  char spec = **fmt;
  if (**fmt) (*fmt)++;
  // Обработка спецификатора
  *p = s21_process_specifier(*p, args, spec, minus, plus, space, sharp,
                             zero_padding, width, precision, is_short, is_long,
                             is_long_double);
  return *p;
}

// Парсинг флагов
void s21_parse_flags(const char **fmt, int *minus, int *plus, int *space,
                     int *sharp, int *zero_padding) {
  while (**fmt == '-' || **fmt == '+' || **fmt == ' ' || **fmt == '#' ||
         **fmt == '0') {
    if (**fmt == '-')
      *minus = 1;
    else if (**fmt == '+')
      *plus = 1;
    else if (**fmt == ' ')
      *space = 1;
    else if (**fmt == '#')
      *sharp = 1;
    else if (**fmt == '0')
      *zero_padding = 1;
    (*fmt)++;
  }
}

// Парсинг ширины
int s21_parse_width(const char **fmt, va_list args) {
  int width = 0;
  if (**fmt == '*') {
    width = va_arg(args, int);
    (*fmt)++;
  } else {
    while (**fmt >= '0' && **fmt <= '9') {
      width = width * 10 + (**fmt - '0');
      (*fmt)++;
    }
  }
  return width;
}

// Парсинг точности
int s21_parse_precision(const char **fmt, va_list args) {
  int precision = -1;
  if (**fmt == '.') {
    (*fmt)++;
    if (**fmt == '*') {
      precision = va_arg(args, int);
      (*fmt)++;
    } else {
      precision = 0;
      while (**fmt >= '0' && **fmt <= '9') {
        precision = precision * 10 + (**fmt - '0');
        (*fmt)++;
      }
    }
  }
  return precision;
}

// Парсинг модификаторов длины
void s21_parse_length(const char **fmt, int *is_short, int *is_long,
                      int *is_long_double) {
  while (**fmt == 'h' || **fmt == 'l' || **fmt == 'L') {
    if (**fmt == 'h') {
      *is_short = 1;
    } else if (**fmt == 'l') {
      if (*(*fmt + 1) == 'l') {
        *is_long = 1;
        (*fmt)++;
      } else {
        *is_long = 1;
      }
    } else if (**fmt == 'L') {
      *is_long_double = 1;
    }
    (*fmt)++;
  }
}

// // Обработка спецификатора
// Функция обработки спецификатора
char *s21_process_specifier(char *p, va_list args, char spec, int minus,
                            int plus, int space, int sharp, int zero_padding,
                            int width, int precision, int is_short, int is_long,
                            int is_long_double) {
  switch (spec) {
    case 'd':
    case 'i':
      p = s21_process_signed_int(p, args, minus, plus, space, zero_padding,
                                 width, is_short, is_long);
      break;

    case 'u':
      p = s21_process_unsigned_int(p, args, minus, zero_padding, width,
                                   is_short, is_long);
      break;

    case 'f':
    case 'g':
    case 'G':
      p = s21_process_float(p, args, minus, zero_padding, width, precision,
                            is_long_double);
      break;

    case 'e':
    case 'E':
      p = s21_process_exp_float(p, args, minus, zero_padding, width, precision,
                                is_long_double, spec);
      break;

    case 'x':
    case 'X':
    case 'o':
      p = s21_process_base_int(p, args, minus, zero_padding, width, spec, sharp,
                               is_short, is_long);
      break;

    case 'p':
      p = s21_process_pointer(p, args, minus, zero_padding, width);
      break;

    case 'c':
      p = s21_process_char(p, args, minus, zero_padding, width);
      break;

    case 's':
      p = s21_process_string(p, args, minus, zero_padding, width, precision);
      break;

    case '%':
      p = s21_process_percent(p, minus, zero_padding, width);
      break;
  }
  return p;
}

// Обработка целых чисел со знаком
char *s21_process_signed_int(char *p, va_list args, int minus, int plus,
                             int space, int zero_padding, int width,
                             int is_short, int is_long) {
  long val = is_short  ? (short)va_arg(args, int)
             : is_long ? va_arg(args, long)
                       : va_arg(args, int);
  return s21_print_signed_int(p, val, minus, plus, space, zero_padding, width);
}

// Обработка целых чисел без знака
char *s21_process_unsigned_int(char *p, va_list args, int minus,
                               int zero_padding, int width, int is_short,
                               int is_long) {
  unsigned long val = is_short  ? (unsigned short)va_arg(args, unsigned int)
                      : is_long ? va_arg(args, unsigned long)
                                : va_arg(args, unsigned int);
  return s21_print_unsigned_int(p, val, minus, zero_padding, width, 10, 'u', 0);
}

// Обработка вещественных чисел
char *s21_process_float(char *p, va_list args, int minus, int zero_padding,
                        int width, int precision, int is_long_double) {
  long double val =
      is_long_double ? va_arg(args, long double) : va_arg(args, double);
  return s21_print_double(p, val, minus, zero_padding, width, precision);
}

// Обработка экспоненциального представления
char *s21_process_exp_float(char *p, va_list args, int minus, int zero_padding,
                            int width, int precision, int is_long_double,
                            char spec) {
  long double val =
      is_long_double ? va_arg(args, long double) : va_arg(args, double);
  return s21_print_double_exp(p, val, minus, zero_padding, width, precision,
                              spec);
}

// Обработка целых чисел с основанием
char *s21_process_base_int(char *p, va_list args, int minus, int zero_padding,
                           int width, char spec, int sharp, int is_short,
                           int is_long) {
  unsigned long val = is_short  ? (unsigned short)va_arg(args, unsigned int)
                      : is_long ? va_arg(args, unsigned long)
                                : va_arg(args, unsigned int);
  int base = (spec == 'o') ? 8 : 16;
  return s21_print_unsigned_int(p, val, minus, zero_padding, width, base, spec,
                                sharp);
}

// Обработка указателя
char *s21_process_pointer(char *p, va_list args, int minus, int zero_padding,
                          int width) {
  void *ptr = va_arg(args, void *);
  unsigned long addr = (unsigned long)ptr;
  char num_str[50];
  int len = int_to_str_unsigned(addr, num_str, 16);
  int prefix_len = 2;
  int total_len = prefix_len + len;
  p = s21_print_padding(p, minus, zero_padding, width, total_len, 0);
  *p++ = '0';
  *p++ = 'x';
  for (int i = 0; i < len; i++) {
    *p++ = num_str[i];
  }
  return s21_print_padding(p, minus, zero_padding, width, total_len, 1);
}

// Обработка символа
char *s21_process_char(char *p, va_list args, int minus, int zero_padding,
                       int width) {
  char val = (char)va_arg(args, int);
  return s21_print_char(p, val, minus, zero_padding, width);
}

// Обработка строки
char *s21_process_string(char *p, va_list args, int minus, int zero_padding,
                         int width, int precision) {
  char *val = va_arg(args, char *);
  return s21_print_string(p, val, minus, zero_padding, width, precision);
}

// Обработка символа '%'
char *s21_process_percent(char *p, int minus, int zero_padding, int width) {
  int total_len = 1;
  p = s21_print_padding(p, minus, zero_padding, width, total_len, 0);
  *p++ = '%';
  return s21_print_padding(p, minus, zero_padding, width, total_len, 1);
}

void reverse(char *str, int len) {
  int i = 0, j = len - 1;
  while (i < j) {
    char temp = str[i];
    str[i] = str[j];
    str[j] = temp;
    i++;
    j--;
  }
}

int int_to_str(long val, char *str, int base) {
  int i = 0, is_negative = 0;
  if (val == 0) {
    str[i++] = '0';
    str[i] = '\0';
    return i;
  }
  if (val < 0 && base == 10) {
    is_negative = 1;
    val = -val;
  }
  while (val != 0) {
    int rem = val % base;
    str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
    val /= base;
  }
  if (is_negative) str[i++] = '-';
  str[i] = '\0';
  reverse(str, i);
  return i;
}

int int_to_str_unsigned(unsigned long val, char *str, int base) {
  int i = 0;
  if (val == 0) {
    str[i++] = '0';
    str[i] = '\0';
    return i;
  }
  while (val != 0) {
    int rem = val % base;
    str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
    val /= base;
  }
  str[i] = '\0';
  reverse(str, i);
  return i;
}

int double_to_str(double val, char *str, int precision) {
  int i = 0;
  int is_negative = val < 0;
  if (is_negative) {
    val = -val;
  }

  if (precision == 0) {
    val = round(val);
  }

  long integer_part = (long)val;
  double fractional_part = val - integer_part;

  char int_str[50];
  int int_len = int_to_str(integer_part, int_str, 10);
  for (int k = 0; k < int_len; k++) {
    str[i++] = int_str[k];
  }

  if (precision > 0) {
    str[i++] = '.';

    double multiplier = 1.0;
    for (int k = 0; k < precision; k++) {
      multiplier *= 10.0;
    }
    fractional_part *= multiplier;
    long frac_int = (long)round(fractional_part);
    char frac_str[50];
    int frac_len = 0;
    while (frac_len < precision) {
      int digit = frac_int % 10;
      frac_int /= 10;
      frac_str[frac_len++] = (char)('0' + digit);
    }
    frac_str[frac_len] = '\0';
    reverse(frac_str, frac_len);
    for (int k = 0; k < frac_len; k++) {
      str[i++] = frac_str[k];
    }
  }

  if (is_negative) {
    for (int k = i; k >= 1; k--) {
      str[k] = str[k - 1];
    }
    str[0] = '-';
    i++;
  }

  str[i] = '\0';
  return i;
}

int double_to_str_exp(double val, char *str, int precision, char exp_char) {
  int i = 0;
  int is_negative = val < 0;
  if (is_negative) {
    val = -val;
  }

  int exponent = 0;
  while (val >= 10.0) {
    val /= 10.0;
    exponent++;
  }
  while (val < 1.0) {
    val *= 10.0;
    exponent--;
  }

  val = round(val * pow(10, precision)) / pow(10, precision);

  long integer_part = (long)val;
  double fractional_part = val - integer_part;

  char int_str[50];
  int int_len = int_to_str(integer_part, int_str, 10);
  for (int k = 0; k < int_len; k++) {
    str[i++] = int_str[k];
  }

  if (precision > 0) {
    str[i++] = '.';

    double multiplier = 1.0;
    for (int k = 0; k < precision; k++) {
      multiplier *= 10.0;
    }
    fractional_part *= multiplier;
    long frac_int = (long)round(fractional_part);

    char frac_str[50];
    int frac_len = 0;
    while (frac_len < precision) {
      int digit = frac_int % 10;
      frac_int /= 10;
      frac_str[frac_len++] = (char)('0' + digit);
    }
    frac_str[frac_len] = '\0';
    reverse(frac_str, frac_len);
    for (int k = 0; k < frac_len; k++) {
      str[i++] = frac_str[k];
    }
  }

  str[i++] = exp_char;
  str[i++] = exponent >= 0 ? '+' : '-';
  int exponent_abs = abs(exponent);
  char exp_str[50];
  int exp_len = int_to_str(exponent_abs, exp_str, 10);
  for (int k = 0; k < exp_len; k++) {
    str[i++] = exp_str[k];
  }

  if (is_negative) {
    for (int k = i; k >= 1; k--) {
      str[k] = str[k - 1];
    }
    str[0] = '-';
    i++;
  }

  str[i] = '\0';
  return i;
}

char *s21_print_padding(char *p, int minus, int zero_padding, int width,
                        int total_len, int pad_after) {
  if ((minus && pad_after) || (!minus && !pad_after)) {
    if (width > total_len) {
      int pad_size = width - total_len;
      char pad_char = (zero_padding && !minus) ? '0' : ' ';
      for (int i = 0; i < pad_size; i++) {
        *p++ = pad_char;
      }
    }
  }
  return p;
}

char *s21_print_string(char *p, const char *val, int minus, int zero_padding,
                       int width, int precision) {
  int len = (int)s21_strlen(val);
  if (precision >= 0 && precision < len) {
    len = precision;
  }
  int total_len = len;

  p = s21_print_padding(p, minus, zero_padding, width, total_len, 0);

  for (int i = 0; i < len; i++) {
    *p++ = val[i];
  }

  p = s21_print_padding(p, minus, zero_padding, width, total_len, 1);
  return p;
}

char *s21_print_char(char *p, char val, int minus, int zero_padding,
                     int width) {
  int total_len = 1;

  p = s21_print_padding(p, minus, zero_padding, width, total_len, 0);

  *p++ = val;

  p = s21_print_padding(p, minus, zero_padding, width, total_len, 1);

  return p;
}

char *s21_print_signed_int(char *p, long val, int minus, int plus, int space,
                           int zero_padding, int width) {
  char num_str[50];
  int len = int_to_str(val, num_str, 10);

  char sign_char = '\0';
  if (val >= 0) {
    if (plus) {
      sign_char = '+';
    } else if (space) {
      sign_char = ' ';
    }
  }
  int total_len = len + (sign_char ? 1 : 0);

  p = s21_print_padding(p, minus, zero_padding, width, total_len, 0);

  if (sign_char) {
    *p++ = sign_char;
  }

  for (int i = 0; i < len; i++) {
    *p++ = num_str[i];
  }

  p = s21_print_padding(p, minus, zero_padding, width, total_len, 1);
  return p;
}

char *s21_print_unsigned_int(char *p, unsigned long val, int minus,
                             int zero_padding, int width, int base, char spec,
                             int sharp) {
  char num_str[50];
  int len = int_to_str_unsigned(val, num_str, base);

  if (spec == 'X') {
    for (int i = 0; i < len; i++) {
      if (num_str[i] >= 'a' && num_str[i] <= 'f') {
        num_str[i] = num_str[i] - 32;
      }
    }
  }

  int prefix_len = 0;
  char prefix[3] = {0};
  if (sharp && val != 0) {
    if (spec == 'x') {
      prefix[0] = '0';
      prefix[1] = 'x';
      prefix_len = 2;
    } else if (spec == 'X') {
      prefix[0] = '0';
      prefix[1] = 'X';
      prefix_len = 2;
    } else if (spec == 'o') {
      prefix[0] = '0';
      prefix_len = 1;
    }
  }

  int total_len = len + prefix_len;

  p = s21_print_padding(p, minus, zero_padding, width, total_len, 0);

  for (int i = 0; i < prefix_len; i++) {
    *p++ = prefix[i];
  }

  for (int i = 0; i < len; i++) {
    *p++ = num_str[i];
  }

  p = s21_print_padding(p, minus, zero_padding, width, total_len, 1);
  return p;
}

char *s21_print_double(char *p, long double val, int minus, int zero_padding,
                       int width, int precision) {
  char num_str[S21_TMP_STR_SIZE];

  int prec = (precision >= 0) ? precision : 6;

  double cast_val = (double)val;
  int len = double_to_str(cast_val, num_str, prec);

  int total_len = len;

  p = s21_print_padding(p, minus, zero_padding, width, total_len, 0);

  for (int i = 0; i < len; i++) {
    *p++ = num_str[i];
  }

  p = s21_print_padding(p, minus, zero_padding, width, total_len, 1);
  return p;
}

char *s21_print_double_exp(char *p, long double val, int minus,
                           int zero_padding, int width, int precision,
                           char spec) {
  char num_str[S21_TMP_STR_SIZE];
  int prec = (precision >= 0) ? precision : 6;

  double cast_val = (double)val;
  int len = double_to_str_exp(cast_val, num_str, prec, spec);

  int total_len = len;

  p = s21_print_padding(p, minus, zero_padding, width, total_len, 0);

  for (int i = 0; i < len; i++) {
    *p++ = num_str[i];
  }

  p = s21_print_padding(p, minus, zero_padding, width, total_len, 1);
  return p;
}
///
int s21_process_specifier2(const char **str, char spec, void *arg, int width,
                           char length_modifier, int assign) {
  int res = -1;

  switch (spec) {
    case 'd':
      res = s21_parse_integer(str, arg, width, 10, 1, length_modifier, assign);
      break;
    case 'i':
      res = s21_parse_integer(str, arg, width, 0, 1, length_modifier, assign);
      break;
    case 'u':
      res = s21_parse_integer(str, arg, width, 10, 0, length_modifier, assign);
      break;
    case 'o':
      res = s21_parse_integer(str, arg, width, 8, 0, length_modifier, assign);
      break;
    case 'x':
    case 'X':
      res = s21_parse_integer(str, arg, width, 16, 0, length_modifier, assign);
      break;
    case 'p':
      res = s21_parse_pointer(str, (void **)arg, width, assign);
      break;
    case 'c': {
      char buffer[S21_SSCANF_MAX_FIELD_WIDTH + 1];
      res = s21_parse_chars(str, buffer, width, assign);
      if (res == 0 && assign) {
        int w = (width == 0) ? 1 : width;
        for (int i = 0; i < w; i++) ((char *)arg)[i] = buffer[i];
      }
      break;
    }
    case 's': {
      char buffer[S21_SSCANF_MAX_FIELD_WIDTH + 1];
      res = s21_parse_string(str, buffer, width, assign);
      if (res == 0 && assign) {
        s21_strncpy((char *)arg, buffer, S21_SSCANF_MAX_FIELD_WIDTH);
      }
      break;
    }
    case 'f':
    case 'e':
    case 'E':
    case 'g':
    case 'G':
      res = s21_parse_float(str, arg, width, length_modifier, assign);
      break;
    case '%':
      res = s21_parse_percent(str);
      break;
    default:
      res = -1;
      break;
  }

  return res;
}

int s21_parse_float(const char **str, void *arg, int width,
                    char length_modifier, int assign) {
  int res = -1;
  if (!assign) {
    long double dummy;
    res = s21_parse_float_number(str, &dummy, width, length_modifier, assign);
  } else {
    if (length_modifier == 'L') {
      res = s21_parse_float_number(str, (long double *)arg, width,
                                   length_modifier, assign);
    } else if (length_modifier == 'l') {
      res = s21_parse_float_number(str, (double *)arg, width, length_modifier,
                                   assign);
    } else {
      res = s21_parse_float_number(str, (float *)arg, width, length_modifier,
                                   assign);
    }
  }
  return res;
}

int s21_parse_percent(const char **str) {
  if (**str == '%') {
    (*str)++;
    return 0;
  }
  return -1;
}
//////////////
int s21_is_space(char c) {
  return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' ||
         c == '\v';
}

void s21_skip_spaces(const char **str) {
  while (s21_is_space(**str)) {
    (*str)++;
  }
}

int s21_return_failure(const char *str, int fields_matched) {
  if (fields_matched == 0) {
    if (*str == '\0') {
      return -1;
    } else {
      return 0;
    }
  } else {
    return fields_matched;
  }
}

const char *s21_parse_sign(const char *str, int *sign, int *width) {
  *sign = 1;
  if (*width > 0 && (*str == '-' || *str == '+')) {
    if (*str == '-') *sign = -1;
    str++;
    (*width)--;
  }
  return str;
}

int s21_parse_integer(const char **str_ptr, void *dest, int width, int base,
                      int sign_flag, char length_modifier, int assign) {
  const char *str = *str_ptr;
  s21_skip_spaces(&str);

  if (width <= 0 || width > S21_SSCANF_MAX_FIELD_WIDTH)
    width = S21_SSCANF_MAX_FIELD_WIDTH;

  int sign = 1;
  if (sign_flag) str = s21_parse_sign(str, &sign, &width);

  if (base == 0) {
    base = 10;
    if (width > 1 && *str == '0') {
      if ((width > 2) && (tolower((unsigned char)*(str + 1)) == 'x')) {
        base = 16;
        str += 2;
        width -= 2;
      } else {
        base = 8;
        str++;
        width--;
      }
    }
  } else {
    if (base == 16 && width > 1 && *str == '0' &&
        tolower((unsigned char)*(str + 1)) == 'x') {
      str += 2;
      width -= 2;
    } else if (base == 8 && width > 0 && *str == '0') {
      str++;
      width--;
    }
  }

  if (width <= 0) return -1;

  unsigned long long val = 0;
  int count = 0;
  while (width > 0 && *str) {
    unsigned char c = (unsigned char)*str;
    int digit = -1;
    if (isdigit(c))
      digit = c - '0';
    else if (isalpha(c))
      digit = (tolower(c) - 'a') + 10;
    if (digit < 0 || digit >= base) break;

    val = val * base + digit;
    str++;
    width--;
    count++;
  }

  if (count == 0) return -1;  // Нет цифр

  long long sval = (long long)val;
  if (sign_flag && sign == -1) {
    sval = -sval;
  }

  // Сохраняем только если assign=1
  if (assign) {
    if (sign_flag) {
      if (length_modifier == 'h')
        *(short *)dest = (short)sval;
      else if (length_modifier == 'l')
        *(long *)dest = (long)sval;
      else if (length_modifier == 'L')
        *(long long *)dest = (long long)sval;
      else
        *(int *)dest = (int)sval;
    } else {
      if (length_modifier == 'h')
        *(unsigned short *)dest = (unsigned short)val;
      else if (length_modifier == 'l')
        *(unsigned long *)dest = (unsigned long)val;
      else if (length_modifier == 'L')
        *(unsigned long long *)dest = val;
      else
        *(unsigned int *)dest = (unsigned int)val;
    }
  }

  *str_ptr = str;
  return 0;
}

int s21_parse_float_number(const char **str_ptr, void *dest, int width,
                           char length_modifier, int assign) {
  const char *str = *str_ptr;
  s21_skip_spaces(&str);

  if (width <= 0 || width > S21_SSCANF_MAX_FIELD_WIDTH)
    width = S21_SSCANF_MAX_FIELD_WIDTH;

  if (width <= 0) return -1;

  int sign = 1;
  if (width > 0 && (*str == '+' || *str == '-')) {
    if (*str == '-') sign = -1;
    str++;
    width--;
  }

  int digits_before = 0, digits_after = 0;
  int have_exp = 0;

  long double value = 0.0L;

  while (width > 0 && isdigit((unsigned char)*str)) {
    value = value * 10.0L + (*str - '0');
    str++;
    width--;
    digits_before++;
  }

  if (width > 0 && *str == '.') {
    str++;
    width--;
    long double fraction = 1.0L;
    while (width > 0 && isdigit((unsigned char)*str)) {
      fraction /= 10.0L;
      value += fraction * (*str - '0');
      str++;
      width--;
      digits_after++;
    }
  }

  int exp_sign = 1;
  int exp_value = 0;
  if (width > 0 && (tolower((unsigned char)*str) == 'e')) {
    have_exp = 1;
    str++;
    width--;
    if (width > 0 && (*str == '-' || *str == '+')) {
      if (*str == '-') exp_sign = -1;
      str++;
      width--;
    }

    int exp_digits = 0;
    while (width > 0 && isdigit((unsigned char)*str)) {
      exp_value = exp_value * 10 + (*str - '0');
      str++;
      width--;
      exp_digits++;
    }
    if (exp_digits == 0) {
      return -1;
    }
  }

  if (digits_before == 0 && digits_after == 0 && !have_exp) {
    return -1;
  }

  if (have_exp) {
    long double pow10 = 1.0L;
    for (int i = 0; i < exp_value; i++) {
      pow10 *= 10.0L;
    }
    if (exp_sign == -1)
      value /= pow10;
    else
      value *= pow10;
  }

  if (sign == -1) value = -value;

  if (assign) {
    if (length_modifier == 'L')
      *(long double *)dest = value;
    else if (length_modifier == 'l')
      *(double *)dest = (double)value;
    else
      *(float *)dest = (float)value;
  }

  *str_ptr = str;
  return 0;
}

int s21_parse_string(const char **str_ptr, char *dest, int width, int assign) {
  const char *str = *str_ptr;
  s21_skip_spaces(&str);

  if (width <= 0 || width > S21_SSCANF_MAX_FIELD_WIDTH)
    width = S21_SSCANF_MAX_FIELD_WIDTH;

  int count = 0;
  while (width > 0 && *str && !s21_is_space(*str)) {
    if (assign) dest[count] = *str;
    str++;
    width--;
    count++;
  }
  if (count == 0) return -1;
  if (assign) dest[count] = '\0';

  *str_ptr = str;
  return 0;
}

int s21_parse_chars(const char **str_ptr, char *dest, int width, int assign) {
  const char *str = *str_ptr;
  if (width <= 0) width = 1;
  int count = 0;
  while (width > 0 && *str) {
    if (assign) dest[count] = *str;
    str++;
    width--;
    count++;
  }
  if (count == 0) return -1;
  if (assign && count < S21_SSCANF_MAX_FIELD_WIDTH) {
  }
  *str_ptr = str;
  return 0;
}

int s21_parse_pointer(const char **str_ptr, void **dest, int width,
                      int assign) {
  const char *str = *str_ptr;
  s21_skip_spaces(&str);
  if (width <= 0 || width > S21_SSCANF_MAX_FIELD_WIDTH)
    width = S21_SSCANF_MAX_FIELD_WIDTH;

  if (width > 2 && *str == '0' && tolower((unsigned char)*(str + 1)) == 'x') {
    str += 2;
    width -= 2;
  }

  unsigned long long val = 0;
  int count = 0;
  while (width > 0 && *str) {
    unsigned char c = (unsigned char)*str;
    int digit = -1;
    if (isdigit(c))
      digit = c - '0';
    else if (isalpha(c))
      digit = (tolower(c) - 'a') + 10;
    else
      break;
    if (digit < 0 || digit >= 16) break;
    val = val * 16 + digit;
    str++;
    width--;
    count++;
  }

  if (count == 0) return -1;
  if (assign) *dest = (void *)(uintptr_t)val;
  *str_ptr = str;
  return 0;
}
