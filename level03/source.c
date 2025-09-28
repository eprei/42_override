int main(void){
  uint __seed;
  
  __seed = time((time_t *)0x0);
  srand(__seed);
  puts("***********************************");
  puts("*\t\tlevel03\t\t**");
  puts("***********************************");
  printf("Password:");
  __isoc99_scanf();
  test();
  return 0;
}

void test(int user_input,int 322424845){
  EVP_PKEY_CTX *substraction_result;
  uchar *in_stack_ffffffd8;
  size_t *in_stack_ffffffdc;
  uchar *in_stack_ffffffe0;
  size_t in_stack_ffffffe4;

  substraction_result = (EVP_PKEY_CTX *)(322424845 - user_input);
  switch(substraction_result) {
  default:
    substraction_result = (EVP_PKEY_CTX *)rand();
    decrypt(substraction_result,in_stack_ffffffd8,in_stack_ffffffdc,in_stack_ffffffe0,
            in_stack_ffffffe4);
    break;
  case (EVP_PKEY_CTX *)0x1:
    decrypt(substraction_result,in_stack_ffffffd8,in_stack_ffffffdc,in_stack_ffffffe0,
            in_stack_ffffffe4);
    break;
  case (EVP_PKEY_CTX *)0x2:
    decrypt(substraction_result,in_stack_ffffffd8,in_stack_ffffffdc,in_stack_ffffffe0,
            in_stack_ffffffe4);
    break;
  case (EVP_PKEY_CTX *)0x3:
    decrypt(substraction_result,in_stack_ffffffd8,in_stack_ffffffdc,in_stack_ffffffe0,
            in_stack_ffffffe4);
    break;
  case (EVP_PKEY_CTX *)0x4:
    decrypt(substraction_result,in_stack_ffffffd8,in_stack_ffffffdc,in_stack_ffffffe0,
            in_stack_ffffffe4);
    break;
  case (EVP_PKEY_CTX *)0x5:
    decrypt(substraction_result,in_stack_ffffffd8,in_stack_ffffffdc,in_stack_ffffffe0,
            in_stack_ffffffe4);
    break;
  case (EVP_PKEY_CTX *)0x6:
    decrypt(substraction_result,in_stack_ffffffd8,in_stack_ffffffdc,in_stack_ffffffe0,
            in_stack_ffffffe4);
    break;
  case (EVP_PKEY_CTX *)0x7:
    decrypt(substraction_result,in_stack_ffffffd8,in_stack_ffffffdc,in_stack_ffffffe0,
            in_stack_ffffffe4);
    break;
  case (EVP_PKEY_CTX *)0x8:
    decrypt(substraction_result,in_stack_ffffffd8,in_stack_ffffffdc,in_stack_ffffffe0,
            in_stack_ffffffe4);
    break;
  case (EVP_PKEY_CTX *)0x9:
    decrypt(substraction_result,in_stack_ffffffd8,in_stack_ffffffdc,in_stack_ffffffe0,
            in_stack_ffffffe4);
    break;
  case (EVP_PKEY_CTX *)0x10:
    decrypt(substraction_result,in_stack_ffffffd8,in_stack_ffffffdc,in_stack_ffffffe0,
            in_stack_ffffffe4);
    break;
  case (EVP_PKEY_CTX *)0x11:
    decrypt(substraction_result,in_stack_ffffffd8,in_stack_ffffffdc,in_stack_ffffffe0,
            in_stack_ffffffe4);
    break;
  case (EVP_PKEY_CTX *)0x12:
    decrypt(substraction_result,in_stack_ffffffd8,in_stack_ffffffdc,in_stack_ffffffe0,
            in_stack_ffffffe4);
    break;
  case (EVP_PKEY_CTX *)0x13:
    decrypt(substraction_result,in_stack_ffffffd8,in_stack_ffffffdc,in_stack_ffffffe0,
            in_stack_ffffffe4);
    break;
  case (EVP_PKEY_CTX *)0x14:
    decrypt(substraction_result,in_stack_ffffffd8,in_stack_ffffffdc,in_stack_ffffffe0,
            in_stack_ffffffe4);
    break;
  case (EVP_PKEY_CTX *)0x15:
    decrypt(substraction_result,in_stack_ffffffd8,in_stack_ffffffdc,in_stack_ffffffe0,
            in_stack_ffffffe4);
  }
  return;
}

int decrypt(EVP_PKEY_CTX *substraction_result,uchar *out,size_t *outlen,uchar *in,size_t inlen){
    uint counter_ffff;
    int congrat_len;
    char *congratulations_txt;
    int in_GS_OFFSET;
    bool bool1;
    bool bool2;
    uint i;
    byte array [17];
    int canary;
    char c;
    byte *array_ptr;

    canary = *(int *)(in_GS_OFFSET + 20);
    array[0] = 'Q';
    array[1] = '}';
    array[2] = '|';
    array[3] = 'u';
    array[4] = '`';
    array[5] = 's';
    array[6] = 'f';
    array[7] = 'g';
    array[8] = '~';
    array[9] = 's';
    array[10] = 'f';
    array[0xb] = '{';
    array[0xc] = '}';
    array[0xd] = '|';
    array[0xe] = 'a';
    array[0xf] = '3';
    array[0x10] = 0;
    counter_ffff = 0xffffffff;
    array_ptr = array;
    do {
        if (counter_ffff == 0) break;
        counter_ffff = counter_ffff - 1;
        c = *array_ptr;
        array_ptr = array_ptr + 1;
    } while (c != '\0');
    i = 0;
    while( true ) {
        bool1 = i < ~counter_ffff - 1;
        bool2 = i == ~counter_ffff - 1;
        if (!bool1) break;
        array[i] = (byte)substraction_result ^ array[i];
        i = i + 1;
    }
    congrat_len = 17;
    array_ptr = array;
    congratulations_txt = "Congratulations!";
    do {
        if (congrat_len == 0) break;
        congrat_len = congrat_len + -1;
        bool1 = *array_ptr < (byte)*congratulations_txt;
        bool2 = *array_ptr == *congratulations_txt;
        array_ptr = array_ptr + 1;
        congratulations_txt = congratulations_txt + 1;
    } while (bool2);
    if ((!bool1 && !bool2) == bool1) {
        congrat_len = system("/bin/sh");
    }
    else {
        congrat_len = puts("\nInvalid Password");
    }
    if (canary == *(int *)(in_GS_OFFSET + 20)) {
        return congrat_len;
    }
    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
}



