#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#include "24BitImage.h"

void sig_handler(int signum){
    /* Recover terminal color */
    printf("\x1b[0m");

    exit(0);
    return;
}

void ShowUsage(int argc, char **argv) {
    printf("[Show basic color]           %s color\n", argv[0]);
    printf("[Loop display random image]  %s loop <Row> <Col> <Frame num> <Fps>\n", argv[0]);
    printf("[Test ResizeImage()]         %s resize <From row> <From col> <To row> <To col>\n", argv[0]);

    return;
}

void TestColor() {
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

void TestLoop(int argc, char *argv[]) {
    int nRow   = atoi(argv[2]);
    int nCol   = atoi(argv[3]);
    int nFrame = atoi(argv[4]);
    int nFps   = atoi(argv[5]);
    printf("nRow   = %d\n", nRow);
    printf("nCol   = %d\n", nCol);
    printf("nFrame = %d\n", nFrame);
    printf("nFps   = %d\n", nFps);

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
    for (l = 0; l < nFrame; l++) {
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

void TestResize(int argc, char *argv[]) {
    int nFromRow, nFromCol, nToRow, nToCol;
    image_t *pFromImage = NULL;
    image_t *pToImage = NULL;

    nFromRow = atoi(argv[2]);
    nFromCol = atoi(argv[3]);
    nToRow   = atoi(argv[4]);
    nToCol   = atoi(argv[5]);
    printf("nFromRow = %d\n", nFromRow);
    printf("nFromCol = %d\n", nFromCol);
    printf("nToRow   = %d\n", nToRow);
    printf("nToCol   = %d\n", nToCol);

    /* Malloc two image object */
    pFromImage = MallocImage(nFromRow, nFromCol);
    pToImage = MallocImage(nToRow, nToCol);
    if (NULL == pFromImage || NULL == pToImage) {
        printf("Malloc iamge object failed !\n");
        goto RETURN;
    }

    /* Set the from size image color to random */
    srand((unsigned int)time(NULL));
    int i, j;
    for (i = 0; i < pFromImage->nRow; i++) {
        for (j = 0; j < pFromImage->nCol; j++) {
            pFromImage->ppPixels[i][j].B = rand() % 255;
            pFromImage->ppPixels[i][j].G = rand() % 255;
            pFromImage->ppPixels[i][j].R = rand() % 255;
        }
    }

    ResizeImage(pFromImage, pToImage);

    printf("\nFrom %d * %d:\n", nFromRow, nFromCol);
    DisplayImage(pFromImage);

    printf("\nTo %d * %d:\n", nToRow, nToCol);
    DisplayImage(pToImage);

RETURN:
    if (NULL != pFromImage) {
        FreeImage(&pFromImage);
    }
    if (NULL != pToImage) {
        FreeImage(&pToImage);
    }

    return;
}

int main(int argc, char *argv[]) {
    signal(SIGINT,sig_handler);

    if (argc < 2) {
        ShowUsage(argc, argv);
        return 0;
    }

    if (strcmp("color", argv[1]) == 0) {
        TestColor();
        return;
    }

    if (strcmp("loop", argv[1]) == 0 && argc >= 6) {
        TestLoop(argc, argv);
        return;
    }

    if (strcmp("resize", argv[1]) == 0 && argc >= 6) {
        TestResize(argc, argv);
        return;
    }

    ShowUsage(argc, argv);

    return 0;
}
