//
//  EngineMesh.h
//  TerminalExtraction
//
//  Created by Yarik Smirnov on 11/3/12.
//
//

#ifndef __TerminalExtraction__EngineMesh__
#define __TerminalExtraction__EngineMesh__

#include <iostream>
#include "Geometry.h"

class EngineMesh {
    
    bool _disposed;
    
    GLuint          _indexBuffer;
    GLuint          _vertexBuffer;
    
    long            _vSize;
    long            _iSize;
    long            _primitivesCount;
    
public:
    
    EngineMesh();
    EngineMesh(void * vertices, long vSize, void *indices, long iSize);

    
    ~EngineMesh();

    void Bind() const;
    void Render() const;
    void Unbind() const;
    
    
    virtual void Dispose();
    virtual void LoadFromBuffer(char *buffer, unsigned int bufferLength);
    
    
    static const EngineMesh * CreateCube(void);
    static const EngineMesh * CreateFullscreenQuad(void);
    static const EngineMesh * CreateUnitQuad(void);
    
    static const EngineMesh * CreateUnitQuad(float side);
    
};

#endif /* defined(__TerminalExtraction__EngineMesh__) */

