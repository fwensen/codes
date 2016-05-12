/*  Author: vincent
 *  File name: FileHandler.cpp
 *  description: a wrap class for file handler
 */
#include<iostream>
#include"FileHandler.h"
using namespace std;

FileHandler::FileHandler(char *fileName) {

    m_FileBuffer = new char[BUFFER_SIZE];       //allocate BUFFER_SIZE bytes for buffer
    m_FileName = new char[FILE_NAME_LENGTH];    //allocate FILE_NAME_LENGTH bytes for file name
    memset(m_FileName, 0, sizeof(m_FileName));
    memset(m_FileBuffer, 0, sizeof(m_FileBuffer));
    memcpy(m_FileName, fileName, strlen(fileName));   //copy the file name to m_FileName
    m_nUsedBytesForBuffer = 0;
    m_nStartReadPositionForBuffer = 0;
    m_nFileOpenForWrite = false;
    //m_nFileOpenForRead = false;
}

void FileHandler::OpenFile(int mode) {
    
    //open file with mode mode, and get the file descriptor
    m_Fd = open(m_FileName, mode);    
    if (m_Fd == -1) {
        cout << "Open file error: " << strerror(errno) << endl;
        return;
    }
}

void FileHandler::WriteFile(const char *userBuf, size_t nBytes) {
    
    if (!m_nFileOpenForWrite) 
        m_nFileOpenForWrite = true;

    if (userBuf == 0)
        return;

    if (m_FileBuffer == 0)
        return;
    unsigned int nLeftRoom = BUFFER_SIZE - m_nUsedBytesForBuffer;
    unsigned int lenMsg = strlen(userBuf);

    //if the length of message larger than BUFFER_SIZE, then write them to file
    if (lenMsg > BUFFER_SIZE) {   
        int ret;
        ret = write(m_Fd, userBuf, lenMsg);
        if (ret == -1) {
            cout << "Write error: " << strerror(errno) << endl;
            return;
        }
    }
    //if m_FileBuffer can not load more message, then Flush, that is to say write buffer to file.
    if (lenMsg > nLeftRoom) {    
        WriteFlush();
    }
    //copy the message to the m_FileBuffer
    memcpy(m_FileBuffer + m_nUsedBytesForBuffer, userBuf, lenMsg); 
    m_nUsedBytesForBuffer += lenMsg;   //add the m_nUsedBytesForBuffer
}


void FileHandler::ReadFile(char* userBuf, size_t nBytes) {

    //if (!m_nFileOpenForRead)
    //    m_nFileOpenForRead = true;

    if (userBuf == 0)
        return;

    if (nBytes == 0)
        return;

    // if read bytes larger thar BUFFER_SIZE, read directly
    if (nBytes > BUFFER_SIZE) {

        int ret = read(m_Fd, userBuf, sizeof(userBuf));
        if (ret == -1) {
            cout << "Read error: " << strerror(errno) << endl;
            return;
        }
    }

    
    if (nBytes <= (m_nUsedBytesForBuffer - m_nStartReadPositionForBuffer)) {
        memcpy(userBuf, m_FileBuffer + m_nStartReadPositionForBuffer, nBytes);
        m_nStartReadPositionForBuffer += nBytes;
    } else {
        
        int len = m_nUsedBytesForBuffer - m_nStartReadPositionForBuffer;
        memcpy(userBuf, m_FileBuffer + m_nStartReadPositionForBuffer, len);
        m_nStartReadPositionForBuffer = 0;
        int ret = ReadFlush();
        if (ret == 0) {
            cout << "Read over!" << endl;
            return;
        }
        int cpyLen = nBytes - len;
        if (ret < cpyLen)
            cpyLen = ret;
        memcpy(userBuf + len, m_FileBuffer, cpyLen);
        m_nStartReadPositionForBuffer += cpyLen;
    }
}

//seek file
void FileHandler::SeekFile(off_t offSet, int whence) {
    
    int ret;
    ret = lseek(m_Fd, offSet, whence);
    if (ret == -1) {
        cout << "Seek file error: " << strerror(errno) << endl;
        return;
    }
}

void FileHandler::WriteFlush(){
    
    if (m_Fd == -1) 
        return;
    
    if (m_nUsedBytesForBuffer == 0)
        return;

    if (m_FileBuffer == 0)
        return;

    int ret = write(m_Fd, m_FileBuffer, m_nUsedBytesForBuffer);
    if (ret == -1) {
        cout << "Write file error: " << strerror(errno) << endl;
        return;
    }
    m_nUsedBytesForBuffer = 0;
}


int FileHandler::ReadFlush() {
    
    int ret;
    ret = read(m_Fd, m_FileBuffer, BUFFER_SIZE);
    if (ret == -1) {
        cout << "Read error: " << strerror(errno) << endl;
        return -1;
    }

    //EOF
    if (ret == 0) {
        
        m_nUsedBytesForBuffer = 0;
        m_nStartReadPositionForBuffer = 0;
        return 0;
    }
    m_nUsedBytesForBuffer = ret;
    m_nStartReadPositionForBuffer = 0;
    return ret;
}


FileHandler::~FileHandler() {

    int ret;
    if (m_nFileOpenForWrite)
        WriteFlush();
    delete [] m_FileBuffer;  //deallocate 
    delete [] m_FileName;    //deallocate
    ret = close(m_Fd);  //close file descriptor
    if (ret == -1) {
        cout << "Close file error: " << strerror(errno) << endl;
    }
}
