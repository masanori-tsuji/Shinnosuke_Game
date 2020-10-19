//===============================================
//
// 反転合成処理 (reverse.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "reverse.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "library.h"
#include "player.h"
#include "bullet.h"
#include "game.h"

//=======================================
// マクロ定義
//=======================================

//=======================================
// 静的メンバ変数宣言
//=======================================
LPDIRECT3DTEXTURE9 CReverse::m_pTexture = NULL;

//========================================
// 反転合成のコンストラクタ
//========================================
CReverse::CReverse() :CScene2D(OBJTYPE::OBJTYPE_REVERSE)
{
    m_move = DEFAULT_VECTOR;
    m_col = DEFAULT_COLOR;
}

//========================================
// 反転合成のデストラクタ
//========================================
CReverse::~CReverse()
{

}

//========================================
// 反転合成の初期化処理
//========================================
HRESULT CReverse::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 初期化
    CScene2D::Init(pos, size);

    // テクスチャを割り当て
    BindTexture(m_pTexture);

    return S_OK;
}

//========================================
// 反転合成の終了処理
//========================================
void CReverse::Uninit(void)
{
    // 終了処理
    CScene2D::Uninit();
}

//========================================
// 反転合成の更新処理
//========================================
void CReverse::Update(void)
{
    // 変数宣言
    D3DXVECTOR3 size = GetSize();   // 大きさを取得

    // 大きさを最大まで拡大
    if (size.x <= REVERSE_MAX_SIZE)
    {
        size.x += REVERSE_EXTEND_RATE;
        size.y += REVERSE_EXTEND_RATE;
    }
    // 大きさが最大なら
    else
    {
        // 透明度を下げる
        m_col.a -= REVERSE_FADE_OUT_RATE;
    }

    // 色を変更
    CScene2D::SetColor(m_col);

    // 大きさを反映
    SetSize(size);

    // 頂点座標を変更
    SetVertex();

    // 透明なら消滅
    if (m_col.a < 0)
    {
        Uninit(); // これより下にコードは書かない
    }
}

//========================================
// 反転合成の描画処理
//========================================
void CReverse::Draw(void)
{
    // 反転合成に
    CScene2D::SetNega();

    // 描画処理
    CScene2D::Draw();
}

//========================================
// 反転合成の画像ロード処理
//========================================
HRESULT CReverse::Load(void)
{
    // レンダラーからデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    //テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/reverse000.png", &m_pTexture);	    // 反転合成

    return S_OK;
}

//========================================
// 反転合成の画像破棄処理
//========================================
void CReverse::Unload(void)
{
    // テクスチャの破棄
    if (m_pTexture != NULL)
    {
        m_pTexture->Release();
        m_pTexture = NULL;
    }
}

//================================
// 反転合成の作成
//================================
CReverse *CReverse::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    CReverse *pReverse = NULL;

    // メモリを確保
    pReverse = new CReverse;

    // 初期化
    pReverse->Init(pos, size);

    // 色を結びつける
    pReverse->m_col = DEFAULT_COLOR;

    return pReverse;
}