//===============================================
//
// ブロック処理 (block.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "block.h"
#include "manager.h"
#include "renderer.h"
#include "library.h"
#include "game.h"
#include "effect.h"
#include "sound.h"

//=======================================
// 静的メンバ変数宣言
//=======================================
LPDIRECT3DTEXTURE9 CBlock::m_apTexture[CBlock::TYPE_MAX] = {};

//========================================
// ブロックのコンストラクタ
//========================================
CBlock::CBlock() :CScene2D(OBJTYPE::OBJTYPE_BLOCK)
{
    m_posOld = DEFAULT_VECTOR;
    m_collisionSize = DEFAULT_VECTOR;
    m_move = DEFAULT_VECTOR;
    m_nCntChangeMove = 0;
    m_fRotAngle = 0.0f;
    m_bRotRight = false;
    m_type = CBlock::TYPE_MAX;
}

//========================================
// ブロックのデストラクタ
//========================================
CBlock::~CBlock()
{

}

//========================================
// ブロックの初期化処理
//========================================
HRESULT CBlock::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 初期化
    CScene2D::Init(pos, size);

    // テクスチャを割り当て
    BindTexture(m_apTexture[m_type]);

    return S_OK;
}

//========================================
// ブロックの終了処理
//========================================
void CBlock::Uninit(void)
{
    // 終了処理
    CScene2D::Uninit();
}

//========================================
// ブロックの更新処理
//========================================
void CBlock::Update(void)
{
    // 変数宣言
    D3DXVECTOR3 pos = GetPosition();    // 位置を取得
    D3DXVECTOR3 size = GetSize();       // サイズを取得
    bool bUse = true;                   // 使用するかどうか

    // 1F前の位置を記憶
    m_posOld = pos;

    // 種類に応じた場合分け
    switch (m_type)
    {
    case CBlock::TYPE_APPLE:

        // 移動が変わりだすまでのカウンタを数える
        m_nCntChangeMove++;

        // 落下を開始するまでのカウンタ
        if (m_nCntChangeMove >= BLOCK_APPLE_FALL_COUNT)
        {
            // 通常色
            CScene2D::SetColor(DEFAULT_COLOR);

            // 重力を少し軽めに設定
            m_move.y += GRAVITY / 4;

            // フラグによって、右or左回転
            if (m_bRotRight == true)
            {
                // 右回転
                m_fRotAngle += BLOCK_APPLE_ROT_SPEED;
            }
            else
            {
                // 左回転
                m_fRotAngle -= BLOCK_APPLE_ROT_SPEED;
            }
        }
        // 落下する前は点滅
        else
        {
            // カウンターを進めて、パターンを切り替える
            int nPatternAnim = CountAnimation(2, 2);

            // 点滅
            if (nPatternAnim == 1)
            {
                // 通常色
                CScene2D::SetColor(DEFAULT_COLOR);
            }
            else
            {
                // 透明
                CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
            }
        }

        break;

    case CBlock::TYPE_ROCK:

        // フラグによって回転
        if (m_bRotRight == true)
        {
            // 重力を少し軽めに設定
            m_move.y += GRAVITY / 8;

            // 落下の最高速度を決める
            if (m_move.y >= BLOCK_ROCK_MAX_FALL_SPEED)
            {
                m_move.y = BLOCK_ROCK_MAX_FALL_SPEED;
            }

            // どちらに回っているかで場合分け
            if (m_fRotAngle > 0.0f)
            {
                // 右回転
                m_fRotAngle += BLOCK_ROCK_ROT_SPEED;
            }
            else
            {
                // 左回転
                m_fRotAngle -= BLOCK_ROCK_ROT_SPEED;
            }
        }

        break;

    case TYPE_METEO:

        // 隕石の衝突処理
        bUse = MeteoInpact(pos, size);

        // 左回転
        m_fRotAngle -= BLOCK_METEO_ROT_SPEED;

        // エフェクトを生成(サイズは一回り大きい)
        CEffect::Create(pos, size + D3DXVECTOR3(25.0f, 25.0f, 0.0f),
            EFFECT_PURPLE_FIRE_FIRST_COLOR, CEffect::TYPE_PURPLE_FIRE);

        break;
    }

    // 移動量を位置に加算
    pos += m_move;

    // バウンド
    pos = Bound(pos, size);

    // 位置を反映
    SetPosition(pos);

    // 頂点座標を変更
    SetRotVertex(m_fRotAngle);

    // 画面の端から2倍の場所から出たら
    if (OutGame2D(&pos, &size) == true)
    {
        // 使用フラグをfalseに
        bUse = false;
    }

    // 使用フラグがfalseなら
    if (bUse == false)
    {
        // 未使用に
        Uninit();
    }
}

//========================================
// ブロックの描画処理
//========================================
void CBlock::Draw(void)
{
    // 描画処理
    CScene2D::Draw();
}

//========================================
// ブロックの画像ロード処理
//========================================
HRESULT CBlock::Load(void)
{
    // レンダラーからデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/block000.png", &m_apTexture[CBlock::TYPE_APPLE]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/block001.png", &m_apTexture[CBlock::TYPE_ROCK]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/block002.png", &m_apTexture[CBlock::TYPE_METEO]);

    return S_OK;
}

//========================================
// ブロックの画像破棄処理
//========================================
void CBlock::Unload(void)
{
    // テクスチャの破棄
    for (int nCntTexture = 0; nCntTexture < CBlock::TYPE_MAX; nCntTexture++)
    {
        if (m_apTexture[nCntTexture] != NULL)
        {
            m_apTexture[nCntTexture]->Release();
            m_apTexture[nCntTexture] = NULL;
        }
    }
}

//================================
// ブロックの作成
//================================
CBlock *CBlock::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, bool bRotRight, CBlock::TYPE type)
{
    CBlock *pBlock = NULL;

    // メモリを確保
    pBlock = new CBlock;

    // タイプのみ、テクスチャ割り当てのために結びつけておく
    pBlock->m_type = type;

    // 初期化
    pBlock->Init(pos, size);

    // メンバ変数を結びつける
    pBlock->m_posOld = pos;
    pBlock->m_bRotRight = bRotRight;

    // ブロックの種類によって、固定の値を結びつける
    int nRandNum = 0;       // ランダムな値
    float fAngle = 0.0f;    // 角度
    switch (pBlock->m_type)
    {
    case CBlock::TYPE_APPLE:

        pBlock->m_collisionSize = BLOCK_APPLE_COLLISION_SIZE;
        pBlock->m_move = DEFAULT_VECTOR;

        break;

    case CBlock::TYPE_ROCK:

        pBlock->m_collisionSize = BLOCK_ROCK_COLLISION_SIZE;
        pBlock->m_move.x = -FLOWING_STAGE;

        // 上向きか下向きか
        if (pos.y < SCREEN_HEIGHT / 2)
        {
            // 下向き
            pBlock->m_fRotAngle = D3DX_PI;
        }
        else
        {
            // 上向き
            pBlock->m_fRotAngle = 0.0f;
        }

        // 回転するなら
        if (bRotRight == true)
        {
            // 横移動しない
            pBlock->m_move.x = 0.0f;

            // ランダムに右回転か左回転かを得る
            nRandNum = rand() % 2 + 1;
            switch (nRandNum)
            {
            case 1:

                pBlock->m_fRotAngle = BLOCK_ROCK_ROT_SPEED;

                break;

            case 2:

                pBlock->m_fRotAngle = -BLOCK_ROCK_ROT_SPEED;

                break;
            }
        }

        break;

    case CBlock::TYPE_METEO:

        pBlock->m_collisionSize = BLOCK_METEO_COLLISION_SIZE;
        pBlock->m_fRotAngle = -BLOCK_METEO_ROT_SPEED;

        // 自機の位置に飛んでいく
        fAngle = pBlock->GetAngleToPlayer(pos);
        if (CGame::GetDispPlayer() == true)
        {
            // 自機がいるなら、自機のほうへ
            pBlock->m_move = D3DXVECTOR3(-sinf(fAngle)*BLOCK_METEO_FALL_SPEED, -cosf(fAngle)*BLOCK_METEO_FALL_SPEED, 0.0f);
        }
        else
        {
            // 自機がいないなら、左下へ
            pBlock->m_move = D3DXVECTOR3(-sinf(D3DX_PI / 4)*BLOCK_METEO_FALL_SPEED, cosf(D3DX_PI / 4)*BLOCK_METEO_FALL_SPEED, 0.0f);
        }

        break;
    }

    return pBlock;
}

//====================================================
// 床に当たった時のバウンド
//====================================================
D3DXVECTOR3 CBlock::Bound(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 変数宣言
    D3DXVECTOR3 posBound = pos; // バウンドした際の位置

    // 床の判定を取得
    bool bFloor = false;
    bFloor = CGame::GetFloor();

    // 床との当たり判定
    if (bFloor == true)
    {
        // 床に当たったら
        if (pos.y + (size.y / 2) > FLOOR_COLLISION)
        {
            switch (m_type)
            {
            case TYPE_APPLE:

                // 床にめり込まない
                posBound.y = FLOOR_COLLISION - (size.y / 2);

                // バウンドの方向を場合分け
                if (m_bRotRight == true)
                {
                    // 右へバウンド
                    m_move = BLOCK_APPLE_RIGHT_BOUND;
                }
                else
                {
                    // 左へバウンド
                    m_move = BLOCK_APPLE_LEFT_BOUND;
                }

                break;

            case TYPE_ROCK:

                // 回転しているなら
                if (m_bRotRight == true)
                {
                    // 床にめり込まない
                    posBound.y = FLOOR_COLLISION - (size.y / 2);

                    if (m_fRotAngle > 0.0f)
                    {
                        // 右へバウンド
                        m_move = BLOCK_ROCK_RIGHT_BOUND;
                    }
                    else
                    {
                        // 左へバウンド
                        m_move = BLOCK_ROCK_LEFT_BOUND;
                    }
                }

                break;
            }     
        }
    }

    return posBound;
}

//====================================================
// 隕石から見た自機への角度
//====================================================
float CBlock::GetAngleToPlayer(D3DXVECTOR3 pos)
{
    // 変数宣言
    CPlayer *pPlayer = NULL;                   // プレイヤーを取得
    CScene2D *pPlayerScene2D = NULL;           // プレイヤーをシーン2Dにキャスト
    D3DXVECTOR3 playerPos = DEFAULT_VECTOR;    // プレイヤーの位置
    float fTargetPosX = 0.0f;                  // 目標のX座標
    float fTargetPosY = 0.0f;                  // 目標のY座標
    float fMyPosX = 0.0f;                      // 自身のX座標
    float fMyPosY = 0.0f;                      // 自身のY座標
    float fAngle = 0.0f;                       // 求める角度

    // プレイヤーが存在しているなら
    if (CGame::GetDispPlayer() == true)
    {
        // プレイヤーの情報を取得
        pPlayer = CGame::GetPlayer();
        pPlayerScene2D = (CScene2D*)pPlayer;
        playerPos = pPlayerScene2D->GetPosition();
    }

    // 目標の座標と自身の座標
    fTargetPosX = playerPos.x, fTargetPosY = playerPos.y;
    fMyPosX = pos.x, fMyPosY = pos.y;

    // 角度を決める
    fAngle = atan2f((fMyPosX - fTargetPosX), (fMyPosY - fTargetPosY));

    return fAngle;
}

//====================================================
// 隕石の衝突
//====================================================
bool CBlock::MeteoInpact(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 変数宣言
    bool bUse = true;   // 使用フラグ

    // 床の判定を取得
    bool bFloor = false;
    bFloor = CGame::GetFloor();

    // 床との当たり判定
    if (bFloor == true)
    {
        // 床に当たったら
        if (pos.y + (size.y / 2) > FLOOR_COLLISION)
        {
            // 使用フラグをfalseに
            bUse = false;

            // 爆発音再生
            CSound *pSound = CManager::GetSound();
            pSound->Play(CSound::LABEL_SE_EXPLOSION);

            // エフェクト用の変数宣言
            float fAngle = 0.0f;                // 角度
            float fRadius = 0.0f;               // 半径
            float fSize = 0.0f;                 // 大きさ

            // 複数のパーティクルを一度に出す
            for (int nCnt = 0; nCnt < EFFECT_EXPLOSION_COUNT_LOOP; nCnt++)
            {
                // ランダムな角度と半径を取得
                fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                fRadius = float(rand() % 20000) / 100.0f;
                fSize = float(rand() % EFFECT_EXPLOSION_MAX_SIZE + EFFECT_EXPLOSION_MIN_SIZE) / 100.0f;

                // 爆発エフェクト
                CEffect::Create(D3DXVECTOR3(pos.x - sinf(fAngle)*fRadius, pos.y - cosf(fAngle)*fRadius, 0.0f),
                    D3DXVECTOR3(fSize, fSize, 0.0f), EFFECT_EXPLOSION_FIRST_COLOR, CEffect::TYPE_PART_EXPLOSION);
            }
        }
    }

    return bUse;
}
