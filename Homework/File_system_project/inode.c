// Function to allocate memory for the inode map

#include "inode.h"

static struct inode incore[MAX_SYS_OPEN_FILES] = {0};

// Finds the first free inode in the incore array and returns a pointer to that inode
// Arguments:    none
// Returns:      A pointer to that in-core inode (or NULL if it can't be found)
struct inode *incore_find_free(void)
{
    for (int i = 0; i < MAX_SYS_OPEN_FILES; i++)
    {
        if (incore[i].ref_count == 0)
        {
            return &incore[i];
        }
    }
    return NULL;
}

// Finds an in-core inode record in the incore array
// Arguments:    inode number (unsigned int)
// Returns:      A pointer to that in-core inode (or NULL if it can't be found)
struct inode *incore_find(unsigned int inode_num)
{
    for (int i = 0; i < MAX_SYS_OPEN_FILES; i++)
    {
        if ((incore[i].ref_count != 0) && inode_num == incore[i].inode_num)
        {
            return &incore[i];
        }
    }
    return NULL;
}

void incore_free_all(void)
{
    for (int i = 0; i < MAX_SYS_OPEN_FILES; i++)
    {
        incore[i].ref_count = 0;
    }
}

// Reads from disk into an empty inode struct
// Arguments:    in (empty struct pointer), inode_num (int)
// Returns:      none
void read_inode(struct inode *in, int inode_num)
{
    int block_num = inode_num / INODES_PER_BLOCK + INODE0;
    int offset = inode_num % INODES_PER_BLOCK;
    unsigned char buffer[BLOCK_SIZE];
    bread(block_num, buffer);

    unsigned char *inode_r = buffer + offset * INODE_SIZE;
    // unpacks with pack.c
    in->size = read_u32(inode_r);
    in->owner_id = read_u16(inode_r + 4);
    in->permissions = read_u8(inode_r + 6);
    in->flags = read_u8(inode_r + 7);
    in->link_count = read_u8(inode_r + 8);
    for (int i = 0; i < INODE_PTR_COUNT; i++)
    {
        in->block_ptr[i] = read_u16(inode_r + (i * 2 + 9));
    }
    in->inode_num = inode_num;
}

// Writes the data pointer to by in onto disk
// Arguments:    None
// Returns:      None
void write_inode(struct inode *in)
{
    int inode_num = in->inode_num;

    int block_num = inode_num / INODES_PER_BLOCK + INODE0;
    int offset = inode_num % INODES_PER_BLOCK;

    unsigned char buffer[BLOCK_SIZE];
    bread(block_num, buffer);
    unsigned char *inode_r = buffer + offset * INODE_SIZE;

    write_u32(inode_r + 0, in->size);
    write_u16(inode_r + 4, in->owner_id);
    write_u8(inode_r + 6, in->permissions);
    write_u8(inode_r + 7, in->flags);
    write_u8(inode_r + 8, in->link_count);
    for (int i = 0; i < INODE_PTR_COUNT; i++)
    {
        write_u16(inode_r + 9 + (i * 2), in->block_ptr[i]);
    }

    bwrite(block_num, buffer);
}

// Return a pointer for an in-core inode at inode_num
// Arguments:    inode_num (int)
// Returns:      A pointer to the inode
struct inode *iget(int inode_num)
{
    struct inode *in = incore_find(inode_num);
    if (in != NULL)
    {
        in->ref_count += 1;
        return in;
    }
    in = incore_find_free();
    if (in == NULL)
    {
        return NULL;
    }
    read_inode(in, inode_num);
    in->ref_count += 1;
    in->inode_num = inode_num;
    return in;
}

// decrements inode->ref_count and writes if it reaches 0
// args: struct inode*
// returns : none
void iput(struct inode *in)
{
    if (in->ref_count == 0)
    {
        return;
    }
    in->ref_count--;
    if (in->ref_count == 0)
    {
        write_inode(in);
    }
}

// Finds the lowest free inode from the inode map and returns it after allocating
// Arguments:    none
// Returns:      the bit number of the free inode (or -1 when map is full)
struct inode *ialloc(void)
{
    unsigned char buf[BLOCK_SIZE];
    bread(INODE_MAP, buf);
    int free_inode_num = find_free(buf);
    if (free_inode_num == -1)
    {
        return NULL;
    }

    struct inode *in = iget(free_inode_num);
    if (in == NULL)
    {
        return NULL;
    }
    set_free(buf, free_inode_num, 1);
    bwrite(INODE_MAP, buf);
    in->size = 0;
    in->owner_id = 0;
    in->permissions = 0;
    in->flags = 0;
    in->link_count = 0;
    for (int i = 0; i < INODE_PTR_COUNT; i++)
    {
        in->block_ptr[i] = 0;
    }
    in->inode_num = free_inode_num;
    write_inode(in);
    return in;
}