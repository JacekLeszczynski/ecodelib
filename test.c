#include "LibECode.c"

int main() {
    double d = fStringToDate("2023-01-19");
    int r,m,d2;
    printf("TDate = %f\n",d);
    DecodeDate(d,&r,&m,&d2);
    printf("rok=%i miesiąc=%i dzień=%i\n",r,m,d2);
    return 0;
}

/*
void display(char *tekst, char *s, int len)
{
  int i,a;
  printf("%s: ",tekst);
  for (i=0; i<len; i++)
  {
    a = s[i];
    //printf("%hhx ",s[i]);
    printf("%d ",s[i]);
  }
  printf("\n");
}

void main ()
{
  char *a;
  char *key = "ASDFKLZXCVBNM";
  char *IV = "1234567y90123456";
  char *c,*d;
  int len = 16,len2;

  a = malloc(1024);
  strcpy(a,"Ala ma kota!");

  display("Tekst do zaszyfrowania",a,strlen(a));
  len = StringEncrypt(&a,strlen(a),IV,key);
  display("Tekst zaszyfrowany",a,len);
  len2 = StringDecrypt(&a,len,IV,key);
  display("Tekst odszyfrowany",a,len2);

  free(a);
}
*/
