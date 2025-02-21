// plainSTR.h
// a header string library for C, with dynamic string allocation, without null terminators

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct PS_String {
    int length;
    char *data;
};

PS_String* PS_Create(const char *input) {
    if (!input) return NULL;

    PS_String *str = (PS_String*)malloc(sizeof(PS_String));
    if (!str) return NULL;
    
    str->length = strlen(input);
    str->data = (char*)malloc(str->length * sizeof(char));
    if (!str->data) {
        free(str);
        return NULL;
    }
    
    memcpy(str->data, input, str->length);
    return str;
}

void PS_Free(PS_String *str) {
    if (str) {
        if (str->data) {
            free(str->data);
        }
        free(str);
    }
}

/*char* PS_ToChars(PS_String *str) {
    if (!str || !str->data) return NULL;

    char *nullTerminated = (char*)malloc((str->length + 1) * sizeof(char));
    if (!nullTerminated) return NULL;
    
    memcpy(nullTerminated, str->data, str->length);
    nullTerminated[str->length] = '\0';
    return nullTerminated;
}*/

char* PS_ToChars(PS_String *str) {
    if (!str || !str->data) return NULL;

    char *nullTerminated = (char*)malloc((str->length + 1) * sizeof(char));
    if (!nullTerminated) return NULL;
    
    memcpy(nullTerminated, str->data, str->length);
    nullTerminated[str->length] = '\0';
    return nullTerminated;
}

void PS_Copy(PS_String *dest, PS_String *src) {
    if (!dest || !src || !src->data) return;

    dest->length = src->length;
    dest->data = (char*)realloc(dest->data, dest->length * sizeof(char));
    if (!dest->data) return;

    memcpy(dest->data, src->data, src->length);
}

PS_String* PS_Duplicate(PS_String *src) {
    if (!src || !src->data) return NULL;

    PS_String *copy = (PS_String*)malloc(sizeof(PS_String));
    if (!copy) return NULL;

    copy->length = src->length;
    copy->data = (char*)malloc(copy->length * sizeof(char));
    if (!copy->data) {
        free(copy);
        return NULL;
    }

    memcpy(copy->data, src->data, src->length);

    return copy;
}

PS_String* PS_Concat(PS_String *str1, PS_String *str2) {
    if (!str1 || !str2 || !str1->data || !str2->data) return NULL;

    PS_String *result = (PS_String*)malloc(sizeof(PS_String));
    if (!result) return NULL;

    result->length = str1->length + str2->length;
    result->data = (char*)malloc(result->length * sizeof(char));
    if (!result->data) {
        free(result);
        return NULL;
    }

    memcpy(result->data, str1->data, str1->length);
    memcpy(result->data + str1->length, str2->data, str2->length);

    return result;
}