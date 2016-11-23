/*
  hgeFill class for hGE 1.5!
  Jason A. Petrasko, Public Domain Code

  This class takes a texture and fills a specified screen area with in, using the proper
  blend mode. It also supports texture repeating, scaling, and offsets.
*/

#include "hge.h"
#include "RF2d_AreaFill.h"
#include "math.h"

extern HGE *hge;

hgeFill::hgeFill(HTEXTURE h)
{
   SetTexture(h);
   Quad.blend = BLEND_DEFAULT;
   Quad.v[0].z = 0.0f;
   Quad.v[1].z = 0.0f;
   Quad.v[2].z = 0.0f;
   Quad.v[3].z = 0.0f;
   Quad.v[0].col = 0xFFFFFFFF;
   Quad.v[1].col = 0xFFFFFFFF;
   Quad.v[2].col = 0xFFFFFFFF;
   Quad.v[3].col = 0xFFFFFFFF;
   fDepth = 1.0f;
   fA = 0.0f;
}

hgeFill::hgeFill(const char *fname)
{
   Owned = hge->Texture_Load(fname);
   SetTexture(Owned);
   Quad.blend = BLEND_DEFAULT;
   Quad.v[0].z = 0.0f;
   Quad.v[1].z = 0.0f;
   Quad.v[2].z = 0.0f;
   Quad.v[3].z = 0.0f;
   Quad.v[0].col = 0xFFFFFFFF;
   Quad.v[1].col = 0xFFFFFFFF;
   Quad.v[2].col = 0xFFFFFFFF;
   Quad.v[3].col = 0xFFFFFFFF;
   fDepth = 1.0f;
   fA = 0.0f;
}

hgeFill::~hgeFill()
{
   if (Owned) hge->Texture_Free(Owned);
}

void hgeFill::SetTexture(HTEXTURE h)
{
   Quad.tex = h;
   th = hge->Texture_GetWidth(h);
   tw = hge->Texture_GetHeight(h);
}
                
void hgeFill::SetColor(DWORD col,int i)
{
   if (i == -1)
   {
    Quad.v[0].col = col;
    Quad.v[1].col = col;
    Quad.v[2].col = col;
    Quad.v[3].col = col;
     } else
   {
       Quad.v[i].col = col;
   }
}

void hgeFill::SetZ(float z,int i)
{
   if (i == -1)
   {
    Quad.v[0].z = z;
    Quad.v[1].z = z;
    Quad.v[2].z = z;
    Quad.v[3].z = z;
     } else
   {
           Quad.v[i].z = z;
   }
}

void hgeFill::Render(int x, int y, int w, int h, float ox, float oy, float s)
{
   register float cw, ch, sx, sy, ex, ey;
   
   cw = tw;
   ch = th;
   
   // figure out the offset
   ox = fmod(ox,cw) / fDepth;
   oy = fmod(oy,ch) / fDepth;
   
   sx = ox / cw;
   sy = oy / ch;
   
   ex = ((float)w / (cw*s)) + sx;
   ey = ((float)h / (ch*s)) + sy;
   
   // ok, fill in the quad info.
   Quad.v[0].x = x+fA;
   Quad.v[0].y = y+fA;
   Quad.v[0].tx = sx;
   Quad.v[0].ty = sy;
   Quad.v[1].x = x+w+fA;
   Quad.v[1].y = y+fA;
   Quad.v[1].tx = ex;
   Quad.v[1].ty = sy;
   Quad.v[2].x = x+w+fA;
   Quad.v[2].y = y+h+fA;
   Quad.v[2].tx = ex;
   Quad.v[2].ty = ey;
   Quad.v[3].x = x+fA;
   Quad.v[3].y = y+h+fA;
   Quad.v[3].tx = sx;
   Quad.v[3].ty = ey;

   // draw the damn thing already!
   hge->Gfx_RenderQuad(&Quad);
} 