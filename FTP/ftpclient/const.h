#ifndef _CMD_H
#define _CMD_H

const char ftp_response_331[] = "331 user ok, need passwd!\r\n";
const char ftp_response_230[] = "230 longin succeed!\r\n";
const char ftp_response_215[] = "215 UNIX Type: L8\r\n";
const char ftp_response_502[] = "502 not support!\r\n";
const char ftp_response_257[] = "257 current dir is ";
const char ftp_response_200[] = "200 TYPE I\r\n";
const char ftp_response_227[] = "227 Entering passive mode\r\n";
const char ftp_response_250[] = "250 current dir is ";
const char ftp_response_125[] = "125 opening...\r\n";
const char ftp_response_226[] = "226 transfer complete!\r\n";
const char ftp_response_213[] = "213";
const char ftp_response_530[] = "150 not login\r\n";
const char ftp_response_531[] = "531 anonymous\r\n";
const char ftp_response_150[] = "150 status ok, gooding to open data connection...\r\n";
const char ftp_response_221[] = "221 goodye\r\n";
const char ftp_response_220[] = "220 server ready.\r\n";
const char ftp_response_421[] = "421 cann't serve!\r\n";


const static int OPEN = 600;
const static int LS = 601;
const static int PWD = 602;
const static int CD = 603;
const static int PUT = 604;
const static int GET = 605;
const static int CLOSE = 606;
const static int BYE = 607;
const static int MKDIR = 608;
const static int RMDIR = 609;
const static int DELE = 610;
const static int MODE = 611;

//recv cmd
const int M_SYST = 100;
const int M_FEAT = 101;
const int M_PWD = 102;
const int M_TYPE_I = 103;
const int M_PASV = 104;
const int M_CWD = 105;
const int M_LIST = 106;
const int M_SIZE = 107;
const int M_MDTM = 108;
const int M_RETR = 109;
const int M_PORT = 110;
const int M_STOR = 111;
const int M_QUIT = 112;
const int M_NOTSUPPORT = 130;

const char anony_user[] = "anonymous";
const char anony_pwd[] = "anonymous";

const char default_user[] = "root";
const char default_pwd[] = "1234";

const int DATA_MAXLINE = 8192;
const int CMD_MAXLINE = 1024;


const int SERVER_PORT = 21;
const int SERVER_DATA_PORT = 20;

const int AUTH_NOTVERIFY = -1;
const int AUTH_SUCC = 0;
const int AUTH_ANONY = 1;
const int AUTH_FAILED = 2;

const int MODE_BINARY = 23;
const int MODE_ASIC = 24;

const int PORT_MODE = 40;
const int PASSIVE_MODE = 41;


#endif
