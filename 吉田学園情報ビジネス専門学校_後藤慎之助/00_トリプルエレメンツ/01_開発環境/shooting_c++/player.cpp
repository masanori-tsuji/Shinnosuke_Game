//===============================================
//
// プレイヤー処理 (player.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "player.h"
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
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = NULL;

//========================================
// プレイヤーのコンストラクタ
//========================================
CPlayer::CPlayer() :CScene2D(OBJTYPE::OBJTYPE_PLAYER)
{
    m_nAttackCoolTime = 0;
    m_fShotAngle = D3DX_PI / 4; // 最初は右斜め上の角度
    m_fPlayerAngle = 0.0f;      // 最初の自機の角度は0.0f
    m_nChargeTime = 0;
    m_nBarrier = 0;             // 開始時のバリアは0枚
    m_nCntInvincibleFrame = 0;
    m_fSpeed = 0.0f;
    m_bUse = true;
    m_nSpeedRank = PLAYER_FIRST_SPEED_RANK;
}

//========================================
// プレイヤーのデストラクタ
//========================================
CPlayer::~CPlayer()
{

}

//========================================
// プレイヤーの初期化処理
//========================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 初期化
    CScene2D::Init(pos, size);

    // テクスチャを割り当て
    BindTexture(m_pTexture);

    return S_OK;
}

//========================================
// プレイヤーの終了処理
//========================================
void CPlayer::Uninit(void)
{
    // 終了処理
    CScene2D::Uninit();
}

//========================================
// プレイヤーの更新処理
//========================================
void CPlayer::Update(void)
{
    // 変数宣言
    D3DXVECTOR3 pos;            // 位置
    D3DXVECTOR3 size;           // サイズ
    float fPlayerAngle = 0.0f;  // プレイヤーの角度

    // 位置を取得
    pos = GetPosition();

    // サイズを取得
    size = GetSize();

    // 空の中なら、プレイヤーの角度を調整
    if (CBg::GetInTheSky() == true)
    {
        // だんだん傾ける
        m_fPlayerAngle += PLAYER_TILT_RATE;

        // 45度まで
        if (m_fPlayerAngle > (D3DX_PI / 4))
        {
            m_fPlayerAngle = D3DX_PI / 4;
        }
    }
    else
    {
        // だんだん傾ける
        m_fPlayerAngle -= PLAYER_TILT_RATE;

        // 0度まで
        if (m_fPlayerAngle < 0)
        {
            m_fPlayerAngle = 0;
        }
    }

    // スピードランクを変える
    ChangeSpeed();

    // プレイヤーの移動操作
    pos = ControlPlayer(pos);

    // 水の発射角度調整
    ChangeShotAngle();

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
    if (pos.y <= PLAYER_MOVE_LIMITED_BY_UI)
    {
        pos.y = PLAYER_MOVE_LIMITED_BY_UI;
    }

    // 床との当たり判定
    pos = HitFloor(pos, size);

    // 画面外に出ないようにする
    SteyInScreen2D(&pos, &size);

    // 位置を反映
    SetPosition(pos);

    // 頂点座標を変更
    fPlayerAngle = m_fPlayerAngle * (-1);   // -1をかけることで、角度を加算するほど左に回るように
    SetRotVertex(fPlayerAngle);

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
                // プレイヤーの当たり判定を削る
                D3DXVECTOR3 playerCollisionSize = PLAYER_COLLISION_SIZE;

                // 位置と衝突サイズを得るため、エネミーにキャスト
                CEnemy *pEnemy = (CEnemy*)pScene;

                // 当たり判定の数分回す
                for (int nCntCollision = 0; nCntCollision < pEnemy->GetCntCollision(); nCntCollision++)
                {
                    D3DXVECTOR3 posCollision = pEnemy->GetPosition() + pEnemy->GetCollision(nCntCollision).posToShift;
                    D3DXVECTOR3 sizeCollision = pEnemy->GetCollision(nCntCollision).size;

                    // 当たり判定を計算
                    if (RectangleCollision2D(&pos, &posCollision,
                        &playerCollisionSize, &sizeCollision) == true)
                    {
                        // 当たった判定
                        Hit();

                        // forを抜ける
                        break;
                    }
                }
            }
            // アイテムなら、
            else if (objType == CScene::OBJTYPE_ITEM)
            {
                // シーン2Dにキャスト
                CScene2D *pScene2D = (CScene2D*)pScene;

                // プレイヤーの当たり判定を削る(アイテム用)
                D3DXVECTOR3 playerCollisionSize = PLAYER_COLLISION_ITEM_SIZE;

                // 当たり判定を計算
                if (RectangleCollision2D(&pos, &pScene2D->GetPosition(),
                    &playerCollisionSize, &pScene2D->GetSize()) == true)
                {
                    // サウンドを取得
                    CSound *pSound = CManager::GetSound();

                    // アイテム音
                    pSound->Play(CSound::LABEL_SE_ITEM);

                    // アイテムにキャスト
                    CItem *pItem = (CItem*)pScene;

                    // アイテムのタイプごとに処理を変える
                    switch (pItem->GetType())
                    {
                    case CItem::TYPE_BARRIER:

                        // バリアを加算
                        m_nBarrier++;

                        // 最大数を超えたら、最大数にする
                        if (m_nBarrier > PLAYER_MAX_BARRIER)
                        {
                            m_nBarrier = PLAYER_MAX_BARRIER;
                        }

                        // 衝撃波
                        CEffect::Create(pos, EFFECT_DOUBLE_WAVE_ITEM_SIZE,
                            EFFECT_DOUBLE_WAVE_ITEM_BARRIER_COLOR, CEffect::TYPE_DOUBLE_WAVE);

                        break;

                    case CItem::TYPE_STOCK:

                        // ストックを加算
                        CGame::AddStock();

                        // 衝撃波
                        CEffect::Create(pos, EFFECT_DOUBLE_WAVE_ITEM_SIZE,
                            EFFECT_DOUBLE_WAVE_ITEM_STOCK_COLOR, CEffect::TYPE_DOUBLE_WAVE);

                        break;
                    }

                    // アイテムを消す
                    pItem->Uninit();

                    // forを抜ける
                    break;
                }
            }
            // ブロックなら、
            else if (objType == CScene::OBJTYPE_BLOCK)
            {
                // プレイヤーの当たり判定を削る
                D3DXVECTOR3 playerCollisionSize = PLAYER_COLLISION_SIZE;

                // 位置と衝突サイズを得るため、ブロックにキャスト
                CBlock *pBlock = (CBlock*)pScene;

                // 当たり判定を計算
                if (RectangleCollision2D(&pos, &pBlock->GetPosition(),
                    &playerCollisionSize, &pBlock->GetCollisionSize()) == true)
                {
                    // 当たった判定
                    Hit();

                    // forを抜ける
                    break;
                }
            }
        }
    }

    // 使用フラグがfalseなら
    if (m_bUse == false)
    {
        // プレイヤーがやられたらストックを減らす
        CGame::DefeatPlayer();

        // 終了処理
        Uninit();
    }
}

//========================================
// プレイヤーの描画処理
//========================================
void CPlayer::Draw(void)
{
    // 描画処理
    CScene2D::Draw();
}

//========================================
// プレイヤーの水弾の発射角度調整
//========================================
void CPlayer::ChangeShotAngle(void)
{
    // コントローラを取得
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(PLAYER_1);

    // 水の発射角度を変える
    if (Controller.lZ != 0 || Controller.lRz != 0)
    {
        // 下に倒しすぎた時は
        if (Controller.lRz > 0)
        {
            // 右で固定
            Controller.lZ = 3000;
            Controller.lRz = 0;
        }

        // スティックの角度を求める
        m_fShotAngle = atan2(Controller.lZ, Controller.lRz * (-1));
    }

    // 左スティック押し込みで、発射角度を背面にリセット
    if (pInputJoypad->GetJoypadPress(PLAYER_1, CInputJoypad::BUTTON_L3))
    {
        m_fShotAngle = -D3DX_PI / 4;
    }

    // 右スティック押し込みで、発射角度を正面にリセット
    if (pInputJoypad->GetJoypadPress(PLAYER_1, CInputJoypad::BUTTON_R3))
    {
        m_fShotAngle = D3DX_PI / 4;
    }
}

//========================================
// プレイヤーの移動速度を変える
//========================================
void CPlayer::ChangeSpeed(void)
{
    // キーボードを取得
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // コントローラを取得
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(PLAYER_1);

    // ショック状態でないなら
    if (CGame::GetShockPlayer() == false)
    {
        // Lが押された(減速)
        if (pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_L1) ||
            pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_L2) ||
            pInputKeyboard->GetKeyboardTrigger(DIK_U))
        {
            m_nSpeedRank--;
        }

        // Rが押された(加速)
        if (pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_R1) ||
            pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_R2) ||
            pInputKeyboard->GetKeyboardTrigger(DIK_I))
        {
            m_nSpeedRank++;
        }
    }
    // ショック状態なら
    else
    {
        // スピードランクが常に1
        m_nSpeedRank = 1;
    }

    // スピードランクは1を下回らない＆最大を上回らない
    if (m_nSpeedRank < 1)
    {
        m_nSpeedRank = 1;
    }
    else if (m_nSpeedRank > PLAYER_MAX_SPEED_RANK)
    {
        m_nSpeedRank = PLAYER_MAX_SPEED_RANK;
    }

    // スピードランクによって、速さを得る
    switch (m_nSpeedRank)
    {
    case 1:
        m_fSpeed = PLAYER_SPEED_1;
        break;

    case 2:
        m_fSpeed = PLAYER_SPEED_2;
        break;

    case 3:
        m_fSpeed = PLAYER_SPEED_3;
        break;

    case 4:
        m_fSpeed = PLAYER_SPEED_4;
        break;
    }
}

//========================================
// プレイヤーの移動操作の細かな関数
//========================================
D3DXVECTOR3 CPlayer::ControlPlayer(D3DXVECTOR3 pos)
{
    // 変数宣言
    D3DXVECTOR3 movePos = pos;      // 移動の位置

    // キーボードを取得
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // コントローラを取得
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(PLAYER_1);

    // 動けるなら
    if (CGame::GetMovePlayer() == true)
    {
        // 海底ステージは徐々に下がる
        if (CGame::GetStage() == CManager::STAGE_β)
        {
            movePos.y += PLAYER_POS_Y_DOWN_IN_WATAR;
        }

        if (Controller.lY != 0 || Controller.lX != 0)
        {// スティックが倒れているなら移動

         // スティックの角度を求める
            float fAngle = atan2(Controller.lX, Controller.lY * (-1));

            movePos.x += sinf(fAngle)* m_fSpeed;
            movePos.y += -cosf(fAngle)* m_fSpeed;

            // 最高速度なら
            if (m_nSpeedRank == PLAYER_MAX_SPEED_RANK)
            {
                // 残像を残す
                CAfterimage::Create(pos, PLAYER_SIZE, m_fPlayerAngle, AFTERIMAGE_PLAYER_COLOR,CAfterimage::TYPE_PLAYER);
            }
        }
        else
        {// キーボード操作

         // Aキーが押された(左移動)
            if (pInputKeyboard->GetKeyboardPress(DIK_A))
            {
                if (pInputKeyboard->GetKeyboardPress(DIK_W))// AとWが同時に押された
                {
                    movePos.x -= sinf(D3DX_PI / 4) * m_fSpeed;
                    movePos.y -= cosf(D3DX_PI / 4) * m_fSpeed;
                }
                else if (pInputKeyboard->GetKeyboardPress(DIK_S))// AとSが同時に押された
                {
                    movePos.x -= sinf(D3DX_PI / 4) * m_fSpeed;
                    movePos.y += cosf(D3DX_PI / 4) * m_fSpeed;
                }
                else				// 左だけ押した
                {
                    movePos.x -= m_fSpeed;
                }

                // 最高速度なら
                if (m_nSpeedRank == PLAYER_MAX_SPEED_RANK)
                {
                    // 残像を残す
                    CAfterimage::Create(pos, PLAYER_SIZE, m_fPlayerAngle, AFTERIMAGE_PLAYER_COLOR, CAfterimage::TYPE_PLAYER);
                }
            }

            // Dキーが押された(右移動)
            else if (pInputKeyboard->GetKeyboardPress(DIK_D))
            {
                if (pInputKeyboard->GetKeyboardPress(DIK_W))// DとWが同時に押された
                {
                    movePos.x += sinf(D3DX_PI / 4) * m_fSpeed;
                    movePos.y -= cosf(D3DX_PI / 4) * m_fSpeed;
                }
                else if (pInputKeyboard->GetKeyboardPress(DIK_S))// DとSが同時に押された
                {
                    movePos.x += sinf(D3DX_PI / 4) * m_fSpeed;
                    movePos.y += cosf(D3DX_PI / 4) * m_fSpeed;
                }
                else				// 右だけ押した
                {
                    movePos.x += m_fSpeed;
                }

                // 最高速度なら
                if (m_nSpeedRank == PLAYER_MAX_SPEED_RANK)
                {
                    // 残像を残す
                    CAfterimage::Create(pos, PLAYER_SIZE, m_fPlayerAngle, AFTERIMAGE_PLAYER_COLOR, CAfterimage::TYPE_PLAYER);
                }
            }

            // Wキーが押された(上移動)
            else if (pInputKeyboard->GetKeyboardPress(DIK_W))
            {
                movePos.y -= m_fSpeed;

                // 最高速度なら
                if (m_nSpeedRank == PLAYER_MAX_SPEED_RANK)
                {
                    // 残像を残す
                    CAfterimage::Create(pos, PLAYER_SIZE, m_fPlayerAngle, AFTERIMAGE_PLAYER_COLOR, CAfterimage::TYPE_PLAYER);
                }
            }

            // Sキーが押された(下移動)
            else if (pInputKeyboard->GetKeyboardPress(DIK_S))
            {
                movePos.y += m_fSpeed;

                // 最高速度なら
                if (m_nSpeedRank == PLAYER_MAX_SPEED_RANK)
                {
                    // 残像を残す
                    CAfterimage::Create(pos, PLAYER_SIZE, m_fPlayerAngle, AFTERIMAGE_PLAYER_COLOR, CAfterimage::TYPE_PLAYER);
                }
            }
        }
    }

    return movePos;
}

//========================================
// プレイヤーの弾を撃つ処理
//========================================
void CPlayer::ShotBullet(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // キーボードを取得
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // コントローラを取得
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(PLAYER_1);

    // サウンドを取得
    CSound *pSound = CManager::GetSound();

    // 動けるなら
    if (CGame::GetMovePlayer() == true)
    {
        // Yが押された(雷)
        if (pInputKeyboard->GetKeyboardPress(DIK_B) || pInputJoypad->GetJoypadPress(PLAYER_1, CInputJoypad::BUTTON_Y))
        {
            // クールタイムが0なら、
            if (m_nAttackCoolTime == 0)
            {
                // 他の攻撃をさせないための値
                m_nAttackCoolTime++;

                // チャージ時間
                m_nChargeTime++;
            }
        }

        // 最大チャージを超えたら、その値で固定
        if (m_nChargeTime > MAX_CHARGE_TIME)
        {
            m_nChargeTime = MAX_CHARGE_TIME;
        }

        // 雷攻撃のチャージ時間
        if (m_nChargeTime > 0)
        {
            // エフェクトを設定
            if (m_nChargeTime == 1)
            {
                // チャージエフェクト
                CEffect::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
                    CHARGE_COLOR, CEffect::TYPE_CHARGE);

                // 雷の弾(エフェクト)
                CEffect::Create(D3DXVECTOR3(pos.x + size.x, pos.y, 0.0f), D3DXVECTOR3(BULLET_LIGHT_MIN_SIZE, BULLET_LIGHT_MIN_SIZE,
                    0.0f), DEFAULT_COLOR, CEffect::TYPE_LIGHT_BULLET);
            }

            // 最大までは、パーティクルを自機に吸収
            if (m_nChargeTime < MAX_CHARGE_TIME)
            {
                //0～3.14を出す*(0 or 1)*-1で正負を決める
                float fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                float fRadius = float(rand() % 20000 + 0) / 100.0f;
                D3DXVECTOR3 particlePos = D3DXVECTOR3(pos.x - sinf(fAngle)*fRadius, pos.y - cosf(fAngle)*fRadius, 0.0f);

                // チャージのパーティクル
                CEffect::Create(particlePos, D3DXVECTOR3(CHARGE_PART_SIZE, CHARGE_PART_SIZE,
                    0.0f), BULLET_LIGHT_COLOR, CEffect::TYPE_CHARGE_PART);
            }

            // 離したら発射(ポーズボタンでも発射)
            if (pInputKeyboard->GetKeyboardRelease(DIK_B) || pInputJoypad->GetJoypadRelease(PLAYER_1, CInputJoypad::BUTTON_Y)
                || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START))
            {
                // 最大チャージかどうかで、ショット音を分ける
                if (m_nChargeTime == MAX_CHARGE_TIME)
                {
                    // ショット音
                    pSound->Play(CSound::LABEL_SE_SHOT_LIGHT2);

                    // 衝撃波
                    CEffect::Create(pos, EFFECT_DOUBLE_WAVE_SHOT_SIZE, 
                        EFFECT_DOUBLE_WAVE_SHOT_COLOR, CEffect::TYPE_DOUBLE_WAVE);
                    // 衝撃波
                    CEffect::Create(pos, EFFECT_DOUBLE_WAVE_SHOT_SIZE,
                        D3DXCOLOR(0.0f,1.0f,0.0f,0.6f), CEffect::TYPE_DOUBLE_WAVE);
                }
                else
                {
                    // ショット音
                    pSound->Play(CSound::LABEL_SE_SHOT_LIGHT1);
                }

                // チャージタイムをリセット
                m_nChargeTime = 0;

                // 雷の弾のクールタイム
                m_nAttackCoolTime = BULLET_LIGHT_COOL_TIME;
            }
        }

        // 炎と水の弾は、無敵時間を考慮
        if (m_nCntInvincibleFrame <= PLAYER_ENABLE_SHOT_FRAME)
        {
            // Bが押された(炎)
            if (pInputKeyboard->GetKeyboardPress(DIK_SPACE) || pInputJoypad->GetJoypadPress(PLAYER_1, CInputJoypad::BUTTON_B))
            {
                // クールタイムが0なら、
                if (m_nAttackCoolTime == 0)
                {
                    // ショット音
                    pSound->Play(CSound::LABEL_SE_SHOT_FIRE);

                    // 弾の生成
                    CBullet::Create(pos, BULLET_FIRE_SIZE, 
                        D3DXVECTOR3(cosf(m_fPlayerAngle) * BULLET_FIRE_SPEED, sinf(m_fPlayerAngle) * (-BULLET_FIRE_SPEED), 0.0f),
                        CBullet::TYPE_FIRE, TRIBE_PLAYER);

                    // 炎の弾のクールタイム
                    m_nAttackCoolTime = BULLET_FIRE_COOL_TIME;

                    // 念のため、雷のチャージをなくす
                    m_nChargeTime = 0;
                }
            }

            // Xが押された(水)
            if (pInputKeyboard->GetKeyboardPress(DIK_N) || pInputJoypad->GetJoypadPress(PLAYER_1, CInputJoypad::BUTTON_X))
            {
                // クールタイムが0なら、
                if (m_nAttackCoolTime == 0)
                {
                    // ショット音
                    pSound->Play(CSound::LABEL_SE_SHOT_WATER);

                    // 弾の生成
                    CBullet::Create(D3DXVECTOR3(pos.x, pos.y + PLAYER_ADD_WATAR_POS, pos.z), BULLET_WATER_SIZE,
                        D3DXVECTOR3(sinf(m_fShotAngle) * BULLET_WATER_SPEED, cosf(m_fShotAngle) * (-BULLET_WATER_SPEED), 0.0f),
                        CBullet::TYPE_WATER, TRIBE_PLAYER);

                    // 水の弾のクールタイム
                    m_nAttackCoolTime = BULLET_WATER_COOL_TIME;

                    // 念のため、雷のチャージをなくす
                    m_nChargeTime = 0;
                }
            }
        }
    }
    else
    {// 動けないとき

        // チャージタイムをリセット
        m_nChargeTime = 0;

        // クールタイムリセット
        m_nAttackCoolTime = 0;
    }
}

//========================================
// プレイヤーの画像ロード処理
//========================================
HRESULT CPlayer::Load(void)
{
    // レンダラーからデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player000.png", &m_pTexture);

    return S_OK;
}

//========================================
// プレイヤーの画像破棄処理
//========================================
void CPlayer::Unload(void)
{
    // テクスチャの破棄
    if (m_pTexture != NULL)
    {
        m_pTexture->Release();
        m_pTexture = NULL;
    }
}

//================================
// プレイヤーの作成
//================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, float fAngle, int nCntInvincibleFrame)
{
    CPlayer *pPlayer = NULL;

    // メモリを確保
    // コンストラクタで各情報を紐づけ
    pPlayer = new CPlayer;

    // 初期化
    pPlayer->Init(pos, size);

    // 引数とメンバ変数を結びつける
    pPlayer->m_nCntInvincibleFrame = nCntInvincibleFrame;
    pPlayer->m_fPlayerAngle = fAngle;

    return pPlayer;
}

//====================================================
// プレイヤーに敵の弾や敵が当たったときの処理
//====================================================
void CPlayer::Hit(void)
{
    // 変数宣言
    D3DXVECTOR3 pos;    // 位置
    D3DXVECTOR3 size;   // サイズ

    // 位置を取得
    pos = GetPosition();

    // サイズを取得
    size = GetSize();

    // 無敵でないなら
    if (CGame::GetInvinciblePlayer() == false && m_nCntInvincibleFrame <= 0)
    {
        // バリアの有無で場合分け
        if (m_nBarrier > 0)
        {// バリア有り

            // バリアが1枚減る
            m_nBarrier--;

            // 無敵時間を得る
            m_nCntInvincibleFrame = PLAYER_INVINCIBLE_FRAME;
        }
        else
        {// バリア無し

            // 爆発音再生
            CSound *pSound = CManager::GetSound();
            pSound->Play(CSound::LABEL_SE_EXPLOSION);

            // 大爆発エフェクト (サイズは倍)
            CEffect::Create(pos, size + size,
                DEFAULT_COLOR, CEffect::TYPE_BIG_EXPLOSION);

            // 黒火花エフェクト (位置はちょっと横、サイズは1.5倍)
            CEffect::Create(pos + D3DXVECTOR3(size.x / 2, -(size.y / 4), 0.0f), size + size / 2,
                DEFAULT_COLOR, CEffect::TYPE_BLACK_SPARK);

            // プレイヤーの使用フラグをfalseに
            m_bUse = false;
        }
    }
}

//====================================================
// 床に当たった時の処理
//====================================================
D3DXVECTOR3 CPlayer::HitFloor(D3DXVECTOR3 pos, D3DXVECTOR3 size)
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
                    // バリアの有無で場合分け
                    if (m_nBarrier > 0)
                    {// バリア有り

                     // バリアが1枚減る
                        m_nBarrier--;

                        // 無敵時間を得る
                        m_nCntInvincibleFrame = PLAYER_INVINCIBLE_FRAME;
                    }
                    else
                    {// バリア無し

                     // 爆発音再生
                        CSound *pSound = CManager::GetSound();
                        pSound->Play(CSound::LABEL_SE_EXPLOSION);

                        // 大爆発エフェクト (サイズは倍)
                        CEffect::Create(pos, size + size,
                            DEFAULT_COLOR, CEffect::TYPE_BIG_EXPLOSION);

                        // 黒火花エフェクト (位置はちょっと横、サイズは1.5倍)
                        CEffect::Create(pos + D3DXVECTOR3(size.x / 2, -(size.y / 4), 0.0f), size + size / 2,
                            DEFAULT_COLOR, CEffect::TYPE_BLACK_SPARK);

                        // プレイヤーの使用フラグをfalseに
                        m_bUse = false;
                    }
                }
            }
        }
    }

    return posLimited;
}

//====================================================
// プレイヤーを最初の位置にする
//====================================================
void CPlayer::SetStartPos(void)
{
    // 変数宣言
    D3DXVECTOR3 pos;    // 位置

    // 位置を取得
    pos = GetPosition();

    // 位置を初期位置に変更
    pos = PLAYER_POS;

    // 位置を反映
    SetPosition(pos);

    // 頂点座標を変更
    SetVertex();
}
