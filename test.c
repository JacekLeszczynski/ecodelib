//-lm -lmcrypt -lcjson -lcurl -lcrypto -lssl -lecode_c
//-lm -lecode_c

#include "LibECode.c"
//#include "ecode.h"
//#include "lazc.h"
#include <sys/time.h>
#include <time.h>

char* concat(char *tekst1, char *tekst2) {
  int a = strlen(tekst1);
  int b = strlen(tekst2);
  char wynik[a+b+1];
  char *pom;
  strncpy(wynik,tekst1,a);
  pom = wynik;
  pom+=a;
  strncpy(pom,tekst2,b);
  wynik[a+b] = '\0';
  return strdup(wynik);
}

char* concat_str_char(char *tekst, char znak) {
  int a = strlen(tekst);
  char wynik[a+2];
  strncpy(wynik,tekst,a);
  wynik[a] = znak;
  wynik[a+1] = '\0';
  return strdup(wynik);
}

char* concat_char_str(char znak, char *tekst) {
  int a = strlen(tekst);
  char wynik[a+2];
  char *pom;
  *wynik = znak;
  pom = wynik;
  pom++;
  strncpy(pom,tekst,a);
  wynik[a+1] = '\0';
  return strdup(wynik);
}

char *IntToSys(int aLiczba, int aBaza)
{
     char znaki[62] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
     char *wynik = "";
     int n = aLiczba, pom;
     do {
        pom = n % aBaza;
        wynik = concat_char_str(znaki[pom],wynik);
        n = div(n,aBaza).quot;
     } while (n!=0);
     return strdup(wynik);
}

char *StrBase(char *aValue, int aLength)
{
  int a;
  int i;
  char *wsk;
  a = strlen(aValue);
  if (a>aLength) {
    char *s = aValue;
    return s;
  } else {
    char s[aLength+1];
    wsk=s;
    for (i=0; i<aLength-a; i++)
    {
      *wsk='0';
      wsk++;
    }
    *wsk='\0';
    return concat(s,aValue);
  }
}

long CurrentTimeMillisecond() {
  struct timeval time;
  gettimeofday(&time, NULL);
  return time.tv_sec * 1000 + time.tv_usec;
}

void Randomize()
{
     srand(CurrentTimeMillisecond());
}

void randomize()
{
     srand(time(NULL));
}

int Rand(int min_num, int max_num)
{
    int result = 0, low_num = 0, hi_num = 0;

    if (min_num < max_num)
    {
        low_num = min_num;
        hi_num = max_num + 1; // include max_num in output
    } else {
        low_num = max_num + 1; // include max_num in output
        hi_num = min_num;
    }
    result = (rand() % (hi_num - low_num)) + low_num;
    return result;
}

int UnixTime()
{
    return (int)time(NULL);
}

char *GetUuid()
{
    Randomize();
    int a=Rand(10,2000);
    for (int i=1; i<a; i++) Rand(1,5000);
    char *uuid;
    int unix_time = UnixTime();
    int l1 = Rand(0,65535);
    int l2 = Rand(0,65535);
    int l3 = Rand(0,65535);
    int l4 = Rand(0,16777215);
    int l5 = Rand(0,16777215);
    char *s = IntToSys(unix_time,16);
    char *s1 = StrBase(IntToSys(l1,16),4);
    char *s2 = StrBase(IntToSys(l2,16),4);
    char *s3 = StrBase(IntToSys(l3,16),4);
    char *s4 = StrBase(IntToSys(l4,16),6);
    char *s5 = StrBase(IntToSys(l5,16),6);
    uuid = concat_str_char(s,'-');
    uuid = concat(uuid,s1);
    uuid = concat_str_char(uuid,'-');
    uuid = concat(uuid,s2);
    uuid = concat_str_char(uuid,'-');
    uuid = concat(uuid,s3);
    uuid = concat_str_char(uuid,'-');
    uuid = concat(uuid,s4);
    uuid = concat(uuid,s5);
    return uuid;
}

char *IntToSys2(int aLiczba, int aBaza)
{
     char znaki[62] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
     char *wynik = "";
     int n = aLiczba, pom;
     do {
        pom = n % aBaza;
        wynik = concat_char_str(znaki[pom],wynik);
        n = div(n,aBaza).quot;
     } while (n!=0);
     return strdup(wynik);
}


int main() {

    char *s1 = IntToSys(500,2);
    char *s2 = IntToSys2(500,2);
    printf("s1 = %s\n",s1);
    printf("s2 = %s\n",s2);
    return 0;


    for (int i=1; i<20; i++) {
        char *tekst = GetUuid();
        printf("tekst = %s\n",tekst);
    }
}


