#include "s21_string.h"


int parser_print(char *format, va_list list, form *f) {
    char *tmp = format;
    while (s21_strchr("0 -+#", *format)) {
        f->flag.zero |= (*format == '0');
        if (f->flag.sign != '+')
            f->flag.sign |= (*format == ' ') * ' ' + (*format == '+') * '+';
        f->flag.align |= (*format == '-');
        f->flag.hash |= (*format == '#');
        format += 1;
    }

    if ((*format > '0') & (*format < ':')) {
        format += s21_atoi(format, &(*f).prop.width);
    } else if (*format == '*') {
        f->prop.width = va_arg(list,
        s21_size_t);
        format++;
    }
    if (*format == '.') {
        format++;
        if ((*format >= '0') & (*format < ':')) {
            format += s21_atoi(format, &(*f).prop.accuracy);
        } else if (*format == '*') {
            f->prop.accuracy = va_arg(list,
            s21_size_t);
            format++;
        }
    } else {
        f->prop.acc_flag = 1;
    }
    while (s21_strchr("hlL", *format)) {
        (f->data.length == 'l') ? (*format++) : (f->data.length = *format++);
    }
    if (s21_strchr("cs%diufngGpxXeEo", *format)) f->data.spec = *format++;
    return (format - tmp);
}

int s21_sprintf(char *str, char *format, ...) {
    char *tmp = str;
    va_list list;
    va_start(list, format);
    while (*format) {
        if (*format != '%') {
            *str++ = *format++;
        } else {
            form f = {{0, 0, 0, 0},
                      {0, 0, 0},
                      {0, 0}};
            int p = parser_print(++format, list, &f);
            format += p;
            str += strgen(f, list, str);
        }
    }
    va_end(list);
    return str - tmp + (*str = 0);
}

int strgen(form f, va_list l, char *s) {
    char str[1024] = {0}, *tmp = s;
    int len;
    if (f.data.spec == 'c') {
        if (f.flag.align) {
            *s++ = va_arg(l,
            int);
            for (; --f.prop.width > 0;) *s++ = ' ';
        } else {
            for (; --f.prop.width > 0;) *s++ = 32 + 16 * f.flag.zero;
            *s++ = va_arg(l,
            int);
        }
    } else if (f.data.spec == 's') {
        char *c = va_arg(l,
        char *);
        if (f.prop.acc_flag) {
            f.prop.accuracy = s21_strlen(c);
        } else {
            if ((s21_size_t) f.prop.accuracy > s21_strlen(c))
                f.prop.accuracy = s21_strlen(c);
        }
        if (f.flag.align) {
            if (f.prop.width >= (int) s21_strlen(c)) f.prop.width -= s21_strlen(c);
            for (char *i = c; ((*i) && (f.prop.accuracy--)); i++) *s++ = *i;
            for (; (f.prop.width-- > f.prop.accuracy);) *s++ = ' ';
        } else {
            for (; (f.prop.width-- > f.prop.accuracy);) *s++ = ' ' + 16 * f.flag.zero;
            for (char *i = c; ((*i) && (f.prop.accuracy--)); i++) *s++ = *i;
        }
    } else if (s21_strchr("iud", f.data.spec)) {
        if (f.data.length == 'h') {
            (f.data.spec == 'u') ? (s21_uitoa((unsigned short int) va_arg(l,
            int),
            f.prop.accuracy, str))
            : (s21_itoa((short) va_arg(l,
            int), f.flag.sign,
                    f.prop.accuracy, str));
        } else if (f.data.length == 'l') {
            (f.data.spec == 'u') ? (s21_uitoa((unsigned long) va_arg(l,
            unsigned long),
            f.prop.accuracy, str))
            : (s21_itoa((long int) va_arg(l,
            long int),
            f.flag.sign, f.prop.accuracy, str));
        } else {
            (f.data.spec == 'u')
            ? (s21_uitoa((unsigned int) va_arg(l,
            int), f.prop.accuracy, str))
            : (s21_itoa(va_arg(l,
            int), f.flag.sign, f.prop.accuracy, str));
        }
        int i = 0;
        if ((!f.prop.acc_flag) && (f.prop.accuracy == 0)) {
            if ((str[0] == '0') && (str[1] == 0)) str[0] = 0;
            if ((s21_strchr("+- ", str[0])) && (str[1] == '0') && (str[2] == 0))
                str[1] = 0;
        }

        len = s21_strlen(str);
        if (f.flag.align) {
            for (; str[i]; i++) *s++ = str[i];
            for (; (f.prop.width-- > len);) *s++ = ' ';
        } else {
            if ((s21_strchr("+- ", str[0])) && f.flag.zero) *s++ = str[i++];
            for (; (f.prop.width-- > len);) *s++ = ' ' + 16 * f.flag.zero;
            for (; str[i]; i++) *s++ = str[i];
        }
    } else if (f.data.spec == 'f') {
        int n;
        if (f.prop.acc_flag) f.prop.accuracy = 6;
        if (f.data.length == 'L') {
            long double x = (long double) va_arg(l,
            long double);
            n = isnan(x) + 2 * (x == INFINITY) + 3 * (x == -INFINITY);
            if (!n) s21_ftoa(x, f, str);
        } else {
            double x = va_arg(l,
            double);
            n = isnan(x) + 2 * (x == INFINITY) + 3 * (x == -INFINITY);
            if (!n) s21_ftoa(x, f, str);
        }
        if (n) {
            char *err[3] = {"nan", "inf", "-inf"};
            if (n == 2) str[0] = f.flag.sign;
            s21_strcat(str, err[n - 1]);
        }
        len = s21_strlen(str);

        if (f.flag.align) {
            for (int i = 0; str[i]; i++) *s++ = str[i];
            for (; (f.prop.width-- > len);) *s++ = ' ';
        } else {
            int i = 0;
            if (((s21_strchr("+- ", str[0])) && f.flag.zero) && (!n)) *s++ = str[i++];
            for (; (f.prop.width-- > len);) *s++ = 32 + 16 * f.flag.zero * (!n);
            for (; str[i]; i++) *s++ = str[i];
        }
    } else if (f.data.spec == '%') {
        *s++ = '%';
    }
    return (s - tmp);
}

void s21_uitoa(s21_size_t n, unsigned int eq, char *s) {
    unsigned long int i = 0, m = n;
    for (; m > 0; i++) m /= 10;
    if (eq < i) eq = i;
    i = 0;
    do {
        s[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);
    for (; i < eq; i++) s[i] = '0';
    s[i] = '\0';
    s21_reverse(s);
}

int s21_atoi(const char *str, int *n) {
    int ans = 0;
    while ((str[ans] >= '0') && (str[ans] < ':')) *n = *n * 10 + str[ans++] - '0';
    return ans;
}

void s21_itoa(long n, char f, int eq, char *s) {
    long i = 0, m = n;
    if (n == LONG_MIN) {
        char str[100] = "-9223372036854775808";
        for (; str[i]; i++) s[i] = str[i];
        *(str + i) = 0;
    } else {
        long sign;
        if (m < 0) m = -m;
        for (; m > 0; i++) m /= 10;
        if (eq < i) eq = i;
        i = 0;
        if ((sign = n) < 0) n = -n;
        do {
            s[i++] = n % 10 + '0';
        } while ((n /= 10) > 0);
        for (; i < eq; i++) s[i] = '0';
        (sign < 0) ? (s[i++] = '-') : ((f) ? (s[i++] = f) : (s[i] = 0));
        s[i] = '\0';
        s21_reverse(s);
    }
}

void s21_ftoa(long double n, form f, char *s) {
    int i = 0, sign = (n < 0) ? -1 : 1, j = f.prop.accuracy;
    long long int m = (long long int) n;
    if (sign < 0) m = -m;
    do {
        s[i++] = m % 10 + '0';
    } while ((m /= 10) > 0);
    (sign < 0) ? (s[i++] = '-')
               : ((f.flag.sign) ? (s[i++] = f.flag.sign) : (s[i] = 0));
    s[i] = '\0';
    s21_reverse(s);
    if ((++f.prop.accuracy > 0) || (f.flag.hash)) {
        s[i++] = '.';
        if (n < 0) n = -n;
        n -= (int) n;
        for (; f.prop.accuracy--; n -= (int) n) s[i++] = '0' + (int) (n *= 10);
    }
    int k = 0;
    if ((s[0] == '+') || (s[0] == '-') || (s[0] == ' ')) k++;
    if (s[--i] > '4') s[i - 1] += 1;
    for (int u = i; u > k; u--) {
        if (s[u] == 58) {
            s[u] = '0';
            s[u - 1] += 1;
        } else if (s[u] == 47) {
            s[u] = '.';
            s[u - 1] += 1;
        }
    }
    if (s[k] == 58) {
        s[k] = '0';
        for (int u = i; u >= 0; u--) s[u + 1] = s[u];
        s[k] = '1';
        i++;
    }
    if ((j <= 0) && (!f.flag.hash)) *s21_strchr(s, '.') = 0;
    s[i] = '\0';
}

void s21_reverse(char s[]) {
    int i, j;
    for (i = 0, j = s21_strlen(s) - 1; i < j; i++, j--) {
        char c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}
