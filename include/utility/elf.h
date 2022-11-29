// EPOS ELF Utility Declarations

#ifndef __elf_h
#define	__elf_h

#include <system/config.h>
#include "elf-linux.h"

__BEGIN_UTIL

typedef IF<Traits<CPU>::WORD_SIZE == 32, Elf32_Ehdr, Elf64_Ehdr>::Result Elf_Ehdr;
typedef IF<Traits<CPU>::WORD_SIZE == 32, Elf32_Addr, Elf64_Addr>::Result Elf_Addr;
typedef IF<Traits<CPU>::WORD_SIZE == 32, Elf32_Word, Elf64_Word>::Result Elf_Word;
typedef IF<Traits<CPU>::WORD_SIZE == 32, Elf32_Phdr, Elf64_Phdr>::Result Elf_Phdr;

class ELF: private Elf_Ehdr
{
public:
    ELF() {}

    // check for ELF magic number
    bool valid() {
        // db<ELF>(WRN) << "EI_MAG0 = " << EI_MAG0 << endl;
        // db<ELF>(WRN) << "EI_MAG1 = " << EI_MAG1 << endl;
        // db<ELF>(WRN) << "EI_MAG2 = " << EI_MAG2 << endl;
        // db<ELF>(WRN) << "EI_MAG3 = " << EI_MAG3 << endl;


        db<ELF>(WRN) << "ELF = " << ELFMAG0 << " " << ELFMAG1 << " " << ELFMAG2 << " " << ELFMAG3 << endl;
        // db<ELF>(WRN) << "ELFMAG1 = " << ELFMAG1 << endl;
        // db<ELF>(WRN) << "ELFMAG2 = " << ELFMAG2 << endl;
        // db<ELF>(WRN) << "ELFMAG3 = " << ELFMAG3 << endl;

        db<ELF>(WRN) << "E_IDENT = " << e_ident[0] << " " << e_ident[1] << " " << e_ident[2] << " " << e_ident[3] << endl;
        // db<ELF>(WRN) << "e_ident[1] = " << e_ident[1] << endl;
        // db<ELF>(WRN) << "e_ident[2] = " << e_ident[2] << endl;
        // db<ELF>(WRN) << "e_ident[3] = " << e_ident[3] << endl;

        return (e_ident[EI_MAG0] == ELFMAG0) && (e_ident[EI_MAG1] == ELFMAG1)
            && (e_ident[EI_MAG2] == ELFMAG2) && (e_ident[EI_MAG3] == ELFMAG3);
    }

    Elf_Addr entry() { return e_entry; }

    int segments() { return e_phnum; }

    Elf_Word segment_type(int i) { return (i > segments()) ? PT_NULL : seg(i)->p_type; }

    Elf_Addr segment_address(int i) { return (i > segments()) ? 0 : seg(i)->p_align ? seg(i)->p_vaddr: (seg(i)->p_vaddr & ~(seg(i)->p_align - 1)); }

    int segment_size(int i) { return (i > segments()) ? -1 : (int)(((seg(i)->p_offset % seg(i)->p_align) + seg(i)->p_memsz + seg(i)->p_align - 1) & ~(seg(i)->p_align - 1)); }

    int load_segment(int i, Elf_Addr addr = 0);

private:
    Elf_Phdr * pht() { return (Elf_Phdr *)(((char *) this) + e_phoff); }
    Elf_Phdr * seg(int i) { return &pht()[i];  }
};

__END_UTIL

#endif
