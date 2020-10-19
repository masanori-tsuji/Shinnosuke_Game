//===============================================
//
// アイテム処理 (item.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "item.h"
#include "library.h"
#include "manager.h"
#include "renderer.h"
#include "effect.h"
#include "game.h"

//=======================================
// 静的メンバ変数宣言
//=======================================
LPDIRECT3DTEXTURE9 CItem::m_apTexture[CItem::TYPE_MAX] = {};

//========================================
// アイテムのコンストラクタ
//========================================
CItem::CItem() :CScene2D(OBJTYPE::OBJTYPE_ITEM)
{
    m_type = CItem::TYPE_MAX;
    m_fAngle = 0.0f;
    m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//========================================
// アイテムのデストラクタ
//========================================
CItem::~CItem()
{
}

//========================================
// アイテムの初期化処理
//========================================
HRESULT CItem::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 初期化
    CScene2D::Init(pos, size);

    // テクスチャを割り当て
    BindTexture(m_apTexture[m_type]);

    return S_OK;
}

//========================================
// アイテムの終了処理
//========================================
void CItem::Uninit(void)
{
    // 終了処理
    CScene2D::Uninit();
}

//========================================
// アイテムの更新処理
//========================================
void CItem::Update(void)
{
    // 変数宣言
    D3DXVECTOR3 pos = GetPosition();    // 位置を取得
    D3DXVECTOR3 size = GetSize();       // サイズを取得
    bool bUse = true;                   // 使用するかどうか

    // タイプごとに動きを場合分け
    switch (m_type)
    {
    case CItem::TYPE_BARRIER:

        // エフェクトを生成(サイズは一回り大きい)
        CEffect::Create(pos, size + ITEM_EFFECT_ADD_SIZE,
            BARRIER_COLOR, CEffect::TYPE_PARTICLE_JPG);

        // 角度加算
        m_fAngle += ITEM_ROT_ANGLE;

        // 移動量を位置に加算
        pos.x += sinf(m_fAngle) * ITEM_SPEED - ITEM_FLOWING_SPEED;
        pos.y += -cosf(m_fAngle) * ITEM_SPEED;

        break;

    case CItem::TYPE_STOCK:

        // エフェクトを生成(サイズは一回り大きい)
        CEffect::Create(pos, size + ITEM_EFFECT_ADD_SIZE,
            STOCK_COLOR, CEffect::TYPE_PARTICLE_JPG);

        // 重力(軽めに設定)
        m_move.y += GRAVITY / 4;
        pos.y += m_move.y;

        // 床との当たり判定
        if (CGame::GetFloor() == true)
        {
            // 床に当たっているなら
            if (pos.y + (size.y / 2) >= FLOOR_COLLISION)
            {
                // 海底なら
                if (CGame::GetStage() == CManager::STAGE_β)
                {
                    // 床にめり込まない
                    pos.y = FLOOR_COLLISION - (size.y / 2);

                    // 左へ流れる
                    pos.x -= FLOWING_STAGE;

                    // 重力をリセット
                    m_move.y = 0.0f;
                }
                // 火山なら
                else if (CGame::GetStage() == CManager::STAGE_γ)
                {
                    // 爆発エフェクト(マグマで見えづらいので、大きく)
                    CEffect::Create(pos, EXPLOSION_SIZE * 3,
                        DEFAULT_COLOR, CEffect::TYPE_EXPLOSION);

                    // 未使用に
                    bUse = false;
                }
            }
        }

        break;
    }

    // 位置を反映
    SetPosition(pos);

    // 頂点座標を変更
    SetVertex();

    // 画面外に出たら未使用に
    if (OutGame2D(&pos, &size) == true)
    {
        bUse = false;
    }

    // 使用しないなら、終了処理
    if (bUse == false)
    {
        Uninit(); // これより下にコードは書かない
    }
}

//========================================
// アイテムの描画処理
//========================================
void CItem::Draw(void)
{
    // 描画処理
    CScene2D::Draw();
}

//========================================
// アイテムの画像ロード処理
//========================================
HRESULT CItem::Load(void)
{
    // レンダラーからデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/item000.png", &m_apTexture[CItem::TYPE_BARRIER]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/item001.png", &m_apTexture[CItem::TYPE_STOCK]);

    return S_OK;
}

//========================================
// アイテムの画像破棄処理
//========================================
void CItem::Unload(void)
{
    // テクスチャの破棄
    for (int nCntTexture = 0; nCntTexture < CItem::TYPE_MAX; nCntTexture++)
    {
        if (m_apTexture[nCntTexture] != NULL)
        {
            m_apTexture[nCntTexture]->Release();
            m_apTexture[nCntTexture] = NULL;
        }
    }
}

//================================
// アイテムの生成
//================================
CItem *CItem::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type)
{
    CItem *pItem = NULL;

    // メモリを確保
    pItem = new CItem;

    // タイプのみ、テクスチャ割り当てのために結びつけておく
    pItem->m_type = type;

    // 初期化
    pItem->Init(pos, size);

    return pItem;
}
