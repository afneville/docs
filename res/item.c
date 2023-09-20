#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#define DEFAULTREPRSIZE 6
#define NULLHASH 0

// type aliases
typedef union item_type_U_ item_val;
typedef enum item_type_E_ item_type;
typedef struct item_T_ item;
typedef struct item_T_ *item_p;

// type definitions
union item_type_U_ {
    int int_val;
    float flt_val;
    double dbl_val;
    char char_val;
    char *str_val;
    void *ptr_val;
};

enum item_type_E_ { Integer, Double, Float, Character, String, Pointer };

struct item_T_ {
    item_val val;
    item_type type;
    char *repr;
    int repr_len;
    unsigned long hash;
    bool dync_memb;
};

// allocation / deallocation
item item_new(item_type type, ...);
// dynamic
item_p item_new_p(item_type type, ...);
void item_del_p(item_p self);
item_p item_clone_p(item_p self);

// public functions
void item_modify(item_p self, item_type type, ...);
bool item_compare(item_p self, item_p other);
void item_print(item_p self);
char *item_repr(item_p self);
unsigned long item_hash(item_p self);

static unsigned long hash_str_f_(char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}

static void set_repr(item_p self) {
    int repr_len;
    switch (self->type) {
    case Integer:
        repr_len =
            snprintf(self->repr, self->repr_len, "%d", self->val.int_val);
        break;
    case Float:
        repr_len =
            snprintf(self->repr, self->repr_len, "%f", self->val.flt_val);
        break;
    case Double:
        repr_len =
            snprintf(self->repr, self->repr_len, "%f", self->val.dbl_val);
        break;
    case Character:
        repr_len =
            snprintf(self->repr, self->repr_len, "'%c'", self->val.char_val);
        break;
    case String:
        repr_len =
            snprintf(self->repr, self->repr_len, "\"%s\"", self->val.str_val);
        break;
    case Pointer:
        repr_len =
            snprintf(self->repr, self->repr_len, "%p", self->val.ptr_val);
        break;
    default:
        break;
    }
    if (repr_len >= self->repr_len) {
        self->repr = (char *)realloc(self->repr, sizeof(char) * (repr_len + 1));
        self->repr_len = repr_len + 1;
        set_repr(self);
    }
}

static void set_hash(item_p self) { self->hash = hash_str_f_(self->repr); }

static void item_init_dynamic_members(item_p self) {
    // reset dynamic components of the item according to current state
    self->dync_memb = true;
    self->repr = (char *)malloc(sizeof(char) * DEFAULTREPRSIZE);
    self->repr_len = DEFAULTREPRSIZE;
    set_repr(self);
    set_hash(self);
}

static void item_dealloc_dynamic_members(item_p self) {
    // set dynamic components of the item to null condition
    self->dync_memb = false;
    self->repr_len = 0;
    if (self->repr != NULL)
        free(self->repr);
    self->repr = NULL;
    self->hash = NULLHASH;
}

// public funcitons
item item_new(item_type type, ...) {
    va_list args;
    va_start(args, 1);
    item self;
    self.type = type;
    switch (self.type) {
    case Integer:
        self.val.int_val = va_arg(args, int);
        break;
    case Double:
        self.val.dbl_val = va_arg(args, double);
        break;
    case Float:
        self.val.flt_val = (float)va_arg(args, double);
        break;
    case Character:
        self.val.char_val = (char)va_arg(args, int);
        break;
    case String:
        self.val.str_val = va_arg(args, char *);
        break;
    case Pointer:
        self.val.ptr_val = va_arg(args, void *);
        break;
    default:
        break;
    }
    va_end(args);
    self.repr = NULL;
    item_dealloc_dynamic_members(&self);
    return self;
}

item_p item_new_p(item_type type, ...) {
    va_list args;
    va_start(args, 1);
    item_p self = (item_p)malloc(sizeof(struct item_T_));
    self->type = type;
    switch (self->type) {
    case Integer:
        self->val.int_val = va_arg(args, int);
        break;
    case Double:
        self->val.dbl_val = va_arg(args, double);
        break;
    case Float:
        self->val.flt_val = (float)va_arg(args, double);
        break;
    case Character:
        self->val.char_val = (char)va_arg(args, int);
        break;
    case String:
        self->val.str_val = va_arg(args, char *);
        break;
    case Pointer:
        self->val.ptr_val = va_arg(args, void *);
        break;
    default:
        break;
    }
    va_end(args);
    item_init_dynamic_members(self);
    return self;
}

void item_del_p(item_p self) {
    item_dealloc_dynamic_members(self);
    free(self);
}

item_p item_clone_p(item_p self) {
    item_p new = (item_p)malloc(sizeof(item));
    memcpy(new, self, sizeof(item));
    item_init_dynamic_members(new);
    return new;
}

void item_modify(item_p self, item_type type, ...) {
    va_list args;
    va_start(args, 1);
    self->type = type;
    switch (self->type) {
    case Integer:
        self->val.int_val = va_arg(args, int);
        break;
    case Double:
        self->val.dbl_val = va_arg(args, double);
        break;
    case Float:
        self->val.flt_val = (float)va_arg(args, double);
        break;
    case Character:
        self->val.char_val = (char)va_arg(args, int);
        break;
    case String:
        self->val.str_val = va_arg(args, char *);
        break;
    case Pointer:
        self->val.ptr_val = va_arg(args, void *);
        break;
    default:
        break;
    }
    va_end(args);
    if (self->repr != NULL) {
        free(self->repr);
        item_init_dynamic_members(self);
    }
}

bool item_compare(item_p self, item_p other) {
    if (self->type != other->type)
        return false;
    switch (self->type) {
    case Integer:
        return self->val.int_val == other->val.int_val;
    case Float:
        return self->val.flt_val == other->val.flt_val;
    case Double:
        return self->val.dbl_val == other->val.dbl_val;
    case Character:
        return self->val.char_val == other->val.char_val;
    case String:
        return strcmp(self->val.str_val, other->val.str_val) == 0;
    case Pointer:
        return self->val.ptr_val == other->val.ptr_val;
    default:
        break;
    }
}

void item_print(item_p self) {
    if (self->repr != NULL) {
        printf("%s", self->repr);
        return;
    }
    item_init_dynamic_members(self);
    printf("%s", self->repr);
    item_dealloc_dynamic_members(self);
}

char *item_repr(item_p self) {
    if (self->repr != NULL) {
        return self->repr;
    }
    item_init_dynamic_members(self);
    char *rv = self->repr;
    self->repr = NULL;
    item_dealloc_dynamic_members(self);
    return rv;
}

unsigned long item_hash(item_p self) {
    if (self->repr != NULL && self->hash != NULLHASH) {
        return self->hash;
    }
    item_init_dynamic_members(self);
    ulong rv = self->hash;
    item_dealloc_dynamic_members(self);
    return rv;
}
#define LOCALLEN 9

int main() {
    item_p *a = (item_p *)malloc(sizeof(item_p) * LOCALLEN);
    int i = 0;
    a[i++] = item_new_p(Integer, 4);
    a[i++] = item_new_p(Integer, 4);
    a[i++] = item_new_p(Integer, 5);
    a[i++] = item_new_p(Float, 3.2);
    a[i++] = item_new_p(Character, 't');
    a[i++] = item_new_p(String, "Hello!");
    a[i++] = item_new_p(String, "Hello!");
    a[i++] = item_new_p(String, "Hi!");
    a[i++] = item_new_p(String, "This is quite a long string.");

    printf("%d\n", item_compare(a[5], a[6]));
    printf("%d\n", item_compare(a[5], a[7]));
    printf("%d\n", item_compare(a[0], a[1]));
    printf("%d\n\n", item_compare(a[0], a[2]));
    printf("%s\n", item_repr(a[8]));
    printf("%lu\n", item_hash(a[0]));
    printf("%lu\n", item_hash(a[1]));
    printf("%lu\n", item_hash(a[4]));

    printf("[");
    for (int i = 0; i < LOCALLEN; i++) {
        item_print(a[i]);
        item_del_p(a[i]);
        printf(", ");
    }
    printf("\b\b]\n");
    free(a);

    item b = item_new(Integer, 1234);
    item_p c = item_clone_p(&b);
    item_modify(c, Character, 'T');
    char *string1 = item_repr(&b); // 1234
    char *string2 = item_repr(c);  // 'T'
    printf("%s %s\n", string1, string2);
    free(string1);
    free(string2);
    free(c);

    return 0;
}
