//====================================================================
//
// ロゴの処理 (logo.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _LOGO_H_
#define _LOGO_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene2d.h"

//================================================
// マクロ定義
//================================================
#define LOGO_BG_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f))   // 背景ロゴの位置
#define LOGO_BG_SIZE (D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f))          // 背景ロゴの大きさ

//================================================
// クラス宣言
//================================================

// ロゴクラス
class CLogo : public CScene2D
{
public:
    // ロゴの種類
    typedef enum
    {
        TYPE_TITLE_BG,          // タイトルの背景
        TYPE_RESULT_BG,         // リザルトの背景
        TYPE_MAX			    // 種類の最大数
    }TYPE;
    CLogo();
    ~CLogo();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CLogo *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, TYPE type);

private:
    static LPDIRECT3DTEXTURE9 m_apTexture[TYPE_MAX];   // 読み込んだテクスチャを保存する
    D3DXCOLOR m_col;    // 色
    TYPE m_type;        // ロゴの種類
};

#endif