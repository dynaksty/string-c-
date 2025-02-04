#include <check.h>
#include <string.h>

#include "../s21_string.h"

int compare_float_strings(const char *str1, const char *str2, int precision) {
  if (precision <= 0) precision = 6;
  char *end1, *end2;
  double val1 = strtod(str1, &end1);
  double val2 = strtod(str2, &end2);

  if (end1 == str1 || end2 == str2) {
    return 0;
  }
  double epsilon = pow(10, -precision);
  return fabs(val1 - val2) < epsilon;
}

static int double_equal(double a, double b, double eps) {
  return fabs(a - b) < eps;
}

// Тесты для memchr

START_TEST(test_memchr) {
  char str[] = "Hello, world!";
  ck_assert_ptr_eq(s21_memchr(str, 'w', 13), memchr(str, 'w', 13));
  ck_assert_ptr_eq(s21_memchr(str, 'z', 13), memchr(str, 'z', 13));
  ck_assert_ptr_eq(s21_memchr(str, 'H', 1), memchr(str, 'H', 1));
  ck_assert_ptr_eq(s21_memchr(str, '!', 13), memchr(str, '!', 13));
  ck_assert_ptr_eq(s21_memchr(str, '\0', 14), memchr(str, '\0', 14));
}
END_TEST

// Тесты для memcmp
START_TEST(test_memcmp) {
  char str1[] = "Hello";
  char str2[] = "Hello";
  char str3[] = "World";
  char str4[] = "Hell";

  ck_assert_int_eq(s21_memcmp(str1, str2, 5) == 0, memcmp(str1, str2, 5) == 0);
  ck_assert_int_eq(s21_memcmp(str1, str3, 5) < 0, memcmp(str1, str3, 5) < 0);
  ck_assert_int_eq(s21_memcmp(str3, str1, 5) > 0, memcmp(str3, str1, 5) > 0);
  ck_assert_int_eq(s21_memcmp(str1, str4, 4) > 0, memcmp(str1, str4, 4) > 0);
  ck_assert_int_eq(s21_memcmp(str1, str4, 5) > 0, memcmp(str1, str4, 5) > 0);
}
END_TEST

// Тесты для memcpy
START_TEST(test_memcpy) {
  char src[] = "Hello, world!";
  char dest1[20] = "";
  char dest2[20] = "";

  s21_memcpy(dest1, src, 13);
  memcpy(dest2, src, 13);
  ck_assert_mem_eq(dest1, dest2, 13);

  s21_memcpy(dest1, "Test", 4);
  memcpy(dest2, "Test", 4);
  ck_assert_mem_eq(dest1, dest2, 4);
}
END_TEST

// Тесты для memset
START_TEST(test_memset) {
  char str1[10] = "Hello";
  char str2[10] = "Hello";

  s21_memset(str1, '*', 5);
  memset(str2, '*', 5);
  ck_assert_mem_eq(str1, str2, 5);

  s21_memset(str1, '0', 3);
  memset(str2, '0', 3);
  ck_assert_mem_eq(str1, str2, 3);
}
END_TEST

// Тесты для strlen
START_TEST(test_strlen) {
  ck_assert_int_eq(s21_strlen("Hello"), strlen("Hello"));
  ck_assert_int_eq(s21_strlen(""), strlen(""));
  ck_assert_int_eq(s21_strlen("Hello\0World"), strlen("Hello\0World"));
  ck_assert_int_eq(s21_strlen("12345"), strlen("12345"));
}
END_TEST

// Тесты для strncat
START_TEST(test_strncat) {
  char dest1[20] = "Hello";
  char dest2[20] = "Hello";
  const char *src = ", World!";
  size_t src_len = strlen(src);

  s21_strncat(dest1, src, src_len - 1);
  strncat(dest2, src, src_len - 1);
  ck_assert_str_eq(dest1, dest2);

  s21_strncat(dest1, "123", 2);
  strncat(dest2, "123", 2);
  ck_assert_str_eq(dest1, dest2);
}
END_TEST

// Тесты для strchr
START_TEST(test_strchr) {
  char str[] = "Hello, world!";
  ck_assert_ptr_eq(s21_strchr(str, 'w'), strchr(str, 'w'));
  ck_assert_ptr_eq(s21_strchr(str, 'o'), strchr(str, 'o'));
  ck_assert_ptr_eq(s21_strchr(str, 'z'), strchr(str, 'z'));
  ck_assert_ptr_eq(s21_strchr(str, '\0'), strchr(str, '\0'));
}
END_TEST

// Тесты для strncmp
START_TEST(test_strncmp) {
  ck_assert_int_eq(s21_strncmp("Hello", "Hello", 5),
                   strncmp("Hello", "Hello", 5));
  ck_assert_int_eq(s21_strncmp("Hello", "Hell", 5) > 0,
                   strncmp("Hello", "Hell", 5) > 0);
  ck_assert_int_eq(s21_strncmp("Hell", "Hello", 5) < 0,
                   strncmp("Hell", "Hello", 5) < 0);
  ck_assert_int_eq(s21_strncmp("Hello", "Hello!", 5),
                   strncmp("Hello", "Hello!", 5));
}
END_TEST

// Тесты для strncpy
START_TEST(test_strncpy) {
  char dest1[10] = "";
  char dest2[10] = "";

  // Тест 1: Полное копирование строки с нулевым символом
  s21_strncpy(dest1, "Hello", 6);
  strncpy(dest2, "Hello", 6);
  ck_assert_str_eq(dest1, dest2);

  // Тест 2: Копирование с заполнением нулями
  memset(dest1, 'x', sizeof(dest1));
  memset(dest2, 'x', sizeof(dest2));
  dest1[9] = dest2[9] = '\0';

  s21_strncpy(dest1, "Hi", 5);
  strncpy(dest2, "Hi", 5);
  ck_assert_mem_eq(dest1, dest2, 5);

  // Тест 3: Копирование в буфер большего размера
  char large_dest1[20] = "";
  char large_dest2[20] = "";
  const char *test_str = "Test";
  size_t test_len = strlen(test_str) + 1;

  s21_strncpy(large_dest1, test_str, test_len);
  strncpy(large_dest2, test_str, test_len);
  ck_assert_str_eq(large_dest1, large_dest2);
}
END_TEST

// Тесты для strcspn
START_TEST(test_strcspn) {
  ck_assert_int_eq(s21_strcspn("Hello", "o"), strcspn("Hello", "o"));
  ck_assert_int_eq(s21_strcspn("Hello", "l"), strcspn("Hello", "l"));
  ck_assert_int_eq(s21_strcspn("Hello", "z"), strcspn("Hello", "z"));
  ck_assert_int_eq(s21_strcspn("Hello", ""), strcspn("Hello", ""));
}
END_TEST

// Тесты для strpbrk
START_TEST(test_strpbrk) {
  ck_assert_ptr_eq(s21_strpbrk("Hello", "o"), strpbrk("Hello", "o"));
  ck_assert_ptr_eq(s21_strpbrk("Hello", "l"), strpbrk("Hello", "l"));
  ck_assert_ptr_eq(s21_strpbrk("Hello", "z"), strpbrk("Hello", "z"));
  ck_assert_ptr_eq(s21_strpbrk("Hello", ""), strpbrk("Hello", ""));
}
END_TEST

// Тесты для strrchr
START_TEST(test_strrchr) {
  char str[] = "Hello, world!";
  ck_assert_ptr_eq(s21_strrchr(str, 'l'), strrchr(str, 'l'));
  ck_assert_ptr_eq(s21_strrchr(str, 'o'), strrchr(str, 'o'));
  ck_assert_ptr_eq(s21_strrchr(str, 'z'), strrchr(str, 'z'));
  ck_assert_ptr_eq(s21_strrchr(str, '\0'), strrchr(str, '\0'));
}
END_TEST

// Тесты для strstr
START_TEST(test_strstr) {
  ck_assert_ptr_eq(s21_strstr("Hello, world!", "world"),
                   strstr("Hello, world!", "world"));
  ck_assert_ptr_eq(s21_strstr("Hello, world!", "Hello"),
                   strstr("Hello, world!", "Hello"));
  ck_assert_ptr_eq(s21_strstr("Hello, world!", "xyz"),
                   strstr("Hello, world!", "xyz"));
  ck_assert_ptr_eq(s21_strstr("Hello, world!", ""),
                   strstr("Hello, world!", ""));
}
END_TEST

// Тесты для strtok
START_TEST(test_strtok) {
  char str1[] = "Hello,world;test";
  char str2[] = "Hello,world;test";
  char delim[] = ",;";

  char *token1 = s21_strtok(str1, delim);
  char *token2 = strtok(str2, delim);

  while (token1 != s21_NULL && token2 != NULL) {
    ck_assert_str_eq(token1, token2);
    token1 = s21_strtok(NULL, delim);
    token2 = strtok(NULL, delim);
  }
  ck_assert_ptr_eq(token1, token2);
}
END_TEST

// Тесты для s21_to_upper
START_TEST(test_s21_to_upper) {
  char *result = s21_to_upper("hello");
  ck_assert_str_eq(result, "HELLO");
  free(result);

  result = s21_to_upper("Hello123");
  ck_assert_str_eq(result, "HELLO123");
  free(result);

  result = s21_to_upper("");
  ck_assert_str_eq(result, "");
  free(result);

  ck_assert_ptr_eq(s21_to_upper(NULL), NULL);
}
END_TEST

// Тесты для s21_to_lower
START_TEST(test_s21_to_lower) {
  char *result = s21_to_lower("HELLO");
  ck_assert_str_eq(result, "hello");
  free(result);

  result = s21_to_lower("HeLLo123");
  ck_assert_str_eq(result, "hello123");
  free(result);

  result = s21_to_lower("");
  ck_assert_str_eq(result, "");
  free(result);

  ck_assert_ptr_eq(s21_to_lower(NULL), NULL);
}
END_TEST

// Тесты для s21_insert
START_TEST(test_s21_insert) {
  char *result = s21_insert("Hello!", "123 ", 5);
  ck_assert_str_eq(result, "Hello123 !");
  free(result);

  result = s21_insert("", "123", 0);
  ck_assert_str_eq(result, "123");
  free(result);

  ck_assert_ptr_eq(s21_insert(NULL, "123", 0), NULL);
  ck_assert_ptr_eq(s21_insert("Hello", NULL, 0), NULL);
  ck_assert_ptr_eq(s21_insert("Hello", "123", 10), NULL);
}
END_TEST

// Тесты для s21_trim
START_TEST(test_s21_trim) {
  char *result = s21_trim("   Hello!   ", " ");
  ck_assert_str_eq(result, "Hello!");
  free(result);

  result = s21_trim("***Hello!***", "*");
  ck_assert_str_eq(result, "Hello!");
  free(result);

  result = s21_trim("", " ");
  ck_assert_str_eq(result, "");
  free(result);

  ck_assert_ptr_eq(s21_trim(NULL, " "), NULL);
  ck_assert_ptr_eq(s21_trim("Hello", NULL), NULL);
}
END_TEST

// sprintf

START_TEST(test_s21_sprintf) {
  char buffer1[1024];
  char buffer2[1024];

  int num = 42;
  s21_sprintf(buffer1, "%d", num);
  sprintf(buffer2, "%d", num);
  ck_assert_str_eq(buffer1, buffer2);

  char *text = "Test";
  s21_sprintf(buffer1, "%s", text);
  sprintf(buffer2, "%s", text);
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%c", 'A');
  sprintf(buffer2, "%c", 'A');
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%%");
  sprintf(buffer2, "%%");
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%d, %s, %c, %u", 100, "hello", 'B', 12345u);
  sprintf(buffer2, "%d, %s, %c, %u", 100, "hello", 'B', 12345u);
  ck_assert_str_eq(buffer1, buffer2);

  double pi = 3.14159;
  s21_sprintf(buffer1, "%f", pi);
  sprintf(buffer2, "%f", pi);
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%f", 0.0);
  sprintf(buffer2, "%f", 0.0);
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%f", -2.71828);
  sprintf(buffer2, "%f", -2.71828);
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%+5d", 42);
  sprintf(buffer2, "%+5d", 42);
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "% d", 42);
  sprintf(buffer2, "% d", 42);
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%-5d", 42);
  sprintf(buffer2, "%-5d", 42);
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%10s", "test");
  sprintf(buffer2, "%10s", "test");
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%10u", 123U);
  sprintf(buffer2, "%10u", 123U);

  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%+10d", -100);
  sprintf(buffer2, "%+10d", -100);
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%5s", "");
  sprintf(buffer2, "%5s", "");
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%.3f", 3.14159);
  sprintf(buffer2, "%.3f", 3.14159);
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%.5s", "Hello, World!");
  sprintf(buffer2, "%.5s", "Hello, World!");
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%hd", (short)32767);
  sprintf(buffer2, "%hd", (short)32767);
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%ld", 2147483647L);
  sprintf(buffer2, "%ld", 2147483647L);
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%-10s", "test");
  sprintf(buffer2, "%-10s", "test");
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%5c", 'A');
  sprintf(buffer2, "%5c", 'A');
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%hu", (unsigned short)65535);
  sprintf(buffer2, "%hu", (unsigned short)65535);
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%10f", 3.14);
  sprintf(buffer2, "%10f", 3.14);
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%-5d", 42);
  sprintf(buffer2, "%-5d", 42);
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%-10s", "test");
  sprintf(buffer2, "%-10s", "test");
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%-3c", 'A');
  sprintf(buffer2, "%-3c", 'A');
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%-8u", 12345U);
  sprintf(buffer2, "%-8u", 12345U);
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%-10f", 3.14);
  sprintf(buffer2, "%-10f", 3.14);
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%-5d", -42);
  sprintf(buffer2, "%-5d", -42);
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%-5s", "");
  sprintf(buffer2, "%-5s", "");
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%-3c", 'X');
  sprintf(buffer2, "%-3c", 'X');
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%-5u", 0U);
  sprintf(buffer2, "%-5u", 0U);

  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%-10f", -3.14);
  sprintf(buffer2, "%-10f", -3.14);
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%ld", 2147483647L);
  sprintf(buffer2, "%ld", 2147483647L);
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%lu", 4294967295UL);
  sprintf(buffer2, "%lu", 4294967295UL);
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%ld", -2147483647L);
  sprintf(buffer2, "%ld", -2147483647L);
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%lu", 0UL);
  sprintf(buffer2, "%lu", 0UL);
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%ld", 9223372036854775807L);
  sprintf(buffer2, "%ld", 9223372036854775807L);
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%lu", 18446744073709551615UL);
  sprintf(buffer2, "%lu", 18446744073709551615UL);
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%g %G", 0.000123, 12345.6789);
  sprintf(buffer2, "%g %G", 0.000123, 12345.6789);
  ck_assert(compare_float_strings(buffer1, buffer2, 6));

  s21_sprintf(buffer1, "%e, %E", 12345.6789, 98765.4321);
  sprintf(buffer2, "%e, %E", 12345.6789, 98765.4321);
  ck_assert(compare_float_strings(buffer1, buffer2, 6));

  s21_sprintf(buffer1, "%x, %X", 255, 255);
  sprintf(buffer2, "%x, %X", 255, 255);
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%o", 255);
  sprintf(buffer2, "%o", 255);
  ck_assert_str_eq(buffer1, buffer2);

  void *ptr = (void *)0x12345678;
  s21_sprintf(buffer1, "%p", ptr);
  sprintf(buffer2, "%p", ptr);
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%#x, %#X, %#o", 255, 255, 255);
  sprintf(buffer2, "%#x, %#X, %#o", 255, 255, 255);
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%05d, %05x", 123, 255);
  sprintf(buffer2, "%05d, %05x", 123, 255);
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%*d, %*s", 10, 123, 15, "test");
  sprintf(buffer2, "%*d, %*s", 10, 123, 15, "test");
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%.*f, %.*s", 2, 3.14159, 3, "test");
  sprintf(buffer2, "%.*f, %.*s", 2, 3.14159, 3, "test");
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%Lf", 1234567890.123456789L);
  sprintf(buffer2, "%Lf", 1234567890.123456789L);
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%07d", 123);
  sprintf(buffer2, "%07d", 123);
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%10d", 1);
  sprintf(buffer2, "%10d", 1);
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%-10d", 1);
  sprintf(buffer2, "%-10d", 1);
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%5hd", (short)-123);
  sprintf(buffer2, "%5hd", (short)-123);
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%10ld", (long)999999999);
  sprintf(buffer2, "%10ld", (long)999999999);
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%010u", 123u);
  sprintf(buffer2, "%010u", 123u);
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%-10.2f", 12.34);
  sprintf(buffer2, "%-10.2f", 12.34);
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%0d", 77);
  sprintf(buffer2, "%0d", 77);
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%6hu", (unsigned short)123u);
  sprintf(buffer2, "%6hu", (unsigned short)123u);
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%8s", "Hi");
  sprintf(buffer2, "%8s", "Hi");
  ck_assert_str_eq(buffer1, buffer2);

  long long big_num = 9223372036854775806LL;
  s21_sprintf(buffer1, "%lld", big_num);
  sprintf(buffer2, "%lld", big_num);
  ck_assert_str_eq(buffer1, buffer2);

  unsigned long long big_unum = 18446744073709551614ULL;
  s21_sprintf(buffer1, "%llu", big_unum);
  sprintf(buffer2, "%llu", big_unum);
  ck_assert_str_eq(buffer1, buffer2);

  unsigned short us_val = 1234;
  s21_sprintf(buffer1, "%hu", us_val);
  sprintf(buffer2, "%hu", us_val);
  ck_assert_str_eq(buffer1, buffer2);

  s21_sprintf(buffer1, "%ho", (unsigned short)511);
  sprintf(buffer2, "%ho", (unsigned short)511);
  ck_assert_str_eq(buffer1, buffer2);

  unsigned long ul_val = 07777;
  s21_sprintf(buffer1, "%lo", ul_val);
  sprintf(buffer2, "%lo", ul_val);
  ck_assert_str_eq(buffer1, buffer2);
}
END_TEST
////

// memchr
//  Тест 0:
START_TEST(test_s21_memchr) {
  const char *str = "Hello, World!";
  ck_assert_ptr_eq(s21_memchr(str, 'W', 13), memchr(str, 'W', 13));
  ck_assert_ptr_eq(s21_memchr(str, 'z', 13), memchr(str, 'z', 13));
}
END_TEST

// Тест 1: Проверка поиска символа в начале строки
START_TEST(test_s21_memchr_start) {
  const char *str = "Hello, World!";
  ck_assert_ptr_eq(s21_memchr(str, 'H', 13), memchr(str, 'H', 13));
}
END_TEST

// Тест 2: Проверка поиска символа в конце строки
START_TEST(test_s21_memchr_end) {
  const char *str = "Hello, World!";
  ck_assert_ptr_eq(s21_memchr(str, '!', 13), memchr(str, '!', 13));
}
END_TEST

// Тест 3: Проверка, что символ не найден в строке
START_TEST(test_s21_memchr_not_found) {
  const char *str = "Hello, World!";
  ck_assert_ptr_eq(s21_memchr(str, 'z', 13), memchr(str, 'z', 13));
}
END_TEST

// Тест 4: Проверка поиска при размере 0
START_TEST(test_s21_memchr_zero_size) {
  const char *str = "Hello, World!";
  ck_assert_ptr_eq(s21_memchr(str, 'H', 0), memchr(str, 'H', 0));
}
END_TEST

// Тест 5: Проверка поиска первого вхождения символа, когда он повторяется
START_TEST(test_s21_memchr_multiple_occurrences) {
  const char *str = "Hello, Hello!";
  ck_assert_ptr_eq(s21_memchr(str, 'H', 13), memchr(str, 'H', 13));
}
END_TEST

// Тест 6: Проверка поиска с ограничением длины
START_TEST(test_s21_memchr_length_limit) {
  const char *str = "Hello, World!";
  ck_assert_ptr_eq(s21_memchr(str, 'o', 5), memchr(str, 'o', 5));
  ck_assert_ptr_eq(s21_memchr(str, 'o', 4), memchr(str, 'o', 4));
}
END_TEST

// Тест 7: Проверка поиска в пустой строке
START_TEST(test_s21_memchr_empty_string) {
  const char *str = "";
  ck_assert_ptr_eq(s21_memchr(str, 'H', 0), memchr(str, 'H', 0));
}
END_TEST

// Тест 8: Проверка поиска символа в строке с пробелами
START_TEST(test_s21_memchr_with_spaces) {
  const char *str = "Hello, World!";
  ck_assert_ptr_eq(s21_memchr(str, ' ', 13), memchr(str, ' ', 13));
}
END_TEST

// memcmp
//  Тест 0:
START_TEST(test_s21_memcmp) {
  const char *str1 = "Hello, World!";
  const char *str2 = "Hello, World!";
  const char *str3 = "Goodbye, World!";
  // Проверка на равенство
  ck_assert_int_eq(s21_memcmp(str1, str2, 13) == 0,
                   memcmp(str1, str2, 13) == 0);
  // Проверка на НЕравенство
  ck_assert_int_eq(s21_memcmp(str1, str3, 13) > 0, memcmp(str1, str3, 13) > 0);
}
END_TEST
// Тест 1: Проверка на равенство строк
START_TEST(test_s21_memcmp_equal) {
  const char *str1 = "Hello, World!";
  const char *str2 = "Hello, World!";

  // Проверка на равенство
  ck_assert_int_eq(s21_memcmp(str1, str2, 13), memcmp(str1, str2, 13));
}
END_TEST

// Тест 2: Проверка на НЕравенство строк
START_TEST(test_s21_memcmp_unequal) {
  const char *str1 = "Hello, World!";
  const char *str3 = "Goodbye, World!";

  // Проверка на НЕравенство
  ck_assert_int_eq(s21_memcmp(str1, str3, 13), memcmp(str1, str3, 13));
}
END_TEST

// Тест 3: Проверка на равенство при нулевой длине
START_TEST(test_s21_memcmp_zero_length) {
  const char *str1 = "Hello, World!";
  const char *str2 = "Hello, World!";

  // Проверка на равенство при длине 0
  ck_assert_int_eq(s21_memcmp(str1, str2, 0), memcmp(str1, str2, 0));
}
END_TEST

// Тест 4: Проверка на НЕравенство при нулевой длине
START_TEST(test_s21_memcmp_zero_length_unequal) {
  const char *str1 = "Hello, World!";
  const char *str2 = "Goodbye, World!";

  // Проверка на НЕравенство при длине 0
  ck_assert_int_eq(s21_memcmp(str1, str2, 0), memcmp(str1, str2, 0));
}
END_TEST

// Тест 5: Проверка на частичное равенство строк
START_TEST(test_s21_memcmp_partial_equal) {
  const char *str1 = "Hello, World!";
  const char *str2 = "Hello, World!!";

  // Проверка частичного равенства
  ck_assert_int_eq(s21_memcmp(str1, str2, 13), memcmp(str1, str2, 13));
}
END_TEST

// Тест 6: Проверка на сравнение с символами, которые отличаются по одному
START_TEST(test_s21_memcmp_diff_by_one) {
  const char *str1 = "Hello, World!";
  const char *str2 = "Hello, World?";

  // Проверка на различие в одном символе
  ck_assert_int_eq(s21_memcmp(str1, str2, 13) < 0, memcmp(str1, str2, 13) < 0);
}
END_TEST

// memcpy
START_TEST(test_s21_memcpy) {
  const char src[] = "Hello, World!";
  char dest1[50], dest2[50];
  s21_memcpy(dest1, src, strlen(src) + 1);
  memcpy(dest2, src, strlen(src) + 1);
  ck_assert_str_eq(dest1, dest2);
}
END_TEST

START_TEST(test_s21_memcpy_basic) {
  const char src[] = "Basic test for memcpy.";
  char dest1[50], dest2[50];
  s21_memcpy(dest1, src, strlen(src) + 1);
  memcpy(dest2, src, strlen(src) + 1);
  ck_assert_str_eq(dest1, dest2);
}
END_TEST

START_TEST(test_s21_memcpy_empty) {
  const char src[] = "";
  char dest1[50], dest2[50];
  s21_memcpy(dest1, src, strlen(src) + 1);
  memcpy(dest2, src, strlen(src) + 1);
  ck_assert_str_eq(dest1, dest2);
}
END_TEST

START_TEST(test_s21_memcpy_partial) {
  const char src[] = "Partial copy test.";
  char dest1[50], dest2[50];
  s21_memcpy(dest1, src, 7);
  memcpy(dest2, src, 7);
  ck_assert_int_eq(memcmp(dest1, dest2, 7), 0);
}
END_TEST

START_TEST(test_s21_memcpy_no_overlap) {
  char buffer1[50] = "Non-overlapping memory test.";
  char buffer2[50] = "Non-overlapping memory test.";
  s21_memcpy(buffer1 + 15, buffer1, 10);
  memcpy(buffer2 + 15, buffer2, 10);
  ck_assert_str_eq(buffer1, buffer2);
}
END_TEST

START_TEST(test_s21_memcpy_binary) {
  const unsigned char src[] = {0xDE, 0xAD, 0xBE, 0xEF, 0x00};
  unsigned char dest1[10], dest2[10];
  s21_memcpy(dest1, src, sizeof(src));
  memcpy(dest2, src, sizeof(src));
  ck_assert_int_eq(memcmp(dest1, dest2, sizeof(src)), 0);
}
END_TEST
//

// s21memset
//  Тест 1: Проверка замены символов в строке
START_TEST(test_s21_memset_basic) {
  char str[10] = "abcdefghi";
  s21_memset(str, 'X', 5);

  ck_assert_str_eq(str, "XXXXXfghi");
}
END_TEST

// Тест 2: Проверка, когда размер заполнения равен 0
START_TEST(test_s21_memset_zero) {
  char str[10] = "abcdefghi";
  s21_memset(str, 'X', 0);

  ck_assert_str_eq(str, "abcdefghi");
}
END_TEST

// Тест 3: Проверка замены всей строки
START_TEST(test_s21_memset_full) {
  char str[10] = "abcdefghi";
  s21_memset(str, 'X', 9);

  ck_assert_str_eq(str, "XXXXXXXXX");
}
END_TEST

// Тест 4: Проверка на пустую строку
START_TEST(test_s21_memset_empty) {
  char str[10] = "";
  s21_memset(str, 'X', 0);

  ck_assert_str_eq(str, "");
}
END_TEST

// Тест 5: Проверка на заполнение частично
START_TEST(test_s21_memset_partial) {
  char str[10] = "abcdefghi";
  s21_memset(str + 3, 'X', 3);

  ck_assert_str_eq(str, "abcXXXghi");
}
END_TEST
//

// strncat
START_TEST(test_s21_strncat) {
  char dest1[50] = "Hello, ";
  char dest2[50] = "Hello, ";
  const char *src = "World!";
  s21_strncat(dest1, src, 3);
  strncat(dest2, src, 3);
  ck_assert_str_eq(dest1, dest2);
}
END_TEST
START_TEST(test_s21_strncat_basic) {
  char dest1[50] = "Hello, ";
  char dest2[50] = "Hello, ";
  const char *src = "World!";
  s21_strncat(dest1, src, 5);
  strncat(dest2, src, 5);
  ck_assert_str_eq(dest1, dest2);
}
END_TEST

START_TEST(test_s21_strncat_full_length) {
  char dest1[50] = "Hello, ";
  char dest2[50] = "Hello, ";
  const char *src = "World!";
  s21_strncat(dest1, src, 6);
  strncat(dest2, src, 6);
  ck_assert_str_eq(dest1, dest2);
}
END_TEST

START_TEST(test_s21_strncat_empty_src) {
  char dest1[50] = "Hello, ";
  char dest2[50] = "Hello, ";
  const char *src = "";
  s21_strncat(dest1, src, 3);
  strncat(dest2, src, 3);
  ck_assert_str_eq(dest1, dest2);
}
END_TEST

START_TEST(test_s21_strncat_empty_dest) {
  char dest1[50] = "";
  char dest2[50] = "";
  const char *src = "World!";
  s21_strncat(dest1, src, 3);
  strncat(dest2, src, 3);
  ck_assert_str_eq(dest1, dest2);
}
END_TEST

START_TEST(test_s21_strncat_zero_length) {
  char dest1[50] = "Hello, ";
  char dest2[50] = "Hello, ";
  const char *src = "World!";
  s21_strncat(dest1, src, 0);
  strncat(dest2, src, 0);
  ck_assert_str_eq(dest1, dest2);
}
END_TEST

START_TEST(test_s21_strncat_large_length) {
  char dest1[51] = "Hello, ";
  char dest2[51] = "Hello, ";
  const char *src = "World!";
  s21_strncat(dest1, src, 50);
  strncat(dest2, src, 50);
  ck_assert_str_eq(dest1, dest2);
}
END_TEST

START_TEST(test_s21_strncat_overlap) {
  char dest1[50] = "Overlap";
  char dest2[50] = "Overlap";
  char src1[50] = "Overlap";
  char src2[50] = "Overlap";
  s21_strncat(dest1 + 3, src1, 4);
  strncat(dest2 + 3, src2, 4);
  ck_assert_str_eq(dest1, dest2);
}
END_TEST
//

// strchr
START_TEST(test_s21_strchr_found) {
  const char *str = "Hello, World!";
  ck_assert_ptr_eq(s21_strchr(str, 'W'), strchr(str, 'W'));
}
END_TEST

START_TEST(test_s21_strchr_not_found) {
  const char *str = "Hello, World!";
  ck_assert_ptr_eq(s21_strchr(str, 'z'), strchr(str, 'z'));
}
END_TEST

START_TEST(test_s21_strchr_null_terminator) {
  const char *str = "Hello, World!";
  ck_assert_ptr_eq(s21_strchr(str, '\0'), strchr(str, '\0'));
}
END_TEST

START_TEST(test_s21_strchr_first_character) {
  const char *str = "Hello, World!";
  ck_assert_ptr_eq(s21_strchr(str, 'H'), strchr(str, 'H'));
}
END_TEST

START_TEST(test_s21_strchr_last_character) {
  const char *str = "Hello, World!";
  ck_assert_ptr_eq(s21_strchr(str, '!'), strchr(str, '!'));
}
END_TEST
//

// strncmp
START_TEST(test_s21_strncmp) {
  const char *str1 = "Hello";
  const char *str2 = "Hello";
  const char *str3 = "World";
  ck_assert_int_eq(s21_strncmp(str1, str2, 3), strncmp(str1, str2, 3));
  ck_assert_int_eq(s21_strncmp(str1, str3, 3) < 0, strncmp(str1, str3, 3) < 0);
  // ck_assert_int_ne(s21_strncmp(str1, str3,3), strncmp(str1, str3,3));
}
END_TEST

// Тесты для функции s21_strncmp
START_TEST(test_s21_strncmp_equal) {
  const char *str1 = "Hello, World!";
  const char *str2 = "Hello, World!";
  ck_assert_int_eq(s21_strncmp(str1, str2, 13), strncmp(str1, str2, 13));
}
END_TEST

START_TEST(test_s21_strncmp_not_equal) {
  const char *str1 = "Hello, World!";
  const char *str2 = "Hello, Universe!";
  ck_assert_int_eq(s21_strncmp(str1, str2, 13) > 0,
                   strncmp(str1, str2, 13) > 0);
}
END_TEST

START_TEST(test_s21_strncmp_partial_compare) {
  const char *str1 = "Hello, World!";
  const char *str2 = "Hello, Universe!";
  ck_assert_int_eq(s21_strncmp(str1, str2, 7), strncmp(str1, str2, 7));
}
END_TEST

START_TEST(test_s21_strncmp_empty_strings) {
  const char *str1 = "";
  const char *str2 = "";
  ck_assert_int_eq(s21_strncmp(str1, str2, 5), strncmp(str1, str2, 5));
}
END_TEST

START_TEST(test_s21_strncmp_one_empty) {
  const char *str1 = "Hello, World!";
  const char *str2 = "";
  ck_assert_int_eq(s21_strncmp(str1, str2, 5) > 0, strncmp(str1, str2, 5) > 0);
}
END_TEST

START_TEST(test_s21_strncmp_null_terminator) {
  const char *str1 = "Hello\0World";
  const char *str2 = "Hello\0Universe";
  ck_assert_int_eq(s21_strncmp(str1, str2, 11), strncmp(str1, str2, 11));
}
END_TEST
//

// strncpy
START_TEST(test_s21_strncpy) {
  const char *src = "Hello, World!";
  char dest1[50] = {0}, dest2[50] = {0};

  // Копируем первые 5 символов
  s21_strncpy(dest1, src, 5);
  strncpy(dest2, src, 5);
  ck_assert_str_eq(dest1, dest2);

  // Проверяем копирование с размером больше длины строки
  s21_strncpy(dest1, src, 50);
  strncpy(dest2, src, 50);
  ck_assert_str_eq(dest1, dest2);

  // Проверяем копирование пустой строки
  const char *empty_src = "";
  s21_strncpy(dest1, empty_src, 10);
  strncpy(dest2, empty_src, 10);
  ck_assert_str_eq(dest1, dest2);

  // Проверяем копирование строки с символами '\0' внутри
  const char *src_with_null = "Test\0String";
  s21_strncpy(dest1, src_with_null, 10);
  strncpy(dest2, src_with_null, 10);
  ck_assert_mem_eq(
      dest1, dest2,
      10);  // Используем ck_assert_mem_eq для проверки массива побайтово
}
END_TEST
//

// s21_strcspn
START_TEST(test_s21_strcspn_basic) {
  const char *str = "Hello, World!";
  const char *reject = "lo";
  ck_assert_int_eq(s21_strcspn(str, reject), strcspn(str, reject));
}
END_TEST

START_TEST(test_s21_strcspn_no_match) {
  const char *str = "Hello, World!";
  const char *reject = "xyz";
  ck_assert_int_eq(s21_strcspn(str, reject), strcspn(str, reject));
}
END_TEST

START_TEST(test_s21_strcspn_all_match) {
  const char *str = "abc";
  const char *reject = "abc";
  ck_assert_int_eq(s21_strcspn(str, reject), strcspn(str, reject));
}
END_TEST

START_TEST(test_s21_strcspn_empty_str) {
  const char *str = "";
  const char *reject = "abc";
  ck_assert_int_eq(s21_strcspn(str, reject), strcspn(str, reject));
}
END_TEST

START_TEST(test_s21_strcspn_empty_reject) {
  const char *str = "Hello, World!";
  const char *reject = "";
  ck_assert_int_eq(s21_strcspn(str, reject), strcspn(str, reject));
}
END_TEST

START_TEST(test_s21_strcspn_null_terminator) {
  const char *str = "Hello\0World";
  const char *reject = "lo";
  ck_assert_int_eq(s21_strcspn(str, reject), strcspn(str, reject));
}
END_TEST

START_TEST(test_s21_strcspn_special_chars) {
  const char *str = "Hello, World!";
  const char *reject = "!@#$%^&*";
  ck_assert_int_eq(s21_strcspn(str, reject), strcspn(str, reject));
}
END_TEST

START_TEST(test_s21_strcspn_partial_match) {
  const char *str = "abcdefg";
  const char *reject = "fg";
  ck_assert_int_eq(s21_strcspn(str, reject), strcspn(str, reject));
}
END_TEST
//

// s21_strerror
START_TEST(test_s21_strerror) {
  ck_assert_str_eq(s21_strerror(0), strerror(0));
}
END_TEST
//

// strlen
START_TEST(test_s21_strlen) {
  const char *str = "Hello, World!";
  ck_assert_int_eq(s21_strlen(str), strlen(str));
}
END_TEST
START_TEST(test_s21_strlen_basic) {
  const char *str = "Hello, world!";
  ck_assert_int_eq(s21_strlen(str), strlen(str));
}
END_TEST

START_TEST(test_s21_strlen_empty_string) {
  const char *str = "";
  ck_assert_int_eq(s21_strlen(str), strlen(str));
}
END_TEST

START_TEST(test_s21_strlen_single_char) {
  const char *str = "a";
  ck_assert_int_eq(s21_strlen(str), strlen(str));
}
END_TEST

START_TEST(test_s21_strlen_whitespace) {
  const char *str = "   ";  // String with spaces
  ck_assert_int_eq(s21_strlen(str), strlen(str));
}
END_TEST

START_TEST(test_s21_strlen_special_characters) {
  const char *str = "!@#$%^&*()";
  ck_assert_int_eq(s21_strlen(str), strlen(str));
}
END_TEST
//

// s21_strpbrk
START_TEST(test_s21_strpbrk_basic_match) {
  const char *str = "abcdefg";
  const char *accept = "fg";
  ck_assert_str_eq(s21_strpbrk(str, accept), strpbrk(str, accept));
}
END_TEST

START_TEST(test_s21_strpbrk_no_match) {
  const char *str = "abcdefg";
  const char *accept = "xyz";
  ck_assert_ptr_eq(s21_strpbrk(str, accept), strpbrk(str, accept));
}
END_TEST

START_TEST(test_s21_strpbrk_multiple_matches) {
  const char *str = "abcdefg";
  const char *accept = "ac";
  ck_assert_str_eq(s21_strpbrk(str, accept), strpbrk(str, accept));
}
END_TEST

START_TEST(test_s21_strpbrk_empty_accept) {
  const char *str = "abcdefg";
  const char *accept = "";
  ck_assert_ptr_eq(s21_strpbrk(str, accept), strpbrk(str, accept));
}
END_TEST

START_TEST(test_s21_strpbrk_empty_str) {
  const char *str = "";
  const char *accept = "abc";
  ck_assert_ptr_eq(s21_strpbrk(str, accept), strpbrk(str, accept));
}
END_TEST
//

// s21_strrchr
START_TEST(test_s21_strrchr_basic_match) {
  const char *str = "abcdefg";
  int ch = 'd';
  ck_assert_str_eq(s21_strrchr(str, ch), strrchr(str, ch));
}
END_TEST

START_TEST(test_s21_strrchr_no_match) {
  const char *str = "abcdefg";
  int ch = 'z';
  ck_assert_ptr_eq(s21_strrchr(str, ch), strrchr(str, ch));
}
END_TEST

START_TEST(test_s21_strrchr_multiple_matches) {
  const char *str = "abcdefgabc";
  int ch = 'a';
  ck_assert_str_eq(s21_strrchr(str, ch), strrchr(str, ch));
}
END_TEST

START_TEST(test_s21_strrchr_first_char) {
  const char *str = "abcdefg";
  int ch = 'a';
  ck_assert_str_eq(s21_strrchr(str, ch), strrchr(str, ch));
}
END_TEST

START_TEST(test_s21_strrchr_last_char) {
  const char *str = "abcdefg";
  int ch = 'g';
  ck_assert_str_eq(s21_strrchr(str, ch), strrchr(str, ch));
}
END_TEST

START_TEST(test_s21_strrchr_empty_str) {
  const char *str = "";
  int ch = 'a';
  ck_assert_ptr_eq(s21_strrchr(str, ch), strrchr(str, ch));
}
END_TEST
//

// s21_strstr
START_TEST(test_s21_strstr_basic_match) {
  const char *haystack = "abcdefg";
  const char *needle = "cde";
  ck_assert_str_eq(s21_strstr(haystack, needle), strstr(haystack, needle));
}
END_TEST

START_TEST(test_s21_strstr_no_match) {
  const char *haystack = "abcdefg";
  const char *needle = "xyz";
  ck_assert_ptr_eq(s21_strstr(haystack, needle), strstr(haystack, needle));
}
END_TEST

START_TEST(test_s21_strstr_at_start) {
  const char *haystack = "abcdefg";
  const char *needle = "abc";
  ck_assert_str_eq(s21_strstr(haystack, needle), strstr(haystack, needle));
}
END_TEST

START_TEST(test_s21_strstr_at_end) {
  const char *haystack = "abcdefg";
  const char *needle = "efg";
  ck_assert_str_eq(s21_strstr(haystack, needle), strstr(haystack, needle));
}
END_TEST

START_TEST(test_s21_strstr_empty_needle) {
  const char *haystack = "abcdefg";
  const char *needle = "";
  ck_assert_str_eq(s21_strstr(haystack, needle), strstr(haystack, needle));
}
END_TEST

START_TEST(test_s21_strstr_empty_haystack) {
  const char *haystack = "";
  const char *needle = "abc";
  ck_assert_ptr_eq(s21_strstr(haystack, needle), strstr(haystack, needle));
}
END_TEST
//

// s21_strtok
START_TEST(test_s21_strtok_basic) {
  char str[] = "Hello, World! How are you?";
  char *delim = " ,!?";
  char *token = s21_strtok(str, delim);

  ck_assert_str_eq(token, "Hello");
  token = s21_strtok(NULL, delim);
  ck_assert_str_eq(token, "World");
  token = s21_strtok(NULL, delim);
  ck_assert_str_eq(token, "How");
  token = s21_strtok(NULL, delim);
  ck_assert_str_eq(token, "are");
  token = s21_strtok(NULL, delim);
  ck_assert_str_eq(token, "you");
  token = s21_strtok(NULL, delim);
  ck_assert_ptr_eq(token, NULL);
}
END_TEST

///////////////////////
START_TEST(test_sscanf_int_decimal) {
  int val_s21, val_std;
  s21_sscanf("123", "%d", &val_s21);
  sscanf("123", "%d", &val_std);
  ck_assert_int_eq(val_s21, val_std);

  s21_sscanf("   -456", "%d", &val_s21);
  sscanf("   -456", "%d", &val_std);
  ck_assert_int_eq(val_s21, val_std);
}
END_TEST

START_TEST(test_sscanf_int_with_width) {
  int val_s21, val_std;
  s21_sscanf("12345", "%2d", &val_s21);
  sscanf("12345", "%2d", &val_std);
  ck_assert_int_eq(val_s21, val_std);

  s21_sscanf("0001", "%3d", &val_s21);
  sscanf("0001", "%3d", &val_std);
  ck_assert_int_eq(val_s21, val_std);
}
END_TEST

START_TEST(test_sscanf_int_star) {
  int val_s21 = 0, val_std = 0;
  int res_s21 = s21_sscanf("789", "%*d%d", &val_s21);
  int res_std = sscanf("789", "%*d%d", &val_std);
  ck_assert_int_eq(res_s21, res_std);
  ck_assert_int_eq(val_s21, val_std);
}
END_TEST

START_TEST(test_sscanf_int_octal) {
  unsigned val_s21, val_std;
  s21_sscanf("0123", "%o", &val_s21);
  sscanf("0123", "%o", &val_std);
  ck_assert_uint_eq(val_s21, val_std);

  s21_sscanf("777", "%o", &val_s21);
  sscanf("777", "%o", &val_std);
  ck_assert_uint_eq(val_s21, val_std);
}
END_TEST

START_TEST(test_sscanf_int_hex) {
  unsigned val_s21, val_std;
  s21_sscanf("0x1f", "%x", &val_s21);
  sscanf("0x1f", "%x", &val_std);
  ck_assert_uint_eq(val_s21, val_std);

  s21_sscanf("FF", "%X", &val_s21);
  sscanf("FF", "%X", &val_std);
  ck_assert_uint_eq(val_s21, val_std);
}
END_TEST

START_TEST(test_sscanf_int_i) {
  int val_s21, val_std;
  s21_sscanf("010", "%i", &val_s21);
  sscanf("010", "%i", &val_std);
  ck_assert_int_eq(val_s21, val_std);

  s21_sscanf("0x10", "%i", &val_s21);
  sscanf("0x10", "%i", &val_std);
  ck_assert_int_eq(val_s21, val_std);

  s21_sscanf("15", "%i", &val_s21);
  sscanf("15", "%i", &val_std);
  ck_assert_int_eq(val_s21, val_std);
}
END_TEST

START_TEST(test_sscanf_unsigned) {
  unsigned val_s21, val_std;
  s21_sscanf("99999999", "%u", &val_s21);
  sscanf("99999999", "%u", &val_std);
  ck_assert_uint_eq(val_s21, val_std);
}
END_TEST

START_TEST(test_sscanf_float) {
  float val_s21f, val_stdf;
  s21_sscanf("  +123.456", "%f", &val_s21f);
  sscanf("  +123.456", "%f", &val_stdf);
  ck_assert(double_equal(val_s21f, val_stdf, 1e-6));

  double val_s21d, val_stdd;
  s21_sscanf("-12.34e2", "%lf", &val_s21d);
  sscanf("-12.34e2", "%lf", &val_stdd);
  ck_assert(double_equal(val_s21d, val_stdd, 1e-6));

  long double val_s21ld, val_stdld;
  s21_sscanf("3.14159E0", "%Lf", &val_s21ld);
  sscanf("3.14159E0", "%Lf", &val_stdld);
  ck_assert(double_equal((double)val_s21ld, (double)val_stdld, 1e-12));
}
END_TEST

START_TEST(test_sscanf_float_gG) {
  double val_s21, val_std;
  s21_sscanf("1.23e-4", "%lg", &val_s21);
  sscanf("1.23e-4", "%lg", &val_std);
  ck_assert(double_equal(val_s21, val_std, 1e-7));

  s21_sscanf("1.23E+4", "%lG", &val_s21);
  sscanf("1.23E+4", "%lG", &val_std);
  ck_assert(double_equal(val_s21, val_std, 1e-7));
}
END_TEST

START_TEST(test_sscanf_float_width) {
  double val_s21, val_std;
  s21_sscanf("123.4567", "%5lf", &val_s21);
  sscanf("123.4567", "%5lf", &val_std);
  ck_assert(double_equal(val_s21, val_std, 1e-6));
}
END_TEST

START_TEST(test_sscanf_string) {
  char str_s21[4096], str_std[4096];
  s21_sscanf("hello world", "%s", str_s21);
  sscanf("hello world", "%s", str_std);
  ck_assert_str_eq(str_s21, str_std);

  s21_sscanf("   trimmed", "%s", str_s21);
  sscanf("   trimmed", "%s", str_std);
  ck_assert_str_eq(str_s21, str_std);

  s21_sscanf("abcdefg", "%3s", str_s21);
  sscanf("abcdefg", "%3s", str_std);
  ck_assert_str_eq(str_s21, str_std);

  char dummy[4096];
  int res_s21 = s21_sscanf("test string", "%*s%s", str_s21);
  int res_std = sscanf("test string", "%*s%s", dummy);
  ck_assert_int_eq(res_s21, res_std);
  ck_assert_str_eq(str_s21, dummy);
}
END_TEST

START_TEST(test_sscanf_char) {
  char c_s21, c_std;
  s21_sscanf("X", "%c", &c_s21);
  sscanf("X", "%c", &c_std);
  ck_assert_int_eq(c_s21, c_std);

  char buff_s21[10], buff_std[10];
  s21_sscanf("Hello", "%3c", buff_s21);
  sscanf("Hello", "%3c", buff_std);
  ck_assert_int_eq(strncmp(buff_s21, buff_std, 3), 0);
}
END_TEST

START_TEST(test_sscanf_percent) {
  int res_s21 = s21_sscanf("%", "%%");
  int res_std = sscanf("%", "%%");
  ck_assert_int_eq(res_s21, res_std);
}
END_TEST

START_TEST(test_sscanf_pointer) {
  void *ptr_s21, *ptr_std;
  s21_sscanf("0x7ffdecaf", "%p", &ptr_s21);
  sscanf("0x7ffdecaf", "%p", &ptr_std);
  ck_assert_ptr_eq(ptr_s21, ptr_std);
  s21_sscanf("1234abcd", "%p", &ptr_s21);
  sscanf("1234abcd", "%p", &ptr_std);
  ck_assert_ptr_eq(ptr_s21, ptr_std);
}
END_TEST

START_TEST(test_sscanf_length_modifiers) {
  short sh_s21;
  short sh_std;
  s21_sscanf("32767", "%hd", &sh_s21);
  sscanf("32767", "%hd", &sh_std);
  ck_assert_int_eq(sh_s21, sh_std);

  long l_s21;
  long l_std;
  s21_sscanf("1234567890", "%ld", &l_s21);
  sscanf("1234567890", "%ld", &l_std);
  ck_assert_int_eq(l_s21, l_std);

  long long ll_s21;
  long long ll_std;
  s21_sscanf("9223372036854775807", "%lld", &ll_s21);
  sscanf("9223372036854775807", "%lld", &ll_std);
  ck_assert(ll_s21 == ll_std);

  float f_s21;
  float f_std;
  s21_sscanf("3.14", "%f", &f_s21);
  sscanf("3.14", "%f", &f_std);
  ck_assert(double_equal(f_s21, f_std, 1e-6));

  double d_s21;
  double d_std;
  s21_sscanf("2.71828", "%lf", &d_s21);
  sscanf("2.71828", "%lf", &d_std);
  ck_assert(double_equal(d_s21, d_std, 1e-12));

  long double ld_s21, ld_std;
  s21_sscanf("1.4142135623", "%Lf", &ld_s21);
  sscanf("1.4142135623", "%Lf", &ld_std);
  ck_assert(double_equal((double)ld_s21, (double)ld_std, 1e-12));
}
END_TEST

START_TEST(test_sscanf_invalid) {
  int val_s21, val_std;
  int res_s21 = s21_sscanf("abc", "%d", &val_s21);
  int res_std = sscanf("abc", "%d", &val_std);
  ck_assert_int_eq(res_s21, res_std);

  double dv_s21, dv_std;
  res_s21 = s21_sscanf("e", "%lf", &dv_s21);
  res_std = sscanf("e", "%lf", &dv_std);
  ck_assert_int_eq(res_s21, res_std);

  res_s21 = s21_sscanf("abc", "%%");
  res_std = sscanf("abc", "%%");
  ck_assert_int_eq(res_s21, res_std);
}
END_TEST

START_TEST(test_sscanf_with_spaces_in_format) {
  int val_s21, val_std;
  s21_sscanf("   123", "%d", &val_s21);
  sscanf("   123", "%d", &val_std);
  ck_assert_int_eq(val_s21, val_std);

  s21_sscanf("   456", "   %d", &val_s21);
  sscanf("   456", "   %d", &val_std);
  ck_assert_int_eq(val_s21, val_std);
}
END_TEST

START_TEST(test_sscanf_matching_characters) {
  char str_s21[4096], str_std[4096];
  s21_sscanf("abc123", "abc%s", str_s21);
  sscanf("abc123", "abc%s", str_std);
  ck_assert_str_eq(str_s21, str_std);

  int val_s21, val_std;
  s21_sscanf("xyz456", "xyz%d", &val_s21);
  sscanf("xyz456", "xyz%d", &val_std);
  ck_assert_int_eq(val_s21, val_std);
}
END_TEST

// START_TEST(test_sscanf_format_ends_with_percent)
// {
//     int res_s21 = s21_sscanf("123", "%");
//     int res_std = sscanf("123", "%");
//     ck_assert_int_eq(res_s21, res_std);
// }
// END_TEST

START_TEST(test_sscanf_with_asterisk_modifier) {
  int val_s21, val_std;
  s21_sscanf("123 456", "%*d %d", &val_s21);
  sscanf("123 456", "%*d %d", &val_std);
  ck_assert_int_eq(val_s21, val_std);

  float f_s21, f_std;
  s21_sscanf("1.2 3.4", "%*f %f", &f_s21);
  sscanf("1.2 3.4", "%*f %f", &f_std);
  ck_assert(double_equal(f_s21, f_std, 1e-6));
}
END_TEST
//////////////////////////

Suite *s21_string_suite(void) {
  Suite *suite = suite_create("s21_string");

  TCase *core = tcase_create("Core");
  tcase_add_test(core, test_memchr);
  tcase_add_test(core, test_memcmp);
  tcase_add_test(core, test_memcpy);
  tcase_add_test(core, test_memset);
  tcase_add_test(core, test_strlen);
  tcase_add_test(core, test_strncat);
  tcase_add_test(core, test_strchr);
  tcase_add_test(core, test_strncmp);
  tcase_add_test(core, test_strncpy);
  tcase_add_test(core, test_strcspn);
  tcase_add_test(core, test_strpbrk);
  tcase_add_test(core, test_strrchr);
  tcase_add_test(core, test_strstr);
  tcase_add_test(core, test_strtok);
  tcase_add_test(core, test_s21_to_upper);
  tcase_add_test(core, test_s21_to_lower);
  tcase_add_test(core, test_s21_insert);
  tcase_add_test(core, test_s21_trim);

  // sprintf
  tcase_add_test(core, test_s21_sprintf);

  // // memchr
  tcase_add_test(core, test_s21_memchr);
  tcase_add_test(core, test_s21_memchr_start);
  tcase_add_test(core, test_s21_memchr_end);
  tcase_add_test(core, test_s21_memchr_not_found);
  tcase_add_test(core, test_s21_memchr_zero_size);
  tcase_add_test(core, test_s21_memchr_multiple_occurrences);
  tcase_add_test(core, test_s21_memchr_length_limit);
  tcase_add_test(core, test_s21_memchr_empty_string);
  tcase_add_test(core, test_s21_memchr_with_spaces);
  // // memcmp
  tcase_add_test(core, test_s21_memcmp);
  tcase_add_test(core, test_s21_memcmp_equal);
  tcase_add_test(core, test_s21_memcmp_unequal);
  tcase_add_test(core, test_s21_memcmp_zero_length);
  tcase_add_test(core, test_s21_memcmp_zero_length_unequal);
  tcase_add_test(core, test_s21_memcmp_partial_equal);
  tcase_add_test(core, test_s21_memcmp_diff_by_one);
  // memcpy
  tcase_add_test(core, test_s21_memcpy);
  tcase_add_test(core, test_s21_memcpy);
  tcase_add_test(core, test_s21_memcpy_basic);
  tcase_add_test(core, test_s21_memcpy_empty);
  tcase_add_test(core, test_s21_memcpy_partial);
  tcase_add_test(core, test_s21_memcpy_no_overlap);
  tcase_add_test(core, test_s21_memcpy_binary);
  // memset
  tcase_add_test(core, test_s21_memset_basic);
  tcase_add_test(core, test_s21_memset_zero);
  tcase_add_test(core, test_s21_memset_full);
  tcase_add_test(core, test_s21_memset_empty);
  tcase_add_test(core, test_s21_memset_partial);
  // strncat
  tcase_add_test(core, test_s21_strncat);
  tcase_add_test(core, test_s21_strncat_basic);
  tcase_add_test(core, test_s21_strncat_full_length);
  tcase_add_test(core, test_s21_strncat_empty_src);
  tcase_add_test(core, test_s21_strncat_empty_dest);
  tcase_add_test(core, test_s21_strncat_zero_length);
  tcase_add_test(core, test_s21_strncat_large_length);
  tcase_add_test(core, test_s21_strncat_overlap);
  // strchr
  tcase_add_test(core, test_s21_strchr_found);
  tcase_add_test(core, test_s21_strchr_not_found);
  tcase_add_test(core, test_s21_strchr_null_terminator);
  tcase_add_test(core, test_s21_strchr_first_character);
  tcase_add_test(core, test_s21_strchr_last_character);
  // strncmp
  tcase_add_test(core, test_s21_strncmp);
  tcase_add_test(core, test_s21_strncmp_equal);
  tcase_add_test(core, test_s21_strncmp_not_equal);
  tcase_add_test(core, test_s21_strncmp_partial_compare);
  tcase_add_test(core, test_s21_strncmp_empty_strings);
  tcase_add_test(core, test_s21_strncmp_one_empty);
  tcase_add_test(core, test_s21_strncmp_null_terminator);
  tcase_add_test(core, test_s21_strncpy);
  // s21_strcspn
  tcase_add_test(core, test_s21_strcspn_basic);
  tcase_add_test(core, test_s21_strcspn_no_match);
  tcase_add_test(core, test_s21_strcspn_all_match);
  tcase_add_test(core, test_s21_strcspn_empty_str);
  tcase_add_test(core, test_s21_strcspn_empty_reject);
  tcase_add_test(core, test_s21_strcspn_null_terminator);
  tcase_add_test(core, test_s21_strcspn_special_chars);
  tcase_add_test(core, test_s21_strcspn_partial_match);
  tcase_add_test(core, test_s21_strlen);
  // s21_strerror

  tcase_add_test(core, test_s21_strerror);
  // s21_strlen
  tcase_add_test(core, test_s21_strlen_basic);
  tcase_add_test(core, test_s21_strlen_empty_string);
  tcase_add_test(core, test_s21_strlen_single_char);
  tcase_add_test(core, test_s21_strlen_whitespace);
  tcase_add_test(core, test_s21_strlen_special_characters);
  // s21_strpbrk
  tcase_add_test(core, test_s21_strpbrk_basic_match);
  tcase_add_test(core, test_s21_strpbrk_no_match);
  tcase_add_test(core, test_s21_strpbrk_multiple_matches);
  tcase_add_test(core, test_s21_strpbrk_empty_accept);
  tcase_add_test(core, test_s21_strpbrk_empty_str);
  // s21_strrchr
  tcase_add_test(core, test_s21_strrchr_basic_match);
  tcase_add_test(core, test_s21_strrchr_no_match);
  tcase_add_test(core, test_s21_strrchr_multiple_matches);
  tcase_add_test(core, test_s21_strrchr_first_char);
  tcase_add_test(core, test_s21_strrchr_last_char);
  tcase_add_test(core, test_s21_strrchr_empty_str);
  // s21_strstr
  tcase_add_test(core, test_s21_strstr_basic_match);
  tcase_add_test(core, test_s21_strstr_no_match);
  tcase_add_test(core, test_s21_strstr_at_start);
  tcase_add_test(core, test_s21_strstr_at_end);
  tcase_add_test(core, test_s21_strstr_empty_needle);
  tcase_add_test(core, test_s21_strstr_empty_haystack);
  // s21_strtok
  tcase_add_test(core, test_s21_strtok_basic);

  // sscanf
  tcase_add_test(core, test_sscanf_int_decimal);
  tcase_add_test(core, test_sscanf_int_with_width);
  tcase_add_test(core, test_sscanf_int_star);
  tcase_add_test(core, test_sscanf_int_octal);
  tcase_add_test(core, test_sscanf_int_hex);
  tcase_add_test(core, test_sscanf_int_i);
  tcase_add_test(core, test_sscanf_unsigned);
  tcase_add_test(core, test_sscanf_float);
  tcase_add_test(core, test_sscanf_float_gG);
  tcase_add_test(core, test_sscanf_float_width);
  tcase_add_test(core, test_sscanf_string);
  tcase_add_test(core, test_sscanf_char);
  tcase_add_test(core, test_sscanf_percent);
  tcase_add_test(core, test_sscanf_pointer);
  tcase_add_test(core, test_sscanf_length_modifiers);
  tcase_add_test(core, test_sscanf_invalid);
  tcase_add_test(core, test_sscanf_with_spaces_in_format);
  tcase_add_test(core, test_sscanf_matching_characters);
  // tcase_add_test(core, test_sscanf_format_ends_with_percent);
  tcase_add_test(core, test_sscanf_with_asterisk_modifier);

  suite_add_tcase(suite, core);

  return suite;
}

int main(void) {
  int number_failed;
  Suite *suite = s21_string_suite();
  SRunner *sr = srunner_create(suite);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? 0 : 1;
}
