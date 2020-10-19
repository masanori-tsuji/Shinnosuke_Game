//====================================================================
//
// 泡の処理 (bubble.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _BUBBLE_H_
#define _BUBBLE_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene2d.h"

//================================================
// マクロ定義
//================================================

#define BUBBLE_MINIMUM_SIZE D3DXVECTOR3(10.0f, 10.0f, 0.0f)        // 極小の大きさ
#define BUBBLE_SMALL_SIZE D3DXVECTOR3(15.0f, 15.0f, 0.0f)          // 小の大きさ
#define BUBBLE_NORMAL_SIZE D3DXVECTOR3(20.0f, 20.0f, 0.0f)         // 通常の大きさ
#define BUBBLE_NORMAL2_SIZE D3DXVECTOR3(25.0f, 25.0f, 0.0f)        // 通常の大きさ2
#define BUBBLE_BIG_SIZE D3DXVECTOR3(30.0f, 30.0f, 0.0f)            // 大の大きさ
#define BUBBLE_MAXIMUM_SIZE D3DXVECTOR3(35.0f, 35.0f, 0.0f)        // 極大の大きさ

#define BUBBLE_MINIMUM_MOVE D3DXVECTOR3(7.5f, -7.5f, 0.0f)        // 極小の動き
#define BUBBLE_SMALL_MOVE D3DXVECTOR3(-8.0f, -8.0f, 0.0f)         // 小の動き
#define BUBBLE_NORMAL_MOVE D3DXVECTOR3(8.5f, -8.5f, 0.0f)         // 通常の動き
#define BUBBLE_NORMAL2_MOVE D3DXVECTOR3(-9.0f, -9.0f, 0.0f)       // 通常の動き2
#define BUBBLE_BIG_MOVE D3DXVECTOR3(9.5f, -9.5f, 0.0f)            // 大の動き
#define BUBBLE_MAXIMUM_MOVE D3DXVECTOR3(-10.0f, -10.0f, 0.0f)     // 極大の動き

#define BUBBLE_COLOR D3DXCOLOR(1.0f,1.0f,1.0f,0.55f)            // 泡のカラー
#define BUBBLE_FADE_OUT_RATE 0.01f                              // 泡のフェードアウト割合
#define BUBBLE_RETURN_POS (13.0f)                               // 泡の跳ね返り位置

//================================================
// クラス宣言
//================================================

// 泡クラス
class CBubble : public CScene2D
{
public:
    // 泡の種類
    typedef enum
    {
        TYPE_MINIMUM = 0,	// 極小
        TYPE_SMALL,         // 小
        TYPE_NORMAL,        // 通常
        TYPE_NORMAL2,       // 通常2
        TYPE_BIG,           // 大
        TYPE_MAXIMUM,       // 極大
        TYPE_MAX			// 種類の最大数
    }TYPE;
    CBubble();
    ~CBubble();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CBubble *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);

private:
    static LPDIRECT3DTEXTURE9 m_pTexture;   // 読み込んだテクスチャを保存する
    D3DXVECTOR3 m_move;     // 移動量
    D3DXVECTOR3 m_posSet;   // セット位置を記憶
    D3DXCOLOR m_col;        // 色
    TYPE m_type;            // 泡の種類
};

#endif