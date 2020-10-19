//====================================================================
//
// 反転合成の処理 (reverse.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _REVERSE_H_
#define _REVERSE_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene2d.h"

//================================================
// マクロ定義
//================================================

#define REVERSE_MAX_SIZE 7000.0f            // 最大サイズ
#define REVERSE_EXTEND_RATE 10.0f            // 拡大割合
#define REVERSE_FADE_OUT_RATE 0.01f         // 反転合成のフェードアウト割合

//================================================
// クラス宣言
//================================================

// 反転合成クラス
class CReverse : public CScene2D
{
public:
    CReverse();
    ~CReverse();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CReverse *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

private:
    static LPDIRECT3DTEXTURE9 m_pTexture;   // 読み込んだテクスチャを保存する
    D3DXVECTOR3 m_move;     // 移動量
    D3DXCOLOR m_col;        // 色
};

#endif