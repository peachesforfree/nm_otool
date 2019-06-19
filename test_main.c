/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbalcort <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/18 18:28:18 by sbalcort          #+#    #+#             */
/*   Updated: 2019/06/18 18:28:20 by sbalcort         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <sys/mman.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
//needed for cpy type
#include <mach/machine.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>


void    print_output(int nsyms, int symoff, int stroff, char *ptr)
/*
*   using struct symtab_command
*/
{
    int i;
    char *stringtable;
    struct nlist_64 *array;

    array = (void*)ptr + symoff;
    printf("symoff: %d\n", symoff);
    printf("Array size: %lu\n", sizeof(struct nlist_64));
    stringtable = (void*)ptr + stroff;
    printf("stroff: %d\n", stroff);
    i = 0;
    while (i < nsyms)
    {
        printf("%s\n", stringtable + array[i].n_un.n_strx);
        i++;
    }
}


void print_header_file_type(uint32_t flag)
{
    if (flag == MH_OBJECT)//	0x1		/* relocatable object file */
        printf("\t\trelocatable object file");
    if (flag ==	MH_EXECUTE)//	0x2		/* demand paged executable file */
        printf("\t\tdemand paged executable file");
    if (flag ==	MH_FVMLIB)//	0x3		/* fixed VM shared library file */
        printf("\t\tfixed VM shared library file");
    if (flag ==	MH_CORE)//		0x4		/* core file */
        printf("\t\tcore file");
    if (flag ==	MH_PRELOAD)//	0x5		/* preloaded executable file */
        printf("\t\tpreloaded executable file");
    if (flag ==	MH_DYLIB)//	0x6		/* dynamically bound shared library */
        printf("\t\tdynamically bound shared library");
    if (flag ==	MH_DYLINKER)//	0x7		/* dynamic link editor */
        printf("\t\tdynamic link editor");
    if (flag ==	MH_BUNDLE)//	0x8		/* dynamically bound bundle file */
        printf("\t\tdynamically bound bundle file");
    if (flag ==	MH_DYLIB_STUB)//	0x9		/* shared library stub for static */
        printf("\t\tshared library stub for static linking only, no section contents");                    /*  linking only, no section contents */
    if (flag ==	MH_DSYM)//		0xa		/* companion file with only debug */
        printf("\t\tcompanion file with only debug sections");                /*  sections */
    if (flag ==	MH_KEXT_BUNDLE)//	0xb		/* x86_64 kexts */
        printf("\t\tx86_64 kexts");
}

void    print_cpu_type(cpu_type_t cpu, cpu_subtype_t cpu_sub)
{
    if (cpu == CPU_ARCH_MASK)//	0xff000000		/* mask for architecture bits */
        printf("\t\tCPU_ARCH_MASK");
    if (cpu == CPU_ARCH_ABI64)//	0x01000000		/* 64 bit ABI */
        printf("\t\tCPU_ARCH_ABI64");
/*
 *	Machine types known by all.
 */
 
    if (cpu == CPU_TYPE_ANY)//		((cpu_type_t) -1)
        printf("\t\tCPU_TYPE_ANY");
    if (cpu == CPU_TYPE_VAX)//		((cpu_type_t) 1)
        printf("\t\tCPU_TYPE_VAX");
/* skip				((cpu_type_t) 2)	*/
/* skip				((cpu_type_t) 3)	*/
/* skip				((cpu_type_t) 4)	*/
/* skip				((cpu_type_t) 5)	*/
    if (cpu == CPU_TYPE_MC680x0)//	((cpu_type_t) 6)
        printf("\t\tCPU_TYPE_MC680x0");
    if (cpu == CPU_TYPE_X86)//		((cpu_type_t) 7)
        printf("\t\tCPU_TYPE_X86");
    if (cpu == CPU_TYPE_I386)//		CPU_TYPE_X86		/* compatibility */
        printf("\t\tCPU_TYPE_I386");
    if (cpu == CPU_TYPE_X86_64)//		(CPU_TYPE_X86 | CPU_ARCH_ABI64)
    {
        printf("\t\tCPU_TYPE_X86_64\n");
        if ((cpu_sub & CPU_SUBTYPE_LIB64) == CPU_SUBTYPE_LIB64)
        {
            printf("\t\tLIB64\n");
            cpu_sub = cpu_sub - CPU_SUBTYPE_LIB64;
        }
        else if ((cpu_sub & CPU_SUBTYPE_MASK) == CPU_SUBTYPE_MASK)
        {
            printf("\t\tFLAG_MASK\n");
            cpu_sub = cpu_sub - CPU_SUBTYPE_MASK;
        }
        if (cpu_sub == CPU_SUBTYPE_X86_ALL)//		((cpu_subtype_t)3)
            printf("\t\tCPU_SUBTYPE_X86_ALL");
        if (cpu_sub == CPU_SUBTYPE_X86_64_ALL)//		((cpu_subtype_t)3)
            printf("\t\tCPU_SUBTYPE_X86_64_ALL");
        if (cpu_sub == CPU_SUBTYPE_X86_ARCH1)//		((cpu_subtype_t)4)
            printf("\t\tCPU_SUBTYPE_X86_ARCH1");
        if (cpu_sub == CPU_SUBTYPE_X86_64_H)//		((cpu_subtype_t)8)	/* Haswell feature subset */
            printf("\t\tCPU_SUBTYPE_X86_64_H");
    }
/* skip CPU_TYPE_MIPS		((cpu_type_t) 8)	*/
/* skip 			((cpu_type_t) 9)	*/
    if (cpu == CPU_TYPE_MC98000)//	((cpu_type_t) 10)
        printf("\t\tCPU_TYPE_MC98000");
    if (cpu == CPU_TYPE_HPPA)//           ((cpu_type_t) 11)
        printf("\t\tCPU_TYPE_HPPA");
    if (cpu == CPU_TYPE_ARM)//		((cpu_type_t) 12)
        printf("\t\tCPU_TYPE_ARM");
    if (cpu == CPU_TYPE_ARM64)//          (CPU_TYPE_ARM | CPU_ARCH_ABI64)
        printf("\t\tCPU_TYPE_ARM64");
    if (cpu == CPU_TYPE_MC88000)//	((cpu_type_t) 13)
        printf("\t\tCPU_TYPE_MC88000");
    if (cpu == CPU_TYPE_SPARC)//		((cpu_type_t) 14)
        printf("\t\tCPU_TYPE_SPARC");
    if (cpu == CPU_TYPE_I860)//		((cpu_type_t) 15)
        printf("\t\tCPU_TYPE_I860");
/* skip	CPU_TYPE_ALPHA		((cpu_type_t) 16)	*/
/* skip				((cpu_type_t) 17)	*/
    if (cpu == CPU_TYPE_POWERPC)//		((cpu_type_t) 18)
        printf("\t\tCPU_TYPE_POWERPC");
    if (cpu == CPU_TYPE_POWERPC64)//		(CPU_TYPE_POWERPC | CPU_ARCH_ABI64)
        printf("\t\tCPU_TYPE_POWERPC64");
}

void    print_header_flags(uint32_t flags)
{
    if ((flags & MH_OBJECT) == MH_OBJECT)//	0x1		/* relocatable object file */
        printf("\t\trelocatable object file\n");
    if ((flags & MH_EXECUTE) == MH_EXECUTE)//	0x2		/* demand paged executable file */
        printf("\t\tdemand paged executable file\n");
    if ((flags & MH_FVMLIB) == MH_FVMLIB)//	0x3		/* fixed VM shared library file */
        printf("\t\tfixed VM shared library file\n");
    if ((flags & MH_CORE) == MH_CORE)//		0x4		/* core file */
        printf("\t\tcore file\n");
    if ((flags & MH_PRELOAD) == MH_PRELOAD)//	0x5		/* preloaded executable file */
        printf("\t\tpreloaded executable file\n");
    if ((flags & MH_DYLIB) == MH_DYLIB)//	0x6		/* dynamically bound shared library */
        printf("\t\tdynamically bound shared librar\n");
    if ((flags & MH_DYLINKER) == MH_DYLINKER)//	0x7		/* dynamic link editor */
        printf("\t\tdynamic link editor\n");
    if ((flags & MH_BUNDLE) == MH_BUNDLE)//	0x8		/* dynamically bound bundle file */
        printf("\t\tdynamically bound bundle file\n");
    if ((flags & MH_DYLIB_STUB) == MH_DYLIB_STUB)//	0x9		/* shared library stub for static */
		printf("\t\tshared library stub for static linking only, no section contents\n");			/*  linking only, no section contents */
    if ((flags & MH_DSYM) == MH_DSYM)//		0xa		/* companion file with only debug */
		printf("\t\tcompanion file with only debug sections\n");			/*  sections */
    if ((flags & MH_KEXT_BUNDLE) == MH_KEXT_BUNDLE)//	0xb		/* x86_64 kexts */
        printf("\t\t x86_64 kexts Constants for the flags field of the mach_header\n");
/* Constants for the flags field of the mach_header */
    if ((flags & MH_NOUNDEFS) == MH_NOUNDEFS)//	0x1		/* the object file has no undefined references */
        printf("\t\tthe object file has no undefined references\n");
    if ((flags & MH_INCRLINK) == MH_INCRLINK)//	0x2		/* the object file is the output of an incremental link against a base file and can't be link edited again */
        printf("\t\tthe object file is the output of an incremental link against a base file and can't be link edited again\n");
    if ((flags & MH_DYLDLINK) == MH_DYLDLINK)//	0x4		/* the object file is input for the dynamic linker and can't be staticly link edited again */
        printf("\t\tthe object file is input for the dynamic linker and can't be staticly link edited again\n");
    if ((flags & MH_BINDATLOAD) == MH_BINDATLOAD)//	0x8		/* the object file's undefined references are bound by the dynamic linker when loaded. */
        printf("\t\tthe object file's undefined references are bound by the dynamic linker when loaded.\n");
    if ((flags & MH_PREBOUND) == MH_PREBOUND)//	0x10		/* the file has its dynamic undefined references prebound. */
        printf("\t\tthe file has its dynamic undefined references prebound.\n");
    if ((flags & MH_SPLIT_SEGS) == MH_SPLIT_SEGS)//	0x20		/* the file has its read-only and read-write segments split */
        printf("\t\tthe file has its read-only and read-write segments split\n");
    if ((flags & MH_LAZY_INIT) == MH_LAZY_INIT)//	0x40		/* the shared library init routine is to be run lazily via catching memory faults to its writeable segments (obsolete) */
        printf("\t\tthe shared library init routine is to be run lazily via catching memory faults to its writeable segments (obsolete)\n");
    if ((flags & MH_TWOLEVEL) == MH_TWOLEVEL)//	0x80		/* the image is using two-level name space bindings */
        printf("\t\tthe image is using two-level name space bindings\n");
    if ((flags & MH_FORCE_FLAT) == MH_FORCE_FLAT)//	0x100		/* the executable is forcing all images to use flat name space bindings */
        printf("\t\tthe executable is forcing all images to use flat name space bindings\n");
    if ((flags & MH_NOMULTIDEFS) == MH_NOMULTIDEFS)//	0x200		/* this umbrella guarantees no multiple defintions of symbols in its sub-images so the two-level namespace hints can always be used. */
        printf("\t\tthis umbrella guarantees no multiple defintions of symbols in its sub-images so the two-level namespace hints can always be used.\n");
    if ((flags & MH_NOFIXPREBINDING) == MH_NOFIXPREBINDING)// 0x400	/* do not have dyld notify the prebinding agent about this executable */
        printf("\t\tdo not have dyld notify the prebinding agent about this executabl\n");
    if ((flags & MH_PREBINDABLE) == MH_PREBINDABLE)//  0x800           /* the binary is not prebound but can have its prebinding redone. only used when MH_PREBOUND is not set. */
        printf("\t\tthe binary is not prebound but can have its prebinding redone. only used when MH_PREBOUND is not set.\n");
    if ((flags & MH_ALLMODSBOUND) == MH_ALLMODSBOUND)// 0x1000		/* indicates that this binary binds to all two-level namespace modules of its dependent libraries. only used when MH_PREBINDABLE and MH_TWOLEVEL are both set. */ 
        printf("\t\tindicates that this binary binds to all two-level namespace modules of its dependent libraries. only used when MH_PREBINDABLE and MH_TWOLEVEL are both set.\n");
    if ((flags & MH_SUBSECTIONS_VIA_SYMBOLS) == MH_SUBSECTIONS_VIA_SYMBOLS)// 0x2000/* safe to divide up the sections into sub-sections via symbols for dead code stripping */
        printf("\t\tsafe to divide up the sections into sub-sections via symbols for dead code stripping\n");
    if ((flags & MH_CANONICAL) == MH_CANONICAL)//    0x4000		/* the binary has been canonicalized via the unprebind operation */
        printf("\t\tthe binary has been canonicalized via the unprebind operation.\n");
    if ((flags & MH_WEAK_DEFINES) == MH_WEAK_DEFINES)//	0x8000		/* the final linked image contains external weak symbols */
        printf("\t\tthe final linked image contains external weak symbols\n");
    if ((flags & MH_BINDS_TO_WEAK) == MH_BINDS_TO_WEAK)// 0x10000	/* the final linked image uses weak symbols */
        printf("\t\tthe final linked image uses weak symbols\n");
    if ((flags & MH_ALLOW_STACK_EXECUTION) == MH_ALLOW_STACK_EXECUTION)// 0x20000/* When this bit is set, all stacks in the task will be given stack execution privilege.  Only used in MH_EXECUTE filetypes. */
        printf("\t\tWhen this bit is set, all stacks in the task will be given stack execution privilege.  Only used in MH_EXECUTE filetypes.\n");
    if ((flags & MH_ROOT_SAFE) == MH_ROOT_SAFE) /* 0x40000           /* When this bit is set, the binary declares it is safe for use in processes with uid zero */
        printf("\t\tWhen this bit is set, the binary declares it is safe for use in processes with uid zero \n");
    if ((flags & MH_SETUID_SAFE) == MH_SETUID_SAFE) /*0x80000         /* When this bit is set, the binary declares it is safe for use in processes when issetugid() is true */
        printf("\t\tWhen this bit is set, the binary declares it is safe for use in processes when issetugid() is true\n");
    if ((flags & MH_NO_REEXPORTED_DYLIBS) == MH_NO_REEXPORTED_DYLIBS)/* 0x100000 /* When this bit is set on a dylib, the static linker does not need to examine dependent dylibs to see if any are re-exported */
        printf("\t\tWhen this bit is set on a dylib, the static linker does not need to examine dependent dylibs to see if any are re-exported\n");
    if ((flags & MH_PIE) == MH_PIE)/* 0x200000			/* When this bit is set, the OS will load the main executable at a random address.  Only used in MH_EXECUTE filetypes. */
        printf("\t\tWhen this bit is set, the OS will load the main executable at a random address.  Only used in MH_EXECUTE filetypes.\n");
    if ((flags & MH_DEAD_STRIPPABLE_DYLIB) == MH_DEAD_STRIPPABLE_DYLIB)/* 0x400000 /* Only for use on dylibs. When linking against a dylib that has this bit set, the static linker will automatically not create a LC_LOAD_DYLIB load command to the dylib if no symbols are being referenced from the dylib. */
        printf("\t\tOnly for use on dylibs. When linking against a dylib that has this bit set, the static linker will automatically not create a LC_LOAD_DYLIB load command to the dylib if no symbols are being referenced from the dylib.\n");
    if ((flags & MH_HAS_TLV_DESCRIPTORS) == MH_HAS_TLV_DESCRIPTORS)/* 0x800000 /* Contains a section of type  S_THREAD_LOCAL_VARIABLES */
        printf("\t\tContains a section of type  S_THREAD_LOCAL_VARIABLES \n");
    if ((flags & MH_NO_HEAP_EXECUTION) == MH_NO_HEAP_EXECUTION)/* 0x1000000	/* When this bit is set, the OS will run the main executable with a non-executable heap even on platforms (e.g. i386) that don't require it. Only used in MH_EXECUTE filetypes. */
        printf("\t\tWhen this bit is set, the OS will run the main executable with a non-executable heap even on platforms (e.g. i386) that don't require it. Only used in MH_EXECUTE filetypes.\n");
    if ((flags & MH_APP_EXTENSION_SAFE) == MH_APP_EXTENSION_SAFE)/* 0x02000000 /* The code was linked for use in an application extension. */
        printf("\t\tThe code was linked for use in an application extension.\n");
    if ((flags & MH_NLIST_OUTOFSYNC_WITH_DYLDINFO) == MH_NLIST_OUTOFSYNC_WITH_DYLDINFO)/* 0x04000000 The external symbols listed in the nlist symbol table do not include all the symbols listed in the dyld info. */
        printf("\t\tThe external symbols listed in the nlist symbol table do not include all the symbols listed in the dyld info.\n");
}       

void    handle_64(char *ptr)
{
    int                     ncmds;
    int                     i;
    struct mach_header_64   *header;
    struct load_command     *lc;
    struct symtab_command   *sym;

    header = (struct mach_header_64 *)ptr;
    //number of load commands from mach_header
    ncmds = header->ncmds;

    i = 0;
    //load command starts imediately after mach_header
    lc = (void *)ptr + sizeof(struct mach_header_64);

    
    /////////////// struct mach_header_64
    printf("MH_MAGIC_64\n");
    printf("\tcpu_type: %d\n", header->cputype);
    print_cpu_type(header->cputype, header->cpusubtype);
    printf("\n");
    printf("\tfile_type: %d\n", header->filetype);
    print_header_file_type(header->filetype);
    printf("\n");
    printf("\tflags:%d\n", header->flags);
    print_header_flags(header->flags);
    printf("\tmagic uint32_t:%u\n", header->magic);
    printf("\treserved:%d\n", header->reserved);
    printf("\tsizeofcmds:%d\n\t\tThe size of all load commands", header->sizeofcmds);
    printf("\n\nLOAD COMMANDS\n");
     
    //////////////////struct load_command
    while (i < ncmds)
    {
        if (lc->cmd == LC_SYMTAB)
        {
            //now we can read whats in load command
            sym = (struct symtab_command *) lc;
            printf("nb symbol: %d\n", sym->nsyms);
            print_output(sym->nsyms, sym->symoff, sym->stroff, ptr);
            break ;
        }
        lc = (void *)lc + lc->cmdsize;
        i++;
    }
}

void    nm(char *ptr)
{
    int magic_number;

    magic_number = *(int *) ptr;
    //need to include MH_MAGIC_64
    if (magic_number == MH_MAGIC_64)
    {
        handle_64(ptr);
    }
}

int     main(int argc, char **argv)
{
    int         fd;
    char        *ptr;
    struct stat buff;

    if (argc != 2)
    {
        fprintf(stderr, "Please give me an arg\n");
        return (EXIT_FAILURE);
    }

    fd = open (argv[1], O_RDONLY);
    if (fd < 0)
    {
        perror("OPEN");
        return (EXIT_FAILURE);
    }
    
    
    if (fstat(fd, &buff) < 0)
    {
        perror("fstat");
        return (EXIT_FAILURE);
    }

    ptr = mmap(0, buff.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (ptr == MAP_FAILED)
    {
        perror("mmap");
        return (EXIT_FAILURE);
    }
    nm(ptr);
    if (munmap(ptr, buff.st_size) < 0)
    {
        perror("munmap");
        return (EXIT_FAILURE);
    }

    return (0);
}
