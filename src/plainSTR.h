// plainSTR.h
// a header string library for C with dynamic string allocation
// todo: remove null terminators grrr

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *data;
    size_t size;
} PS_String;

PS_String* PS_Create(size_t initial_size) {
    PS_String *str = (PS_String*)malloc(sizeof(PS_String));
    if (str != NULL) {
        str->data = (char*)malloc(initial_size);
        if (str->data != NULL) {
            str->size = initial_size;
            str->data[0] = '\0';
        } else {
            free(str);
            return NULL;
        }
    }
    return str;
}

void PS_Set(PS_String *dest, const char *new_str) {
    size_t new_len = strlen(new_str) + 1;
    if (new_len > dest->size) {
        dest->data = (char*)realloc(dest->data, new_len);
        if (dest->data == NULL) {
            dest->size = 0;
            return;
        }
        dest->size = new_len;
    }
    strcpy(dest->data, new_str);
}

const char* PS_Get(const PS_String *str) {
    return str->data;
}

void PS_Free(PS_String *str) {
    free(str->data);
    free(str);
    str = NULL;
}

/* example usage
int main() {
    PS_String *my_str = PS_Create(10);
    if (my_str != NULL) {
        PS_Set(my_str, "Hello, World!");
        printf("String: %s\n", PS_Get(my_str));
        PS_Free(my_str);
    } else {
        printf("Memory allocation failed.\n");
    }

    return 0;
}
*/