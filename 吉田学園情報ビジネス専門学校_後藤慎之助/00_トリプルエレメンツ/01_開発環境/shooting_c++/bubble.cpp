//===============================================
//
// 泡処理 (bubble.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "bubble.h"
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
LPDIRECT3DTEXTURE9 CBubble::m_pTexture = NULL;

//========================================
// 泡のコンストラクタ
//========================================
CBubble::CBubble() :CScene2D(OBJTYPE::OBJTYPE_BUBBLE)
{
    m_move = DEFAULT_VECTOR;
    m_posSet = DEFAULT_VECTOR;
    m_col = DEFAULT_COLOR;
    m_type = CBubble::TYPE_MAX;
}

//========================================
// 泡のデストラクタ
//========================================
CBubble::~CBubble()
{

}

//========================================
// 泡の初期化処理
//========================================
HRESULT CBubble::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 初期化
    CScene2D::Init(pos, size);

    // テクスチャを割り当て
    BindTexture(m_pTexture);

    return S_OK;
}

//========================================
// 泡の終了処理
//========================================
void CBubble::Uninit(void)
{
    // 終了処理
    CScene2D::Uninit();
}

//========================================
// 泡の更新処理
//========================================
void CBubble::Update(void)
{
    // 変数宣言
    D3DXVECTOR3 pos = GetPosition();                   // 位置を取得

    // 移動量を位置に反映(過去から似たものを引っ張ったら、速かったから遅くしている)
    pos += m_move / 3;

    // 左右の移動範囲
    if (pos.x < m_posSet.x - BUBBLE_RETURN_POS || 
        pos.x > m_posSet.x + BUBBLE_RETURN_POS)
    {
        // 跳ね返る
        m_move.x *= -1;
    }

    // 透明度を下げる
    m_col.a -= BUBBLE_FADE_OUT_RATE;

    // 色を変更
    CScene2D::SetColor(m_col);

    // 位置を反映
    SetPosition(pos);

    // 頂点座標を変更
    SetVertex();

    // 透明なら消滅
    if (m_col.a < 0)
    {
        Uninit(); // これより下にコードは書かない
    }
}

//========================================
// 泡の描画処理
//========================================
void CBubble::Draw(void)
{
    // 加算合成
    SetAdditiveSynthesis();

    // 描画処理
    CScene2D::Draw();
}

//========================================
// 泡の画像ロード処理
//========================================
HRESULT CBubble::Load(void)
{
    // レンダラーからデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    //テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect000.jpg", &m_pTexture);	    // 泡

    return S_OK;
}

//========================================
// 泡の画像破棄処理
//========================================
void CBubble::Unload(void)
{
    // テクスチャの破棄
    if (m_pTexture != NULL)
    {
        m_pTexture->Release();
        m_pTexture = NULL;
    }
}

//================================
// 泡の作成
//================================
CBubble *CBubble::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, CBubble::TYPE type)
{
    CBubble *pBubble = NULL;

    // メモリを確保
    pBubble = new CBubble;

    // タイプのみ、テクスチャを割り当てのために結びつけておく
    pBubble->m_type = type;

    // 初期化
    pBubble->Init(pos, size);

    // 初期位置、色を結びつける
    pBubble->m_posSet = pos;
    pBubble->m_col = BUBBLE_COLOR;

    // 種類によって、移動量を変える
    switch (pBubble->m_type)
    {
    case TYPE_MINIMUM:
        pBubble->m_move = BUBBLE_MINIMUM_MOVE;
        break;

    case TYPE_SMALL:
        pBubble->m_move = BUBBLE_SMALL_MOVE;
        break;

    case TYPE_NORMAL:
        pBubble->m_move = BUBBLE_NORMAL_MOVE;
        break;

    case TYPE_NORMAL2:
        pBubble->m_move = BUBBLE_NORMAL2_MOVE;
        break;

    case TYPE_BIG:
        pBubble->m_move = BUBBLE_BIG_MOVE;
        break;

    case TYPE_MAXIMUM:
        pBubble->m_move = BUBBLE_MAXIMUM_MOVE;
        break;
    }

    return pBubble;
}