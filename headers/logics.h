#ifndef LOGICS_H
#define LOGICS_H

typedef void (*PlayerFireFunc)(Player* p, Player* opponent, int x, int y);
typedef void (*PlayerPositionFunc)(Player* p);
typedef void (*EnemyPositionFunc)(Enemy* e);
typedef void (*EnemyFireFunc)(Enemy* e, Player* p);

#endif 