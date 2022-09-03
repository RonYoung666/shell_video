#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "24BitImage.h"

/*******************************************************************
* NAME
*     MallocImage
* DESCRIPTION
*     Allocate a 24 bit image object
* INPUTS
*     nRow: Row size of the image
*     nCol: Col size of the image
* OUTPUTS
*     NONE
* RETURN
*     24 bit image object
* AUTHOR
*     Ron Young @2022-09-03
*******************************************************************/
image_t *MallocImage(int nRow, int nCol) {
    if (nRow <= 0 || nCol <= 0) {
        return NULL;
    }

    image_t *pImage = (image_t *)malloc(sizeof(image_t));
    if (NULL == pImage) {
        return NULL;
    }

    pImage->nRow = nRow;
    pImage->nCol = nCol;

    /*          |<- Pointer to each Row ->|   |<---- Pixels matrix ---->| */
    int nMSize = sizeof(pixel_t *) * nRow + sizeof(pixel_t) * nRow * nCol;
    pImage->ppPixels = (pixel_t **)malloc(nMSize);
    if (NULL == pImage->ppPixels) {
        free(pImage);
        return NULL;
    }

    /* Pointer to pixels matrix */
    pixel_t *pMatrix = (pixel_t *)(pImage->ppPixels + nRow);
 
    /* Initialize each "Row Pointer" */
    int i;
    for(i = 0; i < nRow; i++) {
        pImage->ppPixels[i] = (pMatrix + nCol * i);
    }

    return pImage;
}

/*******************************************************************
* NAME
*     FreeImage
* DESCRIPTION
*     Free a 24 bit image object's memory
* INPUTS
*     ppImage: Address of iamge object
* OUTPUTS
*     NONE
* RETURN
*     NONE
* AUTHOR
*     Ron Young @2022-09-03
*******************************************************************/
void FreeImage(image_t **ppImage) {
    if (NULL == ppImage) {
        return;
    }

    image_t *pImage = *ppImage;
    if(NULL == pImage) {
        return;
    }

    if (NULL != pImage->ppPixels) {
        free(pImage->ppPixels);
    }

    free(pImage);

    *ppImage = NULL;
    
    return;
}

/*******************************************************************
* NAME
*     CalcImageStringSize
* DESCRIPTION
*     Calculate the size of the image string
* INPUTS
*     pImage: The iamge object
* OUTPUTS
*     NONE
* RETURN
*     The size of the image string
* AUTHOR
*     Ron Young @2022-09-03
*******************************************************************/
int CalcImageStringSize(image_t *pImage) {
    if (NULL == pImage) {
        return 0;
    }

    /*     |<----- Row Num ----->|     |<---------- Pixels ---------->|   \n   |< Recover char >|  \0 */
    return ((pImage->nRow + 1) / 2) * (pImage->nCol * COLORED_CHAR_SIZE + 1) + COLORED_CHAR_SIZE + 1;
}

/*******************************************************************
* NAME
*     TwoPixel2String
* DESCRIPTION
*     Convert two vertically adjacent pixels to one colored character
* INPUTS
*     pPixerHigher: The higher pixel
*     pPixerLower: The Lower pixel
*     pStrBuf: The buffer to store convert result
*     nStrBufSize: Size of pStrBuf, at least COLORED_CHAR_SIZE
* OUTPUTS
*     pStrBuf: The buffer to store convert result
* RETURN
*     Size(Bytes) of the character converted in pStrBuf
* AUTHOR
*     Ron Young @2022-09-03
*******************************************************************/
static int TwoPixel2String(pixel_t *pPixHigher, pixel_t *pPixLower, char *pStrBuf, int nStrBufSize) {
    if (NULL == pPixHigher || NULL == pPixLower || NULL == pStrBuf || nStrBufSize <= 0) {
        return 0;
    }

    int nColorHigher = 16 + (pPixHigher->B * 6 / 256) + 6 * (pPixHigher->G * 6 / 256) + 36 * (pPixHigher->R * 6 / 256);
    int nColorLower  = 16 + (pPixLower->B  * 6 / 256) + 6 * (pPixLower->G  * 6 / 256) + 36 * (pPixLower->R  * 6 / 256);

    return snprintf(pStrBuf, nStrBufSize, "\x1b[38;5;%d;48;5;%dm▀", nColorHigher, nColorLower);
}

/*******************************************************************
* NAME
*     Image2String
* DESCRIPTION
*     Convert image to a string, the string can be displayed directly
* INPUTS
*     pImage: The iamge object
*     pStrBuf: The buffer to store convert result
*     nStrBufSize: Size of pStrBuf, at least "CalcImageStringSize(pImage)"
* OUTPUTS
*     pStrBuf: The buffer to store convert result
* RETURN
*     Size(Bytes) of the character converted in pStrBuf
* AUTHOR
*     Ron Young @2022-09-03
*******************************************************************/
int Image2String(image_t *pImage, char *pStrBuf, int nStrBufSize) {
    if (NULL == pImage || NULL == pStrBuf || nStrBufSize <= 0) {
        return 0;
    }

    int nOffset;
    int nRow, nCol;
    pixel_t *pPix1, *pPix2;
    pixel_t stBlackPix = {0, 0, 0};

    nOffset = 0;
    for (nRow = 0; nRow < pImage->nRow; nRow += 2) { /* Convert two vertically adjacent pixels to one colored character, so "nRow += 2" each time */
        for (nCol = 0; nCol < pImage->nCol; nCol++) {
            pPix1 = &pImage->ppPixels[nRow][nCol];
            pPix2 = nRow + 1 < pImage->nCol ? &pImage->ppPixels[nRow + 1][nCol] : &stBlackPix;
            nOffset += TwoPixel2String(pPix1, pPix2, pStrBuf + nOffset, nStrBufSize - nOffset);
        }
        nOffset += snprintf(pStrBuf + nOffset, nStrBufSize - nOffset, "\n");
    }
    nOffset += snprintf(pStrBuf + nOffset, nStrBufSize - nOffset, "\x1b[0m");

    return nOffset;
}

/*******************************************************************
* NAME
*     DisplayImage
* DESCRIPTION
*     Display an image to screen
* INPUTS
*     pImage: The iamge object
* OUTPUTS
*     NONE
* RETURN
*     NONE
* AUTHOR
*     Ron Young @2022-09-03
*******************************************************************/
void DisplayImage(image_t *pImage) {
    if (NULL == pImage) {
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

    Image2String(pImage, pStrBuf, nStrBufSize);

    printf("%s", pStrBuf);

    free(pStrBuf);

    return;
}
