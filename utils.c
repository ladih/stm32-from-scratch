// utils.c

int parse_int(const char *s) {
    if (*s < '0' || *s > '9') return -1;
    int result = 0;
    int i = 0;
    while (*s >= '0' && *s <= '9' && i < 6) {
        result = result * 10 + (*s - '0');
        s++;
        i++;
    }
    return result;
}

int starts_with(const char *s, const char *prefix) {
    while (*prefix) {
        if (*s != *prefix) return 0;
        s++;
        prefix++;
    }
    return 1;
}

void scpy(char* s1, const char* s2) {
    // copy s2 to s1
    while (*s2 != '\0') {
        *s1 = *s2;
        s1++;
        s2++;
    }
    *s1 = '\0';
}

int slen(const char *s) {
    // return len of s
    int i = 0;
    while (*s != '\0') {
        s++;
        i++;
    }
    return i;
}

int scmp(const char *s1, const char *s2) {
    while (*s1 && *s2 && *s1 == *s2) {
        s1++;
        s2++;
    }
    return !(*s1 == *s2);
}

