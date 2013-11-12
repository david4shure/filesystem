#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include "disk.h"

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef unsigned int uint;

typedef struct File {
  char filename [8];
  char extension [3];
  uint8 attributes;
  char reserved [10];
  uint16 time;
  uint16 date;
  uint16 first_cluster;
  uint32 size;
} File;

#define NUM_FILES (BLOCK_SIZE / sizeof(File))
typedef File FAT [NUM_FILES];  // Takes up one block
const uint16 starting_cluster = 1;

// Initializes a disk to an empty state
int formatDisk() {
  // Allocate four empty file entries
  FAT fat;
  for (int i=0; i<NUM_FILES; i++) {
    fat[i].filename[0] = 0;
  }
  return dWrite(0, (char*)fat);
}

typedef struct FileDescriptor {
  uint8 active;
  uint8 inode;
  uint16 first_cluster;
  uint16 current_cluster;
  uint32 position;
  uint32 size;
} FileDescriptor;

FileDescriptor* fds = NULL;
uint num_fds = 0;

int fOpen (const char* name) {
  // Analyze and validate name
  uint name_len = 0;
  uint ext_len = 0;
  while (name[name_len] != '.' && name[name_len] != 0) {
    name_len++;
  }
  if (name_len == 0 || name_len ) return -1;
  if (name[name_len] == '.') {
    const char* ext = name + name_len + 1;
    while(ext[ext_len] != 0) {
      ext_len++;
    }
    if (ext_len > 3) return -1;
  }
  FAT fat;
  if (!dRead(0, (char*)&fat)) return -1;
  uint i;
  int first_unused = -1;
  for (i = 0; i < NUM_FILES; i++) {
    if (first_unused == -1 && fat[i].filename[0] == 0) first_unused = i;
    else if (strncmp(fat[i].filename, name, name_len) == 0
	     && strncmp(fat[i].extension, name + name_len + 1, ext_len) == 0) {
      break;
    }
  }
  // No file match
  if (i == 4) {
    if (first_unused == -1) {  // No more room for files
      return -1;
    }
    i = first_unused;
    // Create new
    for (uint j = 0; j < 8; j++) {
      fat[i].filename[j] = j < name_len ? name[j] : 0;
    }
    for (uint j = 0; j < 3; j++) {
      fat[i].extension[j] = j < ext_len ? name[name_len + 1 + j] : 0;
    }
    fat[i].first_cluster = 0;
    fat[i].size = 0;
    if (!dWrite(0, (char*)&fat)) return -1;
  }
  FileDescriptor fd;
  fd.active = 1;
  fd.inode = i;
  fd.first_cluster = fat[i].first_cluster;
  fd.current_cluster = fat[i].first_cluster;
  fd.size = fat[i].size;
  fd.position = 0;
  // Allocate an FD
  for (uint i = 0; i < num_fds; i++) {
    if (!fds[i].active) {
      fds[i] = fd;
      return i;
    }
  }
  num_fds++;
  fds = realloc(fds, num_fds * sizeof(FileDescriptor));
  fds[num_fds - 1] = fd;
  return num_fds - 1;
}

int fClose (int fileID) {
  if (fileID > num_fds) return -1;
  if (!fds[fileID].active) return -1;
  fds[fileID].active = 0;
  return 0;
}

int fRead(int fileID, char *buffer, int length) {
  if (fileID > num_fds) return -1;
  if (!fds[fileID].active) return -1;
  char *bufPtr;

}

int fLs() {
  FAT fat;
  int read = dRead(0, (char *) &fat);
  if (read == 0) {
    return -1;
  }
  for( int i = 0; i < NUM_FILES; i++ ) {
    printf("%8s", fat[i].filename);
  }
  return 0;
}

//Absolute - 0 is relative 1 is absolute position
int fSeek(int fileID, int position, int absolute) {
  if (fileID > num_fds) return -1;
  if (!fds[fileID].active) return -1;
  if (absolute == 1) {
    fds[fileID].position = position;
  } else if (absolute == 0) {
    fds[fileID].position += position;
  } else {
    return -1;
  }
}



