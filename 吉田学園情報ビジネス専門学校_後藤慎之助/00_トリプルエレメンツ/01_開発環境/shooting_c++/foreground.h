//====================================================================
//
// 前景の処理 (foreground.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _FOREGROUND_H_
#define _FOREGROUND_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene2d.h"

//================================================
// マクロ定義
//================================================
#define FOREGROUND_FOREST_FADE_RATE (0.1f)                                                // 森の前景のフェードアウト割合
#define FOREGROUND_FOREST_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT,0.0f))         // 森の前景の位置
#define FOREGROUND_FOREST_SIZE (D3DXVECTOR3(SCREEN_WIDTH, 350.0f,0.0f))                   // 森の前景の大きさ
#define FOREGROUND_FOREST_FLOWING_SPEED 175                                               // 森の前景の流れる速度

#define FOREGROUND_CORAL_POS  (D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 120.0f,0.0f))// サンゴの前景の位置
#define FOREGROUND_CORAL_SIZE (D3DXVECTOR3(SCREEN_WIDTH + 250.0f, 250.0f,0.0f))           // サンゴの前景の大きさ
#define FOREGROUND_CORAL_FLOWING_SPEED 400                                                // サンゴの前景の流れる速度

//================================================
// クラス宣言
//================================================

// 前景クラス
class CForeground : public CScene2D
{
public:
    // 前景の種類
    typedef enum
    {
        TYPE_FOREST,          // 森
        TYPE_CORAL,           // サンゴ
        TYPE_MAX			  // 種類の最大数
    }TYPE;
    CForeground();
    ~CForeground();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CForeground *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, TYPE type);

private:
    static LPDIRECT3DTEXTURE9 m_apTexture[TYPE_MAX];   // 読み込んだテクスチャを保存する
    D3DXCOLOR m_col;    // 色
    TYPE m_type;        // 前景の種類
};

#endif