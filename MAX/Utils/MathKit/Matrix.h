//
//  Matrix.h
//  3DTest
//
//  Created by Katekov Anton on 13.02.12.
//  Copyright (c) 2012 SoftFacade. All rights reserved.
//
#include "GLKMatrix4.h"
#include "GLKMatrix3.h"
#include "GLKMathUtils.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __clang__
static __inline__ GLKMatrix4 GLKMatrix4MakeBillboardEasy(GLKMatrix4 view, GLKVector3 objectPositon)
#else
static GLKMatrix4 GLKMatrix4MakeBillboard(GLKMatrix4 view, GLKVector3 objectPositon)
#endif
{
    GLKMatrix4 result = GLKMatrix4Transpose(view);
    
    result.m[12] = objectPositon.v[0];
    result.m[13] = objectPositon.v[1];
    result.m[14] = objectPositon.v[2];
    
    return  result;
}


#ifndef __clang__
static __inline__ GLKMatrix4 GLKMatrix4MakeBillboard(GLKMatrix4 view, GLKVector3 objectPositon, float scalex, float scaley)
#else
static GLKMatrix4 GLKMatrix4MakeScaledBillboard(GLKMatrix4 view, GLKVector3 objectPositon, float scalex, float scaley)
#endif
{
    GLKMatrix4 result = view;
    result.m00 = view.m00*scalex;
    result.m01 = view.m10*scalex;
    result.m02 = view.m20*scalex;
    result.m10 = view.m01*scaley;
    result.m11 = view.m11*scaley;
    result.m12 = view.m21*scaley;
    result.m20 = view.m02;
    result.m21 = view.m12;
    result.m22 = view.m22;
    
    
    
    result.m30 = objectPositon.x;
    result.m31 = objectPositon.y;
    result.m32 = objectPositon.z;
    return  result;
}


#ifndef __clang__
static __inline__ GLKVector3 GLKMatrix4GetTranslation(GLKMatrix4 matrix)
#else
static GLKVector3 GLKMatrix4GetTranslation(GLKMatrix4 matrix)
#endif
{
    GLKVector3 v;
    v.x = matrix.m30;
    v.y = matrix.m31;
    v.z = matrix.m32;
    return v;
}

#ifndef __clang__
static __inline__ GLKMatrix4 GLKMatrix4MakeCylBB(GLKMatrix4 baseTransformMatrix, GLKMatrix4 cameraView, GLKVector3 objectConstrAxis)
#else
static GLKMatrix4 GLKMatrix4MakeCylBB(GLKMatrix4 baseTransformMatrix, GLKMatrix4 cameraView, GLKVector3 objectConstrAxis)
#endif
{
    GLKMatrix4 result = GLKMatrix4Identity;
    GLKVector3 translation = GLKVector3Make(baseTransformMatrix.m30, baseTransformMatrix.m31, baseTransformMatrix.m32);
    
    result.m30 = translation.x;
    result.m31 = translation.y;
    result.m32 = translation.z;
   
    
    GLKVector3 v0 = GLKVector3Make(cameraView.m02,cameraView.m12,cameraView.m22);
    v0 = GLKVector3Normalize(v0);
    result.m20 = v0.x;
    result.m21 = v0.y;
    result.m22 = v0.z;
    
    
    GLKVector3 v2 = GLKMatrix3MultiplyVector3(GLKMatrix4GetMatrix3(baseTransformMatrix), objectConstrAxis);
    v2 = GLKVector3Normalize(v2);
    result.m10 = v2.x;
    result.m11 = v2.y;
    result.m12 = v2.z;
    
    
    GLKVector3 v1 = GLKVector3CrossProduct(v2, v0);
    v1 = GLKVector3Normalize(v1);
    result.m00 = v1.x;
    result.m01 = v1.y;
    result.m02 = v1.z;
    
    
    return result;
}

#ifndef __clang__
static __inline__ GLKMatrix4 GLKMatrix4MakeTranslationV(GLKVector3 position)
#else
static GLKMatrix4 GLKMatrix4MakeTranslationV(GLKVector3 position)
#endif
{
    return GLKMatrix4MakeTranslation(position.x, position.y, position.z);
}

#ifndef __clang__
static __inline__ float GLKVector3LengthSquired(GLKVector3 vector)
#else
static float GLKVector3LengthSquired(GLKVector3 vector)
#endif
{
    return vector.x * vector.x + vector.y * vector.y + vector.z * vector.z;
}

//#ifndef __clang__
//static __inline__ GLKVector3 UnprojectPoint(GLKVector3 screenSpace, GLKMatrix4 projection, GLKMatrix4 view, GLKMatrix4 world)  
//#else
//static GLKVector3 UnprojectPoint(GLKVector3 screenSpace, GLKMatrix4 projection, GLKMatrix4 view, GLKMatrix4 world)
//#endif
//{ 
//    //First, convert raw screen coords to unprojectable ones 
//    Vector3 position = new Vector3(); 
//    position.X = (((screenSpace.X - (float)viewport.X) / ((float)viewport.Width)) * 2f) - 1f; 
//    position.Y = -((((screenSpace.Y - (float)viewport.Y) / ((float)viewport.Height)) * 2f) - 1f); 
//    position.Z = (screenSpace.Z - viewport.MinDepth) / (viewport.MaxDepth - viewport.MinDepth); 
//    
//    //Unproject by transforming the 4d vector by the inverse of the projecttion matrix, followed by the inverse of the view matrix.   
//    Vector4 us4 = new Vector4(position, 1f); 
//    Vector4 up4 = Vector4.Transform(us4,  Matrix.Invert(Matrix.Multiply(Matrix.Multiply(world, view), projection))); 
//    Vector3 up3 = new Vector3(up4.X, up4.Y, up4.Z); 
//    return up3 / up4.W; //better to do this here to reduce precision loss..   
//} 

#ifndef __clang__
static __inline__ GLKVector3 UnprojectPointWIdent(GLKVector3 screenSpace, GLKMatrix4 projection, GLKMatrix4 view, float minDepth, float maxDepth, GSize2D screenSize)
#else
static GLKVector3 UnprojectPointWIdent(GLKVector3 screenSpace, GLKMatrix4 projection, GLKMatrix4 view, float minDepth, float maxDepth, GSize2D screenSize)
#endif
{ 
    //First, convert raw screen coords to unprojectable ones 
    GLKVector3 position;
    position.x = (((screenSpace.x/* - (float)viewport.x*/) / (screenSize.width)) * 2.0) - 1.0; 
    position.y = -((((screenSpace.y/* - (float)viewport.y*/) / (screenSize.height)) * 2.0) - 1.0); 
    position.z = (screenSpace.y - minDepth) / (maxDepth - minDepth); 
    
    //Unproject by transforming the 4d vector by the inverse of the projecttion matrix, followed by the inverse of the view matrix.  
    GLKVector4 us4 = GLKVector4MakeWithVector3(position, 1.0);

    bool l;
    GLKMatrix4 m = GLKMatrix4Invert(GLKMatrix4Multiply(view, projection), &l);
    
    GLKVector4 up4 = GLKMatrix4MultiplyVector4(m, us4); 
    GLKVector3 up3 = GLKVector3Make(up4.x/up4.w, up4.y/up4.w, up4.z/up4.w); //better to do this here to reduce precision loss..   
    return up3;
} 

#ifdef __cplusplus
}
#endif


