//====================================================================
//
// 背景処理 (bg.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _BG_H_
#define _BG_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene.h"

//================================================
// 前方宣言(不完全なクラスは、cppの方でヘッダを呼び出す)
//================================================
class CScene2D;

//================================================
// マクロ定義
//================================================

//================================================
// クラス宣言
//================================================

// 背景クラス
class CBg : public CScene
{
public:

    // 背景のセット
    typedef enum
    {
        SET_NONE = 0,   // なし
        SET_α,         // α
        SET_β,         // β
        SET_γ,         // γ
        SET_MAX         // セットの最大数
    }SET;

    // 背景の種類
    typedef enum
    {
        TYPE_SKY = 0,   // 空
        TYPE_MOUNTAIN,  // 山
        TYPE_WOODS,     // 林
        TYPE_FOREST,    // 森（ボスのかつら）
        TYPE_SAND,      // 砂
        TYPE_OCEAN,     // 海
        TYPE_ROCKS,     // 岩
        TYPE_FISHES,    // 魚群
        TYPE_DEAD_END,  // 行き止まり
        TYPE_RED_SKY,   // 赤い空
        TYPE_RED_GROUND,// 赤い地面
        TYPE_VOLCANO,   // 火山
        TYPE_THE_TOP,   // 頂上
        TYPE_MAX        // 背景の最大数
    }TYPE;

    CBg();
    ~CBg();
    static HRESULT Load(void);
    static void Unload(void);
    static CBg *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, SET set);
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static void Shake(int nCntState) { m_bShake = true; m_nCntState = nCntState; }  // 受け取った分揺らす
    static void EraseForest(void) { m_bEraseForest = true; }                        // ボス森を消す

    static bool GetInTheSky(void) { return m_bInTheSky; }                           // 空の中にいるかどうかを返す

private:
    static LPDIRECT3DTEXTURE9 m_apTexture[TYPE_MAX];   // テクスチャへのポインタ
    CScene2D *m_apScene2D[MAX_POLYGON];                // 2Dポリゴンへのポインタ
    D3DXCOLOR m_col;                                   // 色
    SET m_set;                                         // 描画するもののセット
    int m_nCntTime;                                    // 時間をカウント

    static bool m_bShake;                              // 揺らすかどうか
    static int m_nCntState;                            // 状態カウンタ
    static bool m_bEraseForest;                        // 森を消すかどうか

    static bool m_bInTheSky;                           // 空の中かどうか
};

#endif