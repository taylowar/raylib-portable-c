///
/// File system interaction extension 
///
#ifndef CAKE_FS_H_
#define CAKE_FS_H_

#ifndef CAKE_STRING_H_
_Static_assert (0, "This extension need the `cake_string` extension to be loaded to work");
#endif // CAKE_STRING_H_

typedef struct {
    size_t size;
    size_t capacity;
    Cake_String *es;
} Cake_FileText;

void cake_fs_file_text_print(Cake_FileText cft);
void cake_fs_file_text_from_file(FILE* fd, Cake_FileText *cft);

#ifdef CAKE_FS_IMPLEMENTATION

void cake_fs_file_text_print(Cake_FileText cft)
{
    for (size_t i=0; i<cft.size; ++i) {
        cake_string_print(cft.es[i]);
    }
}

void cake_fs_file_text_from_file(FILE* fd, Cake_FileText *cft)
{
    // Just know that this function does not close the file descriptor for your
    assert(fd != NULL && "Your file descriptor is NULL mang");
    char line[512];
    while (fgets(line, sizeof(line), fd) != NULL) {
        Cake_String cs_line = cake_string_from_cstr(line);
        cake_string_trim_right(&cs_line);
        if (cs_line.size > 0) {
            cake_da_append(cft, cs_line);
        }
    }
}

#endif // CAKE_FS_IMPLEMENTATION
#endif // CAKE_FS_H_
