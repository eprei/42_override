int main(void){
  uint maxUInt;
  char *ptr;
  byte zero;
  char buf [100];
  uint i;
  char c;
  
  zero = 0;
  fgets(buf,100,stdin);
  i = 0;
  do {
    maxUInt = 0xffffffff;
    ptr = buf;
    do {
      if (maxUInt == 0) break;
      maxUInt = maxUInt - 1;
      c = *ptr;
      ptr = ptr + (uint)zero * -2 + 1;
    } while (c != '\0');
    if (~maxUInt - 1 <= i) {
      printf(buf);
      exit(0);
    }
    if (('@' < buf[i]) && (buf[i] < '[')) {
      buf[i] = buf[i] ^ 0b00100000;
    }
    i = i + 1;
  } while( true );
}
