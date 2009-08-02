/*
Copyright (c) 2009 Peter "Corsix" Cawley

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef CORSIX_TH_TH_GFX_SDL_H_
#define CORSIX_TH_TH_GFX_SDL_H_
#include "config.h"
#ifdef CORSIX_TH_USE_SDL_RENDERER
#ifdef CORSIX_TH_HAS_RENDERING_ENGINE
#error More than one rendering engine enabled in config file
#endif
#define CORSIX_TH_HAS_RENDERING_ENGINE

struct SDL_Surface;
typedef SDL_Surface THRenderTarget;

struct THClipRect
{
    // Conveniently the same as an SDL_Rect
	int16_t x, y;
	uint16_t w, h;
};

void THRenderTarget_GetClipRect(const THRenderTarget* pTarget, THClipRect* pRect);
void THRenderTarget_SetClipRect(THRenderTarget* pTarget, const THClipRect* pRect);

void THRenderTarget_StartNonOverlapping(THRenderTarget* pTarget);
void THRenderTarget_FinishNonOverlapping(THRenderTarget* pTarget);

class THPalette
{
public:
    THPalette();

    bool loadFromTHFile(const unsigned char* pData, size_t iDataLength);

    void assign(THRenderTarget* pTarget, bool bTransparent = true) const;

protected:
    struct colour_t
    {
        // Conveniently the same as an SDL_Color
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char unused;
    } m_aColours[256];
    int m_iNumColours;
};

class THSpriteSheet
{
public:
    THSpriteSheet();
    ~THSpriteSheet();

    void setPalette(const THPalette* pPalette);

    bool loadFromTHFile(const unsigned char* pTableData, size_t iTableDataLength,
                        const unsigned char* pChunkData, size_t iChunkDataLength,
                        bool bComplexChunks, THRenderTarget *pUnused = NULL);

    void setSpriteAltPaletteMap(unsigned int iSprite, const unsigned char* pMap);

    unsigned int getSpriteCount() const;
    bool getSpriteSize(unsigned int iSprite, unsigned int* pX, unsigned int* pY) const;
    void getSpriteSizeUnchecked(unsigned int iSprite, unsigned int* pX, unsigned int* pY) const;

    void drawSprite(THRenderTarget* pCanvas, unsigned int iSprite, int iX, int iY, unsigned long iFlags);

protected:
#pragma pack(push)
#pragma pack(1)
    struct th_sprite_t
    {
        uint32_t position;
        unsigned char width;
        unsigned char height;
    };
#pragma pack(pop)

    struct sprite_t
    {
        THRenderTarget *pBitmap[32];
        unsigned char *pData;
        const unsigned char *pAltPaletteMap;
        unsigned int iWidth;
        unsigned int iHeight;
    } *m_pSprites;
    const THPalette* m_pPalette;
    unsigned int m_iSpriteCount;
    bool m_bHasAnyFlaggedBitmaps;

    void _freeSprites();
    THRenderTarget* _getSpriteBitmap(unsigned int iSprite, unsigned long iFlags);
};

#endif // CORSIX_TH_USE_SDL_RENDERER
#endif // CORSIX_TH_TH_GFX_SDL_H_
