//===============================================
//
// ロゴ処理 (logo.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "logo.h"
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
LPDIRECT3DTEXTURE9 CLogo::m_apTexture[CLogo::TYPE_MAX] = {};

//========================================
// ロゴのコンストラクタ
//========================================
CLogo::CLogo() :CScene2D(OBJTYPE::OBJTYPE_LOGO)
{
    m_col = DEFAULT_COLOR;
    m_type = CLogo::TYPE_MAX;
}

//========================================
// ロゴのデストラクタ
//========================================
CLogo::~CLogo()
{

}

//========================================
// ロゴの初期化処理
//========================================
HRESULT CLogo::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 初期化
    CScene2D::Init(pos, size);

    // テクスチャを割り当て
    BindTexture(m_apTexture[m_type]);

    return S_OK;
}

//========================================
// ロゴの終了処理
//========================================
void CLogo::Uninit(void)
{
    // 終了処理
    CScene2D::Uninit();
}

//========================================
// ロゴの更新処理
//========================================
void CLogo::Update(void)
{
    // 変数宣言
    D3DXVECTOR3 pos = GetPosition();                   // 位置を取得
    D3DXVECTOR3 size = GetSize();                      // サイズを取得

    switch (m_type)
    {
    case TYPE_RESULT_BG:

        // テクスチャの頂点座標を変更
        SetFlowingAnimation(1, 400, true, CScene2D::DIRECT_RIGHT_DOWN);

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
}

//========================================
// ロゴの描画処理
//========================================
void CLogo::Draw(void)
{
    // 描画処理
    CScene2D::Draw();
}

//========================================
// ロゴの画像ロード処理
//========================================
HRESULT CLogo::Load(void)
{
    // レンダラーからデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title000.png", &m_apTexture[CLogo::TYPE_TITLE_BG]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo000.png", &m_apTexture[CLogo::TYPE_RESULT_BG]);

    return S_OK;
}

//========================================
// ロゴの画像破棄処理
//========================================
void CLogo::Unload(void)
{
    // テクスチャの破棄
    for (int nCntTexture = 0; nCntTexture < CLogo::TYPE_MAX; nCntTexture++)
    {
        if (m_apTexture[nCntTexture] != NULL)
        {
            m_apTexture[nCntTexture]->Release();
            m_apTexture[nCntTexture] = NULL;
        }
    }
}

//================================
// ロゴの作成
//================================
CLogo *CLogo::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, CLogo::TYPE type)
{
    CLogo *pLogo = NULL;

    // メモリを確保
    pLogo = new CLogo;

    // タイプのみ、テクスチャを割り当てのために結びつけておく
    pLogo->m_type = type;

    // 初期化
    pLogo->Init(pos, size);

    // メンバ変数を結びつける
    pLogo->m_col = col;

    return pLogo;
}