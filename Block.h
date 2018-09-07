#ifndef BLOCK_H
#define BLOCK_H

class BlockType {

public:
    /**
     * Here are all the Block Type IDs
     */
    static const int FILE_FOOTER = 0;
    static const int MANUAL_SMALL_LOAD_UNLOAD_TASK = 1;
    static const int MANUAL_MEDIUM_LOAD_UNLOAD_TASK = 2;
    static const int WAYPOINT_DELETE = 3;
    static const int WAYPOINT_ADD = 4;
    static const int WAYPOINT_CHANGE_TASK = 5;
    static const int PLAYER = 6;
    static const int PLANETS = 7;
    static const int FILE_HEADER = 8; // (unencrypted)
    static const int FILE_HASH = 9;
    static const int WAYPOINT_REPEAT_ORDERS = 10;
    static const int UNKNOWN_BLOCK_11 = 11;
    static const int EVENTS = 12;
    static const int PLANET = 13;
    static const int PARTIAL_PLANET = 14;
    static const int UNKNOWN_BLOCK_15 = 15;
    static const int FLEET = 16;
    static const int PARTIAL_FLEET = 17;
    static const int UNKNOWN_BLOCK_18 = 18;
    static const int WAYPOINT_TASK = 19;
    static const int WAYPOINT = 20;
    static const int FLEET_NAME = 21;
    static const int UNKNOWN_BLOCK_22 = 22;
    static const int MOVE_SHIPS = 23;
    static const int FLEET_SPLIT = 24;
    static const int MANUAL_LARGE_LOAD_UNLOAD_TASK = 25;
    static const int DESIGN = 26;
    static const int DESIGN_CHANGE = 27;
    static const int PRODUCTION_QUEUE = 28;
    static const int PRODUCTION_QUEUE_CHANGE = 29;
    static const int BATTLE_PLAN = 30;
    static const int BATTLE = 31; // (content isn't decoded yet)
    static const int COUNTERS = 32;
    static const int MESSAGES_FILTER = 33;
    static const int RESEARCH_CHANGE = 34;
    static const int PLANET_CHANGE = 35;
    static const int CHANGE_PASSWORD = 36;
    static const int FLEETS_MERGE = 37;
    static const int PLAYERS_RELATION_CHANGE = 38;
    static const int BATTLE_CONTINUATION = 39; // (content isn't decoded yet)
    static const int MESSAGE = 40;
    static const int AI_H_FILE_RECORD = 41;
    static const int SET_FLEET_BATTLE_PLAN = 42;
    static const int OBJECT = 43;
    static const int RENAME_FLEET = 44;
    static const int PLAYER_SCORES = 45;
    static const int SAVE_AND_SUBMIT = 46;

    // Default
    static const int UNKNOWN_BAD = -1;
};


class Block {
public:
    static const int BLOCK_PADDING = 4;      // bytes

    int typeId;
    int size;
    bool encrypted;

    /**
    * This holds the original block data, padded to a multiple of 4
    */
    char * data;


    /**
    * Used to enforce proper object usage
    */
    bool hasData;
    bool hasDecryptedData;

    /**
    * This holds the decrypted block data, padded to a multiple of 4
    */
    char * decryptedData;
    Block () {
        typeId = BlockType::UNKNOWN_BAD;
        size = 0;
        encrypted = true;
        hasData = false;
        hasDecryptedData = false;
    };
};
#endif // BLOCK_H
