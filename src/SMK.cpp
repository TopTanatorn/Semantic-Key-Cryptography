#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

class Encryptor
{
private:
    unsigned char *key; //256 byte
    unsigned char key16[16]; //16 byte

public:
    Encryptor(unsigned char *k)
    {
        key = k;

        for (int i = 0; i < 256; i += 16)
        {
            for (int j = 0; j < 16; j++)
            {
                if (i < 16)
                {
                    key16[j] = key[j];
                }
                else
                {
                    key16[j] = key16[j] ^ key[i + j];
                }
            }
        }
        srand(time(0));
    }
    string encrypt(string txt)
    {
        int totalRounds = (txt.size() / 256);
        if (txt.size() % 256)
            totalRounds++;
        string cipher(totalRounds * 16 + txt.size(), 0);

        for (int i = 0; i < totalRounds; i++)
        {
            unsigned char randKey[16];
            int txtIndex = i * 256;
            int cipherIndex = i * (16 + 256);
            int txtSize = (i == (totalRounds - 1)) ? txt.size() % 256 : 256;

            for (int j = 0; j < 16; j++)
            {
                randKey[j] = random(1, 254);
                cipher[cipherIndex] = key16[j] ^ randKey[j];
                cipherIndex++;
            }

            for (int j = 0; j < txtSize; j++)
            {
                cipher[cipherIndex] = key[j] ^ randKey[j % 16] ^ txt[txtIndex];
                cipherIndex++;
                txtIndex++;
            }
        }
        return cipher;
    }
    string decrypt(string cipher)
    {
        int totalRounds = (cipher.size() / (256 + 16));
        if (cipher.size() % (256 + 16))
            totalRounds++;

        string txt(cipher.size() - totalRounds * 16, 0);

        for (int i = 0; i < totalRounds; i++)
        {
            unsigned char randKey[16];
            int txtIndex = i * 256;
            int cipherIndex = i * (16 + 256);
            int txtSize = (i == (totalRounds - 1)) ? (cipher.size() % (256 + 16)) - 16 : 256;

            for (int j = 0; j < 16; j++)
            {
                randKey[j] = cipher[cipherIndex] ^ key16[j];
                cipherIndex++;
            }

            for (int j = 0; j < txtSize; j++)
            {
                txt[txtIndex] = cipher[cipherIndex] ^ key[j] ^ randKey[j % 16];
                cipherIndex++;
                txtIndex++;
            }
        }
        return txt;
    }
    int random(int lower, int upper)
    {
        return (rand() % (upper - lower + 1)) + lower;
    }
};