#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#include "24BitImage.h"

void sig_handler(int signum){
    printf("\x1b[0m");

    exit(0);
    return;
}

void DisplayBaseImage() {
    image_t *pImage = MallocImage(3, 3);
    if (NULL == pImage) {
        printf("MallocImage(3, 3) Error\n");
        return;
    }

    pImage->ppPixels[0][0].B = 255;
    pImage->ppPixels[0][0].G = 0;
    pImage->ppPixels[0][0].R = 0;

    pImage->ppPixels[0][1].B = 0;
    pImage->ppPixels[0][1].G = 255;
    pImage->ppPixels[0][1].R = 0;

    pImage->ppPixels[0][2].B = 0;
    pImage->ppPixels[0][2].G = 0;
    pImage->ppPixels[0][2].R = 255;


    pImage->ppPixels[1][0].B = 0;
    pImage->ppPixels[1][0].G = 255;
    pImage->ppPixels[1][0].R = 255;

    pImage->ppPixels[1][1].B = 255;
    pImage->ppPixels[1][1].G = 0;
    pImage->ppPixels[1][1].R = 255;

    pImage->ppPixels[1][2].B = 255;
    pImage->ppPixels[1][2].G = 255;
    pImage->ppPixels[1][2].R = 0;


    pImage->ppPixels[2][0].B = 0;
    pImage->ppPixels[2][0].G = 0;
    pImage->ppPixels[2][0].R = 0;

    pImage->ppPixels[2][1].B = 127;
    pImage->ppPixels[2][1].G = 127;
    pImage->ppPixels[2][1].R = 127;

    pImage->ppPixels[2][2].B = 255;
    pImage->ppPixels[2][2].G = 255;
    pImage->ppPixels[2][2].R = 255;

    DisplayImage(pImage);

    FreeImage(&pImage);

    return;
}

void LoopDisplayRandom(int nRow, int nCol, int nLoopTimes, int nFps) {
    image_t *pImage = MallocImage(nRow, nCol);
    if (NULL == pImage) {
        printf("MallocImage(nRow, nCol) Error\n");
        return;
    }

    int nStrBufSize = CalcImageStringSize(pImage);
    if (nStrBufSize <= 0) {
        return;
    }

    char *pStrBuf = (char *)malloc(nStrBufSize);
    if (NULL == pStrBuf) {
        return;
    }
    pStrBuf[0] = '\0';

    srand((unsigned int)time(NULL));

    int l;
    for (l = 0; l < nLoopTimes; l++) {
        if (l != 0) {
            printf("\x1b[%dA", (nRow + 1) / 2);
        }

        int i, j;
        for (i = 0; i < nRow; i++) {
            for (j = 0; j < nCol; j++) {
                pImage->ppPixels[i][j].B = rand() % 255;
                pImage->ppPixels[i][j].G = rand() % 255;
                pImage->ppPixels[i][j].R = rand() % 255;
            }
        }

        Image2String(pImage, pStrBuf, nStrBufSize);
        printf("%s", pStrBuf);
        usleep(1000 * 1000 / nFps);
    }

    FreeImage(&pImage);
    free(pStrBuf);

    return;
}


int main(int argc, char *argv[]) {
    signal(SIGINT,sig_handler);

    if (argc < 5) {
        printf("Usage: %s <Row> <Col> <frame num> <fps>\n", argv[0]);
        return 0;
    }

    int nRow   = atoi(argv[1]);
    int nCol   = atoi(argv[2]);
    int nFrame = atoi(argv[3]);
    int nFps   = atoi(argv[4]);
    printf("nRow   = %d\n", nRow);
    printf("nCol   = %d\n", nCol);
    printf("nFrame = %d\n", nFrame);
    printf("nFps   = %d\n", nFps);

    //DisplayBaseImage();

    LoopDisplayRandom(nRow, nCol, nFrame, nFps);

    return 0;
}
