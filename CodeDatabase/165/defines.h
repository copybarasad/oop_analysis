#ifndef DEFINES
#define DEFINES
#define BLOCK 'B'
#define NOTHING '-'
#define ENEMY 'E'
#define CASTLE 'C'
#define HERO 'K'
#define TRAP 'T'
#define MAXHEIGHT 25
#define MINHEIGHT 10
#define MAXLENGTH 25
#define MINLENGTH 10
#define FIRERANGE 50
#define CASTLEHP 50
#define ENEMYHP 20
#define ENEMYAD 5
#define STDHP 50
#define STDMP 10
#define MANAREGEN 2
#define STDATTACK 50
#define STDSPAWNTIMER 6
#define ICESPEARDMG 15
#define ICESPEARRNG 3
#define FLAMESTORMDMG 10
#define FLAMESTORMRNG 2
#define FLAMESTORMBLAST 2
#define NUMBEROFSPELLS 2
#define STDINVLIMIT 6
#define MANACOST 10
#define MODIFICATOR 0.1
#define SAVEPATH "./game/saves"
#define SAVESLIMIT 9
#define MENU "\tRPG-like game\n\n" << "\tMENU\n" << "\t New_game\n" << "\t Continue\n" << "\t Load_game\n" << "\t Save_game\n" << "\t Exit\n"
#define HELLOPLAYERTEXT "Thanks for playing that code-containing program. In order to start a game enter commands.\n"
#define ICESPEARNAME "Ice Spear"
#define FLAMESTORMNAME "Flamestorm"
#define SCENARIOPATH "./game/scenarios"
#define SCENARIONUM 4
#define SCENARIOS "10 10 2 4 1 9 \n10 10 5 4 0 4 \n14 12 0 9 3 7 \n10 10 3 5 2 1 \n"
#endif