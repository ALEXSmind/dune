/*
 * sandbox.h - the main local header
 */

#include <sys/types.h>
#include <stdint.h>

#include "libdune/dune.h"

// address layout
// - Each untrusted memory reference must be less then
//   APP_MAX_VADDR. (2 for GB .code and .data)
// - Or can be between mmap_base - APP_MMAP_BASE_OFF
//   and mmap_base - APP_MMAP_END_OFF.
//   (8 GB for stack, heap, and mappings)
#define LOADER_VADDR_OFF	0x6F000000
#define APP_MAX_ELF_VADDR	0x70000000
#define APP_MMAP_BASE_OFF	0x200000000
#define APP_MMAP_LEN		0x200000000
#define APP_STACK_SIZE		0x800000 /* 8 megabytes */

/**
 * mem_ref_is_safe - determines if a memory range belongs to the sandboxed app
 * @ptr: the base address
 * @len: the length
 */
static inline bool mem_ref_is_safe(const void *ptr, size_t len)
{
	uintptr_t begin = (uintptr_t) ptr;
	uintptr_t end = (uintptr_t) (ptr + len);

	if (len <= APP_MAX_ELF_VADDR &&
	    begin >= 0 &&
	    end <= APP_MAX_ELF_VADDR)
		return true;

	if (len <= APP_MMAP_LEN &&
	    begin >= mmap_base &&
	    end < mmap_base + APP_MMAP_LEN)
		return true;

	return false;
}

extern int elf_load(const char *path);

extern unsigned long umm_brk(unsigned long brk);
extern unsigned long
umm_mmap(void *addr, size_t length, int prot,
	 int flags, int fd, off_t offset);
extern int umm_munmap(void *addr, size_t len);
extern int umm_mprotect(void *addr, size_t len, unsigned long prot);
extern int umm_alloc_stack(uintptr_t *stack_top);

extern int trap_init(void);
