//
// Created by Maryam Ashraf on 11/24/25.
// This is my hardened version of the CrackMe. I removed all the obvious stuff
// that Ghidra found in the original and tried to make it annoying to reverse.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/*
 * This transform function is how I hide the real password.
 * Instead of checking if the input equals 0x52b23, I run it through
 * a bunch of random-looking operations. Only the correct password
 * makes this function return 0. So Ghidra can't just show a simple compare.
 *
 * Also, the real password value is never written anywhere in the code.
 */
static uint32_t transform(uint32_t x) {
    x ^= 0xDEADB;          // first weird step
    x += 0x12345;          // add another random number
    x ^= 0x0F0F0;          // more XOR nonsense
    x += 0xFFF6EA33u;      // picked this so the correct password -> 0
    return x;
}

/*
 * XOR key for the encoded messages.
 * I made it volatile so the compiler does not optimize it away
 * or pre-decode the strings. This keeps the messages hidden in the binary.
 */
static volatile unsigned char XOR_KEY = 0x37;

/*
 * These are the XOR-encoded versions of:
 * "Password OK!!! :)"
 * I encoded each character by doing char ^ 0x37.
 * This way the real messages do NOT appear in strings or Ghidra.
 */
static const unsigned char OK_MSG_ENC[] = {
    0x67, 0x56, 0x44, 0x44, 0x40, 0x58, 0x45, 0x53,
    0x17, 0x78, 0x7c, 0x16, 0x16, 0x16, 0x17, 0x0d, 0x1e
};

/*
 * Same thing here but for:
 * "Invalid Password!"
 * Every character is XORed so the plaintext never shows up in the binary.
 */
static const unsigned char FAIL_MSG_ENC[] = {
    0x7e, 0x59, 0x41, 0x56, 0x5b, 0x5e, 0x53, 0x17,
    0x67, 0x56, 0x44, 0x44, 0x40, 0x58, 0x45, 0x53, 0x16
};

/*
 * This function actually decodes the XOR messages at runtime.
 * So the only time the real text exists is AFTER the user types something.
 * Reverse engineers can't find the string statically anymore.
 */
static void print_decoded(const unsigned char *enc, size_t len) {
    char buf[64];
    size_t i;

    // just making sure it doesn't overflow my small buffer
    if (len >= sizeof(buf)) {
        len = sizeof(buf) - 1;
    }

    // decode each byte using the volatile key
    for (i = 0; i < len; i++) {
        buf[i] = (char)(enc[i] ^ XOR_KEY);
    }
    buf[len] = '\0';

    puts(buf);
}

// easy wrappers for cleaner code
static void print_encoded_ok(void) {
    print_decoded(OK_MSG_ENC, sizeof(OK_MSG_ENC));
}

static void print_encoded_fail(void) {
    print_decoded(FAIL_MSG_ENC, sizeof(FAIL_MSG_ENC));
}

/*
 * This was part of the original program. I kept it just as extra noise.
 * It shifts a string by -3. Not super relevant but reverse engineers
 * have to mentally filter it out, which is funny.
 */
int shift(char *s)
{
    size_t sVar1;
    int iVar2;
    char v3[120];
    size_t i;

    i = 0;
    while (1) {
        sVar1 = strlen(s);
        if (sVar1 <= i) break;
        v3[i] = s[i] - 3;
        i++;
    }
    v3[i] = '\0';
    iVar2 = printf("%s\n", v3);
    return iVar2;
}

/*
 * This is the actual password check.
 * Instead of comparing the number directly, I use my transform() function.
 * If the result is 0, the input was the real password.
 * Otherwise it prints the fail message.
 */
int test(uint32_t a1)
{
    uint32_t t = transform(a1);

    if (t == 0) {
        print_encoded_ok();      // decoded only at runtime
        return 0;
    } else {
        print_encoded_fail();
        return -1;
    }
}

/*
 * Main program. Same idea as the original but with all the hardened logic.
 * It prints the banner, asks for the password, and calls test().
 */
int main(int argc, char **argv)
{
    unsigned int passwd;

    puts("IOLI Crackme Level 0x03 Hardened");
    printf("Password: ");

    if (scanf("%u", &passwd) != 1) {
        puts("Input error");
        return 1;
    }

    test(passwd);
    return 0;
}
