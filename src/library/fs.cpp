// fs.cpp: File System

#include "sfs/fs.h"

#include <algorithm>

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>

using std::string;

// Debug file system -----------------------------------------------------------

void FileSystem::debug(Disk *disk) {
    Block block;

    // Read Superblock
    disk->read(0, block.Data);

    if(block.Super.MagicNumber != MAGIC_NUMBER) return;

    printf("SuperBlock:\n");
    printf("    magic number is valid\n");
    printf("    %u blocks\n"         , block.Super.Blocks);
    printf("    %u inode blocks\n"   , block.Super.InodeBlocks);
    printf("    %u inodes\n"         , block.Super.Inodes);

    // Read Inode blocks
    Block table, pointers;
    for(uint32_t b = 0; b < block.Super.InodeBlocks; b++) {
        disk->read(b + 1, table.Data);
        for(uint32_t i = 0; i < INODES_PER_BLOCK; i++) {
            Inode inode = table.Inodes[i];
            if(!inode.Valid) continue;

            printf("Inode %u:\n", b * INODES_PER_BLOCK + i);
            printf("    size: %u bytes\n", inode.Size);
            
            printf("    direct blocks: ");
            string s;
            for(uint32_t p = 0; p < POINTERS_PER_INODE; p++)
                if(inode.Direct[p]) s += std::to_string(inode.Direct[p]) + " ";
            printf("%s\n", s.substr(0, s.size() - 1).c_str());

            if(inode.Indirect) {
                printf("    indirect block: %u\n", inode.Indirect);
                printf("    indirect data blocks: ");
                disk->read(inode.Indirect, pointers.Data);

                string s;
                for(uint32_t p = 0; p < POINTERS_PER_BLOCK; p++)
                    if(pointers.Pointers[p]) s += std::to_string(pointers.Pointers[p]) + " ";
                printf("%s\n", s.substr(0, s.size() - 1).c_str());
            }
        }
    }
}

// Format file system ----------------------------------------------------------

bool FileSystem::format(Disk *disk) {
    // Write superblock
    Block block;
    block.Super.MagicNumber = MAGIC_NUMBER;
    block.Super.Blocks = disk->size();
    block.Super.InodeBlocks = (disk->size() + 9) / 10;
    block.Super.Inodes = block.Super.InodeBlocks * INODES_PER_BLOCK;
    disk->write(0, block.Data);

    // Clear all other blocks
    Block empty;
    memset(empty.Data, 0, Disk::BLOCK_SIZE);
    for(uint32_t i = 1; i < block.Super.Blocks; i++) {
        disk->write(i, empty.Data);
    }
/* 
    printf("SuperBlock:\n");
    printf("    magic number is valid\n");
    printf("    %u blocks\n"         , block.Super.Blocks);
    printf("    %u inode blocks\n"   , block.Super.InodeBlocks);
    printf("    %u inodes\n"         , block.Super.Inodes); */

    return true;
}

// Mount file system -----------------------------------------------------------

bool FileSystem::mount(Disk *disk) {
    // Read superblock

    // Set device and mount

    // Copy metadata

    // Allocate free block bitmap

    return true;
}

// Create inode ----------------------------------------------------------------

ssize_t FileSystem::create() {
    // Locate free inode in inode table

    // Record inode if found
    return 0;
}

// Remove inode ----------------------------------------------------------------

bool FileSystem::remove(size_t inumber) {
    // Load inode information

    // Free direct blocks

    // Free indirect blocks

    // Clear inode in inode table
    return true;
}

// Inode stat ------------------------------------------------------------------

ssize_t FileSystem::stat(size_t inumber) {
    // Load inode information
    return 0;
}

// Read from inode -------------------------------------------------------------

ssize_t FileSystem::read(size_t inumber, char *data, size_t length, size_t offset) {
    // Load inode information

    // Adjust length

    // Read block and copy to data
    return 0;
}

// Write to inode --------------------------------------------------------------

ssize_t FileSystem::write(size_t inumber, char *data, size_t length, size_t offset) {
    // Load inode
    
    // Write block and copy to data
    return 0;
}
