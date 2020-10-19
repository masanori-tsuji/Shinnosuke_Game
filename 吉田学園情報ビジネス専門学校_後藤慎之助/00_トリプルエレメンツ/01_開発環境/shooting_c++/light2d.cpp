//===============================================
//
// ライト2D処理 (light2d.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "light2d.h"
#include "manager.h"
#include "renderer.h"
#include "library.h"
#include "game.h"
#include "enemy.h"

//=======================================
// マクロ定義
//=======================================

//=======================================
// 静的メンバ変数宣言
//=======================================
LPDIRECT3DTEXTURE9 CLight2D::m_pTexture = NULL;

//========================================
// ライト2Dのコンストラクタ
//========================================
CLight2D::CLight2D() :CScene2D(OBJTYPE::OBJTYPE_LIGHT2D)
{
    m_col = LIGHT_SET_COLOR;
    m_nStage = -1;
}

//========================================
// ライト2Dのデストラクタ
//========================================
CLight2D::~CLight2D()
{

}

//========================================
// ライト2Dの初期化処理
//========================================
HRESULT CLight2D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 初期化
    CScene2D::Init(pos, size);

    // テクスチャを割り当て
    BindTexture(m_pTexture);

    return S_OK;
}

//========================================
// ライト2Dの終了処理
//========================================
void CLight2D::Uninit(void)
{
    // 終了処理
    CScene2D::Uninit();
}

//========================================
// ライト2Dの更新処理
//========================================
void CLight2D::Update(void)
{
    // 変数宣言
    D3DXVECTOR3 pos = GetPosition();    // 位置を取得
    D3DXVECTOR3 size = GetSize();       // 大きさを取得
    D3DXVECTOR3 move = DEFAULT_VECTOR;  // 移動量
    float fTargetPosX = 0.0f;           // 目標のX座標
    float fTargetPosY = 0.0f;           // 目標のY座標
    float fMyPosX = 0.0f;               // 自身のX座標
    float fMyPosY = 0.0f;               // 自身のY座標
    float fAngle = 0.0f;                // 求める角度
    float fDistance = 0.0f;             // 距離
    
    if (m_nStage == 3)
    {
        // 目標の座標と自身の座標
        fTargetPosX = ENEMY_BOSS_DRAGON_STOP_POS_X, fTargetPosY = ENEMY_BOSS_DRAGON_STOP_POS_Y;
        fMyPosX = pos.x, fMyPosY = pos.y;

        // 角度を決める
        fAngle = atan2f((fMyPosX - fTargetPosX), (fMyPosY - fTargetPosY));

        // 目標の位置との、現時点での距離
        float fDistance = sqrtf(
            powf((pos.x - fTargetPosX), 2) +
            powf((pos.y - fTargetPosY), 2));

        // 目的の位置と近いかどうか
        if (fDistance <= LIGHT_CONSIDER_CLOSE_DEST)
        {
            // 一気に回りも明るく
            size.x += LIGHT_EXTEND_RATE_X;
            size.y += LIGHT_EXTEND_RATE_Y;
        }
        else
        {
            // 遠いなら近づけていく
            move = D3DXVECTOR3(-sinf(fAngle)*LIGHT_SPOT_SPEED_4, -cosf(fAngle)*LIGHT_SPOT_SPEED_4, 0.0f);
        }
    }
    else if (m_nStage == 2)
    {
        // 目標の座標と自身の座標
        fTargetPosX = LIGHT_SPOT_POS_X_3, fTargetPosY = LIGHT_SPOT_POS_Y_3;
        fMyPosX = pos.x, fMyPosY = pos.y;

        // 角度を決める
        fAngle = atan2f((fMyPosX - fTargetPosX), (fMyPosY - fTargetPosY));

        // 目標の位置との、現時点での距離
        float fDistance = sqrtf(
            powf((pos.x - fTargetPosX), 2) +
            powf((pos.y - fTargetPosY), 2));

        // 目的の位置と近いかどうか
        if (fDistance <= LIGHT_CONSIDER_CLOSE_DEST)
        {
            // ライトの照らす場所を変える
            m_nStage = 3;
        }
        else
        {
            // 遠いなら近づけていく
            move = D3DXVECTOR3(-sinf(fAngle)*LIGHT_SPOT_SPEED_3, -cosf(fAngle)*LIGHT_SPOT_SPEED_3, 0.0f);
        }
    }
    else if (m_nStage == 1)
    {
        // 目標の座標と自身の座標
        fTargetPosX = LIGHT_SPOT_POS_X_2, fTargetPosY = LIGHT_SPOT_POS_Y_2;
        fMyPosX = pos.x, fMyPosY = pos.y;

        // 角度を決める
        fAngle = atan2f((fMyPosX - fTargetPosX), (fMyPosY - fTargetPosY));

        // 目標の位置との、現時点での距離
        float fDistance = sqrtf(
            powf((pos.x - fTargetPosX), 2) +
            powf((pos.y - fTargetPosY), 2));

        // 目的の位置と近いかどうか
        if (fDistance <= LIGHT_CONSIDER_CLOSE_DEST)
        {
            // ライトの照らす場所を変える
            m_nStage = 2;
        }
        else
        {
            // 遠いなら近づけていく
            move = D3DXVECTOR3(-sinf(fAngle)*LIGHT_SPOT_SPEED_2, -cosf(fAngle)*LIGHT_SPOT_SPEED_2, 0.0f);
        }
    }
    else if (m_nStage == 0)
    {
        // 目標の座標と自身の座標
        fTargetPosX = LIGHT_SPOT_POS_X_1, fTargetPosY = LIGHT_SPOT_POS_Y_1;
        fMyPosX = pos.x, fMyPosY = pos.y;

        // 角度を決める
        fAngle = atan2f((fMyPosX - fTargetPosX), (fMyPosY - fTargetPosY));

        // 目標の位置との、現時点での距離
        float fDistance = sqrtf(
            powf((pos.x - fTargetPosX), 2) +
            powf((pos.y - fTargetPosY), 2));

        // 目的の位置と近いかどうか
        if (fDistance <= LIGHT_CONSIDER_CLOSE_DEST)
        {
            // ライトの照らす場所を変える
            m_nStage = 1;
        }
        else
        {
            // 遠いなら近づけていく
            move = D3DXVECTOR3(-sinf(fAngle)*LIGHT_SPOT_SPEED_1, -cosf(fAngle)*LIGHT_SPOT_SPEED_1, 0.0f);
        }
    }
    else if (m_nStage == -1)
    {
        // だんだん暗くする
        m_col.a += LIGHT_FADE_OUT_RATE;

        // 最大まで暗くしたら
        if (m_col.a >= 1.0f)
        {
            // 最大値で固定
            m_col.a = 1.0f;

            // ライトの照らす場所を変える
            m_nStage = 0;
        }
    }

    // 移動量を加算
    pos += move;

    // 色を変更
    CScene2D::SetColor(m_col);

    // 位置を反映
    SetPosition(pos);

    // サイズを反映
    SetSize(size);

    // 頂点座標を変更
    SetVertex();

    // 一定の大きさで消滅
    if (size.x >= LIGHT_MAX_SIZE_X)
    {
        Uninit(); // これより下にコードは書かない
    }
}

//========================================
// ライト2Dの描画処理
//========================================
void CLight2D::Draw(void)
{
    // 描画処理
    CScene2D::Draw();
}

//========================================
// ライト2Dの画像ロード処理
//========================================
HRESULT CLight2D::Load(void)
{
    // レンダラーからデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    //テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/black.png", &m_pTexture);	    // ライト2D

    return S_OK;
}

//========================================
// ライト2Dの画像破棄処理
//========================================
void CLight2D::Unload(void)
{
    // テクスチャの破棄
    if (m_pTexture != NULL)
    {
        m_pTexture->Release();
        m_pTexture = NULL;
    }
}

//================================
// ライト2Dの作成
//================================
CLight2D *CLight2D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    CLight2D *pLight2D = NULL;

    // メモリを確保
    pLight2D = new CLight2D;

    // 初期化
    pLight2D->Init(pos, size);

    return pLight2D;
}