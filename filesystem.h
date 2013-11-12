#ifndef _FILESYSYSTEM_H
#define _FILESYSYSTEM_H

int formatDisk();
int fLs(); // Johnny
int fOpen(char *name); // Lewis 
int fClose(int fileID); // Lewis
int fRead(int fileID, char *buffer, int length); // Daniel
int fWrite(int fileID, char *buffer, int length); // writes length bytes starting at current position
int fSeek(int fileID, int position, int absolute); // Nolan

#endif
