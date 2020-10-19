//====================================================================
//
// アイテムの処理 (item.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene2d.h"

//================================================
// マクロ定義
//================================================
#define ITEM_SIZE D3DXVECTOR3(55.0f, 55.0f, 0.0f)	 // アイテムのサイズ
#define ITEM_SPEED 5.0f                              // アイテムの速さ
#define ITEM_FLOWING_SPEED 2.5f                      // アイテムの流れる速さ
#define ITEM_ROT_ANGLE 0.05f                         // アイテムが流れる角度

#define ITEM_EFFECT_ADD_SIZE D3DXVECTOR3(50.0f, 50.0f, 0.0f)// アイテムのエフェクトの追加サイズ
#define BARRIER_COLOR D3DXCOLOR(0.2f, 0.858f, 0.98f, 1.0f)  // バリアの色
#define STOCK_COLOR D3DXCOLOR(1.0f, 0.262f, 0.447f, 1.0f)   // ストックの色

//================================================
// クラス宣言
//================================================

// アイテムクラス
class CItem : public CScene2D
{
public:
    // アイテムの種類
    typedef enum
    {
        TYPE_BARRIER,   // バリア
        TYPE_STOCK,     // ハート
        TYPE_MAX        // 種類の最大数
    }TYPE;
    CItem();
    ~CItem();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CItem *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);
    TYPE GetType(void) { return m_type; }   // タイプを取得

private:
    static LPDIRECT3DTEXTURE9 m_apTexture[TYPE_MAX];   // 読み込んだテクスチャを保存する
    TYPE m_type;        // アイテムの種類
    float m_fAngle;     // 角度
    D3DXVECTOR3 m_move; // 移動量
};

#endif