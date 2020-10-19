//===============================================
//
// ヘルパー処理 (helper.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "helper.h"
#include "manager.h"
#include "input.h"
#include "bullet.h"
#include "renderer.h"
#include "library.h"
#include "sound.h"
#include "effect.h"
#include "item.h"
#include "game.h"
#include "enemy.h"
#include "block.h"
#include "afterimage.h"
#include "bg.h"

//=======================================
// 静的メンバ変数宣言
//=======================================
LPDIRECT3DTEXTURE9 CHelper::m_pTexture = NULL;

//========================================
// ヘルパーのコンストラクタ
//========================================
CHelper::CHelper() :CScene2D(OBJTYPE::OBJTYPE_HELPER)
{
    m_nAttackCoolTime = 0;
    m_fHelperAngle = 0.0f;
    m_move = DEFAULT_VECTOR;
    m_nCntInvincibleFrame = 0;
    m_fSpeed = HELPER_SPEED;
}

//========================================
// ヘルパーのデストラクタ
//========================================
CHelper::~CHelper()
{

}

//========================================
// ヘルパーの初期化処理
//========================================
HRESULT CHelper::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 初期化
    CScene2D::Init(pos, size);

    // テクスチャを割り当て
    BindTexture(m_pTexture);

    return S_OK;
}

//========================================
// ヘルパーの終了処理
//========================================
void CHelper::Uninit(void)
{
    // 終了処理
    CScene2D::Uninit();
}

//========================================
// ヘルパーの更新処理
//========================================
void CHelper::Update(void)
{
    // 変数宣言
    D3DXVECTOR3 pos;            // 位置
    D3DXVECTOR3 size;           // サイズ
    float fHelperAngle = 0.0f;  // ヘルパーの角度

    // 位置を取得
    pos = GetPosition();

    // サイズを取得
    size = GetSize();

    // 空の中なら、ヘルパーの角度を調整
    if (CBg::GetInTheSky() == true)
    {
        // だんだん傾ける
        m_fHelperAngle += HELPER_TILT_RATE;

        // 45度まで
        if (m_fHelperAngle > (D3DX_PI / 4))
        {
            m_fHelperAngle = D3DX_PI / 4;
        }
    }
    else
    {
        // だんだん傾ける
        m_fHelperAngle -= HELPER_TILT_RATE;

        // 0度まで
        if (m_fHelperAngle < 0)
        {
            m_fHelperAngle = 0;
        }
    }

    //ノックバックを制御
    if (m_move.x != 0.0f || m_move.y != 0.0f)
    {
        m_move.x *= HELPER_KNOCKBACK_CONTROL;
        m_move.y *= HELPER_KNOCKBACK_CONTROL;
    }

    //移動量と位置を紐づけ
    pos += m_move;

    // ヘルパーの移動操作
    pos = ControlHelper(pos);

    // 弾を撃つ処理
    ShotBullet(pos, size);

    // 攻撃のクールタイム
    if (m_nAttackCoolTime > 0)
    {
        m_nAttackCoolTime--;
    }

    // 無敵時間のカウンタ
    if (m_nCntInvincibleFrame > 0)
    {
        // 無敵時間を数える
        m_nCntInvincibleFrame--;

        // カウンターを進めて、パターンを切り替える
        int nPatternAnim = CountAnimation(2, 2);

        // 点滅
        if (nPatternAnim == 1)
        {
            // 通常
            CScene2D::SetColor(DEFAULT_COLOR);
        }
        else
        {
            // 透明
            CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
        }

        // 必ず元の色に戻るようにする
        if (m_nCntInvincibleFrame == 0)
        {
            // 通常
            CScene2D::SetColor(DEFAULT_COLOR);
        }
    }

    // UIとの当たり判定
    if (pos.y <= HELPER_MOVE_LIMITED_BY_UI)
    {
        pos.y = HELPER_MOVE_LIMITED_BY_UI;
    }

    // 床との当たり判定
    pos = HitFloor(pos, size);

    // 画面外に出ないようにする
    SteyInScreen2D(&pos, &size);

    // 位置を反映
    SetPosition(pos);

    // 頂点座標を変更
    fHelperAngle = m_fHelperAngle * (-1);   // -1をかけることで、角度を加算するほど左に回るように
    SetRotVertex(fHelperAngle);

    // 当たり判定
    for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
    {
        // シーンを取得
        CScene *pScene = GetScene(nCntScene);

        // 中身があるなら
        if (pScene != NULL)
        {
            // タイプを取得
            OBJTYPE objType = pScene->GetObjType();

            // エネミーなら、
            if (objType == CScene::OBJTYPE_ENEMY)
            {
                // ヘルパーの当たり判定を削る
                D3DXVECTOR3 helperCollisionSize = HELPER_COLLISION_SIZE;

                // 位置と衝突サイズを得るため、エネミーにキャスト
                CEnemy *pEnemy = (CEnemy*)pScene;

                // 当たり判定の数分回す
                for (int nCntCollision = 0; nCntCollision < pEnemy->GetCntCollision(); nCntCollision++)
                {
                    D3DXVECTOR3 posCollision = pEnemy->GetPosition() + pEnemy->GetCollision(nCntCollision).posToShift;
                    D3DXVECTOR3 sizeCollision = pEnemy->GetCollision(nCntCollision).size;
                    D3DXVECTOR3 posOld = pEnemy->GetPosOld();

                    // 当たり判定を計算
                    if (RectangleCollision2D(&pos, &posCollision,
                        &helperCollisionSize, &sizeCollision) == true)
                    {
                        // 当たった判定
                        Hit(posOld);

                        // forを抜ける
                        break;
                    }
                }
            }
            // ブロックなら、
            else if (objType == CScene::OBJTYPE_BLOCK)
            {
                // ヘルパーの当たり判定を削る
                D3DXVECTOR3 helperCollisionSize = HELPER_COLLISION_SIZE;

                // 位置と衝突サイズを得るため、ブロックにキャスト
                CBlock *pBlock = (CBlock*)pScene;

                // 当たり判定を計算
                if (RectangleCollision2D(&pos, &pBlock->GetPosition(),
                    &helperCollisionSize, &pBlock->GetCollisionSize()) == true)
                {
                    // 当たった判定
                    Hit(pBlock->GetPosOld());

                    // forを抜ける
                    break;
                }
            }
        }
    }
}

//========================================
// ヘルパーの描画処理
//========================================
void CHelper::Draw(void)
{
    // 描画処理
    CScene2D::Draw();
}

//========================================
// ヘルパーの移動操作の細かな関数
//========================================
D3DXVECTOR3 CHelper::ControlHelper(D3DXVECTOR3 pos)
{
    // 変数宣言
    D3DXVECTOR3 movePos = pos;      // 移動の位置

    // コントローラを取得
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(PLAYER_2);

    // 動けるなら
    if (CGame::GetMovePlayer() == true)
    {
        // 海底ステージは徐々に下がる
        if (CGame::GetStage() == CManager::STAGE_β)
        {
            movePos.y += HELPER_POS_Y_DOWN_IN_WATAR;
        }

        // スティックが倒れているなら移動
        if (Controller.lY != 0 || Controller.lX != 0)
        {

            // スティックの角度を求める
            float fAngle = atan2(Controller.lX, Controller.lY * (-1));

            movePos.x += sinf(fAngle)* m_fSpeed;
            movePos.y += -cosf(fAngle)* m_fSpeed;
        }
    }

    return movePos;
}

//========================================
// ヘルパーの弾を撃つ処理
//========================================
void CHelper::ShotBullet(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // コントローラを取得
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(PLAYER_2);

    // サウンドを取得
    CSound *pSound = CManager::GetSound();

    // プレイヤーのストックが0以上なら
    if (CGame::GetStock() >= 0)
    {
        // 動けるなら
        if (CGame::GetMovePlayer() == true)
        {
            // 無敵時間を考慮
            if (m_nCntInvincibleFrame <= HELPER_ENABLE_SHOT_FRAME)
            {
                // Bが押された(炎)
                if (pInputJoypad->GetJoypadPress(PLAYER_2, CInputJoypad::BUTTON_B))
                {
                    // クールタイムが0なら、
                    if (m_nAttackCoolTime == 0)
                    {
                        // ショット音
                        pSound->Play(CSound::LABEL_SE_SHOT_FIRE);

                        // 弾の生成
                        CBullet::Create(pos, HELPER_BULLET_SIZE, 
                            D3DXVECTOR3(cosf(m_fHelperAngle) * HELPER_BULLET_SPEED, sinf(m_fHelperAngle) * (-HELPER_BULLET_SPEED), 0.0f),
                            CBullet::TYPE_FIRE, TRIBE_PLAYER);

                        // 炎の弾のクールタイム
                        m_nAttackCoolTime = HELPER_BULLET_FIRE_COOL_TIME;
                    }
                }

                // Xが押された(水)
                if (pInputJoypad->GetJoypadPress(PLAYER_2, CInputJoypad::BUTTON_X))
                {
                    // クールタイムが0なら、
                    if (m_nAttackCoolTime == 0)
                    {
                        // ショット音
                        pSound->Play(CSound::LABEL_SE_SHOT_FIRE);

                        // 弾の生成
                        CBullet::Create(pos, HELPER_BULLET_SIZE, 
                            D3DXVECTOR3(cosf(m_fHelperAngle) * HELPER_BULLET_SPEED, sinf(m_fHelperAngle) * (-HELPER_BULLET_SPEED), 0.0f),
                            CBullet::TYPE_HELPER_WATER, TRIBE_PLAYER);

                        // 水の弾のクールタイム
                        m_nAttackCoolTime = HELPER_BULLET_WATER_COOL_TIME;
                    }
                }

                // Yが押された(雷)
                if (pInputJoypad->GetJoypadPress(PLAYER_2, CInputJoypad::BUTTON_Y))
                {
                    // クールタイムが0なら、
                    if (m_nAttackCoolTime == 0)
                    {
                        // ショット音
                        pSound->Play(CSound::LABEL_SE_SHOT_FIRE);

                        // 弾の生成
                        CBullet::Create(pos, HELPER_BULLET_SIZE,
                            D3DXVECTOR3(cosf(m_fHelperAngle) * HELPER_BULLET_SPEED, sinf(m_fHelperAngle) * (-HELPER_BULLET_SPEED), 0.0f),
                            CBullet::TYPE_LIGHT, TRIBE_PLAYER);

                        // 雷の弾のクールタイム
                        m_nAttackCoolTime = HELPER_BULLET_LIGHT_COOL_TIME;
                    }
                }
            }
        }
        else
        {// 動けないとき

         // クールタイムリセット
            m_nAttackCoolTime = 0;
        }
    }
}

//========================================
// ヘルパーの画像ロード処理
//========================================
HRESULT CHelper::Load(void)
{
    // レンダラーからデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/helper000.png", &m_pTexture);

    return S_OK;
}

//========================================
// ヘルパーの画像破棄処理
//========================================
void CHelper::Unload(void)
{
    // テクスチャの破棄
    if (m_pTexture != NULL)
    {
        m_pTexture->Release();
        m_pTexture = NULL;
    }
}

//================================
// ヘルパーの作成
//================================
CHelper *CHelper::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, float fAngle, int nCntInvincibleFrame)
{
    CHelper *pHelper = NULL;

    // メモリを確保
    // コンストラクタで各情報を紐づけ
    pHelper = new CHelper;

    // 初期化
    pHelper->Init(pos, size);

    // 引数とメンバ変数を結びつける
    pHelper->m_nCntInvincibleFrame = nCntInvincibleFrame;
    pHelper->m_fHelperAngle = fAngle;

    return pHelper;
}

//====================================================
// ヘルパーに敵の弾や敵やブロックが当たったときの処理
//====================================================
void CHelper::Hit(D3DXVECTOR3 posOld)
{
    // 変数宣言
    D3DXVECTOR3 pos;    // 位置

    // 位置を取得
    pos = GetPosition();

    // 無敵でないなら
    if (CGame::GetInvinciblePlayer() == false && m_nCntInvincibleFrame <= 0)
    {
        // ノックバック
        float fKnockback = HELPER_KNOCKBACK_VALUE;
        m_move.x = pos.x - posOld.x;
        m_move.y = pos.y - posOld.y;
        D3DXVec3Normalize(&m_move, &m_move);  //ベクトルを正規化
        m_move.x = m_move.x*fKnockback;
        m_move.y = m_move.y*fKnockback;

        // 無敵時間を得る
        m_nCntInvincibleFrame = HELPER_INVINCIBLE_FRAME;
    }
}

//====================================================
// 床に当たった時の処理
//====================================================
D3DXVECTOR3 CHelper::HitFloor(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 変数宣言
    D3DXVECTOR3 posLimited = pos;   // 床に当たった時の移動制限の位置

    // 床の判定を取得
    bool bFloor = false;
    bFloor = CGame::GetFloor();

    // 床との当たり判定
    if (bFloor == true)
    {
        if (pos.y + (size.y / 2) >= FLOOR_COLLISION)
        {
            // 床にめり込まない
            posLimited.y = FLOOR_COLLISION - (size.y / 2);

            // 森ステージ以外は床に当たり判定あり
            if (CGame::GetStage() != CManager::STAGE_α)
            {
                // 無敵でないなら
                if (CGame::GetInvinciblePlayer() == false && m_nCntInvincibleFrame <= 0)
                {
                    // ノックバック
                    m_move.y = -HELPER_KNOCKBACK_VALUE;

                    // 無敵時間を得る
                    m_nCntInvincibleFrame = HELPER_INVINCIBLE_FRAME;
                }
            }
        }
    }

    return posLimited;
}

//====================================================
// ヘルパーを最初の位置にする
//====================================================
void CHelper::SetStartPos(void)
{
    // 変数宣言
    D3DXVECTOR3 pos;    // 位置

    // 位置を取得
    pos = GetPosition();

    // 位置を初期位置に変更
    pos = HELPER_POS;

    // 移動量をリセット
    m_move = DEFAULT_VECTOR;

    // 位置を反映
    SetPosition(pos);

    // 頂点座標を変更
    SetVertex();
}
