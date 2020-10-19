//===============================================
//
// 波処理 (wave.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "wave.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "library.h"
#include "game.h"
#include "effect.h"
#include "sound.h"
#include "whiteFade.h"
#include "player.h"

//=======================================
// マクロ定義
//=======================================

//=======================================
// 静的メンバ変数宣言
//=======================================
LPDIRECT3DTEXTURE9 CWave::m_apTexture[CWave::TYPE_MAX] = {};

//========================================
// 波のコンストラクタ
//========================================
CWave::CWave() :CScene2D(OBJTYPE::OBJTYPE_WAVE)
{
    m_bExtend = false;
    m_bToBlue = true;
    m_move = DEFAULT_VECTOR;
    m_col = DEFAULT_COLOR;
    m_type = CWave::TYPE_MAX;
}

//========================================
// 波のデストラクタ
//========================================
CWave::~CWave()
{

}

//========================================
// 波の初期化処理
//========================================
HRESULT CWave::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 初期化
    CScene2D::Init(pos, size);

    // テクスチャを割り当て
    BindTexture(m_apTexture[m_type]);

    return S_OK;
}

//========================================
// 波の終了処理
//========================================
void CWave::Uninit(void)
{
    // 終了処理
    CScene2D::Uninit();
}

//========================================
// 波の更新処理
//========================================
void CWave::Update(void)
{
    // 変数宣言
    D3DXVECTOR3 pos = GetPosition();                   // 位置を取得
    D3DXVECTOR3 size = GetSize();                      // サイズを取得
    float fAngle = 0.0f;                               // 角度
    float fRadius = 0.0f;                              // 半径

    switch (m_type)
    {
    case TYPE_BIG_WAVE:

        // 青に近づけるか、遠ざけるか
        if (m_bToBlue == true)
        {
            m_col.r -= WAVE_FADE_RATE;
            m_col.g -= WAVE_FADE_RATE;
        }
        else
        {
            m_col.r += WAVE_FADE_RATE;
            m_col.g += WAVE_FADE_RATE;
        }

        // 拡大するか、縮小するか
        if (m_bExtend == true)
        {
            size.y += WAVE_EXTEND_RATE;
        }
        else
        {
            size.y -= WAVE_EXTEND_RATE;
        }

        // テクスチャの頂点座標を変更
        SetFlowingAnimation(1, 400, true, CScene2D::DIRECT_RIGHT_DOWN);

        break;

    case TYPE_BIG_WAVE2:

        // 青に近づけるか、遠ざけるか
        if (m_bToBlue == true)
        {
            m_col.r -= WAVE_FADE_RATE2;
            m_col.g -= WAVE_FADE_RATE2;
        }
        else
        {
            m_col.r += WAVE_FADE_RATE2;
            m_col.g += WAVE_FADE_RATE2;
        }

        // 拡大するか、縮小するか
        if (m_bExtend == true)
        {
            size.y += WAVE_EXTEND_RATE2;
        }
        else
        {
            size.y -= WAVE_EXTEND_RATE2;
        }

        // テクスチャの頂点座標を変更
        SetFlowingAnimation(1, 500, true, CScene2D::DIRECT_HORIZON);

        break;
    }

    // サイズを変える
    if (m_col.r > 1.0f)
    {
        m_bToBlue = true;
    }
    else if (m_col.r < WAVE_MIN_COLOR)
    {
        m_bToBlue = false;
    }

    // サイズを変える
    if (size.y > SCREEN_HEIGHT)
    {
        m_bExtend = false;
    }
    else if(size.y < WAVE_MIN_SIZE)
    {
        m_bExtend = true;
    }

    // 色を反映
    SetColor(m_col);

    // 位置を反映
    SetPosition(pos);

    // サイズを反映
    SetSize(size);

    // 頂点座標を設定
    SetVertex();

    // 透明なら消滅
    if (m_col.a < 0)
    {
        Uninit(); // これより下にコードは書かない
    }
}

//========================================
// 波の描画処理
//========================================
void CWave::Draw(void)
{
    // 描画処理
    CScene2D::Draw();
}

//========================================
// 波の画像ロード処理
//========================================
HRESULT CWave::Load(void)
{
    // レンダラーからデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/wave000.png", &m_apTexture[CWave::TYPE_BIG_WAVE]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/wave001.png", &m_apTexture[CWave::TYPE_BIG_WAVE2]);

    return S_OK;
}

//========================================
// 波の画像破棄処理
//========================================
void CWave::Unload(void)
{
    // テクスチャの破棄
    for (int nCntTexture = 0; nCntTexture < CWave::TYPE_MAX; nCntTexture++)
    {
        if (m_apTexture[nCntTexture] != NULL)
        {
            m_apTexture[nCntTexture]->Release();
            m_apTexture[nCntTexture] = NULL;
        }
    }
}

//================================
// 波の作成
//================================
CWave *CWave::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, CWave::TYPE type)
{
    CWave *pWave = NULL;

    // メモリを確保
    pWave = new CWave;

    // タイプのみ、テクスチャを割り当てのために結びつけておく
    pWave->m_type = type;

    // 初期化
    pWave->Init(pos, size);

    // メンバ変数を結びつける
    pWave->m_col = col;

    // 種類に応じた場合分け
    switch (type)
    {
    case TYPE_BIG_WAVE:

        pWave->m_bExtend = false;

        pWave->m_bToBlue = true;

        break;

    case TYPE_BIG_WAVE2:

        pWave->m_bExtend = true;

        pWave->m_bToBlue = false;

        break;
    }

    return pWave;
}