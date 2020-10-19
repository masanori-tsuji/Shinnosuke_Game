//====================================================================
//
// 描画処理 (renderer.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"

//================================================
// クラス宣言
//================================================

// レンダリングクラス
class CRenderer
{
public:
    CRenderer();
    ~CRenderer();
    HRESULT Init(HWND hWnd, BOOL bWindow);
    void Uninit(void);
    void Update(void);    // オブジェクトのインスタンスがないと、関数が呼べないため持ってくる
    void Draw(void);      // オブジェクトのインスタンスがないと、関数が呼べないため持ってくる
    //static LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; } // パブリックのstaticは、C言語風になる(なので、mainにGetRenderer)
    LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }

private:
    LPDIRECT3D9				        m_pD3D;			// Direct3Dオブジェクト
    //static LPDIRECT3DDEVICE9		m_pD3DDevice;	// Deviceオブジェクト(描画に必要)(この場合のstaticは、C言語風)
    LPDIRECT3DDEVICE9		m_pD3DDevice;	// Deviceオブジェクト(描画に必要)
#ifdef _DEBUG
    void DrawFPS(void);
    LPD3DXFONT				m_pFont;        // フォントへのポインタ
#endif
};

#endif