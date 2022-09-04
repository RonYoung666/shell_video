#ifndef __24_BIT_IMAGE_
#define __24_BIT_IMAGE_

#define COLORED_CHAR_SIZE 23

/* Single Pixel */
typedef struct {
    unsigned char B;
    unsigned char G;
    unsigned char R;
} pixel_t;

/* 24 Bit Image */
typedef struct {
    int nRow;
    int nCol;
    pixel_t **ppPixels;
} image_t;


#ifdef __cplusplus
extern "C" 
{
#endif

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
image_t *MallocImage(int nRow, int nCol);

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
void FreeImage(image_t **ppImage);

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
int CalcImageStringSize(image_t *pImage);

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
int Image2String(image_t *pImage, char *pStrBuf, int nStrBufSize);

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
void DisplayImage(image_t *pImage);

#ifdef __cplusplus
}
#endif

#endif /* __24_BIT_IMAGE_ */
