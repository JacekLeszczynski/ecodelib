//-lm -lecode_c

#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

char *BUF = NULL;
int BUF_LEN = 0;

#define DecimalPoint fDecimalPoint
#define GetLineToStr fGetLineToStr
#define GetLineCount fGetLineCount
#define IsSpace fIsSpace
#define IsDigit fIsDigit
#define StrToL fStrToL
#define StrToD fStrToD
#define AToI fAToI
#define GToS fGToS
#define QSort fQSort
#define DecodeTimeToInteger fDecodeTimeToInteger
#define TimeToInteger fTimeToInteger
#define NowTimeToInteger fNowTimeToInteger
#define IntegerToTime fIntegerToTime
#define IntegerToTimeNoMs fIntegerToTimeNoMs
#define TimeTruncateInt fTimeTruncateInt
#define TimeTruncateDT fTimeTruncateDT
#define SecToTime fSecToTime
#define SecToInteger fSecToInteger
#define MiliSecToTime fMiliSecToTime
#define MiliSecToInteger fMiliSecToInteger
#define StringToDate fStringToDate
#define ReadConfigFile fReadConfigFile
#define ReadConfigFileString fReadConfigFileString

extern char *Realloc(char **a, int l);
extern char fDecimalPoint();
extern int fGetLineToStr(char *aStr, int l, char separator, char textseparator, char *wynik, char **wartosc, int *wartosc_len);
extern int fGetLineCount(char *aStr, char separator, char textseparator);
extern bool fIsSpace(char c);
extern bool fIsDigit(char c);
extern int fStrToL(char *s, char **reszta, int baza);
extern double fStrToD(char *s, char **reszta);
extern int fAToI(char *s);
extern int fGToS(double d, int l, char** result);
extern void fQSort(void *base, size_t num, size_t size, int (*compar)(const void *, const void*));
extern unsigned long fDecodeTimeToInteger(unsigned short Hour, unsigned short Minutes, unsigned short Second, unsigned short Milisecond);
extern unsigned long fTimeToInteger(double Time);
extern unsigned long fNowTimeToInteger();
extern double fIntegerToTime(unsigned long uczas);
extern double fIntegerToTimeNoMs(unsigned long uczas);
extern unsigned long fTimeTruncateInt(unsigned long a);
extern double fTimeTruncateDT(double a);
extern double fSecToTime(unsigned long aSec);
extern unsigned long fSecToInteger(unsigned long aSec);
extern double fMiliSecToTime(unsigned long aMiliSec);
extern unsigned long fMiliSecToInteger(unsigned long aMiliSec);
extern void DecodeDate(double Date, int* Year, int* Month, int* Day);
extern bool IsLeapYear(unsigned int Year);
extern bool EncodeDate(unsigned int Year, unsigned int Month, unsigned int Day, double* Date);
extern double fStringToDate(char *str);
extern int fReadConfigFile(const char* filename, char* buffer, size_t buffer_size);
extern char *fReadConfigFileString(char *elementy, char *element);

/* KOD SCIĄGAJĄCY DANE Z SIECI */

#define SetSizeMem fSetSizeMem
#define GetLiveChatId fGetLiveChatId
#define GetLiveChatAll fGetLiveChatAll
#define GetLiveChatText fGetLiveChatText

extern int get_youtube_data(char *url, char **result);
extern int fSetSizeMem(char **result, int size);
extern int fGetLiveChatId(char *video_id, char *api_key, char **result, int *len);
extern int fGetLiveChatAll(char *video_id, char *api_key, char **result, int *len);
extern int fGetLiveChatText(char *chat_id, char *api_key, char **aPageToken, int *aPageTokenLen, char **result, int *len, int *aPollingIntervalMillis);

/* FUNKCJE SZYFRUJĄCE */

#define EncryptStr fEncryptStr
#define DecryptStr fDecryptStr
#define Base64EncodeBin fBase64EncodeBin
#define Base64DecodeBin fBase64DecodeBin
#define CalcBuffer fCalcBuffer
#define EncryptString fEncryptString
#define DecryptString fDecryptString
#define EncryptBinary fEncryptBinary
#define DecryptBinary fDecryptBinary
#define EncryptStringOnlyPassword fEncryptStringOnlyPassword
#define DecryptStringOnlyPassword fDecryptStringOnlyPassword
#define EncryptStr fEncryptStr
#define DecryptStr fDecryptStr

extern int _encrypt(void* buffer, int buffer_len, char* IV, char* key, int key_len);
extern int _decrypt(void* buffer, int buffer_len, char* IV, char* key, int key_len);
extern void _SHA1Hash(char *ALFA, unsigned char *hash);
extern char *_HashSHA1Create(char *text);
extern char *fBase64EncodeBin(const char *bufor, int len);
extern int fBase64DecodeBin(const char *text, char **bufor);
extern int fCalcBuffer(int aLen);
extern int fEncryptString(char **buf, int len, char *IV, char *key);
extern int fDecryptString(char **buf, int len, char *IV, char *key);
extern int fEncryptBinary(char **buf, int len, char *IV, char *key);
extern int fDecryptBinary(char **buf, int len, char *IV, char *key);
extern int fEncryptStringOnlyPassword(char **buf, int len, char *key);
extern int fDecryptStringOnlyPassword(char **buf, int len, char *key);
extern char *fEncryptStr(char *text, char *key);
extern char *fDecryptStr(char *text, char *key);

