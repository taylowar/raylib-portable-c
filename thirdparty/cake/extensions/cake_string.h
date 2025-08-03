#ifndef CAKE_STRING_H_ 
#define CAKE_STRING_H_ 

typedef struct {
    size_t size;
    size_t capacity;
    char *es;
} Cake_String;

void cake_string_print(Cake_String cs); 

Cake_String cake_string_from_cstr(char* cstr); 

void cake_string_trim_right(Cake_String *cs); 
bool cake_string_eq(Cake_String cs1, Cake_String cs2); 

#ifdef CAKE_STRING_IMPLEMENTATION 

void cake_string_print(Cake_String cs)
{
    printf("`");
    for (size_t i=0; i<cs.size; ++i) {
        printf("%c", cs.es[i]);
    }
    printf("`\n");
}

Cake_String cake_string_from_cstr(char* cstr)
{
    Cake_String cs = {0};
    //cs.es = malloc(sizeof(cstr)+strlen(cstr));
    cs.es = (char*)cake_talloc_poll(sizeof(cstr)*strlen(cstr)+1);
    cs.size = strlen(cstr);
    for (size_t i=0; i<cs.size; ++i) {
        cs.es[i] = cstr[i];
    }
    cs.es[cs.size] = 0;
    cs.size+=1;
    return cs;
}

void cake_string_trim_right(Cake_String *cs)
{
    size_t new_size = cs->size;
    while (new_size > 0 && (cs->es[new_size-1] == ' ' || cs->es[new_size-1] == '\n')) {
        new_size -= 1;
    }
    cs->size = new_size;
}

bool cake_string_eq(Cake_String cs1, Cake_String cs2)
{
    if (cs1.size != cs2.size) return false;
    for (size_t i=0; i<cs1.size; ++i) {
        if (cs1.es[i] != cs2.es[i]) {
            return false;
        }
    }
    return true;
}

#endif // CAKE_STRING_IMPLEMENTATION 
#endif // CAKE_STRING_H_ 
