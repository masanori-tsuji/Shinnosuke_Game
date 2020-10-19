//====================================================================
//
// 白フェードの処理 (whiteFade.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _WHITEFADE_H_
#define _WHITEFADE_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene2d.h"

//================================================
// マクロ定義
//================================================
#define WHITE_FADE_RATE (0.005f)   // 白フェード割合
#define WHITE_FADE_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f)) // 白フェードの位置
#define WHITE_FADE_SIZE (D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f))        // 白フェードのサイズ
#define WHITE_FADE_END_TIME (240)               // 白フェードが終わる時間
#define WHITE_FADE_CONSIDER_TRANSPARENT (0.3f)  // 透明とみなすα値の値

//================================================
// クラス宣言
//================================================

// 白フェードクラス
class CWhiteFade : public CScene2D
{
public:
    // 白フェードの種類
    typedef enum
    {
        TYPE_α = 0,	// α
        TYPE_β,	    // β
        TYPE_MAX		// 種類の最大数
    }TYPE;

    CWhiteFade();
    ~CWhiteFade();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CWhiteFade *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nTime, D3DXCOLOR col, TYPE type);

private:
    static LPDIRECT3DTEXTURE9 m_apTexture[TYPE_MAX];   // 読み込んだテクスチャを保存する
    D3DXCOLOR m_col;    // 色
    int m_nTimer;       // カウンタ
    TYPE m_type;        // 白フェードの種類

    bool m_bAlphaDown;  // 透明度を下げるフラグ
};

#endif