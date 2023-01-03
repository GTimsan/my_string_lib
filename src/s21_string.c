#include "s21_string.h"

s21_size_t s21_strlen(const char *str) {
    const char *sc;
    for (sc = str; *sc != '\0';) ++sc;
    return sc - str;
}

int s21_memcmp(const void *str1, const void *str2, s21_size_t count) {
    const unsigned char *stru1, *stru2;
    int res = 0;
    for (stru1 = str1, stru2 = str2; 0 < count; ++stru1, ++stru2, count--)
        if ((res = *stru1 - *stru2) != 0) break;
    return res;
}

void *s21_memchr(const void *str, int c, s21_size_t n) {
    const unsigned char *p = str;
    void *ans = s21_NULL;
    while (n--) {
        if ((unsigned char) c == *p++) {
            ans = (void *) (p - 1);
            break;
        }
    }
    return ans;
}

char *s21_strchr(const char *s, int c) {
    char *ans = s21_NULL;
    do {
        if (*s == c) {
            ans = (char *) s;
            break;
        }
    } while (*s++);
    return ans;
}

char *s21_strerror(int errnum) {
    static char *arr[] = ERROR;
    static char res[N];
    int i = 15;
    if (errnum < 0 || errnum > N) {
        int div;
        char remain[10] = "";
        char str[40] = "Unknown error: ";
        s21_memcpy(res, str, i);
        if (errnum < 0) {
            div = errnum * (-1);
            s21_memcpy(res + i, "-", 1);
            i++;
        } else {
            div = errnum;
        }
        int n = 0;
        while (div / 10 != 0) {
            remain[n] = div % 10 + 48;
            div /= 10;
            n++;
        }
        remain[n] = div + 48;
        for (int j = n; j >= 0; j--) {
            s21_memcpy(res + i, remain + j, 1);
            i++;
        }
        res[i] = '\0';
    } else {
        for (s21_size_t i = 0; i < N; i++) {
            res[i] = '\0';
        }
        s21_memcpy(res, arr[errnum], s21_strlen(arr[errnum]));
    }
    return res;
}

char *s21_strpbrk(const char *str1, const char *str2) {
    const char *sc1, *sc2;
    void *ans = s21_NULL;
    for (sc1 = str1; ((*sc1) && (!ans)); ++sc1) {
        for (sc2 = str2; ((*sc2) && (!ans)); ++sc2) {
            if (*sc1 == *sc2) ans = (char *) sc1;
        }
    }
    return ans;
}

char *s21_strrchr(const char *str, int c) {
    const char *last = s21_NULL;
    do {
        if (*str == (char) c) last = str;
    } while (*str++);
    return (char *) last;
}

char *s21_strstr(const char *haystack, const char *needle) {
    s21_size_t l1 = s21_strlen(haystack), l2 = s21_strlen(needle);
    char *ans = s21_NULL;
    if (!l2) ans = (char *) haystack;
    while ((l1 >= l2) && (!ans)) {
        l1--;
        if (!s21_memcmp(haystack, needle, l2)) ans = (char *) haystack;
        haystack++;
    }
    return ans;
}

void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
    char *tmp = dest;
    const char *s = src;
    while (n--) *tmp++ = *s++;
    return dest;
}

void *s21_memmove(void *dest, const void *src, s21_size_t n) {
    char *tmp;
    const char *s;
    if (dest <= src) {
        tmp = dest;
        s = src;
        while (n--) *tmp++ = *s++;
    } else {
        tmp = dest;
        tmp += n;
        s = src;
        s += n;
        while (n--) *--tmp = *--s;
    }
    return dest;
}

void *s21_memset(void *str, int c, s21_size_t n) {
    char *xs = str;
    while (n--) *xs++ = c;
    return str;
}

char *s21_strcpy(char *dest, const char *src) {
    char *tmp = dest;
    while ((*dest++ = *src++) != '\0') {
    }
    return tmp;
}

char *s21_strncpy(char *dest, const char *src, s21_size_t n) {
    char *tmp = dest;
    while (n) {
        if ((*tmp = *src) != 0) src++;
        tmp++;
        n--;
    }
    return dest;
}

s21_size_t s21_strspn(const char *str1, const char *str2) {
    s21_size_t item = 0;
    const char *stru1, *stru2;
    for (stru1 = str1; *stru1 != '\0'; ++stru1) {
        for (stru2 = str2; *stru2 != '\0'; ++stru2) {
            if (*stru1 == *stru2) break;
        }
        if (*stru2 == '\0') break;
        ++item;
    }
    return item;
}

s21_size_t s21_strcspn(const char *str1, const char *str2) {
    const char *p, *r;
    s21_size_t count = 0;
    for (p = str1; *p != '\0'; ++p) {
        for (r = str2; *r != '\0'; ++r) {
            if (*p == *r) break;
        }
        if (*p == *r) break;
        ++count;
    }
    return count;
}

int s21_strcmp(const char *str1, const char *str2) {
    int item = 0;
    while (1) {
        unsigned char stru1 = *str1++, stru2 = *str2++;
        if (stru1 != stru2) {
            stru1 < stru2 ? item-- : item++;
            break;
        }
        if (!stru1) break;
    }
    return item;
}

int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
    int item = 0;
    while (n) {
        unsigned char stru1 = *str1++, stru2 = *str2++;
        if (stru1 != stru2) {
            stru1 < stru2 ? item-- : item++;
            break;
        }
        if (!stru1) break;
        n--;
    }
    return item;
}

char *s21_strtok(char *str, const char *delim) {
    static unsigned char *last;
    unsigned char *str2, map[32];
    char *tmp;
    const unsigned char *ctrl = (const unsigned char *) delim;
    int count;

    for (count = 0; count < 32; count++) {
        map[count] = 0;
    }
    do {
        map[*ctrl >> 3] |= (1 << (*ctrl & 7));
    } while (*ctrl++);
    if (str) {
        str2 = (unsigned char *) str;
    } else {
        str2 = last;
    }
    while ((map[*str2 >> 3] & (1 << (*str2 & 7))) && *str2) {
        str2++;
    }
    str = (char *) str2;
    for (; *str2; str2++) {
        if (map[*str2 >> 3] & (1 << (*str2 & 7))) {
            *str2++ = '\0';
            break;
        }
    }
    last = str2;
    if (str == (char *) str2) {
        tmp = s21_NULL;
    } else {
        tmp = str;
    }
    return tmp;
}

char *s21_strcat(char *dest, const char *src) {
    char *tmp = dest;
    while (*dest) dest++;
    while ((*dest++ = *src++) != '\0') {
    }
    return tmp;
}

char *s21_strncat(char *dest, const char *src, s21_size_t n) {
    char *tmp = dest;
    if (n) {
        while (*dest) dest++;
        while ((*dest++ = *src++) != 0) {
            if (--n == 0) {
                *dest = '\0';
                break;
            }
        }
    }
    return tmp;
}
