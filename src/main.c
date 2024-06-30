// ****************************************************************************
// EXAMPLE_PATHFINDER.C
//
// To compile this example: pcompile example_pathfinder.c mr32.c
// ****************************************************************************
//
//.................. Fundo Branco em Pista Preta ........................
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++     Branco -> '0'       ++++++++++++++++++++++++++
// ++++++++++++++++++     Preto  -> '1'       ++++++++++++++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//                            Pista:
//                  |                         |
//                  |                         |
//       FOTO4    FOTO3       FOTO2         FOTO1        FOTO0
//        ---      ---         ---           ---          ---
//        ---      ---         ---           ---          ---
//        ---      ---         ---           ---          ---
//                  |                         |
//                  |                         |
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "mr32.h"
#include "map.h"


int baseSpeed = 50;
// #define baseSpeed 50
#define k 25
#define turnSpeed 60

#define timeLimit 10
int timeUp = 0;

void run();
int turnLeft(char hasRight);
void turnRight();
void turnBack();
void moveStraight(int groundSensor, int lastGroundSensor);

int main(int argc, char *argv[]) {
    initPIC32();

    T1CONbits.TCKPS = 3;
    PR1 = 39062;
    TMR1 = 0;
    T1CONbits.TON = 1;
    IPC1bits.T1IP = 6;
    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 0;


    initMap();
    closedLoopControl(true);
    setVel2(0, 0);

    printf("PathFinder example\n\n\n");

    run();
    return 0;
}

void run() {
    printf("Press start to continue\n");
    while(!startButton()) waitTick80ms();

    int groundSensor;
    int lastGroundSensor = 0;
    resetNextTurn();
    IEC0bits.T1IE = 1;

    do {
        if (timeUp) {
            setVel2(0, 0);
            timeUp = 0;
            run();
        }
        waitTick10ms();

        groundSensor = readLineSensors(0);

        printInt(groundSensor, 2 | 5 << 16);
        putChar('\n');

        if (isMapFinished()) {
            double x, y, h;
            if ((groundSensor & 0x11) == 0x00) {
                moveStraight(groundSensor, lastGroundSensor);
            } else {
                setRobotPos(0, 0, 0);
                x = 0;
                while (x < 25) {
                    moveStraight(groundSensor, lastGroundSensor);
                    getRobotPos(&x, &y, &h);
                }

                int newRead = readLineSensors(0);
                if (newRead == 0x1F) {
                    baseSpeed = 65;
                    setVel2(0, 0);
                    IEC0bits.T1IE = 0;
                    run();
                }
                if (newRead == 0x00) {
                    if ((groundSensor & 0x11) == 0x10) {
                        // turn left
                        while (x < 60) {
                            moveStraight(groundSensor, lastGroundSensor);
                            getRobotPos(&x, &y, &h);
                        }
                        while (1) {
                            setVel2(-turnSpeed, turnSpeed);
                            getRobotPos(&x, &y, &h);
                            if (h >= PI/2 - 0.18) break;
                        }
                        setVel2(0, 0);
                        delay(500);
                        continue;
                    } else if ((groundSensor & 0x11) == 0x01) {
                        // turn right
                        while (x < 60) {
                            moveStraight(groundSensor, lastGroundSensor);
                            getRobotPos(&x, &y, &h);
                        }
                        while (1) {
                            setVel2(turnSpeed, -turnSpeed);
                            getRobotPos(&x, &y, &h);
                            if (h <= -PI/2 + 0.18) break;
                        }
                        setVel2(0, 0);
                        delay(500);
                        continue;
                    }
                }
                switch (getNextTurn()) {
                    case 'L':
                        // turn left
                        while (x < 60) {
                            moveStraight(groundSensor, lastGroundSensor);
                            getRobotPos(&x, &y, &h);
                        }
                        while (1) {
                            setVel2(-turnSpeed, turnSpeed);
                            getRobotPos(&x, &y, &h);
                            if (h >= PI/2 - 0.18) break;
                        }
                        setVel2(0, 0);
                        delay(500);
                        break;
                    case 'R':
                        // turn right
                        while (x < 60) {
                            moveStraight(groundSensor, lastGroundSensor);
                            getRobotPos(&x, &y, &h);
                        }
                        while (1) {
                            setVel2(turnSpeed, -turnSpeed);
                            getRobotPos(&x, &y, &h);
                            if (h <= -PI/2 + 0.18) break;
                        }
                        setVel2(0, 0);
                        delay(500);
                        break;
                    case 'B':
                        while (1) {
                            setVel2(turnSpeed, -turnSpeed);
                            getRobotPos(&x, &y, &h);
                            if (h <= -PI + 0.18) break;
                        }
                        setVel2(0, 0);
                        delay(500);
                        break;
                    default:
                        break;
                }
            }
        } else {
            if ((groundSensor == 0x00) && (lastGroundSensor < 0x10) && (lastGroundSensor > 0x01)) {
                turnBack();
                appendTurn('B');
            } else if ((groundSensor < 0x10) && (groundSensor > 0x01)) {
                moveStraight(groundSensor, lastGroundSensor);
            } else if (groundSensor > 0x01) {
                lastGroundSensor = 0x00;
                if (turnLeft(readLineSensors(0) & 0x01)) {
                    printf("Final Map: %s\n", getMapPath());
                    setMapFinished();
                    IEC0bits.T1IE = 0;
                    run();
                }
                groundSensor = 0x00;
            } else if (groundSensor < 0x10) {
                if (groundSensor == 0x01) {
                    lastGroundSensor = 0x00;
                    turnRight();
                    groundSensor = 0x00;
                }
            }
        }

        lastGroundSensor = groundSensor;

    } while (!stopButton());  // Pressione o botao de stop para terminar o programa
}

void moveStraight(int groundSensor, int lastGroundSensor) {
    int detourFactor = ((groundSensor & 0x2) | ((groundSensor & 0x4) >> 2 | (groundSensor & 0x1) << 2)) - (groundSensor >> 2);

    int leftSpeed = baseSpeed + k*detourFactor / 10;
    int rightSpeed = baseSpeed - k*detourFactor / 10;

    if ((lastGroundSensor & 0x01) && !(groundSensor & 0x01)) appendTurn('F');

    setVel2(leftSpeed, rightSpeed);
}

int turnLeft(char hasRight) {
    double x, y, h;

    // This will define initial values
    setRobotPos(0, 0, 0);

    // Go a little further.
    while (1) {
        getRobotPos(&x, &y, &h);
        if (x > 25 && readLineSensors(0) == 0x1F) {
            setVel2(0, 0);
            return 1;
        }
        if (x > 60) {
            // printf("hasRight: %d\nfront: ", hasRight);
            // int val = readLineSensors(0);
            // printInt(val, 2 | 5 << 16);
            // printf("\n");
            if (hasRight || (readLineSensors(0) & 0x0E) != 0x00)
                appendTurn('L');
            break;
        }
    }

    // Make a left-hand turn
    while (1) {
        setVel2(-turnSpeed, turnSpeed);
        getRobotPos(&x, &y, &h);
        if (h >= PI/2 - 0.18) break;
    }

    setVel2(0, 0);
    delay(500);

    return 0;
}


void turnRight() {
    double x, y, h;

    // This will define initial values
    setRobotPos(0, 0, 0);

    // Go a little further.
    while (1) {
        getRobotPos(&x, &y, &h);
        if (x > 60) break;
    }

    // Make a left-hand turn
    while (1) {
        setVel2(turnSpeed, -turnSpeed);
        getRobotPos(&x, &y, &h);
        if (h <= -PI/2 + 0.18) break;
    }

    setVel2(0, 0);
    delay(500);
}


void turnBack() {
    double x, y, h;

    // This will define initial values
    setRobotPos(0, 0, 0);

    // Make a left-hand turn
    while (1) {
        setVel2(turnSpeed, -turnSpeed);
        getRobotPos(&x, &y, &h);
        if (h <= -PI + 0.18) break;
    }

    setVel2(0, 0);
    delay(500);
}

void _int_(_TIMER_1_VECTOR) isr_t1(void) {
    static int count = 0;

    if (++count >= timeLimit) {
        count = 0;
        IEC0bits.T1IE = 0;
        timeUp = 1;
    }

    IFS0bits.T1IF = 0;
}
