//===================================================================
//
// エネミー処理 (enemy.cpp)
// Author : 後藤慎之助
//
//===================================================================

//========================
// インクルードファイル
//========================
#include "enemy.h"
#include "manager.h"
#include "renderer.h"
#include "library.h"
#include "effect.h"
#include "score.h"
#include "bullet.h"
#include "item.h"
#include "game.h"
#include "player.h"
#include "block.h"
#include "sound.h"
#include "ui.h"
#include "afterimage.h"
#include "reverse.h"
#include "bg.h"

//===========================================================
// マクロ定義
//===========================================================
#define COLOR_YELLOW D3DXCOLOR(0.929f, 0.427f, 0.0f, 1.0f)  // 黄色だと見づらいため、オレンジに近い

//===========================================================
// 静的メンバ変数宣言
//===========================================================
LPDIRECT3DTEXTURE9 CEnemy::m_apTexture[CEnemy::TYPE_MAX] = {};

//============================================================
// エネミーのコンストラクタ
//============================================================
CEnemy::CEnemy() :CScene2D(OBJTYPE::OBJTYPE_ENEMY)
{
    m_posOld = DEFAULT_VECTOR;
    m_move = DEFAULT_VECTOR;
    for (int nCnt = 0; nCnt < MAX_ENEMY_COLLISION; nCnt++)
    {
        m_aCollision[nCnt].posToShift = DEFAULT_VECTOR;
        m_aCollision[nCnt].size = DEFAULT_VECTOR;
    }
    m_nCntCollision = 0;
    m_nWhereHit = -1;
    m_nLife = 0;
    m_nMaxLife = 0;
    m_nCounterState = 0;
    m_nCounterHitWater = 0;
    m_nCounterHitLight = 0;
    m_nCounterAttackCoolTime = 0;
    m_nCntChangeMove = 0;
    m_nBound = 1;   // スコア計算時にこれをかけるので、バウンド回数は1
    m_fMoveAngle = 0.0f;
    m_fDestAngle = 0.0f;
    m_fMinAngle = 0.0f;
    m_bMoveUp = false;
    m_bDefeatWithWeak = false;
    m_bDefeatOneHit = false;
    m_type = CEnemy::TYPE_MAX;
    m_element = ELEMENT_MAX;
}

//============================================================
// エネミーのデストラクタ
//============================================================
CEnemy::~CEnemy()
{

}

//============================================================
// エネミーの初期化処理
//============================================================
HRESULT CEnemy::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 初期化
    CScene2D::Init(pos, size);

    // テクスチャを割り当て
    BindTexture(m_apTexture[m_type]);

    return S_OK;
}

//============================================================
// エネミーの終了処理
//============================================================
void CEnemy::Uninit(void)
{
    // 終了処理
    CScene2D::Uninit();
}

//============================================================
// エネミーの更新処理
//============================================================
void CEnemy::Update(void)
{
    // 変数宣言
    D3DXVECTOR3 pos = GetPosition();    // 位置を取得
    D3DXVECTOR3 size = GetSize();       // サイズを取得
    bool bUse = true;                   // 使用するかどうか
    float fAngle = 0.0f;                // 角度
    int nRandNum = 0;                   // ランダムな数

    // プレイヤー情報の変数宣言
    CPlayer *pPlayer = NULL;                   // プレイヤーを取得
    D3DXVECTOR3 playerPos = PLAYER_POS;        // プレイヤーの位置

    // プレイヤーが存在しているなら
    if (CGame::GetDispPlayer() == true)
    {
        // プレイヤー情報を取得
        pPlayer = CGame::GetPlayer();
        playerPos = pPlayer->GetPosition();
    }

    // 1F前の位置を記憶
    m_posOld = pos;

    //==================================================================================================================
    // 種類に応じた場合分け
    switch (m_type)
    {
    case CEnemy::TYPE_FLY:

        // 移動を変えるカウント
        if (m_nCntChangeMove != DONT_CHANGE_MOVE)
        {
            if (m_nCntChangeMove > 0)
            {
                m_nCntChangeMove--;
            }
        }

        // 移動量を変える
        if (m_nCntChangeMove <= 0)
        {
            // 上移動
            if (m_bMoveUp == true)
            {
                // 20フレーム(角度が180.0f ～ 178.0f (半周))で方向転換
                if (m_fMoveAngle > 178.0f)
                {
                    // 徐々に角度を変える
                    m_fMoveAngle -= 0.1f;

                    // 縦移動を追加
                    m_move.y = cosf(m_fMoveAngle) * ENEMY_FLY_SPEED;
                }
                else
                {
                    // 縦移動を削除
                    m_move.y = 0.0f;
                }
            }

            // 下移動
            else
            {
                // 20フレーム(角度が180.0f ～ 178.0f (半周))で方向転換
                if (m_fMoveAngle > 178.0f)
                {
                    // 徐々に角度を変える
                    m_fMoveAngle -= 0.1f;

                    // 縦移動を追加
                    m_move.y = -cosf(m_fMoveAngle) * ENEMY_FLY_SPEED;
                }
                else
                {
                    // 縦移動を削除
                    m_move.y = 0.0f;
                }
            }
        }

        // 横移動
        m_move.x = sinf(m_fMoveAngle) * ENEMY_FLY_SPEED;

        // 左に移動しているなら
        if (m_move.x <= 0.0f)
        {
            // テクスチャ座標を更新
            SetAnimation(1, 2);
        }
        // 右に移動しているなら
        else
        {
            // テクスチャ座標を逆向きに更新
            SetReverseAnimation(1, 2);
        }

        break;

    case CEnemy::TYPE_SKELETON:

        // 左に移動しているなら
        if (m_move.x <= 0.0f)
        {
            // テクスチャ座標を更新
            SetAnimation(3, 4);
        }
        // 右に移動しているなら
        else
        {
            // テクスチャ座標を逆向きに更新
            SetReverseAnimation(3, 4);
        }

        break;

    case CEnemy::TYPE_GREEN_BIRD:

        // 移動が変わりだすまでのカウンタを数える
        m_nCntChangeMove++;

        // ストップするまでのカウンタ
        if (m_nCntChangeMove == ENEMY_GREEN_BIRD_STOP)
        {
            m_move.x = 0.0f;
        }
        // ゴッドバードするまでのカウンタ
        else if (m_nCntChangeMove == ENEMY_GREEN_BIRD_GOD)
        {
            // ゴッドバードのフラグをtrueに
            m_bMoveUp = true;

            // HPを1にする
            m_nLife = 1;

            // 自機の位置を一瞬取得し、その位置へ突っ込む
            fAngle = GetAngleToPlayer();
            m_move = D3DXVECTOR3(-sinf(fAngle)*ENEMY_GREEN_BIRD_GOD_SPEED, -cosf(fAngle)*ENEMY_GREEN_BIRD_GOD_SPEED, 0.0f);
        }

        if (m_bMoveUp == false)
        {// 通常時

            // プレイヤーが存在しているなら
            if (CGame::GetDispPlayer() == true)
            {
                // カウンタの間隔で撃つ
                if (m_nCntChangeMove % ENEMY_GREEN_BIRD_ATTACK_COUNTER == 0)
                {
                    // 自機の位置を取得し、その位置へ撃つ
                    fAngle = GetAngleToPlayer();

                    // 弾の生成
                    CBullet::Create(pos, ENEMY_GREEN_BIRD_BULLET_SIZE,
                        D3DXVECTOR3(-sinf(fAngle)*ENEMY_GREEN_BIRD_BULLET_SPEED, -cosf(fAngle)*ENEMY_GREEN_BIRD_BULLET_SPEED, 0.0f),
                        CBullet::TYPE_GRASS, TRIBE_ENEMY);
                }
            }

            // テクスチャ座標を更新
            SetParagraphAnimation(1, 2, 6, 4);
        }
        else
        {// ゴッドバード時

            // カウンタによって、エフェクトを生成
            if (m_nCntChangeMove % ENEMY_GREEN_BIRD_SET_EFFECT_COUNT == 0)
            {
                // エフェクトを生成(サイズは一回り小さい)
                CEffect::Create(pos + m_move, size - D3DXVECTOR3(-10.0f, -10.0f, 0.0f),
                    EFFECT_YELLOW_FIRE_FIRST_COLOR, CEffect::TYPE_YELLOW_FIRE);
            }

            // テクスチャ座標を更新
            SetParagraphAnimation(2, 2, 3, 4);
        }

        // ゴッドバード前の、デンジャー表記
        if (m_nCntChangeMove == ENEMY_GREEN_BIRD_DANGER)
        {
            CUI::Create(D3DXVECTOR3(pos.x, pos.y - (size.y / 2), 0.0f), DANGER_SIZE, DEFAULT_COLOR, CUI::TYPE_DANGER);
        }

        break;

    case CEnemy::TYPE_CROW:

        // 左に移動しているなら
        if (m_move.x <= 0.0f)
        {
            // テクスチャ座標を更新
            SetAnimation(6, 3);
        }

        // 右に移動しているなら
        else
        {
            // テクスチャ座標を逆向きに更新
            SetReverseAnimation(6, 3);
        }

        break;

    case CEnemy::TYPE_BLUE_BIRD:

        // 移動が変わりだすまでのカウンタを数える
        m_nCntChangeMove++;

        // ストップするまでのカウンタ
        if (m_nCntChangeMove == ENEMY_BLUE_BIRD_STOP)
        {
            m_move.x = 0.0f;
            m_move.y = 0.0f;
        }

        // 一面のボス戦時なら
        if (m_bMoveUp == true)
        {
            // 去るまでのカウンタ
            if (m_nCntChangeMove == ENEMY_BLUE_BIRD_LEAVE)
            {
                m_move.x = ENEMY_BLUE_BIRD_SPEED;
                m_move.y = -ENEMY_BLUE_BIRD_SPEED;
            }
        }
        // 道中なら
        else
        {
            // 去るまでのカウンタ
            if (m_nCntChangeMove == ENEMY_BLUE_BIRD_WAY_LEAVE)
            {
                m_move.x = ENEMY_BLUE_BIRD_SPEED;
                m_move.y = -ENEMY_BLUE_BIRD_SPEED;
            }
        }

        // プレイヤーが存在しているなら
        if (CGame::GetDispPlayer() == true)
        {
            // カウンタの間隔で撃つ
            if (m_nCntChangeMove % ENEMY_BLUE_BIRD_ATTACK_COUNTER == 0)
            {
                // 自機の位置を取得し、その位置へ撃つ
                fAngle = GetAngleToPlayer();

                // 弾の生成
                CBullet::Create(pos, ENEMY_BLUE_BIRD_BULLET_SIZE,
                    D3DXVECTOR3(-sinf(fAngle)*ENEMY_BLUE_BIRD_BULLET_SPEED, -cosf(fAngle)*ENEMY_BLUE_BIRD_BULLET_SPEED, 0.0f),
                    CBullet::TYPE_WATER, TRIBE_ENEMY);

                // 弾の生成
                CBullet::Create(pos, ENEMY_BLUE_BIRD_BULLET_SIZE,
                    D3DXVECTOR3(-sinf(fAngle + (D3DX_PI / 6))*ENEMY_BLUE_BIRD_BULLET_SPEED, -cosf(fAngle + (D3DX_PI / 6))*ENEMY_BLUE_BIRD_BULLET_SPEED, 0.0f),
                    CBullet::TYPE_WATER, TRIBE_ENEMY);

                // 弾の生成
                CBullet::Create(pos, ENEMY_BLUE_BIRD_BULLET_SIZE,
                    D3DXVECTOR3(-sinf(fAngle - (D3DX_PI / 6))*ENEMY_BLUE_BIRD_BULLET_SPEED, -cosf(fAngle - (D3DX_PI / 6))*ENEMY_BLUE_BIRD_BULLET_SPEED, 0.0f),
                    CBullet::TYPE_WATER, TRIBE_ENEMY);
            }
        }

        // プレイヤーの位置によって、向く方向を変える
        if (playerPos.x <= pos.x)
        {
            // 左向きでテクスチャ座標を更新
            SetAnimation(6, 4);
        }
        else
        {
            // 右向きでテクスチャ座標を更新
            SetReverseAnimation(6, 4);
        }

        break;

    case CEnemy::TYPE_BOSS_FOREST:

        // ボス森の行動
        ForestMovement(playerPos);

        break;

    case CEnemy::TYPE_CATERPILLAR:

        // 移動が変わりだすまでのカウンタを数える
        m_nCntChangeMove++;

        // 落下を開始するまでのカウンタ
        if (m_nCntChangeMove >= ENEMY_CATERPILLAR_FALL_COUNT)
        {
            // 重力を少し軽めに設定
            m_move.y += GRAVITY / 4;

            // フラグによって、右or左回転
            if (m_bMoveUp == true)
            {
                // 右回転
                m_fMoveAngle += ENEMY_CATERPILLAR_ROT_SPEED;
            }
            else
            {
                // 左回転
                m_fMoveAngle -= ENEMY_CATERPILLAR_ROT_SPEED;
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
                // 通常
                CScene2D::SetColor(DEFAULT_COLOR);
            }
            else
            {
                // 透明
                CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
            }
        }

        // アニメーションを設定
        SetAnimation(10, 2);

        break;

    case CEnemy::TYPE_GUSOKU:

        // 左に移動しているなら
        if (m_move.x <= 0.0f)
        {
            // テクスチャ座標を更新
            SetAnimation(6, 2);
        }

        // 右に移動しているなら
        else
        {
            // テクスチャ座標を逆向きに更新
            SetReverseAnimation(6, 2);
        }

        break;

    case CEnemy::TYPE_SARDINE:

        // 攻撃のカウンタ加算
        m_nBound++;

        // プレイヤーが存在しているなら
        if (CGame::GetDispPlayer() == true)
        {
            // 一定のX軸以上なら
            if (pos.x >= ENEMY_SARDINE_DONT_SHOT_POS_X)
            {
                // カウンタの間隔で撃つ
                if (m_nBound % ENEMY_SARDINE_ATTACK_COUNTER == 0)
                {
                    // 自機の位置を取得し、その位置へ撃つ
                    fAngle = GetAngleToPlayer();

                    // 弾の生成
                    CBullet::Create(pos, ENEMY_SARDINE_BULLET_SIZE,
                        D3DXVECTOR3(-sinf(fAngle)*ENEMY_SARDINE_BULLET_SPEED, -cosf(fAngle)*ENEMY_SARDINE_BULLET_SPEED, 0.0f),
                        CBullet::TYPE_WATER, TRIBE_ENEMY);
                }
            }
        }

        // 移動のパターン(一定範囲内で折り返す)
        switch (m_nCntChangeMove)
        {
        case SARDINE_MOVE_UPPER:

            if (pos.y < ENEMY_SARDINE_INVERSIO_POS_Y_1)
            {
                m_move.y *= -1;
            }
            else if(pos.y > ENEMY_SARDINE_INVERSIO_POS_Y_2)
            {
                m_move.y *= -1;
            }

            break;

        case SARDINE_MOVE_MIDDLE:

            if (pos.y < ENEMY_SARDINE_INVERSIO_POS_Y_3)
            {
                m_move.y *= -1;
            }
            else if (pos.y > ENEMY_SARDINE_INVERSIO_POS_Y_4)
            {
                m_move.y *= -1;
            }

            break;

        case SARDINE_MOVE_LOWER:

            if (pos.y < ENEMY_SARDINE_INVERSIO_POS_Y_5)
            {
                m_move.y *= -1;
            }
            else if (pos.y > ENEMY_SARDINE_INVERSIO_POS_Y_6)
            {
                m_move.y *= -1;
            }

            break;

        case SARDINE_MOVE_STRAIGHT:

            // 左に進むか、右に進むか
            if (m_bMoveUp == true)
            {
                // 左に進む
                m_move.x = -ENEMY_SARDINE_STRAIGHT_SPEED;
                m_move.y = 0.0f;
            }
            else
            {
                // 右に進む
                m_move.x = ENEMY_SARDINE_STRAIGHT_SPEED;
                m_move.y = 0.0f;
            }

            break;
        }

        // 左に移動しているなら
        if (m_move.x <= 0.0f)
        {
            // テクスチャ座標を更新
            SetAnimation(3, 6);
        }

        // 右に移動しているなら
        else
        {
            // テクスチャ座標を逆向きに更新
            SetReverseAnimation(3, 6);
        }

        break;

    case CEnemy::TYPE_CANNON:

        // 攻撃時間を数える
        m_nCntChangeMove++;

        // プレイヤーが存在しているなら
        if (CGame::GetDispPlayer() == true)
        {
            //自機のほうを徐々に見るようにする
            m_fDestAngle = GetAngleToPlayer();

            // 回転の制限
            if (m_fMoveAngle > D3DX_PI)
            {
                m_fMoveAngle -= D3DX_PI * 2;
            }
            if (m_fMoveAngle < -D3DX_PI)
            {
                m_fMoveAngle += D3DX_PI * 2;
            }

            m_fMinAngle = m_fDestAngle - m_fMoveAngle;

            // 目的の角度の回転を制限
            if (m_fMinAngle > D3DX_PI)
            {
                m_fDestAngle -= D3DX_PI * 2;
            }
            if (m_fMinAngle < -D3DX_PI)
            {
                m_fDestAngle += D3DX_PI * 2;
            }

            // 回転の制限
            if (m_fMoveAngle > D3DX_PI)
            {
                m_fMoveAngle -= D3DX_PI * 2;
            }
            if (m_fMoveAngle < -D3DX_PI)
            {
                m_fMoveAngle += D3DX_PI * 2;
            }

            if (fabsf(m_fMinAngle) < 0)
            {
                m_fMinAngle = m_fDestAngle;
            }
            else
            {
                m_fMoveAngle += (m_fDestAngle - m_fMoveAngle) * ENEMY_CANNON_TURN_SPEED;
            }

            // カウンタの間隔で撃つ
            if (m_nCntChangeMove % ENEMY_CANNON_ATTACK_COUNTER == 0)
            {
                // 弾の生成(発射位置は砲台の先から)
                CBullet::Create(pos + D3DXVECTOR3(-sinf(m_fMoveAngle)*(size.x / 2), -cosf(m_fMoveAngle)*(size.y / 2), 0.0f), ENEMY_CANNON_BULLET_SIZE,
                    D3DXVECTOR3(-sinf(m_fMoveAngle)*ENEMY_CANNON_BULLET_SPEED, -cosf(m_fMoveAngle)*ENEMY_CANNON_BULLET_SPEED, 0.0f),
                    CBullet::TYPE_LIGHT, TRIBE_ENEMY);
            }
        }

        break;

    case CEnemy::TYPE_TORPEDO:

        // テクスチャ座標を更新
        SetAnimation(4, 4);

        break;

    case CEnemy::TYPE_BOSS_DRAGON:

        // ドラゴンの行動
        pos = DragonMovement(pos, size);

        break;

    case CEnemy::TYPE_RED_BIRD:

        // 移動が変わりだすまでのカウンタを数える
        m_nCntChangeMove++;

        // ストップするまでのカウンタ
        if (m_nCntChangeMove == ENEMY_RED_BIRD_STOP)
        {
            m_move.x = 0.0f;
        }
        // ゴッドバードするまでのカウンタ
        else if (m_nCntChangeMove == ENEMY_RED_BIRD_GOD)
        {
            // ゴッドバードのフラグをtrueに
            m_bMoveUp = true;

            // HPを1にする
            m_nLife = 1;

            // 自機の位置を一瞬取得し、その位置へ突っ込む
            fAngle = GetAngleToPlayer();

            // プレイヤーが生存しているなら
            if (CGame::GetDispPlayer() == true)
            {
                m_move = D3DXVECTOR3(-sinf(fAngle)*ENEMY_RED_BIRD_GOD_SPEED, -cosf(fAngle)*ENEMY_RED_BIRD_GOD_SPEED, 0.0f);
            }
            // プレイヤーがいないなら、右へまっすぐ
            else
            {
                m_move = D3DXVECTOR3(ENEMY_RED_BIRD_GOD_SPEED, 0.0f, 0.0f);
            }
        }

        if (m_bMoveUp == false)
        {// 通常時

         // プレイヤーが存在しているなら
            if (CGame::GetDispPlayer() == true)
            {
                // カウンタの間隔で撃つ
                if (m_nCntChangeMove % ENEMY_RED_BIRD_ATTACK_COUNTER == 0)
                {
                    // 自機の位置を取得し、その位置へ撃つ
                    fAngle = GetAngleToPlayer();

                    // 弾の生成
                    CBullet::Create(pos, ENEMY_RED_BIRD_BULLET_SIZE,
                        D3DXVECTOR3(-sinf(fAngle)*ENEMY_RED_BIRD_BULLET_SPEED, -cosf(fAngle)*ENEMY_RED_BIRD_BULLET_SPEED, 0.0f),
                        CBullet::TYPE_FIRE_BURST, TRIBE_ENEMY);
                }
            }

            // テクスチャ座標を更新
            SetParagraphAnimation(1, 2, 6, 4);
        }
        else
        {// ゴッドバード時
            
            // カウンタによって、エフェクトを生成
            if (m_nCntChangeMove % ENEMY_RED_BIRD_SET_EFFECT_COUNT == 0)
            {
                // エフェクトを生成(サイズは一回り小さい)
                CEffect::Create(pos + m_move, size - D3DXVECTOR3(-10.0f, -10.0f, 0.0f),
                    EFFECT_RED_FIRE_FIRST_COLOR, CEffect::TYPE_RED_FIRE);
            }

            // テクスチャ座標を更新
            SetParagraphAnimation(2, 2, 3, 4);
        }

        // ゴッドバード前の、デンジャー表記
        if (m_nCntChangeMove == ENEMY_RED_BIRD_DANGER)
        {
            CUI::Create(D3DXVECTOR3(pos.x, pos.y - (size.y / 2), 0.0f), DANGER_SIZE, DEFAULT_COLOR, CUI::TYPE_DANGER);
        }

        break;

    case CEnemy::TYPE_BAT:

        // 移動を変えるためのカウンタを加算
        m_nCntChangeMove++;

        // プレイヤーへの角度取得
        fAngle = GetAngleToPlayer();

        // 一定時間内のみ、追尾する
        if (m_nCntChangeMove <= ENEMY_BAT_STOP_HOMING)
        {
            // プレイヤーが存在するなら
            if (CGame::GetDispPlayer() == true)
            {
                m_move = D3DXVECTOR3(-sinf(fAngle)*ENEMY_BAT_SPEED, -cosf(fAngle)*ENEMY_BAT_SPEED, 0.0f);
            }
        }

        // エフェクトを生成するカウンタ
        if (m_nCntChangeMove % ENEMY_BAT_SET_EFFECT_COUNT == 0)
        {
            // エフェクトを生成(サイズは一回り大きい)
            CEffect::Create(pos, size + D3DXVECTOR3(15.0f, 15.0f, 0.0f),
                EFFECT_RED_FIRE_FIRST_COLOR, CEffect::TYPE_RED_FIRE);
        }

        // 移動の向きによって、向く方向を変える
        if (m_move.x <= 0.0f)
        {
            // 左向きでテクスチャ座標を更新
            SetAnimation(8, 2);
        }
        else
        {
            // 右向きでテクスチャ座標を更新
            SetReverseAnimation(8, 2);
        }

        break;

    case TYPE_MULTI_BIRD:

        // マルチバードの行動
        MultiBird(pos);

        break;

    case CEnemy::TYPE_BOSS_BLAZE:

        // ブレイズの行動
        pos = BlazeMovement(pos, size);

        break;
    }
    //==================================================================================================================

    // 移動量を位置に加算
    pos += m_move;

    // イモムシは、床に当たったらバウンド
    if (m_type == CEnemy::TYPE_CATERPILLAR)
    {
        pos = Bound(pos, size);
    }

    // 位置を反映
    SetPosition(pos);

    // 頂点座標を変更(イモムシ、砲台は回転)
    if (m_type == CEnemy::TYPE_CATERPILLAR)
    {
        SetRotVertex(m_fMoveAngle);
    }
    else if (m_type == CEnemy::TYPE_CANNON)
    {
        SetRotVertex(-m_fMoveAngle);
    }
    else
    {
        // ボス森なら
        if (m_type == CEnemy::TYPE_BOSS_FOREST)
        {
            // 状態カウンタあるなら
            if (m_nCounterState > 0)
            {
                // 幹に当たっているなら
                if (m_nWhereHit == 0)
                {
                    // 登場中かどうか
                    if (m_nCntChangeMove < ENEMY_BOSS_FOREST_STOP_X)
                    {
                        // 登場中はずらさない
                        SetVertex();
                    }
                    else
                    {
                        // 偶数で割れるなら、右に揺れる
                        if (m_nCounterState % 2 == 0)
                        {
                            // 見かけ上の位置を設定
                            SetVisualVertex(D3DXVECTOR3(pos.x + ENEMY_BOSS_FOREST_SHAKE_POS_X, pos.y, 0.0f), size);
                        }
                        // 奇数なら左
                        else
                        {
                            // 見かけ上の位置を設定
                            SetVisualVertex(D3DXVECTOR3(pos.x - ENEMY_BOSS_FOREST_SHAKE_POS_X, pos.y, 0.0f), size);
                        }
                    }
                }
                // 葉っぱに当たっているなら
                else if (m_nWhereHit == 1)
                {
                    // 揺らす
                    CBg::Shake(m_nCounterState);

                    // 幹は通常
                    SetVertex();
                }
            }
            else
            {
                // 状態カウンタがないなら通常
                SetVertex();
            }
        }
        else
        {
            // ボス森でないなら通常
            SetVertex();
        }
    }

    // 状態カウンタを確認
    if (m_nCounterState > 0)
    {
        // 0より大きいなら1引く
        m_nCounterState--;
    }
    else
    {
        // 落下中のイモムシ以外は通常色に
        if (m_type == CEnemy::TYPE_CATERPILLAR && m_nCntChangeMove < ENEMY_CATERPILLAR_FALL_COUNT)
        {

        }
        else
        {
            // 通常色に
            SetColor(DEFAULT_COLOR);

            // 当たった場所のリセット
            m_nWhereHit = -1;
        }
    }

    // 水の多段ヒット回避のカウンタを確認
    if (m_nCounterHitWater > 0)
    {
        m_nCounterHitWater--;
    }

    // 雷の多段ヒット回避のカウンタを確認
    if (m_nCounterHitLight > 0)
    {
        m_nCounterHitLight--;
    }

    // 反撃のクールタイムを確認
    if (m_nCounterAttackCoolTime > 0)
    {
        m_nCounterAttackCoolTime--;
    }

    // エネミーの寿命がなくなったかどうか
    bUse = DefeatEnemy(pos, size);
   
    // トーピードは床に当たったら爆発
    if (m_type == CEnemy::TYPE_TORPEDO)
    {
        // 体力があるなら
        if (m_nLife > 0)
        {
            bUse = TorpedoExplosion(pos, size);
        }
    }

    // 画面の端から2倍の場所から出たら
    if (OutGame2D(&pos, &size) == true)
    {
        // 未使用に
        bUse = false;
    }

    // 未使用判定なら
    if (bUse == false)
    {
        Uninit(); // これより下にコードは書かない
    }
}

//============================================================
// エネミーの描画処理
//============================================================
void CEnemy::Draw(void)
{
    // 描画処理
    CScene2D::Draw();
}

//============================================================
// エネミーの画像ロード処理
//============================================================
HRESULT CEnemy::Load(void)
{
    // レンダラーからデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    //=======================================================================================================
    // 種類ごとのテクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy000.png", &m_apTexture[CEnemy::TYPE_FLY]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy001.png", &m_apTexture[CEnemy::TYPE_SKELETON]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy002.png", &m_apTexture[CEnemy::TYPE_GREEN_BIRD]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy003.png", &m_apTexture[CEnemy::TYPE_CROW]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy004.png", &m_apTexture[CEnemy::TYPE_BLUE_BIRD]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy005.png", &m_apTexture[CEnemy::TYPE_BOSS_FOREST]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy006.png", &m_apTexture[CEnemy::TYPE_CATERPILLAR]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy007.png", &m_apTexture[CEnemy::TYPE_GUSOKU]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy008.png", &m_apTexture[CEnemy::TYPE_SARDINE]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy009.png", &m_apTexture[CEnemy::TYPE_CANNON]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy010.png", &m_apTexture[CEnemy::TYPE_TORPEDO]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy011.png", &m_apTexture[CEnemy::TYPE_BOSS_DRAGON]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy012.png", &m_apTexture[CEnemy::TYPE_RED_BIRD]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy013.png", &m_apTexture[CEnemy::TYPE_BAT]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy014.png", &m_apTexture[CEnemy::TYPE_MULTI_BIRD]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy015.png", &m_apTexture[CEnemy::TYPE_BOSS_BLAZE]);
    //=======================================================================================================

    return S_OK;
}

//============================================================
// エネミーの画像破棄処理
//============================================================
void CEnemy::Unload(void)
{
    // テクスチャの破棄
    for (int nCntTexture = 0; nCntTexture < CEnemy::TYPE_MAX; nCntTexture++)
    {
        if (m_apTexture[nCntTexture] != NULL)
        {
            m_apTexture[nCntTexture]->Release();
            m_apTexture[nCntTexture] = NULL;
        }
    }
}

//====================================================
// エネミーの作成
//====================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nCntChangeMove, bool bMoveUp, CEnemy::TYPE type)
{
    CEnemy *pEnemy = NULL;

    // メモリを確保
    pEnemy = new CEnemy;

    // タイプのみ、テクスチャ割り当てのために結びつけておく
    pEnemy->m_type = type;

    // 初期化
    pEnemy->Init(pos, size);

    // メンバ変数を結びつける
    pEnemy->m_posOld = pos;
    pEnemy->m_nCntChangeMove = nCntChangeMove;
    pEnemy->m_bMoveUp = bMoveUp;

    //========================================================
    // エネミーの種類によって、結びつける値を変える
    switch (pEnemy->m_type)
    {
    case CEnemy::TYPE_FLY:

        pEnemy->m_nCntCollision = 1;
        pEnemy->m_aCollision[0].posToShift = DEFAULT_VECTOR;
        pEnemy->m_aCollision[0].size = ENEMY_FLY_COLLISION_SIZE;
        pEnemy->m_element = ELEMENT_GRASS;
        pEnemy->m_nLife = ENEMY_FLY_LIFE;
        pEnemy->m_nMaxLife = ENEMY_FLY_LIFE;

        pEnemy->m_fMoveAngle = 180.0f;

        break;

    case CEnemy::TYPE_SKELETON:

        pEnemy->m_nCntCollision = 1;
        pEnemy->m_aCollision[0].posToShift = ENEMY_SKELETON_COLLISION_POS;
        pEnemy->m_aCollision[0].size = ENEMY_SKELETON_COLLISION_SIZE;
        pEnemy->m_element = ELEMENT_FIRE;
        pEnemy->m_nLife = ENEMY_SKELETON_LIFE;
        pEnemy->m_nMaxLife = ENEMY_SKELETON_LIFE;

        if (bMoveUp == true)
        {// 左に進む
            pEnemy->m_move.x = -ENEMY_SKELETON_SPEED;
        }
        else
        {// 右に進む
            pEnemy->m_move.x = ENEMY_SKELETON_SPEED;
        }

        break;

    case CEnemy::TYPE_GREEN_BIRD:

        pEnemy->m_nCntCollision = 1;
        pEnemy->m_aCollision[0].posToShift = DEFAULT_VECTOR;
        pEnemy->m_aCollision[0].size = ENEMY_GREEN_BIRD_COLLISION_SIZE;
        pEnemy->m_element = ELEMENT_GRASS;
        pEnemy->m_nLife = ENEMY_GREEN_BIRD_LIFE;
        pEnemy->m_nMaxLife = ENEMY_GREEN_BIRD_LIFE;

        // 左に進む
        pEnemy->m_move.x = -ENEMY_GREEN_BIRD_SPEED;

        break;

    case CEnemy::TYPE_CROW:

        pEnemy->m_nCntCollision = 1;
        pEnemy->m_aCollision[0].posToShift = DEFAULT_VECTOR;
        pEnemy->m_aCollision[0].size = ENEMY_CROW_COLLISION_SIZE;
        pEnemy->m_element = ELEMENT_MULTI;
        pEnemy->m_nLife = ENEMY_CROW_LIFE;
        pEnemy->m_nMaxLife = ENEMY_CROW_LIFE;

        if (bMoveUp == true)
        {// 左に進む
            pEnemy->m_move.x = -ENEMY_CROW_SPEED;
        }
        else
        {// 右に進む
            pEnemy->m_move.x = ENEMY_CROW_SPEED;
        }

        break;

    case CEnemy::TYPE_BLUE_BIRD:

        pEnemy->m_nCntCollision = 1;
        pEnemy->m_aCollision[0].posToShift = DEFAULT_VECTOR;
        pEnemy->m_aCollision[0].size = ENEMY_BLUE_BIRD_COLLISION_SIZE;
        pEnemy->m_element = ELEMENT_WATER;
        pEnemy->m_nLife = ENEMY_BLUE_BIRD_LIFE;
        pEnemy->m_nMaxLife = ENEMY_BLUE_BIRD_LIFE;

        // ボス戦時は左下へ進む
        if (pEnemy->m_bMoveUp == true)
        {
            pEnemy->m_move.x = -ENEMY_BLUE_BIRD_SPEED;
            pEnemy->m_move.y = ENEMY_BLUE_BIRD_SPEED;
        }
        // 道中は、HP高め
        else
        {
            pEnemy->m_move.x = -ENEMY_BLUE_BIRD_SPEED;
            pEnemy->m_move.y = ENEMY_BLUE_BIRD_SPEED;

            pEnemy->m_nLife = ENEMY_BLUE_BIRD_WAY_LIFE;
            pEnemy->m_nMaxLife = ENEMY_BLUE_BIRD_WAY_LIFE;
        }

        break;

    case CEnemy::TYPE_BOSS_FOREST:

        pEnemy->m_nCntCollision = 2;
        pEnemy->m_aCollision[0].posToShift = ENEMY_BOSS_FOREST_COLLISION_POS_1;
        pEnemy->m_aCollision[0].size = ENEMY_BOSS_FOREST_COLLISION_SIZE_1;
        pEnemy->m_aCollision[1].posToShift = ENEMY_BOSS_FOREST_COLLISION_POS_2;
        pEnemy->m_aCollision[1].size = ENEMY_BOSS_FOREST_COLLISION_SIZE_2;
        pEnemy->m_element = ELEMENT_GRASS;
        pEnemy->m_nLife = ENEMY_BOSS_FOREST_LIFE;
        pEnemy->m_nMaxLife = ENEMY_BOSS_FOREST_LIFE;

        // 左に進む
        pEnemy->m_move.x = -ENEMY_BOSS_FOREST_SPEED_X;

        // 上に進む
        pEnemy->m_move.y = -ENEMY_BOSS_FOREST_SPEED_Y;

        break;

    case CEnemy::TYPE_CATERPILLAR:

        pEnemy->m_nCntCollision = 1;
        pEnemy->m_aCollision[0].posToShift = DEFAULT_VECTOR;
        pEnemy->m_aCollision[0].size = ENEMY_CATERPILLAR_SIZE;
        pEnemy->m_element = ELEMENT_GRASS;
        pEnemy->m_nLife = ENEMY_CATERPILLAR_LIFE;
        pEnemy->m_nMaxLife = ENEMY_CATERPILLAR_LIFE;

        break;

    case CEnemy::TYPE_GUSOKU:

        pEnemy->m_nCntCollision = 1;
        pEnemy->m_aCollision[0].posToShift = DEFAULT_VECTOR;
        pEnemy->m_aCollision[0].size = ENEMY_GUSOKU_COLLISION_SIZE;
        pEnemy->m_element = ELEMENT_MULTI;
        pEnemy->m_nLife = ENEMY_GUSOKU_LIFE;
        pEnemy->m_nMaxLife = ENEMY_GUSOKU_LIFE;

        if (bMoveUp == true)
        {// 左に進む
            pEnemy->m_move.x = -ENEMY_GUSOKU_SPEED;
        }
        else
        {// 右に進む
            pEnemy->m_move.x = ENEMY_GUSOKU_SPEED;
        }

        break;

    case CEnemy::TYPE_SARDINE:

        pEnemy->m_nCntCollision = 1;
        pEnemy->m_aCollision[0].posToShift = DEFAULT_VECTOR;
        pEnemy->m_aCollision[0].size = ENEMY_SARDINE_COLLISION_SIZE;
        pEnemy->m_element = ELEMENT_WATER;
        pEnemy->m_nLife = ENEMY_SARDINE_LIFE;
        pEnemy->m_nMaxLife = ENEMY_SARDINE_LIFE;

        if (bMoveUp == true)
        {// 左上に進む
            pEnemy->m_move = ENEMY_SARDINE_MOVE;
            pEnemy->m_move.x *= -1;
        }
        else
        {// 右上に進む
            pEnemy->m_move = ENEMY_SARDINE_MOVE;
        }

        break;

    case CEnemy::TYPE_CANNON:

        pEnemy->m_nCntCollision = 1;
        pEnemy->m_aCollision[0].posToShift = DEFAULT_VECTOR;
        pEnemy->m_aCollision[0].size = ENEMY_CANNON_COLLISION_SIZE;
        pEnemy->m_element = ELEMENT_LIGHT;
        pEnemy->m_nLife = ENEMY_CANNON_LIFE;
        pEnemy->m_nMaxLife = ENEMY_CANNON_LIFE;

        pEnemy->m_move.x = -FLOWING_STAGE;

        break;

    case CEnemy::TYPE_TORPEDO:

        pEnemy->m_nCntCollision = 1;
        pEnemy->m_aCollision[0].posToShift = DEFAULT_VECTOR;
        pEnemy->m_aCollision[0].size = ENEMY_TORPEDO_COLLISION_SIZE;
        pEnemy->m_element = ELEMENT_LIGHT;
        pEnemy->m_nLife = ENEMY_TORPEDO_LIFE;
        pEnemy->m_nMaxLife = ENEMY_TORPEDO_LIFE;

        // ボスバトル中以外は流れる
        if (CGame::GetBossBattle() == false)
        {
            pEnemy->m_move.y = ENEMY_TORPEDO_FALLING_SPEED;
            pEnemy->m_move.x = -FLOWING_STAGE;
        }
        else
        {
            pEnemy->m_move.y = ENEMY_TORPEDO_FALLING_SPEED_IN_BOSS;
        }

        // 火山ステージでは、左に流れず、落下スピードはボス戦時のもの
        if (CGame::GetStage() == CManager::STAGE_γ)
        {
            pEnemy->m_move.x = 0.0f;
            pEnemy->m_move.y = ENEMY_TORPEDO_FALLING_SPEED_IN_BOSS;
        }

        break;

    case CEnemy::TYPE_BOSS_DRAGON:

        pEnemy->m_nCntCollision = 3;
        pEnemy->m_aCollision[0].posToShift = ENEMY_BOSS_DRAGON_COLLISION_POS_1;
        pEnemy->m_aCollision[0].size = ENEMY_BOSS_DRAGON_COLLISION_SIZE_1;
        pEnemy->m_aCollision[1].posToShift = ENEMY_BOSS_DRAGON_COLLISION_POS_2;
        pEnemy->m_aCollision[1].size = ENEMY_BOSS_DRAGON_COLLISION_SIZE_2;
        pEnemy->m_aCollision[2].posToShift = ENEMY_BOSS_DRAGON_COLLISION_POS_3;
        pEnemy->m_aCollision[2].size = ENEMY_BOSS_DRAGON_COLLISION_SIZE_3;
        pEnemy->m_element = ELEMENT_WATER;
        pEnemy->m_nLife = ENEMY_BOSS_DRAGON_LIFE;
        pEnemy->m_nMaxLife = ENEMY_BOSS_DRAGON_LIFE;

        // 左に進む
        pEnemy->m_move.x = -FLOWING_STAGE;

        // 行動管理のため、バウンド回数リセット
        pEnemy->m_nBound = DRAGON_ATTACK_NONE;

        break;

    case CEnemy::TYPE_RED_BIRD:

        pEnemy->m_nCntCollision = 1;
        pEnemy->m_aCollision[0].posToShift = DEFAULT_VECTOR;
        pEnemy->m_aCollision[0].size = ENEMY_RED_BIRD_COLLISION_SIZE;
        pEnemy->m_element = ELEMENT_FIRE;
        pEnemy->m_nLife = ENEMY_RED_BIRD_LIFE;
        pEnemy->m_nMaxLife = ENEMY_RED_BIRD_LIFE;

        // 右に進む
        pEnemy->m_move.x = ENEMY_RED_BIRD_SPEED;

        break;

    case CEnemy::TYPE_BAT:

        pEnemy->m_nCntCollision = 1;
        pEnemy->m_aCollision[0].posToShift = DEFAULT_VECTOR;
        pEnemy->m_aCollision[0].size = ENEMY_BAT_COLLISION_SIZE;
        pEnemy->m_element = ELEMENT_FIRE;
        pEnemy->m_nLife = ENEMY_BAT_LIFE;
        pEnemy->m_nMaxLife = ENEMY_BAT_LIFE;

        break;

    case CEnemy::TYPE_MULTI_BIRD:

        pEnemy->m_nCntCollision = 1;
        pEnemy->m_aCollision[0].posToShift = DEFAULT_VECTOR;
        pEnemy->m_aCollision[0].size = ENEMY_MULTI_BIRD_COLLISION_SIZE;
        pEnemy->m_element = ELEMENT_MULTI;
        pEnemy->m_nLife = ENEMY_MULTI_BIRD_LIFE;
        pEnemy->m_nMaxLife = ENEMY_MULTI_BIRD_LIFE;

        pEnemy->m_move.x = -ENEMY_BLUE_BIRD_SPEED;
        pEnemy->m_move.y = ENEMY_BLUE_BIRD_SPEED;

        break;

    case CEnemy::TYPE_BOSS_BLAZE:

        pEnemy->m_nCntCollision = 2;
        pEnemy->m_aCollision[0].posToShift = ENEMY_BOSS_BLAZE_COLLISION_POS_1;
        pEnemy->m_aCollision[0].size = ENEMY_BOSS_BLAZE_COLLISION_SIZE_1;
        pEnemy->m_aCollision[1].posToShift = ENEMY_BOSS_BLAZE_COLLISION_POS_2;
        pEnemy->m_aCollision[1].size = ENEMY_BOSS_BLAZE_COLLISION_SIZE_2;
        pEnemy->m_element = ELEMENT_FIRE;
        pEnemy->m_nLife = ENEMY_BOSS_BLAZE_LIFE;
        pEnemy->m_nMaxLife = ENEMY_BOSS_BLAZE_LIFE;

        // 左に進む
        pEnemy->m_move.x = -ENEMY_BOSS_BLAZE_SPEED;

        // 行動管理のため、バウンド回数リセット
        pEnemy->m_nBound = BLAZE_ATTACK_NONE;

        break;
    }
    //========================================================

    return pEnemy;
}

//====================================================
// エネミーに弾が当たった時の処理
//====================================================
void CEnemy::Hit(D3DXVECTOR3 posBullet, const int nCounterState, int nDamage, const int nWhereHit, const ELEMENT element)
{
    // 変数宣言
    D3DXVECTOR3 pos = GetPosition();    // 位置を取得

    // ヒット音再生
    CSound *pSound = CManager::GetSound();
    pSound->Play(CSound::LABEL_SE_HIT);

    // 状態カウンタを結びつける
    m_nCounterState = nCounterState;

    // どこに当たったか
    m_nWhereHit = nWhereHit;

    // 貫通するなら、水と雷のヒットカウンタを結びつける
    if (nCounterState != DONT_GO_THROUGH)
    {
        switch (element)
        {
        case ELEMENT_WATER:
            m_nCounterHitWater = BULLET_WATER_AVOID_MULTIPLE_HITS_FRAME;
            break;

        case ELEMENT_LIGHT:
            m_nCounterHitLight = BULLET_LIGHT_AVOID_MULTIPLE_HITS_FRAME;
            break;
        }
    }

    // 弱点をついたかどうかのフラグを、一旦falseに
    m_bDefeatWithWeak = false;

    // ワンパン判定用の変数宣言
    bool bMaxLife = false;  // 最大HPかどうか

    // 最大HPなら、一発で倒したかどうかの計算をするフラグをtrueに
    if (m_nLife == m_nMaxLife)
    {
        bMaxLife = true;
    }

    // ダメージ計算
    DamageCalculation(posBullet, element, nDamage);

    // 一発で倒したかどうか
    if (bMaxLife == true)
    {
        if (m_nLife <= 0)
        {
            // 一発で倒したフラグをtrueに
            m_bDefeatOneHit = true;
        }
    }
}

//====================================================
// エネミーのダメージ計算
//====================================================
void CEnemy::DamageCalculation(D3DXVECTOR3 pos, const ELEMENT element, int nDamage)
{
    // 最終的なダメージ
    int nFinalDamage = 0;

    // 弱点のフラグ
    bool bWeak = false;

    // 半減のフラグ
    bool bHalf = false;

    // エネミーの属性ごとにダメージ計算
    switch (m_element)
    {
    case ELEMENT_FIRE:

        // 弾の属性ごとにダメージ計算
        // 弾の属性は、1つの弱点、2つの半減、1つの通常を持つ
        switch (element)
        {
        case ELEMENT_FIRE:

            // 敵の色を赤に
            SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

            // 反撃
            CounterAttack();

            // ブレイズなら、属性値を上げる
            if (m_type == TYPE_BOSS_BLAZE)
            {
                m_fDestAngle += GOD_FIRE_PLAYER_FIRE_ATTACK_VALUE;

                // 神炎モードなら、等倍でダメージを通す
                if (m_bMoveUp == true)
                {
                    // 通常
                    nFinalDamage = nDamage;
                }
            }

            break;

        case ELEMENT_WATER:

            // 敵の色を青に
            SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));

            // 弱点
            nFinalDamage = nDamage * 2;
            bWeak = true;

            // 弱点のフラグをtrueに
            m_bDefeatWithWeak = true;

            break;

        case ELEMENT_LIGHT:

            // 敵の色を黄色に
            SetColor(COLOR_YELLOW);

            // 通常
            nFinalDamage = nDamage;

            break;
        }

        break;

    case ELEMENT_WATER:

        // 弾の属性ごとにダメージ計算
        switch (element)
        {
        case ELEMENT_FIRE:

            // 敵の色を赤に
            SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

            // 半減
            nFinalDamage = nDamage / 2;
            bHalf = true;

            break;

        case ELEMENT_WATER:

            // 敵の色を青に
            SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));

            // 反撃
            CounterAttack();

            break;

        case ELEMENT_LIGHT:

            // 敵の色を黄色に
            SetColor(COLOR_YELLOW);

            // 弱点
            nFinalDamage = nDamage * 2;
            bWeak = true;

            // 弱点のフラグをtrueに
            m_bDefeatWithWeak = true;

            break;
        }

        break;

    case ELEMENT_LIGHT:

        // 弾の属性ごとにダメージ計算
        switch (element)
        {
        case ELEMENT_FIRE:

            // 敵の色を赤に
            SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

            // 通常
            nFinalDamage = nDamage;

            break;

        case ELEMENT_WATER:

            // 敵の色を青に
            SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));

            // 通常
            nFinalDamage = nDamage;

            break;

        case ELEMENT_LIGHT:

            // 敵の色を黄色に
            SetColor(COLOR_YELLOW);

            // 反撃
            CounterAttack();

            // 半減
            nFinalDamage = nDamage / 2;
            bHalf = true;

            break;
        }

        break;

    case ELEMENT_GRASS:

        // ボス森なら
        if (m_type == TYPE_BOSS_FOREST)
        {
            // 弾の属性ごとにダメージ計算
            switch (element)
            {
            case ELEMENT_FIRE:

                // 幹なら
                if (m_nWhereHit == 0)
                {
                    // 敵の色を赤に
                    SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
                }

                // 弱点
                nFinalDamage = nDamage * 2;
                bWeak = true;

                // 弱点のフラグをtrueに
                m_bDefeatWithWeak = true;

                break;

            case ELEMENT_WATER:

                // 幹なら
                if (m_nWhereHit == 0)
                {
                    // 敵の色を青に
                    SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
                }

                // 半減
                nFinalDamage = nDamage / 2;
                bHalf = true;

                break;

            case ELEMENT_LIGHT:

                // 幹なら
                if (m_nWhereHit == 0)
                {
                    // 敵の色を黄色に
                    SetColor(COLOR_YELLOW);
                }

                // 通常
                nFinalDamage = nDamage;

                break;
            }
        }
        else
        {
            // 普通の敵なら
            // 弾の属性ごとにダメージ計算
            switch (element)
            {
            case ELEMENT_FIRE:

                // 敵の色を赤に
                SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

                // 弱点
                nFinalDamage = nDamage * 2;
                bWeak = true;

                // 弱点のフラグをtrueに
                m_bDefeatWithWeak = true;

                break;

            case ELEMENT_WATER:

                // 敵の色を青に
                SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));

                // 半減
                nFinalDamage = nDamage / 2;
                bHalf = true;

                break;

            case ELEMENT_LIGHT:

                // 敵の色を黄色に
                SetColor(COLOR_YELLOW);

                // 通常
                nFinalDamage = nDamage;

                break;
            }
        }

        break;

        // マルチタイプは、すべて等倍
    case ELEMENT_MULTI:

        switch (element)
        {
        case ELEMENT_FIRE:

            // 敵の色を赤に
            SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

            // 通常
            nFinalDamage = nDamage;

            break;

        case ELEMENT_WATER:

            // 敵の色を青に
            SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));

            // 通常
            nFinalDamage = nDamage;

            break;

        case ELEMENT_LIGHT:

            // 敵の色を黄色に
            SetColor(COLOR_YELLOW);

            // 通常
            nFinalDamage = nDamage;

            break;
        }

        break;
    }

    // ボスには、弱点部位が存在する
    switch (m_type)
    {
    case TYPE_BOSS_FOREST:

        // 幹なら
        if (m_nWhereHit == 0)
        {
            // 弱点ダメージ
            nFinalDamage = (int)((float)nFinalDamage * WEAK_HIT_DAMAGE_UP_RATE);
            m_nLife -= nFinalDamage;
        }
        else
        {
            // ダメージ
            m_nLife -= nFinalDamage;
        }

        break;

    case TYPE_BOSS_DRAGON:

        // 顔なら
        if (m_nWhereHit == 0)
        {
            // 弱点ダメージ
            nFinalDamage = (int)((float)nFinalDamage * WEAK_HIT_DAMAGE_UP_RATE);
            m_nLife -= nFinalDamage;
        }
        else
        {
            // ダメージ
            m_nLife -= nFinalDamage;
        }

        break;

    case TYPE_BOSS_BLAZE:

        // 顔なら
        if (m_nWhereHit == 0)
        {
            // 弱点ダメージ
            nFinalDamage = (int)((float)nFinalDamage * WEAK_HIT_DAMAGE_UP_RATE);
            m_nLife -= nFinalDamage;
        }
        else
        {
            // ダメージ
            m_nLife -= nFinalDamage;
        }

        break;

    default:

        // ダメージ
        m_nLife -= nFinalDamage;

        break;
    }

    // 弱点なら
    if (bWeak == true)
    {
        // ダメージ表示
        CScore *pScore = CGame::GetScore();
        pScore->SetDamage(nFinalDamage, pos, SCORE_DEFEAT_WITH_WEAK_COLOR);
    }

    // 半減なら
    if (bHalf == true)
    {
        // ダメージ表示
        CScore *pScore = CGame::GetScore();
        pScore->SetDamage(nFinalDamage, pos, SCORE_DEFEAT_HALF_COLOR);
    }

    // 通常なら
    if (bWeak == false && bHalf == false)
    {
        // ダメージ表示
        CScore *pScore = CGame::GetScore();
        pScore->SetDamage(nFinalDamage, pos, DEFAULT_COLOR);
    }
}

//====================================================
// エネミーがやられたかどうか
//====================================================
bool CEnemy::DefeatEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 変数宣言
    bool bUse = true;               // 敵を使うかどうか
    int nScore = 0;                 // スコア
    D3DXCOLOR col = DEFAULT_COLOR;  // スコアの表示色

    // ライフが0以下になったら
    if (m_nLife <= 0)
    {
        // 爆発音再生
        CSound *pSound = CManager::GetSound();
        pSound->Play(CSound::LABEL_SE_EXPLOSION);

        // 大爆発エフェクト (サイズは倍)
        CEffect::Create(pos, size + size,
            DEFAULT_COLOR, CEffect::TYPE_BIG_EXPLOSION);

        // 属性ごとの火花エフェクト
        switch (m_element)
        {
        case ELEMENT_FIRE:

            // 赤火花エフェクト (位置はちょっと横、サイズは1.5倍)
            CEffect::Create(pos + D3DXVECTOR3(size.x / 2, -(size.y / 4), 0.0f), size + size / 2,
                DEFAULT_COLOR, CEffect::TYPE_RED_SPARK);

            break;

        case ELEMENT_WATER:

            // 青火花エフェクト (位置はちょっと横、サイズは1.5倍)
            CEffect::Create(pos + D3DXVECTOR3(size.x / 2, -(size.y / 4), 0.0f), size + size / 2,
                DEFAULT_COLOR, CEffect::TYPE_BLUE_SPARK);

            break;

        case ELEMENT_LIGHT:

            // 黒火花エフェクト (位置はちょっと横、サイズは1.5倍)
            CEffect::Create(pos + D3DXVECTOR3(size.x / 2, -(size.y / 4), 0.0f), size + size / 2,
                DEFAULT_COLOR, CEffect::TYPE_BLACK_SPARK);

            break;

        case ELEMENT_GRASS:

            // 緑火花エフェクト (位置はちょっと横、サイズは1.5倍)
            CEffect::Create(pos + D3DXVECTOR3(size.x / 2, -(size.y / 4), 0.0f), size + size / 2,
                DEFAULT_COLOR, CEffect::TYPE_GREEN_SPARK);

            break;

        case ELEMENT_MULTI:

            // 黒火花エフェクト (位置はちょっと横、サイズは1.5倍)
            CEffect::Create(pos + D3DXVECTOR3(size.x / 2, -(size.y / 4), 0.0f), size + size / 2,
                DEFAULT_COLOR, CEffect::TYPE_BLACK_SPARK);

            break;
        }

        //==================================================================
        // スコア加算
        // 種類に応じた場合分け
        switch (m_type)
        {
        case CEnemy::TYPE_FLY:

            nScore = ENEMY_FLY_SCORE;

            break;

        case CEnemy::TYPE_SKELETON:

            nScore = ENEMY_SKELETON_SCORE;

            break;

        case CEnemy::TYPE_GREEN_BIRD:

            // バリアアイテムを生成
            CItem::Create(pos, ITEM_SIZE, CItem::TYPE_BARRIER);

            if (m_bMoveUp == false)
            {// 通常時
                nScore = ENEMY_GREEN_BIRD_SCORE;
            }
            else
            {// ゴッドバード中
                nScore = ENEMY_GREEN_BIRD_SCORE * 4;

                // ボーナスポイント表記
                CEffect::Create(D3DXVECTOR3(pos.x, pos.y + BONUS_POINT_ADD_POS_Y, 0.0f),
                    BONUS_POINT_SIZE, DEFAULT_COLOR, CEffect::TYPE_BONUS_POINT);
            }

            break;

        case CEnemy::TYPE_CROW:

            // ストックアイテムを生成
            CItem::Create(pos, ITEM_SIZE, CItem::TYPE_STOCK);

            nScore = ENEMY_CROW_SCORE;

            break;

        case CEnemy::TYPE_BLUE_BIRD:

            // バリアアイテムを生成
            CItem::Create(pos, ITEM_SIZE, CItem::TYPE_BARRIER);

            nScore = ENEMY_BLUE_BIRD_SCORE;

            break;

        case CEnemy::TYPE_BOSS_FOREST:

            // 森のかつらを消す
            CBg::EraseForest();

            // ボスBGMを止める
            pSound->Stop(CSound::LABEL_BGM_BOSS1);

            // αクリア
            CUI::Create(CLEAR_POS, CLEAR_SIZE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f), CUI::TYPE_αCLEAR);

            // 残像を残す
            CAfterimage::Create(pos, size, 0.0f, DEFAULT_COLOR, CAfterimage::TYPE_BOSS_FOREST);

            // ゲームのカウンタを止める
            CGame::StopGameCount();

            // 自機を無敵に
            CGame::SetInvinciblePlayer();

            // 弱点部位で倒したかどうかで、入るスコアを変える
            if (m_nWhereHit == ENEMY_BOSS_FOREST_WEEKNESS)
            {
                nScore = ENEMY_BOSS_FOREST_SCORE + ENEMY_BOSS_FOREST_WEEKNESS_ADD_SCORE;

                // ボーナスポイント表記
                CEffect::Create(D3DXVECTOR3(pos.x, pos.y + BONUS_POINT_ADD_POS_Y, 0.0f),
                    BONUS_POINT_SIZE, DEFAULT_COLOR, CEffect::TYPE_BONUS_POINT);
            }
            else
            {
                nScore = ENEMY_BOSS_FOREST_SCORE;
            }

            break;

        case CEnemy::TYPE_CATERPILLAR:

            // イモムシは、バウンドするほどスコアが上がる
            nScore = ENEMY_CATERPILLAR_SCORE * m_nBound;

            break;

        case CEnemy::TYPE_GUSOKU:

            // ストックアイテムを生成
            CItem::Create(pos, ITEM_SIZE, CItem::TYPE_STOCK);

            nScore = ENEMY_GUSOKU_SCORE;

            break;

        case CEnemy::TYPE_SARDINE:

            nScore = ENEMY_SARDINE_SCORE;

            break;

        case CEnemy::TYPE_CANNON:

            nScore = ENEMY_CANNON_SCORE;

            break;

        case CEnemy::TYPE_TORPEDO:

            // 動いているステージでの魚雷なら
            if (m_bMoveUp == true)
            {
                // バリアアイテムを生成
                CItem::Create(pos, ITEM_SIZE, CItem::TYPE_BARRIER);
            }

            // ボスバトル中はスコアが低い
            if (CGame::GetBossBattle() == false)
            {
                nScore = ENEMY_TORPEDO_SCORE;
            }
            else
            {
                nScore = ENEMY_TORPEDO_SCORE_IN_BOSS;
            }

            break;

        case CEnemy::TYPE_BOSS_DRAGON:

            // ボスBGMを止める
            pSound->Stop(CSound::LABEL_BGM_BOSS2);

            // βクリア
            CUI::Create(CLEAR_POS, CLEAR_SIZE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f), CUI::TYPE_βCLEAR);

            // 残像を残す
            CAfterimage::Create(pos, size, 0.0f, DEFAULT_COLOR, CAfterimage::TYPE_BOSS_DRAGON);

            // ゲームのカウンタを止める
            CGame::StopGameCount();

            // 自機を無敵に
            CGame::SetInvinciblePlayer();

            // プレイヤーのショック状態を解除
            CGame::SetShockPlayer(false);

            // 弱点部位で倒したかどうかで、入るスコアを変える
            if (m_nWhereHit == ENEMY_BOSS_DRAGON_WEEKNESS)
            {
                nScore = ENEMY_BOSS_DRAGON_SCORE + ENEMY_BOSS_DRAGON_WEEKNESS_ADD_SCORE;

                // ボーナスポイント表記
                CEffect::Create(D3DXVECTOR3(pos.x, pos.y + BONUS_POINT_ADD_POS_Y, 0.0f),
                    BONUS_POINT_SIZE, DEFAULT_COLOR, CEffect::TYPE_BONUS_POINT);
            }
            else
            {
                nScore = ENEMY_BOSS_DRAGON_SCORE;
            }

            break;

        case CEnemy::TYPE_RED_BIRD:

            if (m_bMoveUp == false)
            {// 通常時
                nScore = ENEMY_RED_BIRD_SCORE;
            }
            else
            {// ゴッドバード中
                nScore = ENEMY_RED_BIRD_SCORE * 4;

                // ボーナスポイント表記
                CEffect::Create(D3DXVECTOR3(pos.x, pos.y + BONUS_POINT_ADD_POS_Y, 0.0f),
                    BONUS_POINT_SIZE, DEFAULT_COLOR, CEffect::TYPE_BONUS_POINT);
            }

            break;

        case CEnemy::TYPE_BAT:

            nScore = ENEMY_BAT_SCORE;

            break;

        case CEnemy::TYPE_MULTI_BIRD:

            // バリアアイテムを生成
            CItem::Create(pos, ITEM_SIZE, CItem::TYPE_BARRIER);

            // ストックアイテムを生成
            CItem::Create(pos, ITEM_SIZE, CItem::TYPE_STOCK);

            nScore = ENEMY_MULTI_BIRD_SCORE;

            break;

        case CEnemy::TYPE_BOSS_BLAZE:

            // ボスBGMを止める
            pSound->Stop(CSound::LABEL_BGM_BOSS3);

            // γクリア
            CUI::Create(CLEAR_POS, CLEAR_SIZE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f), CUI::TYPE_γCLEAR);

            // 残像を残す
            CAfterimage::Create(pos, size, 0.0f, DEFAULT_COLOR, CAfterimage::TYPE_BOSS_BLAZE);

            // ゲームのカウンタを止める
            CGame::StopGameCount();

            // 自機を無敵に
            CGame::SetInvinciblePlayer();

            // 弱点部位で倒したかどうかで、入るスコアを変える
            if (m_nWhereHit == ENEMY_BOSS_BLAZE_WEEKNESS)
            {
                // 神炎モードかどうかで、入るスコアを変える
                if (m_bMoveUp == false)
                {
                    nScore = ENEMY_BOSS_BLAZE_SCORE + ENEMY_BOSS_BLAZE_WEEKNESS_ADD_SCORE;
                }
                else
                {
                    nScore = (ENEMY_BOSS_BLAZE_SCORE * 2) + ENEMY_BOSS_BLAZE_WEEKNESS_ADD_SCORE;
                }

                // ボーナスポイント表記
                CEffect::Create(D3DXVECTOR3(pos.x, pos.y + BONUS_POINT_ADD_POS_Y, 0.0f),
                    BONUS_POINT_SIZE, DEFAULT_COLOR, CEffect::TYPE_BONUS_POINT);
            }
            else
            {
                // 神炎モードかどうかで、入るスコアを変える
                if (m_bMoveUp == false)
                {
                    nScore = ENEMY_BOSS_BLAZE_SCORE;
                }
                else
                {
                    nScore = ENEMY_BOSS_BLAZE_SCORE * 2;
                }
            }

            break;
        }
        //==================================================================

        // 一発で倒したかどうかで場合分け
        if (m_bDefeatOneHit == true)
        {// 一発

            // スコア3倍
            nScore = nScore * 3;

            // ブレイズなら、ストックとバリア分のスコアも加算
            if (m_type == TYPE_BOSS_BLAZE)
            {
                CPlayer *pPlayer = NULL;
                int nPlayerStock = CGame::GetStock();
                int nPlayerBarrier = 0;

                // プレイヤーが生存しているなら、バリアの情報を取得
                if (CGame::GetDispPlayer() == true)
                {
                    // プレイヤー情報を取得
                    pPlayer = CGame::GetPlayer();
                    nPlayerBarrier = pPlayer->GetBarrier();
                }

                // ボーナススコア
                nScore += nPlayerStock * ENEMY_BOSS_BLAZE_SCORE_ADD_STOCK_RATE + nPlayerBarrier * ENEMY_BOSS_BLAZE_SCORE_ADD_BARRIER_RATE;
            }

            // スコア表示色を黄色にする
            col = SCORE_DEFEAT_ONE_SHOT_COLOR;
        }
        else
        {//一発でないなら

            // 弱点で倒したかどうかで場合分け
            if (m_bDefeatWithWeak == false)
            {// 弱点でない

                // ブレイズなら、ストックとバリア分のスコアも加算
                if (m_type == TYPE_BOSS_BLAZE)
                {
                    CPlayer *pPlayer = NULL;
                    int nPlayerStock = CGame::GetStock();
                    int nPlayerBarrier = 0;

                    // プレイヤーが生存しているなら、バリアの情報を取得
                    if (CGame::GetDispPlayer() == true)
                    {
                        // プレイヤー情報を取得
                        pPlayer = CGame::GetPlayer();
                        nPlayerBarrier = pPlayer->GetBarrier();
                    }

                    // ボーナススコア
                    nScore += nPlayerStock * ENEMY_BOSS_BLAZE_SCORE_ADD_STOCK_RATE + nPlayerBarrier * ENEMY_BOSS_BLAZE_SCORE_ADD_BARRIER_RATE;
                }

                // スコア表示色を白にする
                col = DEFAULT_COLOR;
            }
            else
            {// 弱点なら

                // スコア倍
                nScore = nScore * 2;

                // ブレイズなら、ストックとバリア分のスコアも加算
                if (m_type == TYPE_BOSS_BLAZE)
                {
                    CPlayer *pPlayer = NULL;
                    int nPlayerStock = CGame::GetStock();
                    int nPlayerBarrier = 0;

                    // プレイヤーが生存しているなら、バリアの情報を取得
                    if (CGame::GetDispPlayer() == true)
                    {
                        // プレイヤー情報を取得
                        pPlayer = CGame::GetPlayer();
                        nPlayerBarrier = pPlayer->GetBarrier();
                    }

                    // ボーナススコア
                    nScore += nPlayerStock * ENEMY_BOSS_BLAZE_SCORE_ADD_STOCK_RATE + nPlayerBarrier * ENEMY_BOSS_BLAZE_SCORE_ADD_BARRIER_RATE;
                }

                // スコア表示色を赤にする
                col = SCORE_DEFEAT_WITH_WEAK_COLOR;
            }
        }

        // スコアを取得
        CScore *pScore = CGame::GetScore();

        // スコア加算
        pScore->AddScore(nScore);

        // プレイヤーが存在しているなら、プレイヤーの位置を取得
        D3DXVECTOR3 playerPos = PLAYER_POS + SCORE_DISP_UP;
        if (CGame::GetDispPlayer() == true)
        {
            playerPos = CGame::GetPlayer()->GetPosition() + SCORE_DISP_UP;
        }

        // ザコ敵はプレイヤーの場所、ボスはボスの位置にスコア表示
        switch (m_type)
        {
        case TYPE_BOSS_FOREST:

            // ボスの場所に、スコア表示
            pScore->SetScore(nScore, pos, col);

            break;

        case TYPE_BOSS_DRAGON:

            // ボスの場所に、スコア表示
            pScore->SetScore(nScore, pos, col);

            break;

        case TYPE_BOSS_BLAZE:

            // ボスの場所に、スコア表示
            pScore->SetScore(nScore, pos, col);

            break;

        default:

            // プレイヤーの場所に、スコア表示
            pScore->SetScore(nScore, playerPos, col);

            break;
        }

        // 未使用に
        bUse = false;
    }

    return bUse;
}

//====================================================
// エネミーの反撃処理
//====================================================
void CEnemy::CounterAttack(void)
{
    // 変数宣言
    D3DXVECTOR3 pos = GetPosition();            // 位置を取得
    float fSpeed = 0.0f;                        // 反撃弾の速度
    D3DXVECTOR3 sizeBullet = DEFAULT_VECTOR;    // 反撃弾の大きさ

    // 反撃のクールタイムが0なら
    if (m_nCounterAttackCoolTime <= 0)
    {
        // 反撃のクールタイムを設定
        m_nCounterAttackCoolTime = ENEMY_COUNTERATTACK_COOL_TIME;

        switch (m_element)
        {
        case ELEMENT_FIRE:

            fSpeed = BULLET_COUNTERATTACK_FIRE_SPEED;       // 炎の反撃弾の速度
            sizeBullet = BULLET_COUNTERATTACK_FIRE_SIZE;    // 炎の反撃弾の大きさ

            // 八方向の弾の生成
            CBullet::Create(pos, sizeBullet, D3DXVECTOR3(fSpeed, 0.0f, 0.0f),
                CBullet::TYPE_FIRE, TRIBE_ENEMY);

            CBullet::Create(pos, sizeBullet, D3DXVECTOR3(-fSpeed, 0.0f, 0.0f),
                CBullet::TYPE_FIRE, TRIBE_ENEMY);

            CBullet::Create(pos, sizeBullet, D3DXVECTOR3(0.0f, fSpeed, 0.0f),
                CBullet::TYPE_FIRE, TRIBE_ENEMY);

            CBullet::Create(pos, sizeBullet, D3DXVECTOR3(0.0f, -fSpeed, 0.0f),
                CBullet::TYPE_FIRE, TRIBE_ENEMY);

            CBullet::Create(pos, sizeBullet, D3DXVECTOR3(sinf(D3DX_PI / 4) * fSpeed, -cosf(D3DX_PI / 4) * fSpeed, 0.0f),
                CBullet::TYPE_FIRE, TRIBE_ENEMY);

            CBullet::Create(pos, sizeBullet, D3DXVECTOR3(sinf(D3DX_PI / 4) * fSpeed, cosf(D3DX_PI / 4) * fSpeed, 0.0f),
                CBullet::TYPE_FIRE, TRIBE_ENEMY);

            CBullet::Create(pos, sizeBullet, D3DXVECTOR3(-sinf(D3DX_PI / 4) * fSpeed, -cosf(D3DX_PI / 4) * fSpeed, 0.0f),
                CBullet::TYPE_FIRE, TRIBE_ENEMY);

            CBullet::Create(pos, sizeBullet, D3DXVECTOR3(-sinf(D3DX_PI / 4) * fSpeed, cosf(D3DX_PI / 4) * fSpeed, 0.0f),
                CBullet::TYPE_FIRE, TRIBE_ENEMY);

            break;

        case ELEMENT_WATER:

            fSpeed = BULLET_COUNTERATTACK_WATAR_SPEED;      // 水の反撃弾の速度
            sizeBullet = BULLET_COUNTERATTACK_WATAR_SIZE;   // 水の反撃弾の大きさ

            // 八方向の弾の生成
            CBullet::Create(pos, sizeBullet, D3DXVECTOR3(fSpeed, 0.0f, 0.0f),
                CBullet::TYPE_WATER, TRIBE_ENEMY);

            CBullet::Create(pos, sizeBullet, D3DXVECTOR3(-fSpeed, 0.0f, 0.0f),
                CBullet::TYPE_WATER, TRIBE_ENEMY);

            CBullet::Create(pos, sizeBullet, D3DXVECTOR3(0.0f, fSpeed, 0.0f),
                CBullet::TYPE_WATER, TRIBE_ENEMY);

            CBullet::Create(pos, sizeBullet, D3DXVECTOR3(0.0f, -fSpeed, 0.0f),
                CBullet::TYPE_WATER, TRIBE_ENEMY);

            CBullet::Create(pos, sizeBullet, D3DXVECTOR3(sinf(D3DX_PI / 4) * fSpeed, -cosf(D3DX_PI / 4) * fSpeed, 0.0f),
                CBullet::TYPE_WATER, TRIBE_ENEMY);

            CBullet::Create(pos, sizeBullet, D3DXVECTOR3(sinf(D3DX_PI / 4) * fSpeed, cosf(D3DX_PI / 4) * fSpeed, 0.0f),
                CBullet::TYPE_WATER, TRIBE_ENEMY);

            CBullet::Create(pos, sizeBullet, D3DXVECTOR3(-sinf(D3DX_PI / 4) * fSpeed, -cosf(D3DX_PI / 4) * fSpeed, 0.0f),
                CBullet::TYPE_WATER, TRIBE_ENEMY);

            CBullet::Create(pos, sizeBullet, D3DXVECTOR3(-sinf(D3DX_PI / 4) * fSpeed, cosf(D3DX_PI / 4) * fSpeed, 0.0f),
                CBullet::TYPE_WATER, TRIBE_ENEMY);

            break;

        case ELEMENT_LIGHT:

            fSpeed = BULLET_COUNTERATTACK_LIGHT_SPEED;      // 雷の反撃弾の速度
            sizeBullet = BULLET_COUNTERATTACK_LIGHT_SIZE;   // 雷の反撃弾の大きさ

            // 八方向の弾の生成
            CBullet::Create(pos, sizeBullet, D3DXVECTOR3(fSpeed, 0.0f, 0.0f),
                CBullet::TYPE_LIGHT, TRIBE_ENEMY);

            CBullet::Create(pos, sizeBullet, D3DXVECTOR3(-fSpeed, 0.0f, 0.0f),
                CBullet::TYPE_LIGHT, TRIBE_ENEMY);

            CBullet::Create(pos, sizeBullet, D3DXVECTOR3(0.0f, fSpeed, 0.0f),
                CBullet::TYPE_LIGHT, TRIBE_ENEMY);

            CBullet::Create(pos, sizeBullet, D3DXVECTOR3(0.0f, -fSpeed, 0.0f),
                CBullet::TYPE_LIGHT, TRIBE_ENEMY);

            CBullet::Create(pos, sizeBullet, D3DXVECTOR3(sinf(D3DX_PI / 4) * fSpeed, -cosf(D3DX_PI / 4) * fSpeed, 0.0f),
                CBullet::TYPE_LIGHT, TRIBE_ENEMY);

            CBullet::Create(pos, sizeBullet, D3DXVECTOR3(sinf(D3DX_PI / 4) * fSpeed, cosf(D3DX_PI / 4) * fSpeed, 0.0f),
                CBullet::TYPE_LIGHT, TRIBE_ENEMY);

            CBullet::Create(pos, sizeBullet, D3DXVECTOR3(-sinf(D3DX_PI / 4) * fSpeed, -cosf(D3DX_PI / 4) * fSpeed, 0.0f),
                CBullet::TYPE_LIGHT, TRIBE_ENEMY);

            CBullet::Create(pos, sizeBullet, D3DXVECTOR3(-sinf(D3DX_PI / 4) * fSpeed, cosf(D3DX_PI / 4) * fSpeed, 0.0f),
                CBullet::TYPE_LIGHT, TRIBE_ENEMY);

            break;
        }
    }
}

//====================================================
// エネミーから見た自機への角度
//====================================================
float CEnemy::GetAngleToPlayer(void)
{
    // 変数宣言
    D3DXVECTOR3 pos = GetPosition();           // 位置を取得
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
// 床に当たった時のバウンド
//====================================================
D3DXVECTOR3 CEnemy::Bound(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 変数宣言
    D3DXVECTOR3 posBound = pos; // バウンドした際の位置

    // 床の判定を取得
    bool bFloor = false;
    bFloor = CGame::GetFloor();

    // 床との当たり判定
    if (bFloor == true)
    {
        if (pos.y + (size.y / 2) > FLOOR_COLLISION)
        {
            // 床にめり込まない
            posBound.y = FLOOR_COLLISION - (size.y / 2);

            // バウンド回数を加算
            m_nBound++;

            // バウンドの方向を場合分け
            if (m_bMoveUp == true)
            {
                // 右へバウンド
                m_move = ENEMY_CATERPILLAR_RIGHT_BOUND;
            }
            else
            {
                // 左へバウンド
                m_move = ENEMY_CATERPILLAR_LEFT_BOUND;
            }
        }
    }

    return posBound;
}

//====================================================
// ボス森の行動
//====================================================
void CEnemy::ForestMovement(D3DXVECTOR3 playerPos)
{
    // 変数宣言
    D3DXVECTOR3 pos = GetPosition();
    int nRandNum = 0;
    float fRandX = 0.0f;
    float fRandY = 0.0f;

    // 移動が変わりだすまでのカウンタを数える
    m_nCntChangeMove++;

    // X軸がストップするまでのカウンタ
    if (m_nCntChangeMove == ENEMY_BOSS_FOREST_STOP_X)
    {
        m_move.x = 0.0f;
    }
    // Y軸がストップするまでのカウンタ
    else if (m_nCntChangeMove == ENEMY_BOSS_FOREST_STOP_Y)
    {
        m_move.y = 0.0f;
    }

    // 登場が終わってから、カウンタの間隔で撃つ
    if (m_move.x == 0.0f && m_nCntChangeMove % ENEMY_BOSS_FOREST_ATTACK_COUNTER == 0)
    {
        // プレイヤーが存在しているなら
        if (CGame::GetDispPlayer() == true)
        {
            // ランダムな値を得る
            nRandNum = rand() % 2 + 1;

            // 最初はイモムシを撃つが、一定時間でリンゴに変える
            if (m_nCntChangeMove < ENEMY_BOSS_FOREST_CHANGE_ATTACK)
            {
                switch (nRandNum)
                {
                case 1:

                    // 右に転がるイモムシ
                    CEnemy::Create(D3DXVECTOR3(playerPos.x, ENEMY_BOSS_FOREST_SET_ATTACK_POS_Y, 0.0f), ENEMY_CATERPILLAR_SIZE,
                        0, true, CEnemy::TYPE_CATERPILLAR);

                    break;

                case 2:

                    // 左に転がるイモムシ
                    CEnemy::Create(D3DXVECTOR3(playerPos.x, ENEMY_BOSS_FOREST_SET_ATTACK_POS_Y, 0.0f), ENEMY_CATERPILLAR_SIZE,
                        0, false, CEnemy::TYPE_CATERPILLAR);

                    break;
                }
            }
            else
            {
                switch (nRandNum)
                {
                case 1:

                    // 右に転がるリンゴ
                    CBlock::Create(D3DXVECTOR3(playerPos.x, ENEMY_BOSS_FOREST_SET_ATTACK_POS_Y, 0.0f),
                        BLOCK_APPLE_SIZE, true, CBlock::TYPE_APPLE);

                    break;

                case 2:

                    // 左に転がるリンゴ
                    CBlock::Create(D3DXVECTOR3(playerPos.x, ENEMY_BOSS_FOREST_SET_ATTACK_POS_Y, 0.0f),
                        BLOCK_APPLE_SIZE, false, CBlock::TYPE_APPLE);

                    break;
                }
            }
        }
    }

    // 痛がるアニメーション
    if (m_nCounterState > 0)
    {
        // テクスチャ座標を更新
        SetTexturePlace(2, 2);
    }
    // 通常
    else
    {
        // テクスチャ座標を更新
        SetTexturePlace(1, 2);
    }

    // 状態カウンタと攻撃を当てられた場所によって、葉っぱや枝をバラまく
    if (m_nCounterState % ENEMY_BOSS_FOREST_SCATTER == 0)
    {
        switch (m_nWhereHit)
        {
            // 枝
        case 0:

            // 発生位置をランダムに取得
            fRandX = float(rand() % 7500) / 100.0f - float(rand() % 7500) / 100.0f;
            fRandY = float(rand() % 15000) / 100.0f - float(rand() % 15000) / 100.0f;

            // 作る
            CEffect::Create(D3DXVECTOR3(pos.x + fRandX, pos.y + fRandY, 0.0f), EFFECT_BRANCH_SIZE, DEFAULT_COLOR, CEffect::TYPE_BRANCH);

            break;

            // 葉っぱ
        case 1:

            // 発生位置をランダムに取得
            fRandX = float(rand() % 15000) / 100.0f - float(rand() % 15000) / 100.0f;
            fRandY = float(rand() % 7500) / 100.0f - float(rand() % 7500) / 100.0f;

            // 作る
            CEffect::Create(D3DXVECTOR3(pos.x + fRandX, pos.y + fRandY + ENEMY_BOSS_FOREST_SCATTER_LEAF_POS_Y, 0.0f),
                EFFECT_LEAF_SIZE, DEFAULT_COLOR, CEffect::TYPE_LEAF);

            break;
        }
    }
}

//====================================================
// トーピードの爆発
//====================================================
bool CEnemy::TorpedoExplosion(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 変数宣言
    bool bUse = true;   // 使用するかどうか
    float fSpeed = ENEMY_TORPEDO_BULLET_SPEED;  // 爆発弾の速度

    // 床の判定を取得
    bool bFloor = false;
    bFloor = CGame::GetFloor();

    // 床との当たり判定
    if (bFloor == true)
    {
        // 床に当たったら
        if (pos.y + (size.y / 2) > FLOOR_COLLISION)
        {
            // 衝撃波エフェクト
            CEffect::Create(pos + BULLET_HIT_FLOOR_EFFECT_ADD_POS - m_move, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
                BULLET_LIGHT_COLOR, CEffect::TYPE_WAVE);

            // 爆発エフェクト
            CEffect::Create(pos + BULLET_HIT_FLOOR_EFFECT_ADD_POS - m_move, EXPLOSION_SIZE,
                DEFAULT_COLOR, CEffect::TYPE_EXPLOSION);

            // 八方向の弾の生成
            CBullet::Create(pos, ENEMY_TORPEDO_BULLET_SIZE, D3DXVECTOR3(fSpeed, 0.0f, 0.0f),
                CBullet::TYPE_LIGHT, TRIBE_ENEMY);

            CBullet::Create(pos, ENEMY_TORPEDO_BULLET_SIZE, D3DXVECTOR3(-fSpeed, 0.0f, 0.0f),
                CBullet::TYPE_LIGHT, TRIBE_ENEMY);

            CBullet::Create(pos, ENEMY_TORPEDO_BULLET_SIZE, D3DXVECTOR3(0.0f, fSpeed, 0.0f),
                CBullet::TYPE_LIGHT, TRIBE_ENEMY);

            CBullet::Create(pos, ENEMY_TORPEDO_BULLET_SIZE, D3DXVECTOR3(0.0f, -fSpeed, 0.0f),
                CBullet::TYPE_LIGHT, TRIBE_ENEMY);

            CBullet::Create(pos, ENEMY_TORPEDO_BULLET_SIZE, D3DXVECTOR3(sinf(D3DX_PI / 4) * fSpeed, -cosf(D3DX_PI / 4) * fSpeed, 0.0f),
                CBullet::TYPE_LIGHT, TRIBE_ENEMY);

            CBullet::Create(pos, ENEMY_TORPEDO_BULLET_SIZE, D3DXVECTOR3(sinf(D3DX_PI / 4) * fSpeed, cosf(D3DX_PI / 4) * fSpeed, 0.0f),
                CBullet::TYPE_LIGHT, TRIBE_ENEMY);

            CBullet::Create(pos, ENEMY_TORPEDO_BULLET_SIZE, D3DXVECTOR3(-sinf(D3DX_PI / 4) * fSpeed, -cosf(D3DX_PI / 4) * fSpeed, 0.0f),
                CBullet::TYPE_LIGHT, TRIBE_ENEMY);

            CBullet::Create(pos, ENEMY_TORPEDO_BULLET_SIZE, D3DXVECTOR3(-sinf(D3DX_PI / 4) * fSpeed, cosf(D3DX_PI / 4) * fSpeed, 0.0f),
                CBullet::TYPE_LIGHT, TRIBE_ENEMY);

            // 未使用に
            bUse = false;
        }
    }

    return bUse;
}

//====================================================
// ドラゴンの行動
//====================================================
D3DXVECTOR3 CEnemy::DragonMovement(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 変数宣言
    D3DXVECTOR3 movePos = pos;      // 移動の位置
    float fAngle = 0.0f;            // 角度
    float fRadius = 0.0f;           // 半径

    // プレイヤー情報の変数宣言
    CPlayer *pPlayer = NULL;                   // プレイヤーを取得
    D3DXVECTOR3 playerPos = PLAYER_POS;        // プレイヤーの位置
    int nRandNum = 0;                          // ランダムな数

    // プレイヤーが存在しているなら
    if (CGame::GetDispPlayer() == true)
    {
        // プレイヤー情報を取得
        pPlayer = CGame::GetPlayer();
        playerPos = pPlayer->GetPosition();
    }

    // 右から流れてきた際に、止まるX座標
    if (pos.x <= ENEMY_BOSS_DRAGON_STOP_POS_X)
    {
        // 移動量をなくす
        m_move.x = 0.0f;

        // 攻撃のクールタイム
        if (m_nCntChangeMove > 0)
        {
            m_nCntChangeMove--;
        }

        // チャージ時間のカウント
        m_fDestAngle += 1.0f;

        // チャージ時間で、チャージのフラグをtrueに
        if (m_fDestAngle == DRAGON_CHARGE_TIME_1)
        {
            m_bMoveUp = true;
        }
        else if (m_fDestAngle == DRAGON_CHARGE_TIME_2)
        {
            m_bMoveUp = true;
        }
        else if (m_fDestAngle == DRAGON_CHARGE_TIME_3)
        {
            m_bMoveUp = true;
        }
        else if (m_fDestAngle == DRAGON_CHARGE_TIME_4)
        {
            m_bMoveUp = true;
        }
        else if (m_fDestAngle >= DRAGON_CHARGE_STOP)
        {
            // 一定で、チャージ時間のカウントを止める
            m_fDestAngle = DRAGON_CHARGE_STOP;
        }
    }

    // ボスの攻撃の状態によって、動きを場合分け
    switch (m_nBound)
    {
    case DRAGON_ATTACK_NONE:

        // 攻撃
        if (m_nCntChangeMove <= 0)
        {
            // アニメーションのカウンタをリセット
            ResetCountAnim();

            // 通常攻撃
            if (m_bMoveUp == false)
            {
                // 自身が水属性なら
                if (m_element == ELEMENT_WATER)
                {
                    // 攻撃のため、ランダムな値を得る
                    nRandNum = rand() % 4 + 1;

                    switch (nRandNum)
                    {
                    case 1:

                        // マインスイープ
                        m_nBound = DRAGON_ATTACK_MINE_SWEEP;

                        // 攻撃の全体フレーム
                        m_nCntChangeMove = MINE_SWEEP_COUNTER;

                        break;

                    case 2:

                        // レイニーバレット
                        m_nBound = DRAGON_ATTACK_RAINY_BULLET;

                        // 攻撃の全体フレーム
                        m_nCntChangeMove = RAINY_BULLET_COUNTER;

                        break;

                    case 3:

                        // ホーミングショット
                        m_nBound = DRAGON_ATTACK_HOMING_SHOT;

                        // 攻撃の全体フレーム
                        m_nCntChangeMove = HOMING_SHOT_COUNTER;

                        break;

                    case 4:

                        // 岩石落とし
                        m_nBound = DRAGON_ATTACK_ROCK_SLIDE;

                        // 攻撃の全体フレーム
                        m_nCntChangeMove = ROCK_SLIDE_COUNTER;

                        break;
                    }
                }
                // 雷に変化しているなら、マインスイープを使わない
                else if (m_element == ELEMENT_LIGHT)
                {
                    // 攻撃のため、ランダムな値を得る
                    nRandNum = rand() % 3 + 1;

                    switch (nRandNum)
                    {
                    case 1:

                        // レイニーバレット
                        m_nBound = DRAGON_ATTACK_RAINY_BULLET;

                        // 攻撃の全体フレーム
                        m_nCntChangeMove = RAINY_BULLET_COUNTER;

                        break;

                    case 2:

                        // ホーミングショット
                        m_nBound = DRAGON_ATTACK_HOMING_SHOT;

                        // 攻撃の全体フレーム
                        m_nCntChangeMove = HOMING_SHOT_COUNTER;

                        break;

                    case 3:

                        // 岩石落とし
                        m_nBound = DRAGON_ATTACK_ROCK_SLIDE;

                        // 攻撃の全体フレーム
                        m_nCntChangeMove = ROCK_SLIDE_COUNTER;

                        break;
                    }
                }              
            }
            // チャージするフラグがtrueなら
            else
            {
                // チャージ
                m_nBound = DRAGON_ATTACK_CHARGE;

                // 攻撃の全体フレーム
                m_nCntChangeMove = DRAGON_CHARGE_COUNTER;
            }            
        }

        // テクスチャ座標を更新
        SetParagraphAnimation(1, 2, 20, 4);

        break;

    case DRAGON_ATTACK_MINE_SWEEP:

        // クリエイトフレーム
        if (m_nCntChangeMove == MINE_SWEEP_CREATE)
        {
            // 右から降らすか、左から降らすか
            nRandNum = rand() % 2 + 1;

            switch (nRandNum)
            {
            case 1:

                // トーピード
                CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH - 325.0f, -100.0f, 0.0f), ENEMY_TORPEDO_SIZE,
                    DONT_CHANGE_MOVE, false, CEnemy::TYPE_TORPEDO);

                // トーピード
                CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH - 475.0f, -200.0f, 0.0f), ENEMY_TORPEDO_SIZE,
                    DONT_CHANGE_MOVE, false, CEnemy::TYPE_TORPEDO);

                // トーピード
                CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH - 625.0f, -300.0f, 0.0f), ENEMY_TORPEDO_SIZE,
                    DONT_CHANGE_MOVE, false, CEnemy::TYPE_TORPEDO);

                // トーピード
                CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH - 775.0f, -400.0f, 0.0f), ENEMY_TORPEDO_SIZE,
                    DONT_CHANGE_MOVE, false, CEnemy::TYPE_TORPEDO);

                // トーピード
                CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH - 925.0f, -500.0f, 0.0f), ENEMY_TORPEDO_SIZE,
                    DONT_CHANGE_MOVE, false, CEnemy::TYPE_TORPEDO);

                // トーピード
                CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH - 1075.0f, -600.0f, 0.0f), ENEMY_TORPEDO_SIZE,
                    DONT_CHANGE_MOVE, false, CEnemy::TYPE_TORPEDO);

                break;

            case 2:

                // トーピード
                CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH - 475.0f, -600.0f, 0.0f), ENEMY_TORPEDO_SIZE,
                    DONT_CHANGE_MOVE, false, CEnemy::TYPE_TORPEDO);

                // トーピード
                CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH - 625.0f, -500.0f, 0.0f), ENEMY_TORPEDO_SIZE,
                    DONT_CHANGE_MOVE, false, CEnemy::TYPE_TORPEDO);

                // トーピード
                CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH - 775.0f, -400.0f, 0.0f), ENEMY_TORPEDO_SIZE,
                    DONT_CHANGE_MOVE, false, CEnemy::TYPE_TORPEDO);

                // トーピード
                CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH - 925.0f, -300.0f, 0.0f), ENEMY_TORPEDO_SIZE,
                    DONT_CHANGE_MOVE, false, CEnemy::TYPE_TORPEDO);

                // トーピード
                CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH - 1075.0f, -200.0f, 0.0f), ENEMY_TORPEDO_SIZE,
                    DONT_CHANGE_MOVE, false, CEnemy::TYPE_TORPEDO);

                // トーピード
                CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH - 1225.0f, -100.0f, 0.0f), ENEMY_TORPEDO_SIZE,
                    DONT_CHANGE_MOVE, false, CEnemy::TYPE_TORPEDO);

                break;
            }
        }

        // 隙を設定
        DragonSetChance(MINE_SWEEP_CHANCE);

        // テクスチャ座標を更新
        SetParagraphTexturePlace(3, 2, 2, 4);

        break;

    case DRAGON_ATTACK_RAINY_BULLET:

        // クリエイトフレーム
        if (m_nCntChangeMove == RAINY_BULLET_CREATE)
        {
            // 自身が水属性なら
            if (m_element == ELEMENT_WATER)
            {
                CBullet::Create(D3DXVECTOR3(40.0f, RAINY_BULLET_SET_POS_Y, 0.0f), RAINY_BULLET_SIZE, DEFAULT_VECTOR, CBullet::TYPE_RAINY, CScene2D::TRIBE_ENEMY);
                CBullet::Create(D3DXVECTOR3(190.0f, RAINY_BULLET_SET_POS_Y, 0.0f), RAINY_BULLET_SIZE, DEFAULT_VECTOR, CBullet::TYPE_RAINY, CScene2D::TRIBE_ENEMY);
                CBullet::Create(D3DXVECTOR3(340.0f, RAINY_BULLET_SET_POS_Y, 0.0f), RAINY_BULLET_SIZE, DEFAULT_VECTOR, CBullet::TYPE_RAINY, CScene2D::TRIBE_ENEMY);
                CBullet::Create(D3DXVECTOR3(490.0f, RAINY_BULLET_SET_POS_Y, 0.0f), RAINY_BULLET_SIZE, DEFAULT_VECTOR, CBullet::TYPE_RAINY, CScene2D::TRIBE_ENEMY);
                CBullet::Create(D3DXVECTOR3(640.0f, RAINY_BULLET_SET_POS_Y, 0.0f), RAINY_BULLET_SIZE, DEFAULT_VECTOR, CBullet::TYPE_RAINY, CScene2D::TRIBE_ENEMY);
                CBullet::Create(D3DXVECTOR3(790.0f, RAINY_BULLET_SET_POS_Y, 0.0f), RAINY_BULLET_SIZE, DEFAULT_VECTOR, CBullet::TYPE_RAINY, CScene2D::TRIBE_ENEMY);
                CBullet::Create(D3DXVECTOR3(940.0f, RAINY_BULLET_SET_POS_Y, 0.0f), RAINY_BULLET_SIZE, DEFAULT_VECTOR, CBullet::TYPE_RAINY, CScene2D::TRIBE_ENEMY);
                CBullet::Create(D3DXVECTOR3(1090.0f, RAINY_BULLET_SET_POS_Y, 0.0f), RAINY_BULLET_SIZE, DEFAULT_VECTOR, CBullet::TYPE_RAINY, CScene2D::TRIBE_ENEMY);
                CBullet::Create(D3DXVECTOR3(1240.0f, RAINY_BULLET_SET_POS_Y, 0.0f), RAINY_BULLET_SIZE, DEFAULT_VECTOR, CBullet::TYPE_RAINY, CScene2D::TRIBE_ENEMY);
            }
            // 雷に変化しているなら
            else if (m_element == ELEMENT_LIGHT)
            {
                CBullet::Create(D3DXVECTOR3(40.0f, RAINY_BULLET_SET_POS_Y, 0.0f), RAINY_BULLET_LIGHT_SIZE, CHANGE_LIGHT, CBullet::TYPE_RAINY, CScene2D::TRIBE_ENEMY);
                CBullet::Create(D3DXVECTOR3(190.0f, RAINY_BULLET_SET_POS_Y, 0.0f), RAINY_BULLET_LIGHT_SIZE, CHANGE_LIGHT, CBullet::TYPE_RAINY, CScene2D::TRIBE_ENEMY);
                CBullet::Create(D3DXVECTOR3(340.0f, RAINY_BULLET_SET_POS_Y, 0.0f), RAINY_BULLET_LIGHT_SIZE, CHANGE_LIGHT, CBullet::TYPE_RAINY, CScene2D::TRIBE_ENEMY);
                CBullet::Create(D3DXVECTOR3(490.0f, RAINY_BULLET_SET_POS_Y, 0.0f), RAINY_BULLET_LIGHT_SIZE, CHANGE_LIGHT, CBullet::TYPE_RAINY, CScene2D::TRIBE_ENEMY);
                CBullet::Create(D3DXVECTOR3(640.0f, RAINY_BULLET_SET_POS_Y, 0.0f), RAINY_BULLET_LIGHT_SIZE, CHANGE_LIGHT, CBullet::TYPE_RAINY, CScene2D::TRIBE_ENEMY);
                CBullet::Create(D3DXVECTOR3(790.0f, RAINY_BULLET_SET_POS_Y, 0.0f), RAINY_BULLET_LIGHT_SIZE, CHANGE_LIGHT, CBullet::TYPE_RAINY, CScene2D::TRIBE_ENEMY);
                CBullet::Create(D3DXVECTOR3(940.0f, RAINY_BULLET_SET_POS_Y, 0.0f), RAINY_BULLET_LIGHT_SIZE, CHANGE_LIGHT, CBullet::TYPE_RAINY, CScene2D::TRIBE_ENEMY);
                CBullet::Create(D3DXVECTOR3(1090.0f, RAINY_BULLET_SET_POS_Y, 0.0f), RAINY_BULLET_LIGHT_SIZE, CHANGE_LIGHT, CBullet::TYPE_RAINY, CScene2D::TRIBE_ENEMY);
                CBullet::Create(D3DXVECTOR3(1240.0f, RAINY_BULLET_SET_POS_Y, 0.0f), RAINY_BULLET_LIGHT_SIZE, CHANGE_LIGHT, CBullet::TYPE_RAINY, CScene2D::TRIBE_ENEMY);
            }
        }

        // 隙を設定
        DragonSetChance(RAINY_BULLET_CHANCE);

        // テクスチャ座標を更新
        SetParagraphTexturePlace(3, 2, 2, 4);

        break;

    case DRAGON_ATTACK_HOMING_SHOT:

        // クリエイトフレーム
        if (m_nCntChangeMove == HOMING_SHOT_CREATE)
        {
            // 自身が水属性なら
            if (m_element == ELEMENT_WATER)
            {
                CBullet::Create(D3DXVECTOR3(pos.x - (size.x / 2) + HOMING_SHOT_SHIFT_POS, pos.y, 0.0f),
                    HOMING_SHOT_SIZE, DEFAULT_VECTOR, CBullet::TYPE_WATAR_HOMING, CScene2D::TRIBE_ENEMY);
            }
            // 雷に変化しているなら
            else if (m_element == ELEMENT_LIGHT)
            {
                CBullet::Create(D3DXVECTOR3(pos.x - (size.x / 2) + HOMING_SHOT_SHIFT_POS, pos.y, 0.0f),
                    HOMING_SHOT_LIGHT_SIZE, CHANGE_LIGHT, CBullet::TYPE_WATAR_HOMING, CScene2D::TRIBE_ENEMY);
            }
        }

        // 隙を設定
        DragonSetChance(HOMING_SHOT_CHANCE);

        // テクスチャ座標を更新
        SetParagraphAnimation(2, 2, 30, 4);

        break;

    case DRAGON_ATTACK_ROCK_SLIDE:

        // クリエイトフレーム
        if (m_nCntChangeMove == ROCK_SLIDE_CREATE)
        {
            // 岩
            CBlock::Create(D3DXVECTOR3(playerPos.x, ROCK_SLIDE_SET_POS_Y, 0.0f),
                BLOCK_ROCK_SIZE, true, CBlock::TYPE_ROCK);
        }

        // 隙を設定
        DragonSetChance(ROCK_SLIDE_CHANCE);

        // テクスチャ座標を更新
        SetParagraphTexturePlace(3, 2, 2, 4);

        break;

    case DRAGON_ATTACK_CHARGE:

        //0～3.14を出す*(0 or 1)*-1で正負を決める
        fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
        fRadius = float(rand() % 40000 + 0) / 100.0f;
        D3DXVECTOR3 particlePos = D3DXVECTOR3(pos.x - sinf(fAngle)*fRadius, pos.y - cosf(fAngle)*fRadius, 0.0f);

        // チャージのパーティクル
        CEffect::Create(particlePos, DRAGON_CHARGE_SIZE, DEFAULT_COLOR, CEffect::TYPE_REVERSE_CHARGE);

        // クリエイトフレーム
        if (m_nCntChangeMove == DRAGON_CHARGE_CREATE)
        {
            // チャージ段階によって、次のチャージへ
            if (m_fMinAngle == DRAGON_CHARGE_STAGE_3)
            {
                // フルチャージ
                m_fMinAngle = DRAGON_CHARGE_FULL;

                // 100％
                CEffect::Create(D3DXVECTOR3(pos.x, pos.y - (size.y / 2), 0.0f),
                    PERCENTAGE_SIZE, PERCENTAGE_COLOR, CEffect::TYPE_100);

                // HP回復
                m_nLife += DRAGON_CHARGE_HEAL4;

                // 電気タイプになる
                m_element = ELEMENT_LIGHT;

                // 反転合成のオーラ
                CReverse::Create(pos, DEFAULT_VECTOR);

                // プレイヤーをショック状態に
                CGame::SetShockPlayer(true);
            }
            else if (m_fMinAngle == DRAGON_CHARGE_STAGE_2)
            {
                m_fMinAngle = DRAGON_CHARGE_STAGE_3;

                // 75％
                CEffect::Create(D3DXVECTOR3(pos.x, pos.y - (size.y / 2), 0.0f),
                    PERCENTAGE_SIZE, PERCENTAGE_COLOR, CEffect::TYPE_75);

                // HP回復
                m_nLife += DRAGON_CHARGE_HEAL3;
            }
            else if (m_fMinAngle == DRAGON_CHARGE_STAGE_1)
            {
                m_fMinAngle = DRAGON_CHARGE_STAGE_2;

                // 50％
                CEffect::Create(D3DXVECTOR3(pos.x, pos.y - (size.y / 2), 0.0f),
                    PERCENTAGE_SIZE, PERCENTAGE_COLOR, CEffect::TYPE_50);

                // HP回復
                m_nLife += DRAGON_CHARGE_HEAL2;
            }
            else if (m_fMinAngle == 0.0f)
            {
                m_fMinAngle = DRAGON_CHARGE_STAGE_1;

                // 25％
                CEffect::Create(D3DXVECTOR3(pos.x, pos.y - (size.y / 2), 0.0f),
                    PERCENTAGE_SIZE, PERCENTAGE_COLOR, CEffect::TYPE_25);

                // HP回復
                m_nLife += DRAGON_CHARGE_HEAL1;
            }

            // チャージのフラグをfalseに
            m_bMoveUp = false;
        }

        // 隙を設定
        DragonSetChance(DRAGON_CHARGE_CHANCE);

        // テクスチャ座標を更新
        SetParagraphTexturePlace(3, 2, 2, 4);

        break;
    }

    // HP回復をした際の、体力上限
    if (m_nLife >= ENEMY_BOSS_DRAGON_LIFE)
    {
        m_nLife = ENEMY_BOSS_DRAGON_LIFE;
    }

    // フルチャージで、オーラ発生
    if (m_fMinAngle == DRAGON_CHARGE_FULL)
    {
        DragonSetAura(movePos);
    }

    return movePos;
}

//====================================================
// ドラゴンの隙を設定
//====================================================
void CEnemy::DragonSetChance(int nChanceFrame)
{
    // 攻撃の終わり
    if (m_nCntChangeMove == 0)
    {
        // 待機に
        m_nBound = DRAGON_ATTACK_NONE;

        // 待機（隙）時間を設定
        m_nCntChangeMove = nChanceFrame;
    }
}

//====================================================
// ドラゴンのオーラを設定
//====================================================
void CEnemy::DragonSetAura(D3DXVECTOR3 pos)
{
    // 変数宣言
    float fAngle = 0.0f;            // 角度
    float fRadius = 0.0f;           // 半径

    // ランダムな角度と半径を取得
    fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
    fRadius = float(rand() % 22500) / 100.0f;

    // オーラエフェクト
    CEffect::Create(D3DXVECTOR3(pos.x - sinf(fAngle)*fRadius, pos.y - cosf(fAngle)*fRadius + ENEMY_BOSS_DRAGON_AURA_SET_POS_Y, 0.0f),
        DRAGON_CHARGE_SIZE, ENEMY_BOSS_DRAGON_AURA_COLOR, CEffect::TYPE_DRAGON_AURA);
}

//====================================================
// マルチバードの行動
//====================================================
void CEnemy::MultiBird(D3DXVECTOR3 pos)
{
    // プレイヤー情報の変数宣言
    CPlayer *pPlayer = NULL;                   // プレイヤーを取得
    D3DXVECTOR3 playerPos = PLAYER_POS;        // プレイヤーの位置
    int nRandNum = 0;                          // ランダムな数

    // プレイヤーが存在しているなら
    if (CGame::GetDispPlayer() == true)
    {
        // プレイヤー情報を取得
        pPlayer = CGame::GetPlayer();
        playerPos = pPlayer->GetPosition();
    }

    // 移動が変わりだすまでのカウンタを数える
    m_nCntChangeMove++;

    // 移動を変えるまでのカウンタ
    if (m_nCntChangeMove >= ENEMY_MULTI_BIRD_CHANGE_MOVE && m_nCntChangeMove < ENEMY_MULTI_BIRD_LEAVE)
    {
        m_move.y = 0.0f;

        // 一定フレームで横移動を折り返す
        if (m_nCntChangeMove % ENEMY_MULTI_BIRD_RETURN_X == 0)
        {
            m_move.x *= -1;
        }
    }
    // 去るまでのカウンタ
    else if (m_nCntChangeMove >= ENEMY_MULTI_BIRD_LEAVE)
    {
        m_move.x = ENEMY_MULTI_BIRD_SPEED;
        m_move.y = -ENEMY_MULTI_BIRD_SPEED;
    }

    // カウンタの間隔で撃つ
    if (m_nCntChangeMove % ENEMY_MULTI_BIRD_ATTACK_COUNTER == 0)
    {
        // プレイヤーが存在しているなら
        if (CGame::GetDispPlayer() == true)
        {
            // 自機の位置を取得し、その位置へ撃つ
            float fAngle = GetAngleToPlayer();

            // 攻撃のため、ランダムな値を得る
            nRandNum = rand() % 3 + 1;

            // 三色の鳥の攻撃をベースに、弾数やサイズを強化
            switch (nRandNum)
            {
            case 1:

                // 弾の生成
                CBullet::Create(pos, ENEMY_MULTI_BIRD_BULLET_SIZE,
                    D3DXVECTOR3(-sinf(fAngle)*ENEMY_MULTI_BIRD_BULLET_GRASS_SPEED, 
                        -cosf(fAngle)*ENEMY_MULTI_BIRD_BULLET_GRASS_SPEED, 0.0f),
                    CBullet::TYPE_GRASS, TRIBE_ENEMY);

                // 弾の生成
                CBullet::Create(pos, ENEMY_MULTI_BIRD_BULLET_SIZE,
                    D3DXVECTOR3(-sinf(fAngle + (D3DX_PI / 6))*ENEMY_MULTI_BIRD_BULLET_GRASS_SPEED, 
                        -cosf(fAngle + (D3DX_PI / 6))*ENEMY_MULTI_BIRD_BULLET_GRASS_SPEED, 0.0f),
                    CBullet::TYPE_GRASS, TRIBE_ENEMY);

                // 弾の生成
                CBullet::Create(pos, ENEMY_MULTI_BIRD_BULLET_SIZE,
                    D3DXVECTOR3(-sinf(fAngle - (D3DX_PI / 6))*ENEMY_MULTI_BIRD_BULLET_GRASS_SPEED,
                        -cosf(fAngle - (D3DX_PI / 6))*ENEMY_MULTI_BIRD_BULLET_GRASS_SPEED, 0.0f),
                    CBullet::TYPE_GRASS, TRIBE_ENEMY);

                break;

            case 2:

                // 弾の生成
                CBullet::Create(pos, ENEMY_MULTI_BIRD_BULLET_SIZE,
                    D3DXVECTOR3(-sinf(fAngle + (D3DX_PI / 16))*ENEMY_MULTI_BIRD_BULLET_WATER_SPEED, 
                        -cosf(fAngle + (D3DX_PI / 16))*ENEMY_MULTI_BIRD_BULLET_WATER_SPEED, 0.0f),
                    CBullet::TYPE_WATER, TRIBE_ENEMY);

                // 弾の生成
                CBullet::Create(pos, ENEMY_MULTI_BIRD_BULLET_SIZE,
                    D3DXVECTOR3(-sinf(fAngle - (D3DX_PI / 16))*ENEMY_MULTI_BIRD_BULLET_WATER_SPEED,
                        -cosf(fAngle - (D3DX_PI / 16))*ENEMY_MULTI_BIRD_BULLET_WATER_SPEED, 0.0f),
                    CBullet::TYPE_WATER, TRIBE_ENEMY);

                break;

            case 3:

                // 弾の生成
                CBullet::Create(pos, ENEMY_RED_BIRD_BULLET_SIZE,
                    D3DXVECTOR3(-sinf(fAngle)*ENEMY_RED_BIRD_BULLET_SPEED, -cosf(fAngle)*ENEMY_RED_BIRD_BULLET_SPEED, 0.0f),
                    CBullet::TYPE_FIRE_BURST, TRIBE_ENEMY);

                // 弾の生成
                CBullet::Create(pos, ENEMY_MULTI_BIRD_BULLET_SIZE,
                    D3DXVECTOR3(-sinf(fAngle + (D3DX_PI / 8))*ENEMY_MULTI_BIRD_BULLET_FIRE_SPEED, 
                        -cosf(fAngle + (D3DX_PI / 8))*ENEMY_MULTI_BIRD_BULLET_FIRE_SPEED, 0.0f),
                    CBullet::TYPE_FIRE, TRIBE_ENEMY);

                // 弾の生成
                CBullet::Create(pos, ENEMY_MULTI_BIRD_BULLET_SIZE,
                    D3DXVECTOR3(-sinf(fAngle - (D3DX_PI / 8))*ENEMY_MULTI_BIRD_BULLET_FIRE_SPEED, 
                        -cosf(fAngle - (D3DX_PI / 8))*ENEMY_MULTI_BIRD_BULLET_FIRE_SPEED, 0.0f),
                    CBullet::TYPE_FIRE, TRIBE_ENEMY);

                // 弾の生成
                CBullet::Create(pos, ENEMY_MULTI_BIRD_BULLET_SIZE,
                    D3DXVECTOR3(-sinf(fAngle + (D3DX_PI / 16))*ENEMY_MULTI_BIRD_BULLET_FIRE_SPEED, 
                        -cosf(fAngle + (D3DX_PI / 16))*ENEMY_MULTI_BIRD_BULLET_FIRE_SPEED, 0.0f),
                    CBullet::TYPE_FIRE, TRIBE_ENEMY);

                // 弾の生成
                CBullet::Create(pos, ENEMY_MULTI_BIRD_BULLET_SIZE,
                    D3DXVECTOR3(-sinf(fAngle - (D3DX_PI / 16))*ENEMY_MULTI_BIRD_BULLET_FIRE_SPEED,
                        -cosf(fAngle - (D3DX_PI / 16))*ENEMY_MULTI_BIRD_BULLET_FIRE_SPEED, 0.0f),
                    CBullet::TYPE_FIRE, TRIBE_ENEMY);

                break;
            }
        }
    }

    // プレイヤーの位置によって、向く方向を変える
    if (playerPos.x <= pos.x)
    {
        // 左向きでテクスチャ座標を更新
        SetAnimation(6, 4);
    }
    else
    {
        // 右向きでテクスチャ座標を更新
        SetReverseAnimation(6, 4);
    }
}

//====================================================
// ブレイズの行動
//====================================================
D3DXVECTOR3 CEnemy::BlazeMovement(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 変数宣言
    D3DXVECTOR3 movePos = pos;      // 移動の位置
    float fAngle = 0.0f;            // 角度
    float fRadius = 0.0f;           // 半径

    // プレイヤー情報の変数宣言
    CPlayer *pPlayer = NULL;                   // プレイヤーを取得
    D3DXVECTOR3 playerPos = PLAYER_POS;        // プレイヤーの位置
    int nRandNum = 0;                          // ランダムな数

    // プレイヤーが存在しているなら
    if (CGame::GetDispPlayer() == true)
    {
        // プレイヤー情報を取得
        pPlayer = CGame::GetPlayer();
        playerPos = pPlayer->GetPosition();
    }

    // 右から流れてきた際に、止まるX座標
    if (pos.x <= ENEMY_BOSS_BLAZE_STOP_POS_X)
    {
        // ブレイズの待機中なら
        if (m_nBound == BLAZE_ATTACK_NONE)
        {
            // 移動量をなくす
            m_move.x = 0.0f;
        }

        // 攻撃のクールタイム
        if (m_nCntChangeMove > 0)
        {
            m_nCntChangeMove--;
        }
    }

    // 上から流れてきた際に、止まるY座標
    if (pos.y >= ENEMY_BOSS_BLAZE_STOP_POS_Y)
    {
        // ブレイズの待機中なら
        if (m_nBound == BLAZE_ATTACK_NONE)
        {
            // 移動量をなくす
            m_move.y = 0.0f;
        }
    }

    // ボスの攻撃の状態によって、動きを場合分け
    switch (m_nBound)
    {
    case BLAZE_ATTACK_NONE:

        // 攻撃
        if (m_nCntChangeMove <= 0)
        {
            // アニメーションのカウンタをリセット
            ResetCountAnim();

            // 攻撃のため、ランダムな値を得る
            nRandNum = rand() % 4 + 1;

            switch (nRandNum)
            {
            case 1:

                // バーニングバレット
                m_nBound = BLAZE_ATTACK_BURNING_BULLET;

                // 攻撃の全体フレーム
                m_nCntChangeMove = BURNING_BULLET_COUNTER;

                break;

            case 2:

                // ウィンドバレット
                m_nBound = BLAZE_ATTACK_WIND_BULLET;

                // 攻撃の全体フレーム
                m_nCntChangeMove = WIND_BULLET_COUNTER;

                break;

            case 3:

                // ゴッドバード
                m_nBound = BLAZE_ATTACK_GOD_BIRD;

                // 攻撃の全体フレーム
                m_nCntChangeMove = GOD_BIRD_COUNTER;

                break;

            case 4:

                // メテオストライク
                m_nBound = BLAZE_ATTACK_METEO_STRIKE;

                // 攻撃の全体フレーム
                m_nCntChangeMove = METEO_STRIKE_COUNTER;

                break;
            }
        }

        // テクスチャ座標を更新
        SetAnimation(14,3);

        break;

    case BLAZE_ATTACK_BURNING_BULLET:

        // 予備動作
        if (m_nCntChangeMove == BURNING_BULLET_COUNTER - 1)
        {
            // 神通力
            CAfterimage::Create(pos, size, 0.0f, DEFAULT_COLOR, CAfterimage::TYPE_MAGIC);

            // 衝撃波
            CEffect::Create(pos, EFFECT_DOUBLE_WAVE_BLAZE_SIZE,
                D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), CEffect::TYPE_DOUBLE_WAVE);
        }

        // クリエイトフレーム
        if (m_nCntChangeMove == BURNING_BULLET_CREATE)
        {
            // 属性値上昇
            m_fDestAngle += GOD_FIRE_BLAZE_ATTACK_VALUE;

            // バーニングバレット
            BurningBullet();
        }

        // 隙を設定
        BlazeSetChance(BURNING_BULLET_CHANCE);

        // テクスチャ座標を更新
        SetTexturePlace(1, 3);

        // オーラ設定
        BlazeSetAura(movePos, ELEMENT_FIRE);

        break;

    case BLAZE_ATTACK_WIND_BULLET:

        // 予備動作
        if (m_nCntChangeMove == WIND_BULLET_COUNTER - 1)
        {
            // 神通力
            CAfterimage::Create(pos, size, 0.0f, DEFAULT_COLOR, CAfterimage::TYPE_MAGIC);

            // 衝撃波
            CEffect::Create(pos, EFFECT_DOUBLE_WAVE_BLAZE_SIZE,
                D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), CEffect::TYPE_DOUBLE_WAVE);
        }

        // クリエイトフレーム
        if (m_nCntChangeMove == WIND_BULLET_CREATE)
        {
            // 属性値上昇
            m_fDestAngle += GOD_FIRE_BLAZE_ATTACK_VALUE;

            // ウィンドバレット
            WindBullet();
        }

        // 隙を設定
        BlazeSetChance(WIND_BULLET_CHANCE);

        // テクスチャ座標を更新
        SetTexturePlace(1, 3);

        // オーラ設定
        BlazeSetAura(movePos, ELEMENT_GRASS);

        break;

    case BLAZE_ATTACK_GOD_BIRD:

        // クリエイトフレーム
        if (m_nCntChangeMove <= GOD_BIRD_CREATE)
        {
            // 超加速
            m_move.x = -GOD_BIRD_ACCEL_SPEED;

            // 残像を残す
            CAfterimage::Create(pos, size, 0.0f, AFTERIMAGE_PLAYER_COLOR, CAfterimage::TYPE_GOD_BIRD);

            // テクスチャ座標を更新(加速中の姿)
            SetTexturePlace(3, 3);

            // 攻撃の終わり
            if (m_nCntChangeMove == 0)
            {
                // 所定の位置の上にワープ
                movePos = GOD_BIRD_WARP_POS;

                // 移動量を、上から降りてくる感じに変える
                m_move.x = 0.0f;
                m_move.y = GOD_BIRD_FIRST_SPEED;
            }
        }
        // 位置取得待機
        else
        {
            // プレイヤーへの角度取得
            fAngle = GetAngleToPlayer();

            // 縦移動
            m_move = D3DXVECTOR3(0.0f, -cosf(fAngle)*GOD_BIRD_FIRST_SPEED, 0.0f);

            // テクスチャ座標を更新(溜めている姿)
            SetAnimation(6, 3);
        }

        // デンジャー表記
        if (m_nCntChangeMove == GOD_BIRD_DANGER)
        {
            CUI::Create(D3DXVECTOR3(pos.x - (size.x / 4), pos.y - (size.y / 3), 0.0f), 
                GOD_BIRD_DANGER_SIZE, DEFAULT_COLOR, CUI::TYPE_DANGER);
        }

        // 隙を設定
        BlazeSetChance(GOD_BIRD_CHANCE);

        break;

    case BLAZE_ATTACK_METEO_STRIKE:

        // 予備動作
        if (m_nCntChangeMove == METEO_STRIKE_COUNTER - 1)
        {
            // 神通力
            CAfterimage::Create(pos, size, 0.0f, DEFAULT_COLOR, CAfterimage::TYPE_MAGIC);

            // 衝撃波
            CEffect::Create(pos, EFFECT_DOUBLE_WAVE_BLAZE_SIZE,
                EFFECT_DOUBLE_WAVE_BLAZE_COLOR, CEffect::TYPE_DOUBLE_WAVE);
        }

        // クリエイトフレーム
        if (m_nCntChangeMove == METEO_STRIKE_CREATE)
        {
            // メテオ
            CBlock::Create(D3DXVECTOR3(METEO_STRIKE_SET_POS_X, METEO_STRIKE_SET_POS_Y, 0.0f),
                BLOCK_ROCK_SIZE, false, CBlock::TYPE_METEO);
        }

        // 隙を設定
        BlazeSetChance(METEO_STRIKE_CHANCE);

        // テクスチャ座標を更新
        SetTexturePlace(1, 3);

        break;
    }

    // HP回復をした際の、体力上限
    if (m_nLife >= ENEMY_BOSS_BLAZE_LIFE)
    {
        m_nLife = ENEMY_BOSS_BLAZE_LIFE;
    }

    // 属性値をチェックし、一定の値以上でチャージの段階を一段階上げる
    if (m_fDestAngle >= GOD_FIRE_VALUE_FOR_ONE_CHARGE)
    {
        // 属性値をリセット
        m_fDestAngle = 0.0f;

        // 神炎モードでないなら
        if (m_bMoveUp == false)
        {
            // チャージ段階によって、次のチャージへ
            if (m_fMinAngle == GOD_FIRE_STAGE_3)
            {
                // 神炎モード
                m_bMoveUp = true;

                // フルチャージ
                m_fMinAngle = GOD_FIRE_FULL;

                // 100％
                CEffect::Create(D3DXVECTOR3(pos.x, pos.y - (size.y / 3), 0.0f),
                    PERCENTAGE_SIZE, PERCENTAGE_COLOR, CEffect::TYPE_100);
            }
            else if (m_fMinAngle == GOD_FIRE_STAGE_2)
            {
                m_fMinAngle = GOD_FIRE_STAGE_3;

                // 75％
                CEffect::Create(D3DXVECTOR3(pos.x, pos.y - (size.y / 3), 0.0f),
                    PERCENTAGE_SIZE, PERCENTAGE_COLOR, CEffect::TYPE_75);
            }
            else if (m_fMinAngle == GOD_FIRE_STAGE_1)
            {
                m_fMinAngle = GOD_FIRE_STAGE_2;

                // 50％
                CEffect::Create(D3DXVECTOR3(pos.x, pos.y - (size.y / 3), 0.0f),
                    PERCENTAGE_SIZE, PERCENTAGE_COLOR, CEffect::TYPE_50);
            }
            else if (m_fMinAngle == 0.0f)
            {
                m_fMinAngle = GOD_FIRE_STAGE_1;

                // 25％
                CEffect::Create(D3DXVECTOR3(pos.x, pos.y - (size.y / 3), 0.0f),
                    PERCENTAGE_SIZE, PERCENTAGE_COLOR, CEffect::TYPE_25);
            }
        }
    }

    // 神炎モードで、反撃終了、オーラ発生
    if (m_bMoveUp == true)
    {
        // 反撃終了
        m_nCounterAttackCoolTime = GOD_FIRE_STOP_COUNTERATTACK;

        // オーラ発生
        BlazeSetAura(movePos, ELEMENT_MULTI);
    }

    return movePos;
}

//====================================================
// ブレイズの隙を設定
//====================================================
void CEnemy::BlazeSetChance(int nChanceFrame)
{
    // 攻撃の終わり
    if (m_nCntChangeMove == 0)
    {
        // 待機に
        m_nBound = BLAZE_ATTACK_NONE;

        // 待機（隙）時間を設定
        m_nCntChangeMove = nChanceFrame;
    }
}

//====================================================
// ブレイズオーラを設定
//====================================================
void CEnemy::BlazeSetAura(D3DXVECTOR3 pos, ELEMENT element)
{
    // ブレイズのオーラセットカウンタ
    if (m_nCntChangeMove % ENEMY_BOSS_BLAZE_AURA_SET_COUNT == 0)
    {
        // 変数宣言
        float fAngle = 0.0f;            // 角度
        float fRadius = 0.0f;           // 半径

        // ランダムな角度と半径を取得
        fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
        fRadius = float(rand() % 20000) / 100.0f;

        switch (element)
        {
        case ELEMENT_FIRE:
            // オーラエフェクト(赤)
            CEffect::Create(D3DXVECTOR3(pos.x - sinf(fAngle)*fRadius, pos.y - cosf(fAngle)*fRadius + ENEMY_BOSS_BLAZE_AURA_SET_POS_Y, 0.0f),
                ENEMY_BOSS_BLAZE_AURA_SIZE, D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.75f), CEffect::TYPE_RED_FIRE);
            break;

        case ELEMENT_GRASS:
            // オーラエフェクト(緑)
            CEffect::Create(D3DXVECTOR3(pos.x - sinf(fAngle)*fRadius, pos.y - cosf(fAngle)*fRadius + ENEMY_BOSS_BLAZE_AURA_SET_POS_Y, 0.0f),
                ENEMY_BOSS_BLAZE_AURA_SIZE, D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.75f), CEffect::TYPE_YELLOW_FIRE);
            break;

        case ELEMENT_MULTI:
            // オーラエフェクト(紫)
            CEffect::Create(D3DXVECTOR3(pos.x - sinf(fAngle)*fRadius, pos.y - cosf(fAngle)*fRadius + ENEMY_BOSS_BLAZE_AURA_SET_POS_Y, 0.0f),
                GOD_FIRE_SIZE, EFFECT_PURPLE_FIRE_FIRST_COLOR_BLAZE, CEffect::TYPE_PURPLE_FIRE);
            break;
        }
    }
}

//====================================================
// バーニングバレット生成
//====================================================
void CEnemy::BurningBullet(void)
{
    // 神炎モードかどうかで、弾の属性を変える
    if (m_bMoveUp == false)
    {
        // 通常は、炎属性
        // 上
        CBullet::Create(D3DXVECTOR3(40.0f, BURNING_BULLET_SET_POS_Y, 0.0f), BURNING_BULLET_SIZE, DEFAULT_VECTOR, CBullet::TYPE_BURNING, CScene2D::TRIBE_ENEMY);
        CBullet::Create(D3DXVECTOR3(190.0f, BURNING_BULLET_SET_POS_Y, 0.0f), BURNING_BULLET_SIZE, DEFAULT_VECTOR, CBullet::TYPE_BURNING, CScene2D::TRIBE_ENEMY);
        CBullet::Create(D3DXVECTOR3(340.0f, BURNING_BULLET_SET_POS_Y, 0.0f), BURNING_BULLET_SIZE, DEFAULT_VECTOR, CBullet::TYPE_BURNING, CScene2D::TRIBE_ENEMY);
        CBullet::Create(D3DXVECTOR3(490.0f, BURNING_BULLET_SET_POS_Y, 0.0f), BURNING_BULLET_SIZE, DEFAULT_VECTOR, CBullet::TYPE_BURNING, CScene2D::TRIBE_ENEMY);
        CBullet::Create(D3DXVECTOR3(640.0f, BURNING_BULLET_SET_POS_Y, 0.0f), BURNING_BULLET_SIZE, DEFAULT_VECTOR, CBullet::TYPE_BURNING, CScene2D::TRIBE_ENEMY);
        CBullet::Create(D3DXVECTOR3(790.0f, BURNING_BULLET_SET_POS_Y, 0.0f), BURNING_BULLET_SIZE, DEFAULT_VECTOR, CBullet::TYPE_BURNING, CScene2D::TRIBE_ENEMY);
        CBullet::Create(D3DXVECTOR3(940.0f, BURNING_BULLET_SET_POS_Y, 0.0f), BURNING_BULLET_SIZE, DEFAULT_VECTOR, CBullet::TYPE_BURNING, CScene2D::TRIBE_ENEMY);
        CBullet::Create(D3DXVECTOR3(1090.0f, BURNING_BULLET_SET_POS_Y, 0.0f), BURNING_BULLET_SIZE, DEFAULT_VECTOR, CBullet::TYPE_BURNING, CScene2D::TRIBE_ENEMY);
        CBullet::Create(D3DXVECTOR3(1240.0f, BURNING_BULLET_SET_POS_Y, 0.0f), BURNING_BULLET_SIZE, DEFAULT_VECTOR, CBullet::TYPE_BURNING, CScene2D::TRIBE_ENEMY);

        // 左
        CBullet::Create(D3DXVECTOR3(BURNING_BULLET_SET_POS_LEFT, 40.0f, 0.0f), BURNING_BULLET_SIZE, DEFAULT_VECTOR, CBullet::TYPE_BURNING, CScene2D::TRIBE_ENEMY);
        CBullet::Create(D3DXVECTOR3(BURNING_BULLET_SET_POS_LEFT, 160.0f, 0.0f), BURNING_BULLET_SIZE, DEFAULT_VECTOR, CBullet::TYPE_BURNING, CScene2D::TRIBE_ENEMY);
        CBullet::Create(D3DXVECTOR3(BURNING_BULLET_SET_POS_LEFT, 280.0f, 0.0f), BURNING_BULLET_SIZE, DEFAULT_VECTOR, CBullet::TYPE_BURNING, CScene2D::TRIBE_ENEMY);
        CBullet::Create(D3DXVECTOR3(BURNING_BULLET_SET_POS_LEFT, 400.0f, 0.0f), BURNING_BULLET_SIZE, DEFAULT_VECTOR, CBullet::TYPE_BURNING, CScene2D::TRIBE_ENEMY);
        CBullet::Create(D3DXVECTOR3(BURNING_BULLET_SET_POS_LEFT, 520.0f, 0.0f), BURNING_BULLET_SIZE, DEFAULT_VECTOR, CBullet::TYPE_BURNING, CScene2D::TRIBE_ENEMY);
        CBullet::Create(D3DXVECTOR3(BURNING_BULLET_SET_POS_LEFT, 640.0f, 0.0f), BURNING_BULLET_SIZE, DEFAULT_VECTOR, CBullet::TYPE_BURNING, CScene2D::TRIBE_ENEMY);

        // 右
        CBullet::Create(D3DXVECTOR3(BURNING_BULLET_SET_POS_RIGHT, 40.0f, 0.0f), BURNING_BULLET_SIZE, DEFAULT_VECTOR, CBullet::TYPE_BURNING, CScene2D::TRIBE_ENEMY);
        CBullet::Create(D3DXVECTOR3(BURNING_BULLET_SET_POS_RIGHT, 160.0f, 0.0f), BURNING_BULLET_SIZE, DEFAULT_VECTOR, CBullet::TYPE_BURNING, CScene2D::TRIBE_ENEMY);
        CBullet::Create(D3DXVECTOR3(BURNING_BULLET_SET_POS_RIGHT, 280.0f, 0.0f), BURNING_BULLET_SIZE, DEFAULT_VECTOR, CBullet::TYPE_BURNING, CScene2D::TRIBE_ENEMY);
        CBullet::Create(D3DXVECTOR3(BURNING_BULLET_SET_POS_RIGHT, 400.0f, 0.0f), BURNING_BULLET_SIZE, DEFAULT_VECTOR, CBullet::TYPE_BURNING, CScene2D::TRIBE_ENEMY);
        CBullet::Create(D3DXVECTOR3(BURNING_BULLET_SET_POS_RIGHT, 520.0f, 0.0f), BURNING_BULLET_SIZE, DEFAULT_VECTOR, CBullet::TYPE_BURNING, CScene2D::TRIBE_ENEMY);
        CBullet::Create(D3DXVECTOR3(BURNING_BULLET_SET_POS_RIGHT, 640.0f, 0.0f), BURNING_BULLET_SIZE, DEFAULT_VECTOR, CBullet::TYPE_BURNING, CScene2D::TRIBE_ENEMY);
    }
    else
    {
        // 神炎モード中は、マルチ属性
        // 上
        CBullet::Create(D3DXVECTOR3(40.0f, BURNING_BULLET_SET_POS_Y, 0.0f), BURNING_BULLET_SIZE, CHANGE_MULTI, CBullet::TYPE_BURNING, CScene2D::TRIBE_ENEMY);
        CBullet::Create(D3DXVECTOR3(190.0f, BURNING_BULLET_SET_POS_Y, 0.0f), BURNING_BULLET_SIZE, CHANGE_MULTI, CBullet::TYPE_BURNING, CScene2D::TRIBE_ENEMY);
        CBullet::Create(D3DXVECTOR3(340.0f, BURNING_BULLET_SET_POS_Y, 0.0f), BURNING_BULLET_SIZE, CHANGE_MULTI, CBullet::TYPE_BURNING, CScene2D::TRIBE_ENEMY);
        CBullet::Create(D3DXVECTOR3(490.0f, BURNING_BULLET_SET_POS_Y, 0.0f), BURNING_BULLET_SIZE, CHANGE_MULTI, CBullet::TYPE_BURNING, CScene2D::TRIBE_ENEMY);
        CBullet::Create(D3DXVECTOR3(640.0f, BURNING_BULLET_SET_POS_Y, 0.0f), BURNING_BULLET_SIZE, CHANGE_MULTI, CBullet::TYPE_BURNING, CScene2D::TRIBE_ENEMY);
        CBullet::Create(D3DXVECTOR3(790.0f, BURNING_BULLET_SET_POS_Y, 0.0f), BURNING_BULLET_SIZE, CHANGE_MULTI, CBullet::TYPE_BURNING, CScene2D::TRIBE_ENEMY);
        CBullet::Create(D3DXVECTOR3(940.0f, BURNING_BULLET_SET_POS_Y, 0.0f), BURNING_BULLET_SIZE, CHANGE_MULTI, CBullet::TYPE_BURNING, CScene2D::TRIBE_ENEMY);
        CBullet::Create(D3DXVECTOR3(1090.0f, BURNING_BULLET_SET_POS_Y, 0.0f), BURNING_BULLET_SIZE, CHANGE_MULTI, CBullet::TYPE_BURNING, CScene2D::TRIBE_ENEMY);
        CBullet::Create(D3DXVECTOR3(1240.0f, BURNING_BULLET_SET_POS_Y, 0.0f), BURNING_BULLET_SIZE, CHANGE_MULTI, CBullet::TYPE_BURNING, CScene2D::TRIBE_ENEMY);

        // 左
        CBullet::Create(D3DXVECTOR3(BURNING_BULLET_SET_POS_LEFT, 40.0f, 0.0f), BURNING_BULLET_SIZE, CHANGE_MULTI, CBullet::TYPE_BURNING, CScene2D::TRIBE_ENEMY);
        CBullet::Create(D3DXVECTOR3(BURNING_BULLET_SET_POS_LEFT, 160.0f, 0.0f), BURNING_BULLET_SIZE, CHANGE_MULTI, CBullet::TYPE_BURNING, CScene2D::TRIBE_ENEMY);
        CBullet::Create(D3DXVECTOR3(BURNING_BULLET_SET_POS_LEFT, 280.0f, 0.0f), BURNING_BULLET_SIZE, CHANGE_MULTI, CBullet::TYPE_BURNING, CScene2D::TRIBE_ENEMY);
        CBullet::Create(D3DXVECTOR3(BURNING_BULLET_SET_POS_LEFT, 400.0f, 0.0f), BURNING_BULLET_SIZE, CHANGE_MULTI, CBullet::TYPE_BURNING, CScene2D::TRIBE_ENEMY);
        CBullet::Create(D3DXVECTOR3(BURNING_BULLET_SET_POS_LEFT, 520.0f, 0.0f), BURNING_BULLET_SIZE, CHANGE_MULTI, CBullet::TYPE_BURNING, CScene2D::TRIBE_ENEMY);
        CBullet::Create(D3DXVECTOR3(BURNING_BULLET_SET_POS_LEFT, 640.0f, 0.0f), BURNING_BULLET_SIZE, CHANGE_MULTI, CBullet::TYPE_BURNING, CScene2D::TRIBE_ENEMY);

        // 右
        CBullet::Create(D3DXVECTOR3(BURNING_BULLET_SET_POS_RIGHT, 40.0f, 0.0f), BURNING_BULLET_SIZE, CHANGE_MULTI, CBullet::TYPE_BURNING, CScene2D::TRIBE_ENEMY);
        CBullet::Create(D3DXVECTOR3(BURNING_BULLET_SET_POS_RIGHT, 160.0f, 0.0f), BURNING_BULLET_SIZE, CHANGE_MULTI, CBullet::TYPE_BURNING, CScene2D::TRIBE_ENEMY);
        CBullet::Create(D3DXVECTOR3(BURNING_BULLET_SET_POS_RIGHT, 280.0f, 0.0f), BURNING_BULLET_SIZE, CHANGE_MULTI, CBullet::TYPE_BURNING, CScene2D::TRIBE_ENEMY);
        CBullet::Create(D3DXVECTOR3(BURNING_BULLET_SET_POS_RIGHT, 400.0f, 0.0f), BURNING_BULLET_SIZE, CHANGE_MULTI, CBullet::TYPE_BURNING, CScene2D::TRIBE_ENEMY);
        CBullet::Create(D3DXVECTOR3(BURNING_BULLET_SET_POS_RIGHT, 520.0f, 0.0f), BURNING_BULLET_SIZE, CHANGE_MULTI, CBullet::TYPE_BURNING, CScene2D::TRIBE_ENEMY);
        CBullet::Create(D3DXVECTOR3(BURNING_BULLET_SET_POS_RIGHT, 640.0f, 0.0f), BURNING_BULLET_SIZE, CHANGE_MULTI, CBullet::TYPE_BURNING, CScene2D::TRIBE_ENEMY);
    }

}

//====================================================
// ウィンドバレット生成
//====================================================
void CEnemy::WindBullet(void)
{
    // 神炎モードかどうかで、弾の属性と弾の配置を変える
    if (m_bMoveUp == false)
    {
        // 通常は草属性、縦一列
        CBullet::Create(D3DXVECTOR3(WIND_BULLET_SET_POS_RIGHT, 140.0f, 0.0f), WIND_BULLET_SIZE
            , DEFAULT_VECTOR, CBullet::TYPE_WIND, CScene2D::TRIBE_ENEMY);
        CBullet::Create(D3DXVECTOR3(WIND_BULLET_SET_POS_RIGHT, 220.0f, 0.0f), WIND_BULLET_SIZE
            , DEFAULT_VECTOR, CBullet::TYPE_WIND, CScene2D::TRIBE_ENEMY);
        CBullet::Create(D3DXVECTOR3(WIND_BULLET_SET_POS_RIGHT, 300.0f, 0.0f), WIND_BULLET_SIZE
            , DEFAULT_VECTOR, CBullet::TYPE_WIND, CScene2D::TRIBE_ENEMY);
        CBullet::Create(D3DXVECTOR3(WIND_BULLET_SET_POS_RIGHT, 380.0f, 0.0f), WIND_BULLET_SIZE
            , DEFAULT_VECTOR, CBullet::TYPE_WIND, CScene2D::TRIBE_ENEMY);
        CBullet::Create(D3DXVECTOR3(WIND_BULLET_SET_POS_RIGHT, 460.0f, 0.0f), WIND_BULLET_SIZE
            , DEFAULT_VECTOR, CBullet::TYPE_WIND, CScene2D::TRIBE_ENEMY);
        CBullet::Create(D3DXVECTOR3(WIND_BULLET_SET_POS_RIGHT, 540.0f, 0.0f), WIND_BULLET_SIZE
            , DEFAULT_VECTOR, CBullet::TYPE_WIND, CScene2D::TRIBE_ENEMY);
        CBullet::Create(D3DXVECTOR3(WIND_BULLET_SET_POS_RIGHT, 620.0f, 0.0f), WIND_BULLET_SIZE
            , DEFAULT_VECTOR, CBullet::TYPE_WIND, CScene2D::TRIBE_ENEMY);
    }
    else
    {
        // 神炎モード中は、マルチ属性、穴あきの縦一列
        int nRandNum = 0;
        nRandNum = rand() % 7 + 1;  // 7つの弾のうち、どれかがプレイヤーのくぐれる穴になる

        if (nRandNum != 1)
        {
            CBullet::Create(D3DXVECTOR3(WIND_BULLET_SET_POS_RIGHT, 140.0f, 0.0f), WIND_BULLET_SIZE
                , CHANGE_MULTI, CBullet::TYPE_WIND, CScene2D::TRIBE_ENEMY);
        }

        if (nRandNum != 2)
        {
            CBullet::Create(D3DXVECTOR3(WIND_BULLET_SET_POS_RIGHT, 220.0f, 0.0f), WIND_BULLET_SIZE
                , CHANGE_MULTI, CBullet::TYPE_WIND, CScene2D::TRIBE_ENEMY);
        }

        if (nRandNum != 3)
        {
            CBullet::Create(D3DXVECTOR3(WIND_BULLET_SET_POS_RIGHT, 300.0f, 0.0f), WIND_BULLET_SIZE
                , CHANGE_MULTI, CBullet::TYPE_WIND, CScene2D::TRIBE_ENEMY);
        }

        if (nRandNum != 4)
        {
            CBullet::Create(D3DXVECTOR3(WIND_BULLET_SET_POS_RIGHT, 380.0f, 0.0f), WIND_BULLET_SIZE
                , CHANGE_MULTI, CBullet::TYPE_WIND, CScene2D::TRIBE_ENEMY);
        }

        if (nRandNum != 5)
        {
            CBullet::Create(D3DXVECTOR3(WIND_BULLET_SET_POS_RIGHT, 460.0f, 0.0f), WIND_BULLET_SIZE
                , CHANGE_MULTI, CBullet::TYPE_WIND, CScene2D::TRIBE_ENEMY);
        }

        if (nRandNum != 6)
        {
            CBullet::Create(D3DXVECTOR3(WIND_BULLET_SET_POS_RIGHT, 540.0f, 0.0f), WIND_BULLET_SIZE
                , CHANGE_MULTI, CBullet::TYPE_WIND, CScene2D::TRIBE_ENEMY);
        }

        if (nRandNum != 7)
        {
            CBullet::Create(D3DXVECTOR3(WIND_BULLET_SET_POS_RIGHT, 620.0f, 0.0f), WIND_BULLET_SIZE
                , CHANGE_MULTI, CBullet::TYPE_WIND, CScene2D::TRIBE_ENEMY);
        }
    }
}
