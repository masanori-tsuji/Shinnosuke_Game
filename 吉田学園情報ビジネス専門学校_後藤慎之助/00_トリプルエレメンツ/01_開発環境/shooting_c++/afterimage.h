//====================================================================
//
// 残像の処理 (afterimage.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _AFTERIMAGE_H_
#define _AFTERIMAGE_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene2d.h"
#include "player.h"

//================================================
// マクロ定義
//================================================
#define AFTERIMAGE_PLAYER_FADE_RATE (0.07f)                              // プレイヤーの残像のフェードアウト割合
#define AFTERIMAGE_PLAYER_COLOR (D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f))      // プレイヤーの残像の色

#define AFTERIMAGE_PLAYER_DISP_POS (D3DXVECTOR3(-100.0f, PLAYER_START_POS_Y, 0.0f))     // プレイヤーの配置位置

#define AFTERIMAGE_BOSS_FADE_RATE (0.002f)                                       // ボスの残像のフェードアウト割合
#define AFTERIMAGE_BOSS_RADIUS (25000)                                           // ボスの半径
#define AFTERIMAGE_BOSS_SET_EFFECT (15)                                          // ボスのエフェクトをセットするカウンタ
#define AFTERIMAGE_BOSS_EXPLOSION_SIZE (D3DXVECTOR3(250.0f, 250.0f, 0.0f))       // ボスの爆発エフェクトサイズ

#define MAGIC_EXTEND_RATE 20.0f                                          // 神通力の拡大割合
#define MAGIC_FADE_OUT_RATE 0.05f                                        // 神通力のフェードアウト割合

#define AFTERIMAGE_RANKING_FADE_OUT_POS_X -250.0f                        // ランキング画面で、残像が消えるX座標

//================================================
// クラス宣言
//================================================

// 残像クラス
class CAfterimage : public CScene2D
{
public:
    // 残像の種類
    typedef enum
    {
        TYPE_PLAYER = 0,        // プレイヤー
        TYPE_PLAYER_DISP,       // プレイヤー配置
        TYPE_BOSS_FOREST,	    // ボス森
        TYPE_BOSS_DRAGON,	    // ボスドラゴン
        TYPE_BOSS_BLAZE,        // ボスブレイズ
        TYPE_GOD_BIRD,          // ゴッドバード
        TYPE_MAGIC,             // 神通力
        TYPE_RANK_GUSOKU,       // ランキングのグソクムシ
        TYPE_RANK_CROW,         // ランキングの白カラス
        TYPE_RANK_GREEN,        // ランキングの緑鳥
        TYPE_RANK_BLUE,         // ランキングの青鳥
        TYPE_RANK_RED,          // ランキングの赤鳥
        TYPE_MAX			    // 種類の最大数
    }TYPE;
    CAfterimage();
    ~CAfterimage();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CAfterimage *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, float fAngle, D3DXCOLOR col, TYPE type);

    void RankingFadeOut(D3DXVECTOR3 pos);   // ランキング画面で、消える演出

private:
    static LPDIRECT3DTEXTURE9 m_apTexture[TYPE_MAX];   // 読み込んだテクスチャを保存する
    int m_nCntTime;     // カウンタ
    float m_fAngle;     // 角度
    D3DXVECTOR3 m_move; // 移動量
    D3DXCOLOR m_col;    // 色
    TYPE m_type;        // 残像の種類
};

#endif