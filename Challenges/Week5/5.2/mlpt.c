#include <stdio.h>
#include <string.h>

const int PAGE_SIZE = 256;

char physical_ram[65536];

/*
 * Return the starting address for a given page number.
 */
int get_page_start_addr(int page)
{
    // TODO
    return page * PAGE_SIZE;
}

/*
 * For a given physical page and page table entry number, return the
 * entry value. This works on the directory page and the page table
 * page.
 */
int get_page_table_entry(int ppage, int entry)
{
    // TODO
    return physical_ram[get_page_start_addr(ppage) + entry];
}

/*
 * For a given physical page and page table entry number, set the entry
 * value. This works on the directory page and the page table page.
 */
void set_page_table_entry(int ppage, int entry, int value)
{
    // TODO
    physical_ram[get_page_start_addr(ppage) + entry] = value;
    return;
}

/*
 * For a given virtual address, follows the two-level page table and
 * returns the corresponding physical address. Should return 0 if
 * there's a page fault.
 */
int vaddr_to_paddr(int dir_page_num, int vaddr)
{
    // TODO
    int dir = (vaddr >> 16) & 0xFF;
    int page = (vaddr >> 8) & 0xFF;
    int offset = vaddr & 0xFF;

    int pt_page_num = get_page_table_entry(dir_page_num, dir);

    if(pt_page_num == 0)
        return 0; //page fault

    int phys_page = get_page_table_entry(pt_page_num, page);

    if(phys_page == 0)
        return 0; //also a page fault
    
    return get_page_start_addr(phys_page) + offset;
}

/*
 * Main
 */
int main(void)
{
    // Set up the page tables for this process. This would be the job of
    // the OS.
    //
    // The way this is set up, virtual address 3876654 will map to
    // physical address 6958.
    //
    // 3876654 is (59<<16)|(39<<8)|46, directory entry 59, page table
    // page 39. Offset 46.
    //
    // For your solution, you can't reference page 8 or 27 or address
    // 6958 directly--you must start with dir_page_num and follow the
    // pointers.
    //
    int dir_page_num = 4;

    set_page_table_entry(dir_page_num, 59, 8);
    set_page_table_entry(8, 39, 27);

    // Store some data in physical memory for testing at the same
    // place that the virtual location 3876654 maps to.
    strcpy(physical_ram + 6958, "Hello, world!");

    // Look up the physical address for virtual address 3876654
    int paddr = vaddr_to_paddr(dir_page_num, 3876654);

    // Print what's there--which should be "Hello, world!"
    puts(physical_ram + paddr);
}

