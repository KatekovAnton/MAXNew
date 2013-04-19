//
//  MAXContentUtils.cpp
//  MAX
//
//  Created by Anton Katekov on 19.04.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "MAXContentUtils.h"
//#include "png.h"
#include "FileManger.h"
#include "cocos2d.h"

using namespace cocos2d;

//void abort_(const char * s, ...)
//{
//    va_list args;
//    va_start(args, s);
//    vfprintf(stderr, s, args);
//    fprintf(stderr, "\n");
//    va_end(args);
//    abort();
//}


MAXRESTextureData read_png_file(string fileName)
{
    FileManager* fm = FileManager::CreateManager();
    string path = fm->GetFilePath(fileName);
    delete fm;
    CCImage::EImageFormat eImageFormat = CCImage::kFmtPng;
       
    CCImage image;
    unsigned long nSize = 0;
    unsigned char* pBuffer = CCFileUtils::sharedFileUtils()->getFileData(path.c_str(), "rb", &nSize);
    if (! image.initWithImageData((void*)pBuffer, nSize, eImageFormat))
    {
        CC_SAFE_DELETE_ARRAY(pBuffer);
    }
    else
    {
        CC_SAFE_DELETE_ARRAY(pBuffer);
    }
    MAXRESTextureData result;
    result.data = (Color*)malloc(image.getDataLen() * 4);
    memcpy(result.data, image.getData(), image.getDataLen() * 4);
    result.h = image.getHeight();
    result.w = image.getWidth();
    return result;
//    int width, height;
//    png_byte color_type;
//    png_byte bit_depth;
//
//    int number_of_passes;
//    png_bytep * row_pointers;
//
//    png_structp png_ptr;
//    png_infop info_ptr;
//    const char *file_name = path.c_str();
//    unsigned char header[8];    // 8 is the maximum size that can be checked
//    
//    /* open file and test for it being a png */
//    FILE *fp = fopen(file_name, "rb");
//    if (!fp)
//        abort_("[read_png_file] File %s could not be opened for reading", file_name);
//    fread(header, 1, 8, fp);
//    if (png_sig_cmp(header, 0, 8))
//        abort_("[read_png_file] File %s is not recognized as a PNG file", file_name);
//    
//    
//    /* initialize stuff */
//    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
//    
//    if (!png_ptr)
//        abort_("[read_png_file] png_create_read_struct failed");
//    
//    info_ptr = png_create_info_struct(png_ptr);
//    if (!info_ptr)
//        abort_("[read_png_file] png_create_info_struct failed");
//    
//    if (setjmp(png_jmpbuf(png_ptr)))
//        abort_("[read_png_file] Error during init_io");
//    
//    png_init_io(png_ptr, fp);
//    png_set_sig_bytes(png_ptr, 8);
//    
//    png_read_info(png_ptr, info_ptr);
//    
//    width = png_get_image_width(png_ptr, info_ptr);
//    height = png_get_image_height(png_ptr, info_ptr);
//    color_type = png_get_color_type(png_ptr, info_ptr);
//    bit_depth = png_get_bit_depth(png_ptr, info_ptr);
//    
//    number_of_passes = png_set_interlace_handling(png_ptr);
//    png_read_update_info(png_ptr, info_ptr);
//    
//    
//    /* read file */
//    if (setjmp(png_jmpbuf(png_ptr)))
//        abort_("[read_png_file] Error during read_image");
//    
//    size_t size = sizeof(png_bytep) * height;
//    row_pointers = (png_bytep*) malloc(size);
//    for (int y=0; y<height; y++)
//    {
//        size_t rowSize = png_get_rowbytes(png_ptr,info_ptr);
//        row_pointers[y] = (png_byte*) malloc(rowSize);
//    }
//    
//    png_read_image(png_ptr, row_pointers);
//    //png_destroy_info_struct(png_ptr, info_ptr);
//    
//    fclose(fp);
}