//====================================================================
//
// ライト2Dの処理 (light2d.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _LIGHT2D_H_
#define _LIGHT2D_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene2d.h"

//================================================
// マクロ定義
//================================================

#define LIGHT_FIRST_POS D3DXVECTOR3(-400.0f, -400.0f, 0.0f)                 // ライト2Dの初期位置
#define LIGHT_SIZE D3DXVECTOR3(SCREEN_WIDTH * 4, SCREEN_HEIGHT * 4, 0.0f)   // ライト2Dの大きさ
#define LIGHT_SET_COLOR D3DXCOLOR(1.0f,1.0f,1.0f,0.001f)                    // ライト2Dのカラー
#define LIGHT_FADE_OUT_RATE 0.01f                                           // ライト2Dによる、フェードアウト割合
#define LIGHT_CONSIDER_CLOSE_DEST 20.0f                                     // 目的の位置に近いとみなす値

// 照らす場所とそこに到達するためのスピード
#define LIGHT_SPOT_POS_X_1 300.0f
#define LIGHT_SPOT_POS_Y_1 580.0f
#define LIGHT_SPOT_SPEED_1 10.0f

#define LIGHT_SPOT_POS_X_2 680.0f
#define LIGHT_SPOT_POS_Y_2 180.0f
#define LIGHT_SPOT_SPEED_2 7.5f

#define LIGHT_SPOT_POS_X_3 980.0f
#define LIGHT_SPOT_POS_Y_3 380.0f
#define LIGHT_SPOT_SPEED_3 5.0f

// ボスの位置までの到達
#define LIGHT_SPOT_SPEED_4 2.5f
#define LIGHT_EXTEND_RATE_X 180.0f  // 到達後の拡大X（画面比 縦:横 = 2:3）
#define LIGHT_EXTEND_RATE_Y 120.0f  // 到達後の拡大Y（画面比 縦:横 = 2:3）
#define LIGHT_MAX_SIZE_X 54000.0f   // 穴あきの画像の、穴でだいたい画面が明るく見えるときの大きさ

//================================================
// クラス宣言
//================================================

// ライト2Dクラス
class CLight2D : public CScene2D
{
public:

    CLight2D();
    ~CLight2D();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CLight2D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

private:
    static LPDIRECT3DTEXTURE9 m_pTexture;   // 読み込んだテクスチャを保存する
    D3DXCOLOR m_col;        // 色
    int m_nStage;           // ライトを照らす段階
};

#endif