//
//  GLKMathUtils.h
//  GLKit
//
//  Copyright (c) 2011, Apple Inc. All rights reserved.
//

#ifndef __GLK_MATH_UTILS_H
#define __GLK_MATH_UTILS_H

#include <math.h>
#include <stdbool.h>

#include "GLKMathTypes.h"

#ifdef __OBJC__
#import <Foundation/Foundation.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

    static __inline__ float GLKMathDegreesToRadians(float degrees) { return degrees * (M_PI / 180); };
    static __inline__ float GLKMathRadiansToDegrees(float radians) { return radians * (180 / M_PI); };
 
    
    //takes a 3d position and computes the 2d pixel location
    static __inline__ GLKVector3 GLKMathProject(GLKVector3 object, GLKMatrix4 model, GLKMatrix4 projection, float *viewport)
    {
        
        // Compose the matrices into a net row-major transformation
        GLKVector4 transformation[4];
        for (int r = 0; r < 4; ++r)
        {
            for (int c = 0; c < 4; ++c)
            {
                transformation[r].v[c]  = projection.m[r + 0 * 4] * model.m[0 + c * 4];
                transformation[r].v[c] += projection.m[r + 1 * 4] * model.m[1 + c * 4];
                transformation[r].v[c] += projection.m[r + 2 * 4] * model.m[2 + c * 4];
                transformation[r].v[c] += projection.m[r + 3 * 4] * model.m[3 + c * 4];
            }
        }
        
        // Transform the vertex
        GLKVector4 result;
        for (int r = 0; r < 4; ++r) {
            result.v[r] = GLKVector4DotProduct(transformation[r], GLKVector4MakeWithVector3(object, 1));
        }
        
        // Homogeneous division
        const double rhw = 1 / result.w;
        
        return GLKVector3Make((result.x * rhw + 1) * viewport[2] / 2,
                       (1 - result.y * rhw) * viewport[3] / 2,
                       rhw);
    }
    
    
    //takes a 2d position and the depth value, and gives you the 3d location
    static __inline__ GLKVector3 GLKMathUnproject(GLKVector3 window, GLKMatrix4 model, GLKMatrix4 projection, int *viewport, bool *success)
    {
        return GLKVector3Make(0, 0, 0);
    }

    
#ifdef __cplusplus
}
#endif

#endif /* __GLK_MATH_UTILS_H */
