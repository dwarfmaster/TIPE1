
#include "ia.hpp"

#define THIRD (1.0f/3.0f)
#define PI (3.1415926535897932384626433832795028841971f)

#define DFAR  3.0f
#define DNEAR 1.0f
#define DNEXT 0.3f
#define MIND  1.5f

typedef unsigned char Action;
#define ACT_TURN_RIGHT ((Action)0x01) /* 0b00000001 */
#define ACT_TURN_LEFT  ((Action)0x02) /* 0b00000010 */
#define ACT_STRAIGHT   ((Action)0x03) /* 0b00000011 */

/* i = 0 : far
 * i = 1 : near
 * i = 2 : next
 */
#define GENOM_MASK(i)    (0x3 << (2*(i)))
#define GENOM_GET(g,i)   (((g) & GENOM_MASK(i)) >> 2*i)
#define GENOM_SET(g,v,i) ((g) = (((g) & (~GENOM_MASK(i))) | \
                                 (((v) & 0x03) << (2*(i)))))
#define FAR  0
#define NEAR 1
#define NEXT 2
static unsigned char genom;

static Robot* rb;
static float dist;
static float mdist;
static float oldE;

/* Move storage */
typedef struct _move {
    Action act;
    float arg;
} Move;
typedef struct _moves {
    Move* array;
    unsigned long size;
    unsigned long capacity;
} Moves;
static Moves moves;

void moves_init() {
    moves.size     = 0;
    moves.capacity = 10;
    moves.array    = (Move*) rb->alloc(sizeof(Move) * moves.capacity);
}

void moves_expand() {
    moves.capacity += 10;
    moves.array    = (Move*) rb->realloc((void*) moves.array,
                                         sizeof(Move) * moves.capacity);
}

void moves_free() {
    rb->free(moves.array);
}

void moves_push(Action a, float arg) {
    if(moves.size == moves.capacity) {
        moves_expand();
    }

    moves.array[moves.size].act = a;
    moves.array[moves.size].arg = arg;
    ++moves.size;
}

/* Moving */
void move_turn(float rad) {
    moves_push(ACT_TURN_RIGHT, rad);
    rb->turn(rad);
    mdist = 0.0f;
}

void move_go() {
    rb->forward();
}

float move_stop() {
    float d = rb->stop();
    dist  += d;
    mdist += d;
    moves_push(ACT_STRAIGHT, d);
    return d;
}

void move_reset() {
    dist = 0.0f;
    mdist = 0.0f;
}

/* Misc utility functions */
Action rand_act() {
    float rand = rb->rand();
    if(rand < THIRD) {
        return ACT_TURN_RIGHT;
    } else if(rand < 2*THIRD) {
        return ACT_TURN_LEFT;
    } else {
        return ACT_STRAIGHT;
    }
}

float act_to_rad(Action a) {
    switch(a) {
        case ACT_TURN_RIGHT:
            return PI/2.0f;
        case ACT_TURN_LEFT:
            return -PI/2.0f;
        case ACT_STRAIGHT:
            return 0.0f;
    }
    return 0.0f; /* Shouldn't happen */
}

bool evolve(unsigned char rgenom, float rE, float rdist) {
    float p = oldE / (oldE + rE);
    float rand;
    bool ch[3], rch[3];

    if(dist < MIND || rdist < MIND) {
        return false;
    }

    if(p < THIRD) {
        for(int i = 0; i < 3; ++i) {
            ch[i] = false;
            rch[i] = true;
        }
        rand = rb->rand();
        if(rand < THIRD)
            ch[0] = true;
        else if(rand < 2*THIRD)
            ch[1] = true;
        else
            ch[2] = true;
    } 
    
    else if(p < 2*THIRD) {
        rand = rb->rand();
        for(int i = 0; i < 3; ++i) {
            ch[i] = true;
            rch[i] = true;
        }
        rand = rb->rand();
        if(rand < THIRD)
            ch[0] = false;
        else if(rand < 2*THIRD)
            ch[1] = false;
        else
            ch[2] = false;
        rand = rb->rand();
        if(rand < THIRD)
            rch[0] = false;
        else if(rand < 2*THIRD)
            rch[1] = false;
        else
            rch[1] = false;
    }
    
    else {
        for(int i = 0; i < 3; ++i) {
            ch[i] = true;
            rch[i] = false;
        }
        rand = rb->rand();
        if(rand < THIRD)
            rch[0] = true;
        else if(rand < 2*THIRD)
            rch[1] = true;
        else
            rch[2] = true;
    }

    unsigned char ngenom;
    for(int i = 0; i < 3; ++i) {
        if(ch[i] && rch[i]) {
            rand = rb->rand();
            if(rand < 0.5f) {
                GENOM_SET (ngenom,
                        (GENOM_GET (genom, i) & 0x02)
                            | (GENOM_GET (rgenom, i) & 0x01),
                        i);
            } else {
                GENOM_SET (ngenom,
                        (GENOM_GET (genom, i) & 0x01)
                            | (GENOM_GET (rgenom, i) & 0x02),
                        i);
            }
            if(GENOM_GET (ngenom, i) == 0) {
                GENOM_SET (ngenom, rand_act(), i);
            }
        } else if(ch[i]) {
            GENOM_SET (ngenom, GENOM_GET (genom, i), i);
        } else if(rch[i]) {
            GENOM_SET (ngenom, GENOM_GET (rgenom, i), i);
        } else {
            GENOM_SET (ngenom, rand_act(), i);
        }
    }

    genom = ngenom;
    return true;
}

float evalue() {
    float A = 1.0f / ((dist - 1.0f) / 5.0f + 1.0f);
    float E = A * oldE + (1.0f - A) * mdist;
    oldE = E;
    return E;
    
}

void mainloop(Robot* robot)
{
    /*** Initialisation ***/
    rb = robot;
    oldE = 0.0f;
    /* Generate a random genom */
    genom = 0;
    for(int i = 0; i < 3; ++i) {
        GENOM_SET (genom, rand_act(), i);
    }
    /* First movement */
    move_reset();
    switch(rand_act()) {
        case ACT_TURN_RIGHT:
            move_turn(PI/2.0f);
            break;
        case ACT_TURN_LEFT:
            move_turn(-PI/2.0f);
            break;
    }
    move_go();

    /*** Loop ***/
    for(;;) {
        if(rb->discover()) {
            char buffer[256];
            int size;

            move_stop();
            buffer[0] = 0xaa;
            rb->send(buffer, 1);
            size = rb->receive(buffer, 256);
            if(size != 1) {
                /* Taille invalide, on ignore */
                rb->close();
                move_go();
                continue;
            } else {
                if(buffer[0] == (char)0xFF) { /* Connection à l'ordi central */
                    /* TODO */
                    rb->close();
                    move_go();
                    continue;
                } else {
                    float E = evalue();
                    buffer[0] = ((genom & 0x3f) << 1);
                    rb->memcpy(buffer + 1, &dist, 4);
                    rb->memcpy(buffer + 5, &E, 4);
                    rb->send(buffer, 9);

                    float rdist;
                    size = rb->receive(buffer, 256);
                    rb->close();
                    if(size != 9 || (buffer[0] & 0x7e) != 0) {
                        move_go();
                        continue;
                    }

                    rb->memcpy(&rdist, buffer + 1, 4);
                    rb->memcpy(&E, buffer + 5, 4);
                    if(evolve((buffer[0] & 0x3f) >> 1, E, rdist)) {
                        move_reset();
                    }
                    move_go();
                    continue;
                }
            }
        }

        float sc = rb->scan();
        Action act;
        if(sc < DNEXT) {
            act = GENOM_GET(genom, NEXT);
            move_stop();
            switch(act) {
                case ACT_TURN_RIGHT:
                    move_turn(PI/2.0f);
                    break;
                case ACT_TURN_LEFT:
                    move_turn(-PI/2.0f);
                    break;
                case ACT_STRAIGHT:
                    move_turn(PI);
                    break;
            }
            move_go();
        } else if(sc < DNEAR) {
            act = GENOM_GET(genom, NEAR);
            move_stop();
            switch(act) {
                case ACT_TURN_RIGHT:
                    move_turn(PI/2.0f);
                    break;
                case ACT_TURN_LEFT:
                    move_turn(-PI/2.0f);
                    break;
            }
            move_go();
        } else if(sc < DFAR) {
            act = GENOM_GET(genom, FAR);
            move_stop();
            switch(act) {
                case ACT_TURN_RIGHT:
                    move_turn(PI/2.0f);
                    break;
                case ACT_TURN_LEFT:
                    move_turn(-PI/2.0f);
                    break;
            }
            move_go();
        }
    }
}

