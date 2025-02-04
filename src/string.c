#include "s21_string.h"

#include <ctype.h>
#include <stdarg.h>

#include "helper.h"

/*
 * Проект: Библиотека строковых функций
 * Описание: 
 * Данная библиотека предоставляет набор функций для работы со строками в языке C.
 * Она включает в себя основные операции, такие как поиск, копирование, 
 * сравнение и модификация строк. Основная цель библиотеки - 
 * обеспечить разработчикам удобный и эффективный интерфейс для манипуляций 
 * со строковыми данными, что позволяет значительно упростить процесс 
 * разработки и повысить производительность приложений.
 * 
 * Функции библиотеки реализованы с учетом стандартов языка C и 
 * оптимизированы для работы с различными типами строк. 
 * Библиотека может быть использована как в учебных проектах, так и в 
 * коммерческих приложениях, где требуется работа со строками.
 */

void *s21_memchr(const void *str, int symbol, s21_size_t size) {
  const char *result = str;
  const char *found = s21_NULL;  // Переменная для хранения результата
  int status = 0;

  for (s21_size_t x = 0; status == 0 && result && x < size; x++) {
    if (result[x] == symbol) {
      found = &result[x];
      status = 1;
    }
  }

  return (void *)found;
}

// last
int s21_memcmp(const void *mem_1, const void *mem_2, s21_size_t size) {
  unsigned char *str_1 = (unsigned char *)mem_1;
  unsigned char *str_2 = (unsigned char *)mem_2;
  int result = 0;

  for (s21_size_t x = 0; result == 0 && x < size; x++) {
    if (str_1[x] == str_2[x]) {
      result = 0;
    } else {
      result = (str_1[x] > str_2[x]) ? 1 : -1;
    }
  }

  return result;
}

void *s21_memcpy(void *dest, const void *src, s21_size_t size) {
  for (s21_size_t x = 0; x < size; x += 1) {
    ((char *)dest)[x] = ((char *)src)[x];
  }
  return dest;
}

void *s21_memset(void *str, int symbol, s21_size_t size) {
  for (s21_size_t x = 0; x < size; x += 1) {
    ((char *)str)[x] = symbol;
  }
  return str;
}

char *s21_strncat(char *result, const char *str, s21_size_t size) {
  int strLength = s21_strlen(result);

  for (s21_size_t x = 0; x < size && str[x] != '\0'; x += 1) {
    result[strLength + x] = str[x];
    result[strLength + x + 1] = '\0';
  }

  return result;
}

char *s21_strchr(const char *str, int symbol) {
  int strLength = s21_strlen(str) + 1;
  const char *result = s21_NULL;

  for (int x = 0; x < strLength && result == s21_NULL; x += 1)
    if (str[x] == symbol) result = (str + x);

  return (char *)result;
}

int s21_strncmp(const char *str_1, const char *str_2, s21_size_t size) {
  int result = 0;  // Изначально строки считаются равными
  s21_size_t i = 0;

  while (i < size && result == 0 && !(str_1[i] == '\0' && str_2[i] == '\0')) {
    result = (unsigned char)str_1[i] - (unsigned char)str_2[i];
    i++;
  }

  return result;
}

char *s21_strncpy(char *destptr, const char *srcptr, s21_size_t size) {
  s21_size_t x;

  // Копируем символы из srcptr в destptr
  for (x = 0; x < size && srcptr[x] != '\0'; x++) {
    destptr[x] = srcptr[x];
  }

  // Если не все символы были скопированы, заполняем оставшиеся нулями
  for (; x < size; x++) {
    destptr[x] = '\0';
  }

  return destptr;
}

s21_size_t s21_strcspn(const char *str, const char *keys) {
  s21_size_t strLength = s21_strlen(str) + 1;
  s21_size_t result = 0;

  for (result = 0; result <= strLength; result += 1)
    if (s21_strchr(keys, str[result])) break;

  return result;
}

s21_size_t s21_strlen(const char *str) {
  s21_size_t strLength = 0;
  for (; str[strLength];) {
    strLength += 1;
  }
  return strLength;
}

char *s21_strpbrk(const char *str, const char *keys) {
  s21_size_t strLength = s21_strlen(str);
  s21_size_t shift = 0;
  int status = 0;

  for (shift = 0; status == 0 && shift < strLength; shift += 1) {
    if (s21_strchr(keys, str[shift])) status = 1;
  }

  return (char *)(status ? (str + shift - 1) : s21_NULL);
}

char *s21_strrchr(const char *str, int symbol) {
  int strLength = s21_strlen(str);
  const char *result = s21_NULL;

  for (int x = strLength; result == s21_NULL && x >= 0; x -= 1) {
    if (str[x] == symbol) result = (str + x);
  }

  return (char *)result;
}

char *s21_strstr(const char *destptr, const char *srcptr) {
  s21_size_t srcptrLength = s21_strlen(srcptr);
  const char *result = s21_NULL;

  // Если подстрока пуста, возвращаем указатель на начало строки (стандартное
  // поведение strstr)
  if (srcptrLength == 0) {
    result = destptr;
  }

  // Перебор основной строки
  for (int x = 0; destptr[x] != '\0'; x++) {
    if (destptr[x] == srcptr[0]) {
      if (s21_strncmp(destptr + x, srcptr, srcptrLength) == 0) {
        result = destptr + x;
      }
    }
  }

  // Единственный выход: возвращаем результат
  return (char *)result;
}

char *s21_strtok(char *str, const char *delim) {
  static char *addr = s21_NULL;
  char *res = s21_NULL;

  // Если передана новая строка, инициализируем addr
  if (str) {
    addr = str;
  }

  // Если addr указывает на действительный токен
  if (addr && *addr) {
    // Пропускаем начальные разделители
    while (*addr && s21_strchr(delim, *addr)) {
      addr++;
    }

    if (*addr) {
      res = addr;

      // Продвигаем указатель, пока не найдем разделитель или конец строки
      while (*addr && !s21_strchr(delim, *addr)) {
        addr++;
      }

      // Если нашли разделитель, заменяем его на '\0' и перемещаем addr
      if (*addr) {
        *addr = '\0';
        addr++;
      } else {
        addr = s21_NULL;
      }
    }
  }

  return res;  // Возвращаем токен или NULL, если токенов больше нет
}

void *s21_to_upper(const char *str) {
  char *result = s21_NULL;

  if (str) {
    s21_size_t len = s21_strlen(str);
    char *upper_str = malloc(len + 1);

    if (upper_str) {
      for (s21_size_t i = 0; i < len; i++) {
        upper_str[i] =
            (char)((str[i] >= 'a' && str[i] <= 'z') ? (str[i] - ('a' - 'A'))
                                                    : str[i]);
      }
      upper_str[len] = '\0';
      result = upper_str;
    }
  }

  return result;
}

void *s21_to_lower(const char *str) {
  char *result = s21_NULL;

  if (str) {
    s21_size_t len = s21_strlen(str);
    char *lower_str = malloc(len + 1);

    if (lower_str) {
      for (s21_size_t i = 0; i < len; i++) {
        lower_str[i] =
            (char)((str[i] >= 'A' && str[i] <= 'Z') ? (str[i] + ('a' - 'A'))
                                                    : str[i]);
      }
      lower_str[len] = '\0';
      result = lower_str;
    }
  }

  return result;
}

void *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  char *result = s21_NULL;

  if (src && str && start_index <= s21_strlen(src)) {
    s21_size_t src_len = s21_strlen(src);
    s21_size_t str_len = s21_strlen(str);

    char *new_str = malloc(src_len + str_len + 1);

    if (new_str) {
      // Копируем часть до start_index
      for (s21_size_t i = 0; i < start_index; i++) {
        new_str[i] = src[i];
      }

      // Вставляем строку str
      for (s21_size_t i = 0; i < str_len; i++) {
        new_str[start_index + i] = str[i];
      }

      // Копируем остаток src
      for (s21_size_t i = start_index; i < src_len; i++) {
        new_str[str_len + i] = src[i];
      }

      new_str[src_len + str_len] = '\0';
      result = new_str;
    }
  }

  return result;
}

void *s21_trim(const char *src, const char *trim_chars) {
  char *result = s21_NULL;

  if (src && trim_chars) {
    s21_size_t start_index = 0;
    s21_size_t end_index = s21_strlen(src);

    // Определяем начальный индекс
    while (start_index < end_index &&
           s21_strchr(trim_chars, src[start_index])) {
      start_index++;
    }

    // Определяем конечный индекс
    while (end_index > start_index &&
           s21_strchr(trim_chars, src[end_index - 1])) {
      end_index--;
    }

    s21_size_t new_length = end_index - start_index;

    // Выделяем память и создаем строку
    char *trimmed_str = malloc(new_length + 1);
    if (trimmed_str) {
      for (s21_size_t i = 0; i < new_length; i++) {
        trimmed_str[i] = src[start_index + i];
      }
      trimmed_str[new_length] = '\0';
      result = trimmed_str;
    }
  }

  return result;
}

// // sprintf
// Основная функция
int s21_sprintf(char *str, const char *format, ...) {
  va_list args;
  va_start(args, format);
  char *p = str;
  const char *fmt = format;
  while (*fmt) {
    if (*fmt != '%') {
      p = s21_copy_char(p, &fmt);
      continue;
    }
    fmt++;
    p = s21_handle_format(&p, &fmt, args);
  }
  *p = '\0';
  va_end(args);
  return (int)(p - str);
}

// strerror
#if defined(__APPLE__)
#define MAX_ERRLIST 107
#define MIN_ERRLIST -1
#define ERROR "Unknown error: "

static const char *errorList[] = {
    "Undefined error: 0",
    "Operation not permitted",
    "No such file or directory",
    "No such process",
    "Interrupted system call",
    "Input/output error",
    "Device not configured",
    "Argument list too long",
    "Exec format error",
    "Bad file descriptor",
    "No child processes",
    "Resource deadlock avoided",
    "Cannot allocate memory",
    "Permission denied",
    "Bad address",
    "Block device required",
    "Resource busy",
    "File exists",
    "Cross-device link",
    "Operation not supported by device",
    "Not a directory",
    "Is a directory",
    "Invalid argument",
    "Too many open files in system",
    "Too many open files",
    "Inappropriate ioctl for device",
    "Text file busy",
    "File too large",
    "No space left on device",
    "Illegal seek",
    "Read-only file system",
    "Too many links",
    "Broken pipe",
    "Numerical argument out of domain",
    "Result too large",
    "Resource temporarily unavailable",
    "Operation now in progress",
    "Operation already in progress",
    "Socket operation on non-socket",
    "Destination address required",
    "Message too long",
    "Protocol wrong type for socket",
    "Protocol not available",
    "Protocol not supported",
    "Socket type not supported",
    "Operation not supported",
    "Protocol family not supported",
    "Address family not supported by protocol family",
    "Address already in use",
    "Can't assign requested address",
    "Network is down",
    "Network is unreachable",
    "Network dropped connection on reset",
    "Software caused connection abort",
    "Connection reset by peer",
    "No buffer space available",
    "Socket is already connected",
    "Socket is not connected",
    "Can't send after socket shutdown",
    "Too many references: can't splice",
    "Operation timed out",
    "Connection refused",
    "Too many levels of symbolic links",
    "File name too long",
    "Host is down",
    "No route to host",
    "Directory not empty",
    "Too many processes",
    "Too many users",
    "Disc quota exceeded",
    "Stale NFS file handle",
    "Too many levels of remote in path",
    "RPC struct is bad",
    "RPC version wrong",
    "RPC prog. not avail",
    "Program version wrong",
    "Bad procedure for program",
    "No locks available",
    "Function not implemented",
    "Inappropriate file type or format",
    "Authentication error",
    "Need authenticator",
    "Device power is off",
    "Device error",
    "Value too large to be stored in data type",
    "Bad executable (or shared library)",
    "Bad CPU type in executable",
    "Shared library version mismatch",
    "Malformed Mach-o file",
    "Operation canceled",
    "Identifier removed",
    "No message of desired type",
    "Illegal byte sequence",
    "Attribute not found",
    "Bad message",
    "EMULTIHOP (Reserved)",
    "No message available on STREAM",
    "ENOLINK (Reserved)",
    "No STREAM resources",
    "Not a STREAM",
    "Protocol error",
    "STREAM ioctl timeout",
    "Operation not supported on socket",
    "Policy not found",
    "State not recoverable",
    "Previous owner died",
    "Interface output queue is full"};

#elif defined(__linux__)
#define MAX_ERRLIST 134
#define MIN_ERRLIST -1
#define ERROR "Unknown error "

static const char *errorList[] = {
    "Success",
    "Operation not permitted",
    "No such file or directory",
    "No such process",
    "Interrupted system call",
    "Input/output error",
    "No such device or address",
    "Argument list too long",
    "Exec format error",
    "Bad file descriptor",
    "No child processes",
    "Resource temporarily unavailable",
    "Cannot allocate memory",
    "Permission denied",
    "Bad address",
    "Block device required",
    "Device or resource busy",
    "File exists",
    "Invalid cross-device link",
    "No such device",
    "Not a directory",
    "Is a directory",
    "Invalid argument",
    "Too many open files in system",
    "Too many open files",
    "Inappropriate ioctl for device",
    "Text file busy",
    "File too large",
    "No space left on device",
    "Illegal seek",
    "Read-only file system",
    "Too many links",
    "Broken pipe",
    "Numerical argument out of domain",
    "Numerical result out of range",
    "Resource deadlock avoided",
    "File name too long",
    "No locks available",
    "Function not implemented",
    "Directory not empty",
    "Too many levels of symbolic links",
    "Unknown error 41",
    "No message of desired type",
    "Identifier removed",
    "Channel number out of range",
    "Level 2 not synchronized",
    "Level 3 halted",
    "Level 3 reset",
    "Link number out of range",
    "Protocol driver not attached",
    "No CSI structure available",
    "Level 2 halted",
    "Invalid exchange",
    "Invalid request descriptor",
    "Exchange full",
    "No anode",
    "Invalid request code",
    "Invalid slot",
    "Unknown error 58",
    "Bad font file format",
    "Device not a stream",
    "No data available",
    "Timer expired",
    "Out of streams resources",
    "Machine is not on the network",
    "Package not installed",
    "Object is remote",
    "Link has been severed",
    "Advertise error",
    "Srmount error",
    "Communication error on send",
    "Protocol error",
    "Multihop attempted",
    "RFS specific error",
    "Bad message",
    "Value too large for defined data type",
    "Name not unique on network",
    "File descriptor in bad state",
    "Remote address changed",
    "Can not access a needed shared library",
    "Accessing a corrupted shared library",
    ".lib section in a.out corrupted",
    "Attempting to link in too many shared libraries",
    "Cannot exec a shared library directly",
    "Invalid or incomplete multibyte or wide character",
    "Interrupted system call should be restarted",
    "Streams pipe error",
    "Too many users",
    "Socket operation on non-socket",
    "Destination address required",
    "Message too long",
    "Protocol wrong type for socket",
    "Protocol not available",
    "Protocol not supported",
    "Socket type not supported",
    "Operation not supported",
    "Protocol family not supported",
    "Address family not supported by protocol",
    "Address already in use",
    "Cannot assign requested address",
    "Network is down",
    "Network is unreachable",
    "Network dropped connection on reset",
    "Software caused connection abort",
    "Connection reset by peer",
    "No buffer space available",
    "Transport endpoint is already connected",
    "Transport endpoint is not connected",
    "Cannot send after transport endpoint shutdown",
    "Too many references: cannot splice",
    "Connection timed out",
    "Connection refused",
    "Host is down",
    "No route to host",
    "Operation already in progress",
    "Operation now in progress",
    "Stale file handle",
    "Structure needs cleaning",
    "Not a XENIX named type file",
    "No XENIX semaphores available",
    "Is a named type file",
    "Remote I/O error",
    "Disk quota exceeded",
    "No medium found",
    "Wrong medium type",
    "Operation canceled",
    "Required key not available",
    "Key has expired",
    "Key has been revoked",
    "Key was rejected by service",
    "Owner died",
    "State not recoverable",
    "Operation not possible due to RF-kill",
    "Memory page has hardware error"};

#endif

char *s21_strerror(int errorNumber) {
  static char result[512] = {'\0'};

  if (errorNumber <= MIN_ERRLIST || errorNumber >= MAX_ERRLIST) {
    snprintf(result, sizeof(result), "Unknown error: %d", errorNumber);
  } else {
    snprintf(result, sizeof(result), "%s", errorList[errorNumber]);
  }

  return result;
}

int s21_sscanf(const char *str, const char *format, ...) {
  va_list args;
  va_start(args, format);

  int fields_matched = 0;
  const char *f = format;
  int done = 0;

  while (*f && !done) {
    if (s21_is_space(*f)) {
      s21_skip_spaces(&str);
      f++;
    } else if (*f != '%') {
      if (*str == *f && *str != '\0') {
        str++;
        f++;
      } else {
        done = 1;
      }
    } else {
      f++;
      int assign = 1;
      if (*f == '*') {
        assign = 0;
        f++;
      }

      int width = 0;
      while (isdigit((unsigned char)*f)) {
        width = width * 10 + (*f - '0');
        f++;
      }

      char length_modifier = '\0';
      if (*f == 'h') {
        length_modifier = 'h';
        f++;
      } else if (*f == 'l') {
        length_modifier = 'l';
        f++;
        if (*f == 'l') {
          length_modifier = 'L';
          f++;
        }
      } else if (*f == 'L') {
        length_modifier = 'L';
        f++;
      }

      if (*f == '\0') {
        done = 1;
        fields_matched = -1;
      } else {
        char spec = *f++;
        void *arg = s21_NULL;
        int res = -1;

        if (assign && spec != '%') {
          arg = va_arg(args, void *);
        }

        res = s21_process_specifier2(&str, spec, arg, width, length_modifier,
                                     assign);

        if (res < 0) {
          fields_matched = s21_return_failure(str, fields_matched);
          done = 1;
        } else if (assign && spec != '%') {
          fields_matched++;
        }
      }
    }
  }

  va_end(args);
  return fields_matched;
}
