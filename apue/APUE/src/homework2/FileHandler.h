#ifndef _FILEHANDLER_H
#define _FILEHANDLER_H
#include<cstring>
#include<errno.h>
#include<unistd.h>
#include<fcntl.h>

const int BUFFER_SIZE = 4096 * 5;
const int FILE_NAME_LENGTH = 128;

class FileHandler {

public:
    explicit FileHandler(char *);
    ~FileHandler();
    void OpenFile(int);
    void ReadFile(char*, size_t);
    void WriteFile(const char*, size_t);
    void SeekFile(off_t, int);

private:

    FileHandler(const FileHandler&);    //
    FileHandler& operator=(const FileHandler&);
    FileHandler();

private:

    int ReadFlush();     //read flush
    void WriteFlush();    //write flush
    char *m_FileBuffer;     //read or write buffer
    //char *m_writeBuffer;    // write buffer

    int m_Fd;     //file handler
    //unsigned long m_nSeekPosition;

    unsigned int m_nUsedBytesForBuffer;
    unsigned int m_nStartReadPositionForBuffer;
    char *m_FileName;  //  read or write file name

    bool m_nFileOpenForWrite;
    //bool m_nFileOpenForRead;
};
#endif
