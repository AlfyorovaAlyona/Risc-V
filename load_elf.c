#include <err.h>
#include <fcntl.h>
#include <gelf.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <elf.h>

#include "load_elf.h"

extern char mem[134217728];
extern int32_t pc;


void
print_ptype(size_t pt, FILE *log) {
    char *s;

#define C(V) case PT_##V: s = #V; break
    switch (pt) {
        C(NULL);
        C(LOAD);
        C(DYNAMIC);
        C(INTERP);
        C(NOTE);
        C(SHLIB);
        C(PHDR);
        C(TLS);
        C(SUNWBSS);
        C(SUNWSTACK);
        default:
            s = "unknown";
            break;
    }
    (void) fprintf(log, " \"%s\"", s);
#undef  C
}

void print_mem(FILE* logq);

int load_code (int argc, char **argv) {
    int i, fd;
    FILE* log;
    Elf *e;
    char *id, bytes[5];
    size_t n;
    GElf_Phdr phdr;
    GElf_Ehdr ehdr;

    if (argc != 2)
        errx(EXIT_FAILURE, "usage: %s file-name", argv[0]);

    if (elf_version(EV_CURRENT) == EV_NONE)
        errx(EXIT_FAILURE, "ELF library initialization "
                           "failed: %s", elf_errmsg(-1));

    if ((fd = open(argv[1], O_RDONLY, 0)) < 0)
        err(EXIT_FAILURE, "open \"%s\" failed", argv[1]);

    if (!(log = fopen("elf.log", "w")))
        err(EXIT_FAILURE, "open \"%s\" failed", argv[1]);

    if ((e = elf_begin(fd, ELF_C_READ, NULL)) == NULL)
        errx(EXIT_FAILURE, "elf_begin() failed: %s.",
             elf_errmsg(-1));

    if (elf_kind(e) != ELF_K_ELF)
        errx(EXIT_FAILURE, "\"%s\" is not an ELF object.",
             argv[1]);

    if (gelf_getehdr(e, &ehdr) == NULL)
        errx(EXIT_FAILURE, "getehdr() failed: %s.",
             elf_errmsg(-1));

    if ((i = gelf_getclass(e)) == ELFCLASSNONE)
        errx(EXIT_FAILURE, "getclass() failed: %s.",
             elf_errmsg(-1));

    (void) fprintf(log, "%s: %d-bit ELF object\n", argv[1], i  == ELFCLASS32 ? 32 : 64);

    if ((id = elf_getident(e, NULL)) == NULL)
        errx(EXIT_FAILURE, "getident() failed: %s.",
             elf_errmsg(-1));

    (void) fprintf(log, "%3s e_ident[0..%1d] %7s", " ", EI_ABIVERSION, " ");

    for (i = 0; i <= EI_ABIVERSION; i++) {
        //(void) vis(bytes, id[i], VIS_WHITE, 0);
        (void) fprintf(log, " ['%s' %X]", bytes, id[i]);
    }

    (void) printf("\n");

#define        PRINT_FMT        "    %-20s 0x%jx\n"
#define        PRINT_FIELD(N) do { \
        (void) fprintf(log, PRINT_FMT, #N, (uintmax_t) ehdr.N); \
    } while (0)

    PRINT_FIELD(e_type);
    PRINT_FIELD(e_machine);
    PRINT_FIELD(e_version);
    PRINT_FIELD(e_entry);
    PRINT_FIELD(e_phoff);
    PRINT_FIELD(e_shoff);
    PRINT_FIELD(e_flags);
    PRINT_FIELD(e_ehsize);
    PRINT_FIELD(e_phentsize);
    PRINT_FIELD(e_shentsize);

    pc = ehdr.e_entry;

#undef PRINT_FMT
#undef PRINT_FIELD

    if (elf_getphdrnum(e, &n) != 0)

        errx(EXIT_FAILURE, "elf_getphdrnum() failed: %s.",
             elf_errmsg(-1));

    for (i = 0; i < n; i++) {


        if (gelf_getphdr(e, i, &phdr) != &phdr)

            errx(EXIT_FAILURE, "getphdr() failed: %s.",
                 elf_errmsg(-1));

        (void) fprintf(log, "PHDR %d:\n", i);
#define        PRINT_FMT        "    %-20s 0x%jx"
#define        PRINT_FIELD(N) do { \
        (void) fprintf(log, PRINT_FMT, #N, (uintmax_t) phdr.N); \
    } while (0)
#define         NL() do { (void) fprintf(log, "\n"); } while (0)
        PRINT_FIELD(p_type);

        print_ptype(phdr.p_type, log);
        NL();
        PRINT_FIELD(p_offset);
        NL();
        PRINT_FIELD(p_vaddr);
        NL();
        PRINT_FIELD(p_paddr);
        NL();
        PRINT_FIELD(p_filesz);
        NL();
        PRINT_FIELD(p_memsz);
        NL();
        PRINT_FIELD(p_flags);
        (void) fprintf(log, " [");
        if (phdr.p_flags & PF_X)
            (void) fprintf(log, " execute");
        if (phdr.p_flags & PF_R)
            (void) fprintf(log, " read");
        if (phdr.p_flags & PF_W)
            (void) fprintf(log, " write");
        fprintf(log, " ]");
        NL();
        PRINT_FIELD(p_align);
        NL();

        off_t curr_pos = lseek(fd, 0, SEEK_CUR);
        lseek(fd, phdr.p_offset, SEEK_SET);
        read(fd, (mem + phdr.p_vaddr), phdr.p_filesz);
        lseek(fd, curr_pos, SEEK_SET);
    }
    (void) elf_end(e);
    (void) close(fd);
    fclose(log);
    return EXIT_SUCCESS;
}

void print_mem(FILE* log) {
    for (int32_t *i = (int32_t *) mem; (i - (int32_t *) mem) < 33554432; i++) {
        if (*i != 0) fprintf(log, "%x", *i);
    }
}