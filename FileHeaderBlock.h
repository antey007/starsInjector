#ifndef FILEHEADERBLOCK_H
#define FILEHEADERBLOCK_H

#include "Block.h"

//#include <iostream>
//using namespace std;

// Header data
//static const char magicNumberData[] = { 74, 51, 74, 51 } ;

class FileHeaderBlock : public Block {
public:
    long gameId;
    int versionData; // 2.6JRC4
    int versionMajor;
    int versionMinor;
    int versionIncrement;
    int turn;
    int playerNumber;  // zero-indexed
    int encryptionSalt; // just a number seen in a real file
    int fileType;
    char unknownBits;
    bool turnSubmitted;
    bool hostUsing;
    bool multipleTurns;
    bool gameOver;
    bool shareware;

    FileHeaderBlock()
        :Block()
    {
        typeId = BlockType::FILE_HEADER;
    }

    FileHeaderBlock(unsigned char * data)
        :Block()
    {
        typeId = BlockType::FILE_HEADER;
        gameId = *((long*) (data+6));

        versionData = *(short*)(data+10);
        versionMajor = versionData >> 12;         // First 4 bits
        versionMinor = (versionData >> 5) & 0x7F; // Middle 7 bits
        versionIncrement = versionData & 0x1F;    // Last 5 bits

        turn = data[12];
        int playerData = *(unsigned short*)(data+14);
        playerNumber = playerData & 0x1F;

        encryptionSalt = playerData >> 5 ;//fileHeaderBlock.encryptionSalt;

        int flags = data[17];
        turnSubmitted = (flags & 1) > 0;
        hostUsing =     (flags & (1 << 1)) > 0;
        multipleTurns = (flags & (1 << 2)) > 0;
        gameOver =      (flags & (1 << 3)) > 0;
        shareware =     (flags & (1 << 4)) > 0;

        //printf("GameId: %x\n", gameId);
        //printf("Version: %d.%d.%d\n", versionMajor, versionMinor, versionIncrement);
        //printf("Turn: %d; Year: %d\n", turn, turn + 2400);
        //printf("Player Number: %d; Displayed as: %d\n", playerNumber, playerNumber+1);
        //printf("Encryption Salt: %x\n", encryptionSalt);
    };
};

#endif // FILEHEADERBLOCK_H
