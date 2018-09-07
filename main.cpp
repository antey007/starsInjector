#include "Block.h"
#include "decryptor.h"

#include <string>
#include <stdio.h>

static const unsigned char ScanningMinelayer[82] = {7, 129, 29, 124, 136, 19, 13, 0, 0, 50, 0, 0, 0, 50, 0, 0, 0,
                                            1, 0, 15, 3,
                                            0, 1, 6, 3,
                                            0, 1, 6, 3,
                                            0, 1, 6, 3,
                                            0, 1, 6, 3,
                                            0, 1, 6, 3,
                                            0, 1, 6, 3,
                                            0, 8, 2, 3,
                                            0, 8, 2, 3,
                                            0, 8, 2, 3,
                                            0, 8, 2, 3,
                                            0, 8, 2, 3,
                                            2, 0, 12, 3,
                                            12, 194, 213, 22, 100, 109, 128, 219, 70, 37, 30, 34, 143};

int main(int argc, char *argv[])
{
    for (int file=0; file<2; file++) {
        FILE * file_in;
        FILE * file_out;
        switch(file) {
        case 0:
            file_in = fopen("test.hst","rb");
            file_out = fopen("test_modded.hst","wb");
            break;
        case 1:
            file_in = fopen("test.m1","rb");
            file_out = fopen("test_modded.m1","wb");
            break;
        }

        fseek (file_in, 0, SEEK_END);
        int size = ftell (file_in);
        fseek (file_in, 0, SEEK_SET);

        int size2 = size;

        Decryptor decryptor_in;
        Decryptor decryptor_out;
        unsigned char * data = new unsigned char[size];
        unsigned char * decryptedData = new unsigned char[size];
        unsigned char * newencryptedData = new unsigned char[size+100];
        for (int i = 0; i < size; i++) newencryptedData[i] = 0;

        int fileLength = fread((char*) data, 1, size, file_in);

        bool injected = false;
        bool added = false;

        int blockType;
        int blockSize, blockSize2;
        int offset, offset2;
        for (offset = 0, offset2 = 0; offset < fileLength; offset+=blockSize+2, offset2 += blockSize2+2) {
            blockType = (data[offset+1]>>2);
            blockSize = data[offset] + (data[offset+1]&0x03) * 256;
            blockSize2 = blockSize;

            //--------------- DECRYPTION
            if(blockType == BlockType::FILE_HEADER) {
                FileHeaderBlock header(data);
                decryptor_in.initDecryption(header);
                decryptor_out.initDecryption(header);

                printf("GameId: %x\n", header.gameId);
                printf("Version: %d.%d.%d\n", header.versionMajor, header.versionMinor, header.versionIncrement);
                printf("Turn: %d; Year: %d\n", header.turn, header.turn + 2400);
                printf("Player Number: %d; Displayed as: %d\n", header.playerNumber, header.playerNumber+1);
                printf("Encryption Salt: %x\n\n", header.encryptionSalt);

                memcpy(newencryptedData+offset2, data+offset, blockSize+2);

            } else {
                for(int i = 0; i < blockSize; i+=4) {
                    *(long*) (decryptedData+i) = *((long*) (data+offset+2+i)) ^ decryptor_in.starsRandom.nextRandom();
                }
                memcpy(newencryptedData+offset2, data+offset, 2);
                if(blockType == BlockType::DESIGN) {
                    if (!injected) {
                        injected = true;
                        newencryptedData[offset2] = 82;
                        memcpy(decryptedData, ScanningMinelayer, 82);
                        blockSize2 = 82;
                        size2 += blockSize2 - blockSize;
                    }
                }
                if(blockType == BlockType::FLEET) {
                    if (!added) {
                        added = true;
                        decryptedData[14] = 50;
                    }
                }
                for(int i = 0; i < blockSize2; i+=4) {
                    *(long*) (newencryptedData+offset2+2+i) = *((long*) (decryptedData+i)) ^ decryptor_out.starsRandom.nextRandom();
                }
            }
        }
        fwrite( (char*) newencryptedData, 1, size2, file_out);

        fclose(file_in);
        fclose(file_out);
    }

}
