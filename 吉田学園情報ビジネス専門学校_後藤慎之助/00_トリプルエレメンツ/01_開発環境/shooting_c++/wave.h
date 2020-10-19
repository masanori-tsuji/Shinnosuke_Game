//====================================================================
//
// 波の処理 (wave.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _WAVE_H_
#define _WAVE_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene2d.h"

//================================================
// マクロ定義
//================================================
#define WAVE_FADE_RATE (0.025f)                                             // 波のフェードアウト割合
#define WAVE_EXTEND_RATE 0.5f                                               // 波の拡大縮小
#define WAVE_FADE_RATE2 (0.02f)                                             // 波のフェードアウト割合
#define WAVE_EXTEND_RATE2 1.0f                                              // 波の拡大縮小
#define WAVE_MIN_COLOR 0.3f                                                 // 波の最小の色
#define WAVE_COLOR (D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.375f))                    // 波の色
#define WAVE_COLOR2 (D3DXCOLOR(WAVE_MIN_COLOR, WAVE_MIN_COLOR, 1.0f, 0.35f))// 波の色2
#define WAVE_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f))   // 波の位置
#define WAVE_MIN_SIZE 1000.0f                                               // 波の最小の大きさ
#define WAVE_SIZE (D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f))          // 波の大きさ
#define WAVE_SIZE2 (D3DXVECTOR3(SCREEN_WIDTH, WAVE_MIN_SIZE, 0.0f))         // 波の大きさ2

//================================================
// クラス宣言
//================================================

// 波クラス
class CWave : public CScene2D
{
public:
    // 波の種類
    typedef enum
    {
        TYPE_BIG_WAVE,          // 大きい波
        TYPE_BIG_WAVE2,         // 大きい波2
        TYPE_MAX			    // 種類の最大数
    }TYPE;
    CWave();
    ~CWave();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CWave *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, TYPE type);

private:
    static LPDIRECT3DTEXTURE9 m_apTexture[TYPE_MAX];   // 読み込んだテクスチャを保存する
    bool m_bExtend;     // 拡大縮小のフラグ
    bool m_bToBlue;     // 青に近づけるフラグ
    D3DXVECTOR3 m_move; // 移動量
    D3DXCOLOR m_col;    // 色
    TYPE m_type;        // 波の種類
};

#endif