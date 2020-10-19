//===============================================
//
// 前景処理 (foreground.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "foreground.h"
#include "manager.h"
#include "renderer.h"
#include "library.h"
#include "game.h"

//=======================================
// マクロ定義
//=======================================

//=======================================
// 静的メンバ変数宣言
//=======================================
LPDIRECT3DTEXTURE9 CForeground::m_apTexture[CForeground::TYPE_MAX] = {};

//========================================
// 前景のコンストラクタ
//========================================
CForeground::CForeground() :CScene2D(OBJTYPE::OBJTYPE_FOREGROUND)
{
    m_col = DEFAULT_COLOR;
    m_type = CForeground::TYPE_MAX;
}

//========================================
// 前景のデストラクタ
//========================================
CForeground::~CForeground()
{

}

//========================================
// 前景の初期化処理
//========================================
HRESULT CForeground::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 初期化
    CScene2D::Init(pos, size);

    // テクスチャを割り当て
    BindTexture(m_apTexture[m_type]);

    return S_OK;
}

//========================================
// 前景の終了処理
//========================================
void CForeground::Uninit(void)
{
    // 終了処理
    CScene2D::Uninit();
}

//========================================
// 前景の更新処理
//========================================
void CForeground::Update(void)
{
    // 変数宣言
    D3DXVECTOR3 pos = GetPosition();                   // 位置を取得
    D3DXVECTOR3 size = GetSize();                      // サイズを取得

    switch (m_type)
    {
    case TYPE_FOREST:

        // ボスバトルの時、消滅
        if (CGame::GetBossBattle() == true)
        {
            m_col.a -= FOREGROUND_FOREST_FADE_RATE;
        }

        // テクスチャの頂点座標を変更
        SetFlowingAnimation(1, FOREGROUND_FOREST_FLOWING_SPEED, true, CScene2D::DIRECT_HORIZON);

        break;

    case TYPE_CORAL:

        // ボスバトルの時以外、流す
        if (CGame::GetBossBattle() == false)
        {
            // テクスチャの頂点座標を変更
            SetFlowingAnimation(1, FOREGROUND_CORAL_FLOWING_SPEED, true, CScene2D::DIRECT_HORIZON);
        }

        break;
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
// 前景の描画処理
//========================================
void CForeground::Draw(void)
{
    // 描画処理
    CScene2D::Draw();
}

//========================================
// 前景の画像ロード処理
//========================================
HRESULT CForeground::Load(void)
{
    // レンダラーからデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/foreground000.png", &m_apTexture[CForeground::TYPE_FOREST]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/foreground001.png", &m_apTexture[CForeground::TYPE_CORAL]);

    return S_OK;
}

//========================================
// 前景の画像破棄処理
//========================================
void CForeground::Unload(void)
{
    // テクスチャの破棄
    for (int nCntTexture = 0; nCntTexture < CForeground::TYPE_MAX; nCntTexture++)
    {
        if (m_apTexture[nCntTexture] != NULL)
        {
            m_apTexture[nCntTexture]->Release();
            m_apTexture[nCntTexture] = NULL;
        }
    }
}

//================================
// 前景の作成
//================================
CForeground *CForeground::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, CForeground::TYPE type)
{
    CForeground *pForeground = NULL;

    // メモリを確保
    pForeground = new CForeground;

    // タイプのみ、テクスチャを割り当てのために結びつけておく
    pForeground->m_type = type;

    // 初期化
    pForeground->Init(pos, size);

    // メンバ変数を結びつける
    pForeground->m_col = col;

    return pForeground;
}