/* BIBLIOTEKA: libecode_c */
/* -lmcrypt */

#include <locale.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
//#include <mcrypt.h>

char *Realloc(char **a, int l) {
    char *temp = malloc(l);
    memcpy(temp, *a, l);
    free(*a);
    *a = temp;
    return *a;
}

/* FUNKCJE STANDARDOWE */

char fDecimalPoint() {
    struct lconv *locale_data;
    setlocale(LC_ALL, "");
    locale_data = localeconv();
    return locale_data->decimal_point[0];
}

int fGetLineToStr(char *aStr, int l, char separator, char textseparator, char *wynik, char **wartosc) {
  int ll = l - 1;
  char *start, *end;
  bool in_quotes = false;

  // Find start of specified element
  start = aStr;
  for (int i = 0; i < ll; i++) {
    if (*start == textseparator) {
      start++;
      while (*start != textseparator) {
        start++;
      }
    }
    start = strchr(start, separator);
    if (start == NULL) {
      if (*wartosc != NULL) free(*wartosc);
      *wartosc = malloc(strlen(wynik) + 1);
      strcpy(*wartosc, wynik);
      return strlen(*wartosc);
    }
    start++;
  }

  // Find end of element
  end = start;
  while (*end != '\0') {
    if (*end == textseparator) {
      in_quotes = !in_quotes;
    }
    if (!in_quotes && *end == separator) {
      break;
    }
    end++;
  }

  // Declare result for element string
  int r_len = end - start;
  if (*wartosc != NULL) free(*wartosc);
  *wartosc = malloc(r_len + 1);

  // Copy element string to result and add null terminator
  if (start[0] == textseparator) {
    memcpy(*wartosc, start + 1, r_len - 2);
    (*wartosc)[r_len - 2] = '\0';
  } else {
    memcpy(*wartosc, start, r_len);
    (*wartosc)[r_len] = '\0';
  }

  // Return element string or wynik if element is empty
  if ((*wartosc)[0] == '\0') {
      if (*wartosc != NULL) free(*wartosc);
      *wartosc = malloc(strlen(wynik) + 1);
      strcpy(*wartosc,wynik);
  }
  return strlen(*wartosc);
}

int fGetLineCount(char *aStr, char separator, char textseparator) {
  int element_count = 1;
  char *start, *end;
  bool in_quotes = false;

  start = aStr;
  end = start;

  while (*end != '\0') {
    if (*end == textseparator) in_quotes = !in_quotes;
    if (!in_quotes && *end == separator) element_count++;
    end++;
  }

  if (*aStr == '\0') {
    return 0;
  } else {
    return element_count;
  }
}

bool fIsSpace(char c) {
  return isspace(c);
}

bool fIsDigit(char c) {
  return isdigit(c);
}

int fStrToL(char *s, char **reszta, int baza) {
  return (strtol(s,reszta,baza));
}

double fStrToD(char *s, char **reszta) {
  return (strtod(s,reszta));
}

int fAToI(char *s) {
  return atoi(s);
}

int fGToS(double d, int l, char** result) {
    int dl = (int)floor(log10(fabs(d))) + l + 3;
    if (*result != NULL) free(*result);
    *result = malloc(dl);
    snprintf(*result, dl, "%.*f", l, d);
    return strlen(*result);
}

void fQSort(void *base, size_t num, size_t size, int (*compar)(const void *, const void*)) {
    qsort(base,num,size,compar);
}

unsigned long fDecodeTimeToInteger(unsigned short Hour, unsigned short Minutes, unsigned short Second, unsigned short Milisecond) {
    return (Hour*60*60*1000)+(Minutes*60*1000)+(Second*1000)+Milisecond;
}

unsigned long fTimeToInteger(double Time) {
    int hour, min, sec, ms;
    ms = (int)(Time * 86400000) % 1000;
    sec = (int)(Time * 86400) % 60;
    min = (int)(Time * 1440) % 60;
    hour = (int)(Time * 24) % 24;
    return (hour * 60 * 60 * 1000) + (min * 60 * 1000) + (sec * 1000) + ms;
}

unsigned long fNowTimeToInteger()
{
  time_t czas = time(NULL);
  struct timespec ts;
  timespec_get(&ts, TIME_UTC);
  int h, m, s, ms, a;
  h = localtime(&czas)->tm_hour;
  m = localtime(&czas)->tm_min;
  s = localtime(&czas)->tm_sec;
  ms = round(ts.tv_nsec/1000000);
  a = (h * 60 * 60 * 1000) + (m * 60 * 1000) + (s * 1000) + ms;
  return a;
}

double fIntegerToTime(unsigned long uczas) {
  unsigned long c = uczas;
  int hour, min, sec, ms;
  double wynik;
  hour = round(c / 3600000);
  c = c - (hour * 3600000);
  min = round(c / 60000);
  c = c - (min * 60000);
  sec = round(c / 1000);
  ms = c - (sec * 1000);
  wynik = (hour * 3600 + min * 60 + sec + ms / 1000.0) / 86400;
  return wynik;
}

double fIntegerToTimeNoMs(unsigned long uczas) {
  unsigned long c = uczas;
  int hour, min, sec, ms;
  double wynik;
  hour = round(c / 3600000);
  c = c - (hour * 3600000);
  min = round(c / 60000);
  c = c - (min * 60000);
  sec = round(c / 1000);
  ms = 0;
  wynik = (hour * 3600 + min * 60 + sec + ms / 1000.0) / 86400;
  return wynik;
}

unsigned long fTimeTruncateInt(unsigned long a) {
  return fTimeToInteger(fIntegerToTimeNoMs(a));
}

double fTimeTruncateDT(double a) {
  return fIntegerToTimeNoMs(fTimeToInteger(a));
}

double fSecToTime(unsigned long aSec) {
  return aSec/86400.0;
}

unsigned long fSecToInteger(unsigned long aSec) {
  return fTimeToInteger(aSec/86400.0);
}

double fMiliSecToTime(unsigned long aMiliSec) {
  return aMiliSec/1000/86400.0;
}

unsigned long fMiliSecToInteger(unsigned long aMiliSec) {
  return fTimeToInteger(aMiliSec/1000/86400.0);
}

void DecodeDate(double Date, int* Year, int* Month, int* Day) {
  unsigned int ly, ld, lm, j;
  if (Date <= -693594) {
    *Year = 0;
    *Month = 0;
    *Day = 0;
  }
  else {
    if (Date > 0) {
      Date = Date + 1.0 / (86400000 * 2);
    }
    else {
      Date = Date - 1.0 / (86400000 * 2);
    }
    if (Date > 2958465.99999999) {
      Date = 2958465.99999999;
    }
    j = (int)((int)(Date) + 693900) << 2;
    j--;
    ly = trunc(j / 146097);
    j = j - 146097 * ly;
    ld = j >> 2;
    j = trunc(((ld << 2) + 3) / 1461);
    ld = ((ld << 2) + 7 - 1461 * j) >> 2;
    lm = trunc((5 * ld - 3) / 153);
    ld = trunc((5 * ld + 2 - 153 * lm) / 5);
    ly = 100 * ly + j;
    if (lm < 10) {
      lm += 3;
    }
    else {
      lm -= 9;
      ly++;
    }
    *Year = ly;
    *Month = lm;
    *Day = ld;
  }
}

int MonthDays[2][12] = {
  {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
  {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

bool IsLeapYear(unsigned int Year) {
  return (Year % 4 == 0) && ((Year % 100 != 0) || (Year % 400 == 0));
}

bool EncodeDate(unsigned int Year, unsigned int Month, unsigned int Day, double* Date) {
  unsigned int c, ya;
  bool Result = (Year > 0) && (Year < 10000) &&
                (Month >= 1 && Month <= 12) &&
                (Day > 0) && (Day <= MonthDays[IsLeapYear(Year)][Month]);
  if (Result) {
    if (Month > 2) {
      Month -= 3;
    }
    else {
      Month += 9;
      Year--;
    }
    c = Year / 100;
    ya = Year - 100 * c;
    *Date = ((146097 * c) >> 2) + ((1461 * ya) >> 2) + (153 * Month + 2) / 5 + Day;
    *Date = *Date - 693900;
  }
  return Result;
}

double fStringToDate(char *str) {
  int rok,miesiac,dzien;
  char buf[5];
  int i,len = strlen(str), licznik = 0, stat = 1;
  double wynik;
  bool b;
  for (i = 0; i<len; i++) {
    if (str[i] == '-') {
      buf[licznik] = '\0';
      if (stat == 1) {
        rok = atoi(buf);
      } else {
        miesiac = atoi(buf);
      }
      licznik = 0;
      stat++;
      continue;
    }
    buf[licznik] = str[i];
    licznik++;
  }
  buf[licznik] = '\0';
  dzien = atoi(buf);
  b = EncodeDate(rok,miesiac,dzien,&wynik);
  if (b) {
    return wynik;
  } else {
    return 0;
  }
}

/* FUNKCJE SZYFRUJĄCE */

/*int _encrypt(void* buffer, int buffer_len, char* IV, char* key, int key_len) {
  MCRYPT td = mcrypt_module_open("rijndael-128", NULL, "cbc", NULL);
  int blocksize = mcrypt_enc_get_block_size(td);
  if( buffer_len % blocksize != 0 ){return 1;}
  mcrypt_generic_init(td, key, key_len, IV);
  mcrypt_generic(td, buffer, buffer_len);
  mcrypt_generic_deinit (td);
  mcrypt_module_close(td);
  return 0;
}

int _decrypt(void* buffer, int buffer_len, char* IV, char* key, int key_len) {
  MCRYPT td = mcrypt_module_open("rijndael-128", NULL, "cbc", NULL);
  int blocksize = mcrypt_enc_get_block_size(td);
  if( buffer_len % blocksize != 0 ){return 1;}
  mcrypt_generic_init(td, key, key_len, IV);
  mdecrypt_generic(td, buffer, buffer_len);
  mcrypt_generic_deinit (td);
  mcrypt_module_close(td);
  return 0;
}

int CalcBuffer(int aLen)
{
  div_t o = div(aLen,16);
  int r = o.quot;
  if (o.rem>0) r++;
  return r*16;
}

int StringEncrypt(char **buf, int len, char *IV, char *key)
{
  char *tmp = *buf;
  int l = CalcBuffer(len);
  if (len == 0) len = strlen(tmp);
  _encrypt(tmp,l,IV,key,strlen(key));
  return l;
}

int StringDecrypt(char **buf, int len, char *IV, char *key)
{
  char *tmp = *buf;
  int l = len;

  _decrypt(tmp,len,IV,key,strlen(key));
  l = strlen(tmp);
  return l;
}*/

