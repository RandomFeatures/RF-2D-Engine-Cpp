/*
  This class takes a texture and fills a specified screen area with in, using the proper
  blend mode. It also supports texture repeating, scaling, and offsets.
*/

#include "hge.h"

class hgeFill
{
    float tw, th;
    hgeQuad Quad;
    float fDepth, fA;
    HTEXTURE Owned;
public:
     hgeFill(HTEXTURE h);
     hgeFill(const char *fname);     // make texture from filename
     ~hgeFill();
    
     void SetBlendMode(int b) { Quad.blend = b; }
     void SetTexture(HTEXTURE h);
     void SetColor(DWORD col,int i = -1);
     void SetZ(float z,int i = -1);
     void SetDepth(float d) { fDepth = d; }     // divides the offset x, and y to make it appear 'further away'
     void SetA(float a = 0.5f) { fA = a; }      // this shifts the screen dest a partial point, such that it softens the fill
    
     void Render(int x, int y, int w, int h, float ox, float oy, float s);
}; 