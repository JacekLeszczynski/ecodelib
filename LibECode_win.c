/* #BIBLIOTEKA_DLL:64: libecode64_c */
/* #BIBLIOTEKA_DLL:32: libecode_c */

#include <locale.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
//#include <cjson/cJSON.h>
//#include <curl/curl.h>
//#include <mcrypt.h>
//#include <openssl/sha.h>
//#include <openssl/bio.h>
//#include <openssl/evp.h>

#define SHA_DIGEST_LENGTH 20
#define BUFFER_SIZE 4096
#define MAX_LINE_LENGTH 1000

char *Realloc(char **a, int l) {
    char *temp = malloc(l);
    memcpy(temp, *a, l);
    free(*a);
    *a = temp;
    return *a;
}

void _trim(char *str)
{
    // Remove leading white space
    char* start = str;
    while (*start && isspace(*start)) {
        start++;
    }

    // Remove trailing white space
    char* end = start + strlen(start) - 1;
    while (end > start && isspace(*end)) {
        end--;
    }
    *(end+1) = '\0';

    // Shift string to remove leading white space
    if (start != str) {
        memmove(str, start, strlen(start)+1);
    }
}

/* FUNKCJE STANDARDOWE */

char fDecimalPoint() {
    struct lconv *locale_data;
    setlocale(LC_ALL, "");
    locale_data = localeconv();
    return locale_data->decimal_point[0];
}

int fGetLineToStr(char *aStr, int l, char separator, char textseparator, char *wynik, char **wartosc, int *wartosc_len) {
  int ll = l - 1,newlen;
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
      newlen = strlen(wynik);
      if (*wartosc_len<newlen+1) {
        if (*wartosc != NULL) free(*wartosc);
        *wartosc = malloc(newlen+1);
        *wartosc_len = newlen+1;
      }
      strcpy(*wartosc, wynik);
      (*wartosc)[newlen] = '\0';
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
  if (*wartosc_len<r_len+1) {
    if (*wartosc != NULL) free(*wartosc);
    *wartosc = malloc(r_len+1);
    *wartosc_len = r_len+1;
  }

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
      newlen = strlen(wynik);
      if (*wartosc_len<newlen+1) {
        if (*wartosc != NULL) free(*wartosc);
        *wartosc = malloc(newlen+1);
        *wartosc_len = newlen+1;
      }
      strcpy(*wartosc,wynik);
      (*wartosc)[newlen] = '\0';
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
  /*time_t czas = time(NULL);
  struct timespec ts;
  timespec_get(&ts, TIME_UTC);
  int h, m, s, ms, a;
  h = localtime(&czas)->tm_hour;
  m = localtime(&czas)->tm_min;
  s = localtime(&czas)->tm_sec;
  ms = round(ts.tv_nsec/1000000);
  a = (h * 60 * 60 * 1000) + (m * 60 * 1000) + (s * 1000) + ms;
  return a;*/ return 0;
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

int fReadConfigFile(const char* filename, char* buffer, size_t buffer_size)
{
    char file_path[MAX_LINE_LENGTH];
    snprintf(file_path, sizeof(file_path), "/etc/default/%s", filename);

    FILE* fp;
    char line[MAX_LINE_LENGTH];
    size_t buffer_pos = 0;

    fp = fopen(file_path, "r");
    if (fp == NULL) {
        //fprintf(stderr, "Error opening file %s\n", filename);
        return -1;
    }

    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
        // Skip lines that begin with '#', as they are comments
        if (line[0] == '#') continue;

        // Remove newline character from end of line
        line[strcspn(line, "\n")] = '\0';

        // Find first '=' character in line
        char* delimiter = strchr(line, '=');
        if (delimiter == NULL) continue;

        // Extract variable name and value from line
        size_t var_len = delimiter - line;
        size_t val_len = strlen(line) - var_len - 1;
        char* var = (char*) malloc((var_len + 1) * sizeof(char));
        char* val = (char*) malloc((val_len + 1) * sizeof(char));
        strncpy(var, line, var_len);
        strncpy(val, delimiter + 1, val_len);
        var[var_len] = '\0';
        val[val_len] = '\0';

        _trim(var);
        _trim(val);
        var_len = strlen(var);
        val_len = strlen(val);

        // Append variable and value to output buffer
        if (buffer_pos + var_len + val_len + 2 > buffer_size) {
            //fprintf(stderr, "Output buffer too small\n");
            free(var);
            free(val);
            return -2;
        }

        strcpy(buffer + buffer_pos, var);
        buffer_pos += var_len;
        buffer[buffer_pos++] = '=';
        strcpy(buffer + buffer_pos, val);
        buffer_pos += val_len;
        buffer[buffer_pos++] = ';';

        free(var);
        free(val);
    }

    // Null-terminate output buffer
    if (buffer!=NULL) buffer[buffer_pos-1] = '\0';

    fclose(fp);
    return strlen(buffer);
}

char *fReadConfigFileString(char *elementy, char *element) {
    int max = fGetLineCount(elementy,';','"');
    char *result = NULL;
    for (int i = 1; i<=max; i++) {
        char *s = NULL, *s1 = NULL, *s2 = NULL;
        int slen = 0, s1len = 0, s2len = 0;
        fGetLineToStr(elementy,i,';','"',"",&s,&slen);
        fGetLineToStr(s,1,'=','"',"",&s1,&s1len);
        int l = fGetLineToStr(s,2,'=','"',"",&s2,&s2len);
        if (strcmp(element,s1)==0) {
            result = malloc(l+1);
            strncpy(result,s2,l);
            result[l] = '\0';
            free(s);
            free(s1);
            free(s2);
            break;
        } else {
            free(s);
            free(s1);
            free(s2);
        }
    }
    return result;
}

/* KOD SCIĄGAJĄCY DANE Z SIECI */

// Struktura przechowująca dane pobrane z API YouTube
struct youtube_data {
    char buffer[BUFFER_SIZE];
    size_t size;
};

// Funkcja wywoływana przez cURL, która zapisuje dane w buforze
static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    struct youtube_data *data = (struct youtube_data *) userdata;
    size_t bytes = size * nmemb;

    if (data->size + bytes >= BUFFER_SIZE) {
        return 0;
    }

    memcpy(data->buffer + data->size, ptr, bytes);
    data->size += bytes;

    return bytes;
}

// Funkcja pobierająca dane JSON z YouTube API
int get_youtube_data(char *url, char **result) {
    /*int error = 0;
    CURL *curl;
    CURLcode res;
    struct youtube_data data = { "", 0 };

    // Inicjalizacja biblioteki cURL
    curl = curl_easy_init();

    if (curl) {
        // Ustawienie opcji biblioteki cURL
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &data);

        // Wykonanie żądania HTTP
        res = curl_easy_perform(curl);

        // Sprawdzenie czy wystąpił błąd
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            error = -1;
        }

        // Zakończenie połączenia cURL
        curl_easy_cleanup(curl);
    }

    *result = malloc(strlen(data.buffer)+1);
    strcpy(*result,data.buffer);
    return error;*/ return 0;
}

int fSetSizeMem(char **result, int size) {
    if (result == NULL) free(result);
    result = malloc(size);
    return size;
}

int fGetLiveChatId(char *video_id, char *api_key, char **result, int *len) {
    /*char url[BUFFER_SIZE];
    char *jsonString = NULL;
    char *activeLiveChatId,*message;
    int l1,l2;
    cJSON *stringValue,*arrayValue,*x1,*json,*numberValue;

    snprintf(url, BUFFER_SIZE, "https://www.googleapis.com/youtube/v3/videos?part=liveStreamingDetails&id=%s&key=%s", video_id, api_key);
    if (get_youtube_data(url,&jsonString)!=0) {
        free(jsonString);
        return -1;
    }

    json = cJSON_Parse(jsonString);
    if (json == NULL) {
        const char* errorPtr = cJSON_GetErrorPtr();
        if (*result != NULL) free(*result);
        if (errorPtr != NULL) {
          *len = strlen(errorPtr);
          *result = malloc(*len+1);
          strcpy(*result,errorPtr);
        } else {
          *len = 0;
          *result = malloc(1);
          strcpy(*result,"");
        }
        free(jsonString);
        return -2;
    }

    //TEST ERROR
    x1 = cJSON_GetObjectItemCaseSensitive(json, "error");
    if (cJSON_IsObject(x1) && (x1 != NULL)) {
        numberValue = cJSON_GetObjectItemCaseSensitive(x1, "code");
        if (cJSON_IsNumber(numberValue)) {
                l1 = (int)cJSON_GetNumberValue(numberValue);
        }
        stringValue = cJSON_GetObjectItemCaseSensitive(x1, "message");
        if (cJSON_IsString(stringValue) && (stringValue->valuestring != NULL)) {
            message = stringValue->valuestring;
        }
        if (*result != NULL) free(*result);
        if (message != NULL) {
          *len = strlen(message);
          *result = malloc(*len+1);
          strcpy(*result,message);
        } else {
          *len = 0;
          *result = malloc(1);
          strcpy(*result,"");
        }
        cJSON_Delete(json);
        free(jsonString);
        if (l1==0) {
            return -3;
        } else {
            return l1;
        }
    }

    //TEST
    x1 = cJSON_GetObjectItemCaseSensitive(json, "pageInfo");
    if (cJSON_IsObject(x1) && (x1 != NULL)) {
        numberValue = cJSON_GetObjectItemCaseSensitive(x1, "totalResults");
        if (cJSON_IsNumber(numberValue)) {
                l1 = (int)cJSON_GetNumberValue(numberValue);
        }
        numberValue = cJSON_GetObjectItemCaseSensitive(x1, "resultsPerPage");
        if (cJSON_IsNumber(numberValue)) {
                l2 = (int)cJSON_GetNumberValue(numberValue);
        }
    }
    if (l1==0 & l2==0) {
        if (*result != NULL) free(*result);
        *len = 0;
        *result = malloc(1);
        strcpy(*result,"");
        cJSON_Delete(json);
        free(jsonString);
        return 0;
    }

    //KOD GŁÓWNY
    arrayValue = cJSON_GetObjectItemCaseSensitive(json, "items");
    if (cJSON_IsArray(arrayValue)) {
        cJSON* arrayElement = NULL;
        int arraySize = cJSON_GetArraySize(arrayValue);

        arrayElement = cJSON_GetArrayItem(arrayValue, 0);

        x1 = cJSON_GetObjectItemCaseSensitive(arrayElement, "liveStreamingDetails");
        if (cJSON_IsObject(x1) && (x1 != NULL)) {
            stringValue = cJSON_GetObjectItemCaseSensitive(x1, "activeLiveChatId");
            if (cJSON_IsString(stringValue) && (stringValue->valuestring != NULL)) {
                activeLiveChatId = stringValue->valuestring;
            }
        }
    }

    *len = strlen(activeLiveChatId);
    if (*result != NULL) free(*result);
    *result = malloc(*len+1);
    strcpy(*result,activeLiveChatId);

    cJSON_Delete(json);
    free(jsonString);*/
    return 0;
}

int fGetLiveChatAll(char *video_id, char *api_key, char **result, int *len) {
    /*char url[BUFFER_SIZE];
    char *jsonString = NULL;
    char *kind,*etag,*message;
    char *items_kind,*items_etag,*items_id,*scheduledStartTime,*activeLiveChatId;
    int totalResults,resultsPerPage,a,l,l1,l2;
    cJSON *stringValue,*numberValue,*arrayValue,*x1,*x2,*json;

    snprintf(url, BUFFER_SIZE, "https://www.googleapis.com/youtube/v3/videos?part=liveStreamingDetails&id=%s&key=%s", video_id, api_key);
    if (get_youtube_data(url,&jsonString)!=0) {
        free(jsonString);
        return -1;
    }
    json = cJSON_Parse(jsonString);

    if (json == NULL) {
        const char* errorPtr = cJSON_GetErrorPtr();
        if (*result != NULL) free(*result);
        if (errorPtr != NULL) {
          *len = strlen(errorPtr);
          *result = malloc(*len+1);
          strcpy(*result,errorPtr);
        } else {
          *len = 0;
          *result = malloc(1);
          strcpy(*result,"");
        }
        free(jsonString);
        return -2;
    }

    //TEST ERROR
    x1 = cJSON_GetObjectItemCaseSensitive(json, "error");
    if (cJSON_IsObject(x1) && (x1 != NULL)) {
        numberValue = cJSON_GetObjectItemCaseSensitive(x1, "code");
        if (cJSON_IsNumber(numberValue)) {
                l1 = (int)cJSON_GetNumberValue(numberValue);
        }
        stringValue = cJSON_GetObjectItemCaseSensitive(x1, "message");
        if (cJSON_IsString(stringValue) && (stringValue->valuestring != NULL)) {
            message = stringValue->valuestring;
        }
        if (*result != NULL) free(*result);
        if (message != NULL) {
          *len = strlen(message);
          *result = malloc(*len+1);
          strcpy(*result,message);
        } else {
          *len = 0;
          *result = malloc(1);
          strcpy(*result,"");
        }
        cJSON_Delete(json);
        free(jsonString);
        if (l1==0) {
            return -3;
        } else {
            return l1;
        }
    }

    // Sprawdzam czy wogle są jakieś rekordy?
    x1 = cJSON_GetObjectItemCaseSensitive(json, "pageInfo");
    if (cJSON_IsObject(x1) && (x1 != NULL)) {
        numberValue = cJSON_GetObjectItemCaseSensitive(x1, "totalResults");
        if (cJSON_IsNumber(numberValue)) {
                totalResults = (int)cJSON_GetNumberValue(numberValue);
        }
        numberValue = cJSON_GetObjectItemCaseSensitive(x1, "resultsPerPage");
        if (cJSON_IsNumber(numberValue)) {
                resultsPerPage = (int)cJSON_GetNumberValue(numberValue);
        }
    }
    if (totalResults==0 & resultsPerPage==0) {
        if (*result != NULL) free(*result);
        *len = 0;
        *result = malloc(1);
        strcpy(*result,"");
        cJSON_Delete(json);
        free(jsonString);
        return 0;
    }
    // Dostęp do wartości JSON typu string:
    stringValue = cJSON_GetObjectItemCaseSensitive(json, "kind");
    if (cJSON_IsString(stringValue) && (stringValue->valuestring != NULL)) {
        kind = stringValue->valuestring;
    }
    stringValue = cJSON_GetObjectItemCaseSensitive(json, "etag");
    if (cJSON_IsString(stringValue) && (stringValue->valuestring != NULL)) {
        etag = stringValue->valuestring;
    }
    // Dostęp do wartości JSON typu array:
    arrayValue = cJSON_GetObjectItemCaseSensitive(json, "items");
    if (cJSON_IsArray(arrayValue)) {
        cJSON* arrayElement = NULL;
        int arraySize = cJSON_GetArraySize(arrayValue);

        arrayElement = cJSON_GetArrayItem(arrayValue, 0);

        stringValue = cJSON_GetObjectItemCaseSensitive(arrayElement, "kind");
        if (cJSON_IsString(stringValue) && (stringValue->valuestring != NULL)) {
            items_kind = stringValue->valuestring;
        }
        stringValue = cJSON_GetObjectItemCaseSensitive(arrayElement, "etag");
        if (cJSON_IsString(stringValue) && (stringValue->valuestring != NULL)) {
            items_etag = stringValue->valuestring;
        }
        stringValue = cJSON_GetObjectItemCaseSensitive(arrayElement, "id");
        if (cJSON_IsString(stringValue) && (stringValue->valuestring != NULL)) {
            items_id = stringValue->valuestring;
        }
        x1 = cJSON_GetObjectItemCaseSensitive(arrayElement, "liveStreamingDetails");
        if (cJSON_IsObject(x1) && (x1 != NULL)) {
            stringValue = cJSON_GetObjectItemCaseSensitive(x1, "scheduledStartTime");
            if (cJSON_IsString(stringValue) && (stringValue->valuestring != NULL)) {
                scheduledStartTime = stringValue->valuestring;
            }
            stringValue = cJSON_GetObjectItemCaseSensitive(x1, "activeLiveChatId");
            if (cJSON_IsString(stringValue) && (stringValue->valuestring != NULL)) {
                activeLiveChatId = stringValue->valuestring;
            }
        }
    }

    l1 = snprintf(NULL, 0, "%d", totalResults);
    l2 = snprintf(NULL, 0, "%d", resultsPerPage);

    l = strlen(kind)+strlen(etag)+strlen(items_kind)+strlen(items_etag)+strlen(items_id)+strlen(scheduledStartTime)+strlen(activeLiveChatId)+l1+l2+9;
    if (*result != NULL) free(*result);
    *result = malloc(l);
    a = 0;
    strcpy(&(*result)[a],kind);
    a += strlen(kind);
    strcpy(&(*result)[a],",");
    a++;
    strcpy(&(*result)[a],etag);
    a += strlen(etag);
    strcpy(&(*result)[a],",");
    a++;
    strcpy(&(*result)[a],items_kind);
    a += strlen(items_kind);
    strcpy(&(*result)[a],",");
    a++;
    strcpy(&(*result)[a],items_etag);
    a += strlen(items_etag);
    strcpy(&(*result)[a],",");
    a++;
    strcpy(&(*result)[a],items_id);
    a += strlen(items_id);
    strcpy(&(*result)[a],",");
    a++;
    strcpy(&(*result)[a],scheduledStartTime);
    a += strlen(scheduledStartTime);
    strcpy(&(*result)[a],",");
    a++;
    strcpy(&(*result)[a],activeLiveChatId);
    a += strlen(activeLiveChatId);
    strcpy(&(*result)[a],",");
    a++;
    snprintf(&(*result)[a], l1 + 1, "%d", totalResults);
    a += l1;
    strcpy(&(*result)[a],",");
    a++;
    snprintf(&(*result)[a], l2 + 1, "%d", resultsPerPage);

    cJSON_Delete(json);
    free(jsonString);
    *len = strlen(*result);*/
    return 0;
}

int fGetLiveChatText(char *chat_id, char *api_key, char **aPageToken, int *aPageTokenLen, char **result, int *len, int *aPollingIntervalMillis) {
    /*char url[BUFFER_SIZE];
    char *jsonString = NULL,*message,*nextPageToken;
    int a,i,l,l1,l2,l3,pollingIntervalMillis,totalResults,resultsPerPage,licznik=0;
    cJSON *stringValue,*arrayValue,*x1,*x2,*json,*numberValue;
    char z1[] = "A",z2[] = "A"; z1[0] = 1; z2[0] = 2;

    if (*aPageToken == NULL) {
        snprintf(url, BUFFER_SIZE, "https://www.googleapis.com/youtube/v3/liveChat/messages?liveChatId=%s&part=snippet,authorDetails&key=%s", chat_id, api_key);
    } else {
        snprintf(url, BUFFER_SIZE, "https://www.googleapis.com/youtube/v3/liveChat/messages?liveChatId=%s&part=snippet,authorDetails&key=%s&pageToken=%s", chat_id, api_key, *aPageToken);
    }

    if (get_youtube_data(url,&jsonString)!=0) {
        free(jsonString);
        return -1;
    }

    //printf("Długość ramki wewn. = %d\n",strlen(jsonString));
    json = cJSON_Parse(jsonString);
    if (json == NULL) {
        const char* errorPtr = cJSON_GetErrorPtr();
        if (*result != NULL) free(*result);
        if (errorPtr != NULL) {
          *len = strlen(errorPtr);
          *result = malloc(*len+1);
          strcpy(*result,errorPtr);
        } else {
          *len = 0;
          *result = malloc(1);
          strcpy(*result,"");
        }
        free(jsonString);
        return -2;
    }

    //TEST ERROR
    x1 = cJSON_GetObjectItemCaseSensitive(json, "error");
    if (cJSON_IsObject(x1) && (x1 != NULL)) {
        numberValue = cJSON_GetObjectItemCaseSensitive(x1, "code");
        if (cJSON_IsNumber(numberValue)) {
                l1 = (int)cJSON_GetNumberValue(numberValue);
        }
        stringValue = cJSON_GetObjectItemCaseSensitive(x1, "message");
        if (cJSON_IsString(stringValue) && (stringValue->valuestring != NULL)) {
            message = stringValue->valuestring;
        }
        if (*result != NULL) free(*result);
        if (message != NULL) {
          *len = strlen(message);
          *result = malloc(*len+1);
          strcpy(*result,message);
        } else {
          *len = 0;
          *result = malloc(1);
          strcpy(*result,"");
        }
        cJSON_Delete(json);
        free(jsonString);
        if (l1==0) {
            return -3;
        } else {
            if (l1>0) l1 = l1 * (-1);
            return l1;
        }
    }

    //NAGŁÓWEK
    numberValue = cJSON_GetObjectItemCaseSensitive(json, "pollingIntervalMillis");
    if (cJSON_IsNumber(numberValue)) {
        pollingIntervalMillis = (int)cJSON_GetNumberValue(numberValue);
    }
    *aPollingIntervalMillis = pollingIntervalMillis;
    x1 = cJSON_GetObjectItemCaseSensitive(json, "pageInfo");
    if (cJSON_IsObject(x1) && (x1 != NULL)) {
        numberValue = cJSON_GetObjectItemCaseSensitive(x1, "totalResults");
        if (cJSON_IsNumber(numberValue)) {
            totalResults = (int)cJSON_GetNumberValue(numberValue);
        }
        numberValue = cJSON_GetObjectItemCaseSensitive(x1, "resultsPerPage");
        if (cJSON_IsNumber(numberValue)) {
            resultsPerPage = (int)cJSON_GetNumberValue(numberValue);
        }
    }
    stringValue = cJSON_GetObjectItemCaseSensitive(json, "nextPageToken");
    if (cJSON_IsString(stringValue) && (stringValue->valuestring != NULL)) {
        nextPageToken = stringValue->valuestring;
    }
    l = strlen(nextPageToken);
    if (*aPageTokenLen != l | *aPageToken == NULL) {
        if (*aPageToken != NULL) free(*aPageToken);
        *aPageToken = malloc(l+1);
        *aPageTokenLen = l;
    }
    strcpy(*aPageToken,nextPageToken);

    if (totalResults==0 & resultsPerPage==0) {
        cJSON_Delete(json);
        free(jsonString);
        return 0;
    }

    //DANE
    arrayValue = cJSON_GetObjectItemCaseSensitive(json, "items");
    if (cJSON_IsArray(arrayValue)) {
        cJSON* arrayElement = NULL;
        int arraySize = cJSON_GetArraySize(arrayValue);
        char *type,*publishedAt,*messageText;
        char *displayName,*profileImageUrl;
        bool hasDisplayContent,isVerified,isChatOwner,isChatSponsor,isChatModerator;
        *len = 0;
        a = 0;
        if (*result == NULL) *result = malloc(10000);
        for (int i = 0; i < arraySize; i++) {
            arrayElement = cJSON_GetArrayItem(arrayValue, i);

            x1 = cJSON_GetObjectItemCaseSensitive(arrayElement, "snippet");
            if (cJSON_IsObject(x1) && (x1 != NULL)) {
                stringValue = cJSON_GetObjectItemCaseSensitive(x1, "type");
                if (cJSON_IsString(stringValue) && (stringValue->valuestring != NULL)) {
                    type = stringValue->valuestring;
                }
                if (strcmp(type,"textMessageEvent")!=0) continue;
                stringValue = cJSON_GetObjectItemCaseSensitive(x1, "publishedAt");
                if (cJSON_IsString(stringValue) && (stringValue->valuestring != NULL)) {
                    publishedAt = stringValue->valuestring;
                }
                cJSON* boolValue = cJSON_GetObjectItemCaseSensitive(x1, "hasDisplayContent");
                if (cJSON_IsBool(boolValue)) {
                    if (cJSON_IsTrue(boolValue)) {
                        hasDisplayContent = 1;
                    } else {
                        hasDisplayContent = 0;
                    }
                }
                x2 = cJSON_GetObjectItemCaseSensitive(x1, "textMessageDetails");
                if (cJSON_IsObject(x2) && (x2 != NULL)) {
                    stringValue = cJSON_GetObjectItemCaseSensitive(x2, "messageText");
                    if (cJSON_IsString(stringValue) && (stringValue->valuestring != NULL)) {
                        messageText = stringValue->valuestring;
                    }
                }
            }
            x1 = cJSON_GetObjectItemCaseSensitive(arrayElement, "authorDetails");
            cJSON* boolValue;
            if (cJSON_IsObject(x1) && (x1 != NULL)) {
                stringValue = cJSON_GetObjectItemCaseSensitive(x1, "displayName");
                if (cJSON_IsString(stringValue) && (stringValue->valuestring != NULL)) {
                    displayName = stringValue->valuestring;
                }
                stringValue = cJSON_GetObjectItemCaseSensitive(x1, "profileImageUrl");
                if (cJSON_IsString(stringValue) && (stringValue->valuestring != NULL)) {
                    profileImageUrl = stringValue->valuestring;
                }
                boolValue = cJSON_GetObjectItemCaseSensitive(x1, "isVerified");
                if (cJSON_IsBool(boolValue)) {
                    if (cJSON_IsTrue(boolValue)) {
                        isVerified = 1;
                    } else {
                        isVerified = 0;
                    }
                }
                boolValue = cJSON_GetObjectItemCaseSensitive(x1, "isChatOwner");
                if (cJSON_IsBool(boolValue)) {
                    if (cJSON_IsTrue(boolValue)) {
                        isChatOwner = 1;
                    } else {
                        isChatOwner = 0;
                    }
                }
                boolValue = cJSON_GetObjectItemCaseSensitive(x1, "isChatSponsor");
                if (cJSON_IsBool(boolValue)) {
                    if (cJSON_IsTrue(boolValue)) {
                        isChatSponsor = 1;
                    } else {
                        isChatSponsor = 0;
                    }
                }
                boolValue = cJSON_GetObjectItemCaseSensitive(x1, "isChatModerator");
                if (cJSON_IsBool(boolValue)) {
                    if (cJSON_IsTrue(boolValue)) {
                        isChatModerator = 1;
                    } else {
                        isChatModerator = 0;
                    }
                }
            }
            licznik++;
            *len += strlen(publishedAt)+strlen(messageText)+
                    strlen(displayName)+strlen(profileImageUrl)+14;
            strcpy(&(*result)[a],displayName); a += strlen(displayName); strcpy(&(*result)[a],z1); a++;
            strcpy(&(*result)[a],messageText); a += strlen(messageText); strcpy(&(*result)[a],z1); a++;
            strcpy(&(*result)[a],publishedAt); a += strlen(publishedAt); strcpy(&(*result)[a],z1); a++;
            strcpy(&(*result)[a],profileImageUrl); a += strlen(profileImageUrl); strcpy(&(*result)[a],z1); a++;
            if (hasDisplayContent) {
                strcpy(&(*result)[a],"1"); a++; strcpy(&(*result)[a],z1); a++;
            } else {
                strcpy(&(*result)[a],"0"); a++; strcpy(&(*result)[a],z1); a++;
            }
            if (isVerified) {
                strcpy(&(*result)[a],"1"); a++; strcpy(&(*result)[a],z1); a++;
            } else {
                strcpy(&(*result)[a],"0"); a++; strcpy(&(*result)[a],z1); a++;
            }
            if (isChatOwner) {
                strcpy(&(*result)[a],"1"); a++; strcpy(&(*result)[a],z1); a++;
            } else {
                strcpy(&(*result)[a],"0"); a++; strcpy(&(*result)[a],z1); a++;
            }
            if (isChatSponsor) {
                strcpy(&(*result)[a],"1"); a++; strcpy(&(*result)[a],z1); a++;
            } else {
                strcpy(&(*result)[a],"0"); a++; strcpy(&(*result)[a],z1); a++;
            }
            if (isChatModerator) {
                strcpy(&(*result)[a],"1"); a++; strcpy(&(*result)[a],z1); a++;
            } else {
                strcpy(&(*result)[a],"0"); a++; strcpy(&(*result)[a],z2); a++;
            }
        }
        //*result = realloc(*result,*len+1);
    }

    //Kończenie
    cJSON_Delete(json);
    free(jsonString);
    return licznik;
    */ return 0;
}

/* FUNKCJE SZYFRUJĄCE */

int _encrypt(void* buffer, int buffer_len, char* IV, char* key, int key_len) {
  /*MCRYPT td = mcrypt_module_open("rijndael-128", NULL, "cbc", NULL);
  int blocksize = mcrypt_enc_get_block_size(td);
  if( buffer_len % blocksize != 0 ){return 1;}
  mcrypt_generic_init(td, key, key_len, IV);
  mcrypt_generic(td, buffer, buffer_len);
  mcrypt_generic_deinit (td);
  mcrypt_module_close(td);*/
  return 0;
}

int _decrypt(void* buffer, int buffer_len, char* IV, char* key, int key_len) {
  /*MCRYPT td = mcrypt_module_open("rijndael-128", NULL, "cbc", NULL);
  int blocksize = mcrypt_enc_get_block_size(td);
  if( buffer_len % blocksize != 0 ){return 1;}
  mcrypt_generic_init(td, key, key_len, IV);
  mdecrypt_generic(td, buffer, buffer_len);
  mcrypt_generic_deinit (td);
  mcrypt_module_close(td);*/
  return 0;
}

void _SHA1Hash(char *ALFA, unsigned char *hash) {
    //SHA1((const unsigned char*) ALFA, strlen(ALFA), hash);
}

char *_HashSHA1Create(char *text) {
    unsigned char hash[SHA_DIGEST_LENGTH];
    _SHA1Hash(text,hash);
    int i,j=0;
    char *result = malloc(SHA_DIGEST_LENGTH*2+1);
    for (i = 0; i < SHA_DIGEST_LENGTH; i++) {
        snprintf(&result[j],2+1,"%02x",hash[i]);
        j = j + 2;
    }
    return result;
}

char *fBase64EncodeBin(const char *bufor, int len) {
    /*int encoded_len = EVP_ENCODE_LENGTH(len);
    char *encoded_text = (char*) malloc(encoded_len);
    memset(encoded_text,0,encoded_len);
    int encoded_len_actual = EVP_EncodeBlock((unsigned char*) encoded_text, (const unsigned char*) bufor, len);
    return encoded_text;*/
    return '\0';
}

int fBase64DecodeBin(const char *text, char **bufor) {
    /*int len = strlen(text), pom = 0;
    if (text[len-1] == '=') pom++;
    if (text[len-2] == '=') pom++;
    int decoded_len = len*3/4;  // estymowana długość tekstu po dekodowaniu
    if (*bufor != NULL) free(*bufor);
    *bufor = (char*) malloc(decoded_len);
    memset(*bufor, 0, decoded_len);
    int l = EVP_DecodeBlock((unsigned char*) *bufor, (const unsigned char*) text, len);
    return l - pom;*/
    return 0;
}

int fCalcBuffer(int aLen)
{
  div_t o = div(aLen,16);
  int r = o.quot;
  if (o.rem>0) r++;
  return r*16;
}

int fEncryptString(char **buf, int len, char *IV, char *key)
{
  char *tmp = *buf;
  int l = fCalcBuffer(len);
  if (len == 0) len = strlen(tmp);
  _encrypt(tmp,l,IV,key,strlen(key));
  return l;
}

int fDecryptString(char **buf, int len, char *IV, char *key)
{
  char *tmp = *buf;
  _decrypt(tmp,len,IV,key,strlen(key));
  return strlen(tmp);
}

int fEncryptBinary(char **buf, int len, char *IV, char *key)
{
  char *tmp = *buf;
  int l1 = snprintf(NULL,0,"%d",len);
  int l = fCalcBuffer(len+l1+1);
  snprintf(&tmp[l-l1-1],l1+1,"%d",len);
  snprintf(&tmp[l-1],2,"%d",l1);
  _encrypt(tmp,l,IV,key,strlen(key));
  return l;
}

int fDecryptBinary(char **buf, int len, char *IV, char *key)
{
  char *tmp = *buf, s1[2], s2[20];
  int a;
  _decrypt(tmp,len,IV,key,strlen(key));
  strncpy(s1,&tmp[len-1],1);
  s1[1]='\0';
  a = atoi(s1);
  strncpy(s2,&tmp[len-1-a],a);
  s2[a]='\0';
  a = atoi(s2);
  return a;
}

int fEncryptStringOnlyPassword(char **buf, int len, char *key)
{
    char *hash = _HashSHA1Create(key);
    int l = fEncryptString(buf,len,hash,key);
    free(hash);
    return l;
}

int fDecryptStringOnlyPassword(char **buf, int len, char *key)
{
    char *hash = _HashSHA1Create(key);
    int l = fDecryptString(buf,len,hash,key);
    free(hash);
    return l;
}

char *fEncryptStr(char *text, char *key)
{
    int text_len = strlen(text);
    int buf_size = fCalcBuffer(text_len);
    char *buf = malloc(buf_size);
    strcpy(buf,text);
    int len = fEncryptStringOnlyPassword(&buf,text_len,key);
    return fBase64EncodeBin(buf,len);
}

char *fDecryptStr(char *text, char *key)
{
    char *buf = NULL, *result = NULL;
    int len = fBase64DecodeBin(text,&buf);
    len = fDecryptStringOnlyPassword(&buf,len,key);
    result = malloc(len+1);
    strncpy(result,buf,len);
    result[len] = '\0';
    return result;
}

