//===============================================
//
// 弾処理 (bullet.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "bullet.h"
#include "manager.h"
#include "renderer.h"
#include "library.h"
#include "effect.h"
#include "enemy.h"
#include "score.h"
#include "player.h"
#include "game.h"
#include "block.h"
#include "helper.h"
#include "bg.h"

//=======================================
// 静的メンバ変数宣言
//=======================================
LPDIRECT3DTEXTURE9 CBullet::m_apTexture[CBullet::TYPE_MAX] = {};

//========================================
// 弾のコンストラクタ
//========================================
CBullet::CBullet() :CScene2D(OBJTYPE::OBJTYPE_BULLET)
{
    m_posOld = DEFAULT_VECTOR;
    m_move = DEFAULT_VECTOR;
    m_fSpeed = 0.0f;
    m_nLife = 0;
    m_nDamage = 0;
    m_type = CBullet::TYPE_MAX;
    m_element = ELEMENT_MAX;
    m_tribe = TRIBE_MAX;
}

//========================================
// 弾のデストラクタ
//========================================
CBullet::~CBullet()
{

}

//========================================
// 弾の初期化処理
//========================================
HRESULT CBullet::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 初期化
    CScene2D::Init(pos, size);

    // テクスチャを割り当て
    BindTexture(m_apTexture[m_type]);

    return S_OK;
}

//========================================
// 弾の終了処理
//========================================
void CBullet::Uninit(void)
{
    // 終了処理
    CScene2D::Uninit();
}

//========================================
// 弾の更新処理
//========================================
void CBullet::Update(void)
{
    // 変数宣言
    D3DXVECTOR3 pos = GetPosition();    // 位置を取得
    D3DXVECTOR3 size = GetSize();       // サイズを取得
    bool bHit = false;                  // 弾がプレイヤーや敵に当たったかどうか
    float fAngle = 0.0f;                // 角度

    // プレイヤー情報の変数宣言
    CPlayer *pPlayer = NULL;           // プレイヤーを取得
    int  nPlayerInvincible = 0;        // プレイヤーの無敵時間

    // プレイヤーが存在しているなら
    if (CGame::GetDispPlayer() == true)
    {
        // プレイヤー情報を取得
        pPlayer = CGame::GetPlayer();
        nPlayerInvincible = pPlayer->GetInvincibleFrame();
    }

    // 1F前の位置を記憶
    m_posOld = pos;

    // 移動量を位置に加算
    pos += m_move;

    // 種類に応じた場合分け
    switch (m_type)
    {
    case CBullet::TYPE_FIRE:

        // テクスチャ座標を更新
        SetAnimation(3, 2);

        // エフェクトを生成(サイズは一回り大きい)
        CEffect::Create(pos, size + D3DXVECTOR3(15.0f, 15.0f, 0.0f),
            BULLET_FIRE_COLOR, CEffect::TYPE_PARTICLE);

        break;

    case CBullet::TYPE_WATER:

        // プレイヤーの弾なら重力で落ちていく
        if (m_tribe == TRIBE_PLAYER)
        {
            m_move.y += GRAVITY;

            // 空の中では、空気抵抗も受ける
            if (CBg::GetInTheSky() == true)
            {
                m_move.x -= BULLET_WATER_AIR_RESISTANCE;
            }
        }

        // テクスチャ座標を更新
        SetAnimation(2, 5);

        // エフェクトを生成(サイズは一回り大きい)
        CEffect::Create(pos, size + D3DXVECTOR3(10.0f, 10.0f, 0.0f),
            BULLET_WATER_COLOR, CEffect::TYPE_PARTICLE);

        break;

    case CBullet::TYPE_HELPER_WATER:

        // テクスチャ座標を更新
        SetAnimation(2, 5);

        // エフェクトを生成(サイズは一回り大きい)
        CEffect::Create(pos, size + D3DXVECTOR3(10.0f, 10.0f, 0.0f),
            BULLET_WATER_COLOR, CEffect::TYPE_PARTICLE);

        break;

    case CBullet::TYPE_LIGHT:

        // テクスチャ座標を更新
        SetAnimation(2, 5);

        // エフェクトを生成(サイズは一回り大きい)
        CEffect::Create(pos, size + D3DXVECTOR3(10.0f, 10.0f, 0.0f),
            BULLET_LIGHT_COLOR, CEffect::TYPE_PARTICLE);

        break;

    case CBullet::TYPE_GRASS:

        // テクスチャ座標を更新
        SetAnimation(2, 5);

        // エフェクトを生成(サイズは一回り大きい)
        CEffect::Create(pos, size + D3DXVECTOR3(15.0f, 15.0f, 0.0f),
            D3DXCOLOR(0.0f, 1.0f, 0.0f,1.0f), CEffect::TYPE_PARTICLE);

        break;

    case CBullet::TYPE_RAINY:

        // プレイヤーへの角度取得
        fAngle = GetAngleToPlayer();
        
        // プレイヤーが存在するなら
        if (CGame::GetDispPlayer() == true)
        {
            // 最初はゆっくり
            if (m_nLife > BULLET_RAINY_LIFE_ACCEL)
            {
                m_move = D3DXVECTOR3(-sinf(fAngle)*BULLET_RAINY_FIRST_SPEED, -cosf(fAngle)*BULLET_RAINY_FIRST_SPEED, 0.0f);
            }
            // 速さが変わる1Fのみ、移動量を取得
            else if (m_nLife == BULLET_RAINY_LIFE_ACCEL)
            {
                m_move = D3DXVECTOR3(-sinf(fAngle)*BULLET_RAINY_ACCEL_SPEED, -cosf(fAngle)*BULLET_RAINY_ACCEL_SPEED, 0.0f);
            }
        }
        // 存在しないなら、ただ真下に行く
        else
        {
            // 最初はゆっくり
            if (m_nLife > BULLET_RAINY_LIFE_ACCEL)
            {
                m_move = D3DXVECTOR3(0.0f, BULLET_RAINY_FIRST_SPEED, 0.0f);
            }
            // それ以降は真下に加速
            else
            {
                m_move = D3DXVECTOR3(0.0f, BULLET_RAINY_ACCEL_SPEED, 0.0f);
            }
        }

        // テクスチャ座標を更新
        SetAnimation(2, 5);

        // 炎をセットするカウンタ
        if (m_nLife % BULLET_SET_FIRE_EFFECT_COUNT == 0)
        {
            // 水のエフェクト
            if (m_element == ELEMENT_WATER)
            {
                // エフェクトを生成(サイズは一回り大きい)
                CEffect::Create(pos, size + D3DXVECTOR3(10.0f, 10.0f, 0.0f),
                    BULLET_BULE_FIRE_COLOR, CEffect::TYPE_BLUE_FIRE);
            }
            // 雷のエフェクト
            else if (m_element == ELEMENT_LIGHT)
            {
                // エフェクトを生成(サイズは一回り大きい)
                CEffect::Create(pos, size + D3DXVECTOR3(20.0f, 20.0f, 0.0f),
                    BULLET_LIGHT_COLOR, CEffect::TYPE_PARTICLE_JPG);
            }
        }

        break;

    case CBullet::TYPE_WATAR_HOMING:

        // プレイヤーへの角度取得
        fAngle = GetAngleToPlayer();

        // 移動量を変えるのは、プレイヤーの無敵時間がない時のみ
        if (nPlayerInvincible <= 0)
        {
            // プレイヤーが存在するなら
            if (CGame::GetDispPlayer() == true)
            {
                // ホーミングをやめるまで、移動の向きを取得
                if (m_nLife > BULLET_HOMING_LIFE_STOP_HOMING)
                {
                    m_move = D3DXVECTOR3(-sinf(fAngle)*BULLET_HOMING_WATAR_SPEED, -cosf(fAngle)*BULLET_HOMING_WATAR_SPEED, 0.0f);
                }
            }
            // 存在しないなら
            else
            {
                // 左へ移動
                m_move = D3DXVECTOR3(-BULLET_HOMING_WATAR_SPEED, 0.0f, 0.0f);
            }
        }
        // 無敵時間なら
        else
        {
            // 左へ移動
            m_move = D3DXVECTOR3(-BULLET_HOMING_WATAR_SPEED, 0.0f, 0.0f);
        }

        // テクスチャ座標を更新
        SetAnimation(2, 5);

        // 炎をセットするカウンタ
        if (m_nLife % BULLET_SET_FIRE_EFFECT_COUNT == 0)
        {
            // 水のエフェクト
            if (m_element == ELEMENT_WATER)
            {
                // エフェクトを生成(サイズは一回り大きい)
                CEffect::Create(pos, size + D3DXVECTOR3(10.0f, 10.0f, 0.0f),
                    BULLET_BULE_FIRE_COLOR, CEffect::TYPE_BLUE_FIRE);
            }
            // 雷のエフェクト
            else if (m_element == ELEMENT_LIGHT)
            {
                // エフェクトを生成(サイズは一回り大きい)
                CEffect::Create(pos, size + D3DXVECTOR3(20.0f, 20.0f, 0.0f),
                    BULLET_LIGHT_COLOR, CEffect::TYPE_PARTICLE_JPG);
            }
        }

        break;

    case CBullet::TYPE_FIRE_BURST:

        // プレイヤーへの角度取得
        fAngle = GetAngleToPlayer();

        // 減速
        m_fSpeed -= BULLET_FIRE_BURST_DECELERATE;

        // スピードが0より下回らないようにする
        if (m_fSpeed < 0.0f)
        {
            m_fSpeed = 0.0f;
        }

        // プレイヤーが存在するなら
        if (CGame::GetDispPlayer() == true)
        {
            m_move = D3DXVECTOR3(-sinf(fAngle)*m_fSpeed, -cosf(fAngle)*m_fSpeed, 0.0f);
        }

        // 一定時間で、バーストする
        if (m_nLife <= BULLET_FIRE_BURST_LIFE_ACCEL)
        {
            // 炎バースト
            FireBurst(pos);

            // 消す
            m_nLife = -1;
        }

        // テクスチャ座標を更新
        SetAnimation(3, 2);

        // 炎をセットするカウンタ
        if (m_nLife % BULLET_SET_FIRE_EFFECT_COUNT == 0)
        {
            // エフェクトを生成(サイズは一回り大きい)
            CEffect::Create(pos, size + D3DXVECTOR3(15.0f, 15.0f, 0.0f),
                EFFECT_RED_FIRE_FIRST_COLOR, CEffect::TYPE_RED_FIRE);
        }

        break;

    case CBullet::TYPE_BURNING:

        // プレイヤーへの角度取得
        fAngle = GetAngleToPlayer();

        // プレイヤーが存在するなら
        if (CGame::GetDispPlayer() == true)
        {
            // 最初はゆっくり
            if (m_nLife > BULLET_BURNING_LIFE_ACCEL)
            {
                m_move = D3DXVECTOR3(-sinf(fAngle)*BULLET_BURNING_FIRST_SPEED, -cosf(fAngle)*BULLET_BURNING_FIRST_SPEED, 0.0f);
            }
            // 速さが変わる1Fのみ、移動量を取得
            else if (m_nLife == BULLET_BURNING_LIFE_ACCEL)
            {
                m_move = D3DXVECTOR3(-sinf(fAngle)*BULLET_BURNING_ACCEL_SPEED, -cosf(fAngle)*BULLET_BURNING_ACCEL_SPEED, 0.0f);
            }
        }
        // 存在しないなら、ただ左に加速するだけ
        else
        {
            m_move = D3DXVECTOR3(-BULLET_BURNING_ACCEL_SPEED, 0.0f, 0.0f);
        }

        // テクスチャ座標を更新
        SetAnimation(3, 2);

        // 炎属性なら
        if (m_element == ELEMENT_FIRE)
        {
            // 赤炎エフェクトを生成(サイズは一回り大きい)
            CEffect::Create(pos, size + D3DXVECTOR3(15.0f, 15.0f, 0.0f),
                D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.5f), CEffect::TYPE_RED_FIRE);
        }
        // マルチ属性なら
        else if (m_element == ELEMENT_MULTI)
        {
            // 紫炎エフェクトを生成(サイズは一回り大きい)
            CEffect::Create(pos, size + D3DXVECTOR3(15.0f, 15.0f, 0.0f),
                EFFECT_PURPLE_FIRE_FIRST_COLOR, CEffect::TYPE_PURPLE_FIRE);
        }

        break;

    case CBullet::TYPE_WIND:

        // 一定時間で、急加速する
        if (m_nLife > BULLET_WIND_LIFE_ACCEL)
        {
            // 減速
            m_fSpeed -= BULLET_WIND_DECELERATE;

            // スピードが0より下回らないようにする
            if (m_fSpeed < 0.0f)
            {
                m_fSpeed = 0.0f;
            }

            // 移動量を決める
            m_move = D3DXVECTOR3(-m_fSpeed, 0.0f, 0.0f);
        }
        else
        {
            // 急加速
            m_fSpeed = BULLET_WIND_ACCEL_SPEED;

            // 移動量を決める
            m_move = D3DXVECTOR3(-m_fSpeed, 0.0f, 0.0f);
        }

        // テクスチャ座標を更新
        SetAnimation(2, 5);

        // 草属性なら
        if (m_element == ELEMENT_GRASS)
        {
            // 緑炎エフェクトを生成(サイズは一回り大きい)
            CEffect::Create(pos, size + D3DXVECTOR3(15.0f, 15.0f, 0.0f),
                D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.4f), CEffect::TYPE_YELLOW_FIRE);
        }
        // マルチ属性なら
        else if (m_element == ELEMENT_MULTI)
        {
            // 紫炎エフェクトを生成(サイズは一回り大きい)
            CEffect::Create(pos, size + D3DXVECTOR3(15.0f, 15.0f, 0.0f),
                EFFECT_PURPLE_FIRE_FIRST_COLOR, CEffect::TYPE_PURPLE_FIRE);
        }

        break;
    }

    // 位置を反映
    SetPosition(pos);

    // 頂点座標を変更
    SetVertex();

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
                // 弾がプレイヤーのものなら、
                if (m_tribe == TRIBE_PLAYER)
                {
                    // 位置と衝突サイズを得るため、エネミーにキャスト
                    CEnemy *pEnemy = (CEnemy*)pScene;

                    // 画面内なら
                    if (OutScreen2D(&pEnemy->GetPosition(), &pEnemy->GetSize()) == false)
                    {
                        // 当たり判定の数分回す
                        for (int nCntCollision = 0; nCntCollision < pEnemy->GetCntCollision(); nCntCollision++)
                        {
                            D3DXVECTOR3 posCollision = pEnemy->GetPosition() + pEnemy->GetCollision(nCntCollision).posToShift;
                            D3DXVECTOR3 sizeCollision = pEnemy->GetCollision(nCntCollision).size;

                            // 当たり判定を計算
                            if (RectangleCollision2D(&pos, &posCollision,
                                &size, &sizeCollision) == true)
                            {
                                // ヒットのフラグをtrueに
                                bHit = true;

                                // 属性ごとにヒットエフェクトと状態カウンタを変える
                                switch (m_element)
                                {
                                case ELEMENT_FIRE:

                                    // 衝撃波エフェクト
                                    CEffect::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
                                        BULLET_FIRE_COLOR, CEffect::TYPE_WAVE);

                                    // 爆発エフェクト
                                    CEffect::Create(pos, EXPLOSION_SIZE,
                                        DEFAULT_COLOR, CEffect::TYPE_EXPLOSION);

                                    // 敵へのヒット判定
                                    pEnemy->Hit(pos, BULLET_FIRE_CHANGE_ENEMY_COLOR_FRAME, m_nDamage, nCntCollision, m_element);

                                    break;

                                case ELEMENT_WATER:

                                    // ヘルパーの弾なら
                                    if (m_type == TYPE_HELPER_WATER)
                                    {
                                        // 衝撃波エフェクト
                                        CEffect::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
                                            BULLET_WATER_COLOR, CEffect::TYPE_WAVE);

                                        // 水爆発エフェクト
                                        CEffect::Create(pos, WATAR_EXPLOSION_SIZE,
                                            D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f), CEffect::TYPE_WATER_EXPLOSION);

                                        // 貫通を考慮しない、敵へのヒット判定
                                        pEnemy->Hit(pos, DONT_GO_THROUGH, m_nDamage, nCntCollision, m_element);
                                    }
                                    else
                                    {
                                        // 状態カウンタで、貫通の際の多段ヒットを避ける
                                        if (pEnemy->GetCountHitWater() <= 0)
                                        {
                                            // 衝撃波エフェクト
                                            CEffect::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
                                                BULLET_WATER_COLOR, CEffect::TYPE_WAVE);

                                            // 水爆発エフェクト
                                            CEffect::Create(pos, WATAR_EXPLOSION_SIZE,
                                                D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f), CEffect::TYPE_WATER_EXPLOSION);

                                            // 敵へのヒット判定
                                            pEnemy->Hit(pos, BULLET_WATER_CHANGE_ENEMY_COLOR_FRAME, m_nDamage, nCntCollision, m_element);
                                        }
                                    }

                                    break;

                                case ELEMENT_LIGHT:

                                    // ヘルパーの弾なら
                                    if (size == HELPER_BULLET_SIZE)
                                    {
                                        // 雷は衝撃波を二重に見せる
                                        // 衝撃波エフェクト
                                        CEffect::Create(D3DXVECTOR3(pos.x + (size.x / 2), pos.y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
                                            BULLET_LIGHT_COLOR, CEffect::TYPE_WAVE);
                                        // 衝撃波エフェクト
                                        CEffect::Create(D3DXVECTOR3(pos.x + (size.x / 2), pos.y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
                                            BULLET_LIGHT_COLOR, CEffect::TYPE_WAVE);

                                        // 電撃エフェクト(ダメージによって、大きさを変える)
                                        CEffect::Create(D3DXVECTOR3(pos.x + (size.x / 2), pos.y, 0.0f), D3DXVECTOR3((float)m_nDamage * 1.5f, (float)m_nDamage * 1.5f, 0.0f),
                                            DEFAULT_COLOR, CEffect::TYPE_SHOCK);

                                        // 貫通を考慮しない、敵へのヒット判定
                                        pEnemy->Hit(pos, DONT_GO_THROUGH, m_nDamage, nCntCollision, m_element);
                                    }
                                    // プレイヤーの弾なら
                                    else
                                    {
                                        // 最大チャージなら
                                        if (m_nDamage == BULLET_LIGHT_MAX_DAMAGE)
                                        {
                                            // 状態カウンタで、貫通の際の多段ヒットを避ける
                                            if (pEnemy->GetCountHitLight() <= 0)
                                            {
                                                // 雷は衝撃波を二重に見せる
                                                // 衝撃波エフェクト
                                                CEffect::Create(D3DXVECTOR3(pos.x + (size.x / 2), pos.y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
                                                    BULLET_LIGHT_COLOR, CEffect::TYPE_WAVE);
                                                // 衝撃波エフェクト
                                                CEffect::Create(D3DXVECTOR3(pos.x + (size.x / 2), pos.y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
                                                    BULLET_LIGHT_COLOR, CEffect::TYPE_WAVE);

                                                // 電撃エフェクト(ダメージによって、大きさを変える)
                                                CEffect::Create(D3DXVECTOR3(pos.x + (size.x / 2), pos.y, 0.0f), D3DXVECTOR3((float)m_nDamage * 1.5f, (float)m_nDamage * 1.5f, 0.0f),
                                                    DEFAULT_COLOR, CEffect::TYPE_SHOCK);

                                                // 貫通を考慮した敵へのヒット判定
                                                pEnemy->Hit(pos, BULLET_LIGHT_CHANGE_ENEMY_COLOR_FRAME, m_nDamage, nCntCollision, m_element);
                                            }
                                        }
                                        else
                                        {
                                            // 雷は衝撃波を二重に見せる
                                            // 衝撃波エフェクト
                                            CEffect::Create(D3DXVECTOR3(pos.x + (size.x / 2), pos.y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
                                                BULLET_LIGHT_COLOR, CEffect::TYPE_WAVE);
                                            // 衝撃波エフェクト
                                            CEffect::Create(D3DXVECTOR3(pos.x + (size.x / 2), pos.y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
                                                BULLET_LIGHT_COLOR, CEffect::TYPE_WAVE);

                                            // 電撃エフェクト(ダメージによって、大きさを変える)
                                            CEffect::Create(D3DXVECTOR3(pos.x + (size.x / 2), pos.y, 0.0f), D3DXVECTOR3((float)m_nDamage * 1.5f, (float)m_nDamage * 1.5f, 0.0f),
                                                DEFAULT_COLOR, CEffect::TYPE_SHOCK);

                                            // 貫通を考慮しない、敵へのヒット判定
                                            pEnemy->Hit(pos, DONT_GO_THROUGH, m_nDamage, nCntCollision, m_element);
                                        }
                                    }

                                    break;
                                }

                                // 属性ごとに貫通の仕様を変える
                                switch (m_element)
                                {
                                case ELEMENT_WATER:

                                    // 水属性は常に貫通
                                    bHit = false;

                                    break;

                                case ELEMENT_LIGHT:

                                    // 雷は最大チャージのみ、貫通
                                    if (m_nDamage == BULLET_LIGHT_MAX_DAMAGE)
                                    {
                                        bHit = false;
                                    }

                                    break;
                                }

                                // ヘルパーの弾は貫通しない
                                if (m_type == TYPE_HELPER_WATER)
                                {
                                    bHit = true;
                                }

                                //// 同時ヒットしないようにforを抜ける
                                //break;
                            }
                        }
                    }
                }
            }
            // プレイヤーなら、
            else if (objType == CScene::OBJTYPE_PLAYER)
            {
                // 弾がエネミーのものなら、
                if (m_tribe == TRIBE_ENEMY)
                {
                    // シーン2Dにキャスト
                    CScene2D *pScene2D = (CScene2D*)pScene;

                    // プレイヤーの当たり判定を削る
                    D3DXVECTOR3 playerCollisionSize = PLAYER_COLLISION_SIZE;

                    // 当たり判定を計算
                    if (RectangleCollision2D(&pos, &pScene2D->GetPosition(),
                        &size, &playerCollisionSize) == true)
                    {
                        // 自機へのヒット判定
                        CPlayer *pPlayer = (CPlayer*)pScene;
                        pPlayer->Hit();

                        // 同時ヒットしないようにforを抜ける
                        break;
                    }
                }
            }
            // ヘルパーなら、
            else if (objType == CScene::OBJTYPE_HELPER)
            {
                // 弾がエネミーのものなら、
                if (m_tribe == TRIBE_ENEMY)
                {
                    // シーン2Dにキャスト
                    CScene2D *pScene2D = (CScene2D*)pScene;

                    // ヘルパーの当たり判定を削る
                    D3DXVECTOR3 helperCollisionSize = HELPER_COLLISION_SIZE;

                    // 当たり判定を計算
                    if (RectangleCollision2D(&pos, &pScene2D->GetPosition(),
                        &size, &helperCollisionSize) == true)
                    {
                        // ヘルパーへのヒット判定
                        CHelper *pHelper = (CHelper*)pScene;
                        pHelper->Hit(m_posOld);

                        // 同時ヒットしないようにforを抜ける
                        break;
                    }
                }
            }
            // ブロックなら、
            else if (objType == CScene::OBJTYPE_BLOCK)
            {
                // ブロックにキャスト
                CBlock *pBlock = (CBlock*)pScene;

                // 当たり判定を計算
                if (RectangleCollision2D(&pos, &pBlock->GetPosition(),
                    &size, &pBlock->GetCollisionSize()) == true)
                {
                    // ヒットのフラグをtrueに
                    bHit = true;

                    // 属性ごとにヒットエフェクトを変える
                    switch (m_element)
                    {
                    case ELEMENT_FIRE:

                        // 衝撃波エフェクト
                        CEffect::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
                            BULLET_FIRE_COLOR, CEffect::TYPE_WAVE);

                        // 爆発エフェクト
                        CEffect::Create(pos, EXPLOSION_SIZE,
                            DEFAULT_COLOR, CEffect::TYPE_EXPLOSION);

                        break;

                    case ELEMENT_WATER:

                        // 衝撃波エフェクト
                        CEffect::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
                            BULLET_WATER_COLOR, CEffect::TYPE_WAVE);

                        // 水爆発エフェクト
                        CEffect::Create(pos, WATAR_EXPLOSION_SIZE,
                            D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f), CEffect::TYPE_WATER_EXPLOSION);

                        break;

                    case ELEMENT_LIGHT:

                        // 衝撃波エフェクト
                        CEffect::Create(D3DXVECTOR3(pos.x + (size.x / 2), pos.y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
                            BULLET_LIGHT_COLOR, CEffect::TYPE_WAVE);

                        // 電撃エフェクト(ダメージによって、大きさを変える)
                        CEffect::Create(D3DXVECTOR3(pos.x + (size.x / 2), pos.y, 0.0f), D3DXVECTOR3((float)m_nDamage * 1.5f, (float)m_nDamage * 1.5f, 0.0f),
                            DEFAULT_COLOR, CEffect::TYPE_SHOCK);

                        break;
                    }

                    // 同時ヒットしないようにforを抜ける
                    break;
                }
            }
            // 弾なら
            else if (objType == CScene::OBJTYPE_BULLET)
            {
                // 自身が自機の弾なら
                if (m_tribe == TRIBE_PLAYER)
                {
                    // バレットにキャスト
                    CBullet *pBullet = (CBullet*)pScene;

                    // 対象が敵の弾なら
                    if (pBullet->GetTribe() == TRIBE_ENEMY)
                    {
                        // 弱点かどうかのフラグ
                        bool bWeak = false;

                        // 属性ごとに、弱点かどうかの計算
                        switch (m_element)
                        {
                        case ELEMENT_FIRE:

                            if (pBullet->GetElement() == ELEMENT_GRASS)
                            {
                                bWeak = true;
                            }

                            break;

                        case ELEMENT_WATER:

                            if (pBullet->GetElement() == ELEMENT_FIRE)
                            {
                                bWeak = true;
                            }

                            break;

                        case ELEMENT_LIGHT:

                            if (pBullet->GetElement() == ELEMENT_WATER)
                            {
                                bWeak = true;
                            }

                            break;
                        }

                        // 弱点なら
                        if (bWeak == true)
                        {
                            // シーン2Dにキャスト
                            CScene2D *pScene2D = (CScene2D*)pScene;

                            // 当たり判定を計算
                            if (RectangleCollision2D(&pos, &pScene2D->GetPosition(),
                                &size, &pScene2D->GetSize()) == true)
                            {
                                // スコアを取得
                                CScore *pScore = CGame::GetScore();

                                // プレイヤーの位置を取得
                                D3DXVECTOR3 playerPos = PLAYER_POS + SCORE_DISP_UP;

                                // プレイヤーが存在しているなら
                                if (CGame::GetDispPlayer() == true)
                                {
                                    playerPos = CGame::GetPlayer()->GetPosition() + SCORE_DISP_UP;
                                }

                                // 打ち消した弾によって、入るスコアを変える
                                switch (m_element)
                                {
                                case ELEMENT_FIRE:

                                    // スコア加算
                                    pScore->AddScore(BULLET_COUNTERATTACK_GRASS_SCORE);

                                    // スコア表示(黄色にする)
                                    pScore->SetScore(BULLET_COUNTERATTACK_GRASS_SCORE, playerPos, SCORE_DEFEAT_ONE_SHOT_COLOR);

                                    break;

                                case ELEMENT_WATER:

                                    // 炎バーストは3倍
                                    if (pBullet->GetType() == TYPE_FIRE_BURST)
                                    {
                                        // スコア加算
                                        pScore->AddScore(BULLET_FIRE_BURST_SCORE);

                                        // スコア表示(黄色にする)
                                        pScore->SetScore(BULLET_FIRE_BURST_SCORE, playerPos, SCORE_DEFEAT_ONE_SHOT_COLOR);
                                    }
                                    // バーニングは2倍
                                    else if (pBullet->GetType() == TYPE_BURNING)
                                    {
                                        // スコア加算
                                        pScore->AddScore(BULLET_BURNING_BURST_SCORE);

                                        // スコア表示(黄色にする)
                                        pScore->SetScore(BULLET_BURNING_BURST_SCORE, playerPos, SCORE_DEFEAT_ONE_SHOT_COLOR);
                                    }
                                    // 通常
                                    else
                                    {
                                        // スコア加算
                                        pScore->AddScore(BULLET_COUNTERATTACK_FIRE_SCORE);

                                        // スコア表示(黄色にする)
                                        pScore->SetScore(BULLET_COUNTERATTACK_FIRE_SCORE, playerPos, SCORE_DEFEAT_ONE_SHOT_COLOR);
                                    }

                                    break;

                                case ELEMENT_LIGHT:

                                    // スコア加算
                                    pScore->AddScore(BULLET_COUNTERATTACK_WATAR_SCORE);

                                    // スコア表示(黄色にする)
                                    pScore->SetScore(BULLET_COUNTERATTACK_WATAR_SCORE, playerPos, SCORE_DEFEAT_ONE_SHOT_COLOR);

                                    break;
                                }

                                // 敵の弾を消す
                                pBullet->Uninit();

                                //// 同時ヒットしないようにforを抜ける
                                //break;
                            }
                        }                   
                    }
                }
                // 自身が敵の弾なら
                else if (m_tribe == TRIBE_ENEMY)
                {
                    // バレットにキャスト
                    CBullet *pBullet = (CBullet*)pScene;

                    // 対象が自機の弾なら
                    if (pBullet->GetTribe() == TRIBE_PLAYER)
                    {
                        // 弱点かどうかのフラグ
                        bool bWeak = false;

                        // 自身の属性ごとに、弱点かどうかの計算
                        switch (m_element)
                        {
                        case ELEMENT_WATER:

                            if (pBullet->GetElement() == ELEMENT_FIRE)
                            {
                                bWeak = true;
                            }

                            break;

                        case ELEMENT_LIGHT:

                            if (pBullet->GetElement() == ELEMENT_WATER)
                            {
                                bWeak = true;
                            }

                            break;

                        case ELEMENT_GRASS:

                            if (pBullet->GetElement() == ELEMENT_WATER)
                            {
                                bWeak = true;
                            }

                            break;

                        case ELEMENT_MULTI:

                            // マルチ属性は、問答無用で自機の弾を消す
                            bWeak = true;

                            break;
                        }

                        // 弱点なら
                        if (bWeak == true)
                        {
                            // シーン2Dにキャスト
                            CScene2D *pScene2D = (CScene2D*)pScene;

                            // 当たり判定を計算
                            if (RectangleCollision2D(&pos, &pScene2D->GetPosition(),
                                &size, &pScene2D->GetSize()) == true)
                            {
                                // 小爆発
                                SmallExplosion(pScene2D->GetPosition(), pScene2D->GetSize());

                                // 自機の弾を消す
                                pBullet->Uninit();

                                // 同時ヒットしないようにforを抜ける
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    // 弾のライフを減らす
    m_nLife--;

    // 床との当たり判定
    bool bHitFloor = HitFloor(pos,size);

    // 弾の寿命がなくなる、または画面外に出たら消滅
    if (m_nLife <= 0 || OutScreen2D(&pos, &size) == true || bHit == true || bHitFloor == true)
    {
        Uninit(); // これより下にコードは書かない
    }
}

//========================================
// 弾の描画処理
//========================================
void CBullet::Draw(void)
{
    // 描画処理
    CScene2D::Draw();
}

//========================================
// 弾の画像ロード処理
//========================================
HRESULT CBullet::Load(void)
{
    // レンダラーからデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet000.png", &m_apTexture[CBullet::TYPE_FIRE]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet001.png", &m_apTexture[CBullet::TYPE_WATER]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet001.png", &m_apTexture[CBullet::TYPE_HELPER_WATER]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet002.png", &m_apTexture[CBullet::TYPE_LIGHT]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet003.png", &m_apTexture[CBullet::TYPE_GRASS]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet001.png", &m_apTexture[CBullet::TYPE_RAINY]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet001.png", &m_apTexture[CBullet::TYPE_WATAR_HOMING]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet000.png", &m_apTexture[CBullet::TYPE_FIRE_BURST]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet000.png", &m_apTexture[CBullet::TYPE_BURNING]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet003.png", &m_apTexture[CBullet::TYPE_WIND]);

    return S_OK;
}

//========================================
// 弾の画像破棄処理
//========================================
void CBullet::Unload(void)
{
    // テクスチャの破棄
    for (int nCntTexture = 0; nCntTexture < CBullet::TYPE_MAX ; nCntTexture++)
    {
        if (m_apTexture[nCntTexture] != NULL)
        {
            m_apTexture[nCntTexture]->Release();
            m_apTexture[nCntTexture] = NULL;
        }
    }
}

//================================
// 弾の作成
//================================
CBullet *CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, CBullet::TYPE type, CScene2D::TRIBE tribe)
{
    CBullet *pBullet = NULL;

    // メモリを確保
    pBullet = new CBullet;

    // タイプのみ、テクスチャ割り当てのために結びつけておく
    pBullet->m_type = type;

    // 初期化
    pBullet->Init(pos, size);

    // メンバ変数を結びつける
    pBullet->m_posOld = pos;
    pBullet->m_move = move;
    pBullet->m_tribe = tribe;

    // 弾の種類によって、固定の値を結びつける
    switch (pBullet->m_type)
    {
    case CBullet::TYPE_FIRE:

        pBullet->m_element = ELEMENT_FIRE;
        pBullet->m_nLife = BULLET_FIRE_LIFE;
        pBullet->m_nDamage = BULLET_FIRE_DAMAGE;

        // 敵の弾のライフは画面外に出るまで
        if (pBullet->m_tribe == TRIBE_ENEMY)
        {
            pBullet->m_nLife = BULLET_COUNTERATTACK_LIFE;
        }

        break;

    case CBullet::TYPE_WATER:

        pBullet->m_element = ELEMENT_WATER;
        pBullet->m_nLife = BULLET_WATER_LIFE;
        pBullet->m_nDamage = BULLET_WATER_DAMAGE;

        // 敵の弾のライフは画面外に出るまで
        if (pBullet->m_tribe == TRIBE_ENEMY)
        {
            pBullet->m_nLife = BULLET_COUNTERATTACK_LIFE;
        }

        break;

    case CBullet::TYPE_HELPER_WATER:

        pBullet->m_element = ELEMENT_WATER;
        pBullet->m_nLife = BULLET_WATER_LIFE;
        pBullet->m_nDamage = BULLET_WATAR_HELPER_DAMAGE;

        // 敵の弾のライフは画面外に出るまで
        if (pBullet->m_tribe == TRIBE_ENEMY)
        {
            pBullet->m_nLife = BULLET_COUNTERATTACK_LIFE;
        }

        break;

    case CBullet::TYPE_LIGHT:

        pBullet->m_element = ELEMENT_LIGHT;
        pBullet->m_nLife = BULLET_LIGHT_LIFE;
        pBullet->m_nDamage = (int)size.x;   // 雷の弾のダメージはサイズ依存

        // ヘルパーの弾なら、ダメージは低めに
        if (size == HELPER_BULLET_SIZE)
        {
            pBullet->m_nDamage = BULLET_LIGHT_HELPER_DAMAGE;
        }
        // 最大チャージの弾なら、最大ダメージで固定
        else if (size.x == BULLET_LIGHT_MAX_SIZE)
        {
            pBullet->m_nDamage = BULLET_LIGHT_MAX_DAMAGE;
        }

        // 敵の弾のライフは画面外に出るまで
        if (pBullet->m_tribe == TRIBE_ENEMY)
        {
            pBullet->m_nLife = BULLET_COUNTERATTACK_LIFE;
        }

        break;

    case CBullet::TYPE_GRASS:

        pBullet->m_element = ELEMENT_GRASS;
        pBullet->m_nLife = BULLET_GRASS_LIFE;
        pBullet->m_nDamage = BULLET_GRASS_DAMAGE;

        // 敵の弾のライフは画面外に出るまで
        if (pBullet->m_tribe == TRIBE_ENEMY)
        {
            pBullet->m_nLife = BULLET_COUNTERATTACK_LIFE;
        }

        break;

    case CBullet::TYPE_RAINY:

        pBullet->m_element = ELEMENT_WATER;
        pBullet->m_nLife = BULLET_LIFE_COUNTER;
        pBullet->m_nDamage = BULLET_GRASS_DAMAGE;

        // 雷変化の引数なら
        if (move == CHANGE_LIGHT)
        {
            // 雷属性に
            pBullet->m_element = ELEMENT_LIGHT;
        }

        break;

    case CBullet::TYPE_WATAR_HOMING:

        pBullet->m_element = ELEMENT_WATER;
        pBullet->m_nLife = BULLET_LIFE_COUNTER;
        pBullet->m_nDamage = BULLET_GRASS_DAMAGE;

        // 雷変化の引数なら
        if (move == CHANGE_LIGHT)
        {
            // 雷属性に
            pBullet->m_element = ELEMENT_LIGHT;
        }

        break;

    case CBullet::TYPE_FIRE_BURST:

        pBullet->m_element = ELEMENT_FIRE;
        pBullet->m_nLife = BULLET_FIRE_LIFE;
        pBullet->m_nDamage = BULLET_FIRE_DAMAGE;

        pBullet->m_fSpeed = BULLET_FIRE_BURST_SPEED;

        break;

    case CBullet::TYPE_BURNING:

        pBullet->m_element = ELEMENT_FIRE;
        pBullet->m_nLife = BULLET_LIFE_COUNTER;
        pBullet->m_nDamage = BULLET_GRASS_DAMAGE;

        // マルチ変化の引数なら
        if (move == CHANGE_MULTI)
        {
            // マルチ属性に
            pBullet->m_element = ELEMENT_MULTI;
        }

        break;

    case CBullet::TYPE_WIND:

        pBullet->m_element = ELEMENT_GRASS;
        pBullet->m_nLife = BULLET_GRASS_LIFE;
        pBullet->m_nDamage = BULLET_GRASS_DAMAGE;

        pBullet->m_fSpeed = BULLET_WIND_FIRST_SPEED;

        // マルチ変化の引数なら
        if (move == CHANGE_MULTI)
        {
            // マルチ属性に
            pBullet->m_element = ELEMENT_MULTI;
        }

        break;
    }

    return pBullet;
}

//================================
// 床との当たり判定
//================================
bool CBullet::HitFloor(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 変数宣言
    bool bHit = false;  // 床に当たったかどうか

    // 床の判定を取得
    bool bFloor = false;
    bFloor = CGame::GetFloor();

    // 床との当たり判定
    if (bFloor == true)
    {
        // 弾の中心が床より下になったら
        if (pos.y >= FLOOR_COLLISION)
        {
            // 当たったフラグをtrueに
            bHit = true;

            // エフェクトを出す(発生位置は、移動した分を戻している)
            // 属性ごとに衝撃波の色を変える
            switch (m_element)
            {
            case ELEMENT_FIRE:

                // 衝撃波エフェクト
                CEffect::Create(pos + BULLET_HIT_FLOOR_EFFECT_ADD_POS - m_move, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
                    BULLET_FIRE_COLOR, CEffect::TYPE_WAVE);

                // 爆発エフェクト
                CEffect::Create(pos + BULLET_HIT_FLOOR_EFFECT_ADD_POS - m_move, EXPLOSION_SIZE,
                    DEFAULT_COLOR, CEffect::TYPE_EXPLOSION);

                break;

            case ELEMENT_WATER:

                // 衝撃波エフェクト
                CEffect::Create(pos + BULLET_HIT_FLOOR_EFFECT_ADD_POS - m_move, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
                    BULLET_WATER_COLOR, CEffect::TYPE_WAVE);

                // 水爆発エフェクト
                CEffect::Create(pos + BULLET_HIT_FLOOR_EFFECT_ADD_POS - m_move, WATAR_EXPLOSION_SIZE,
                    D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f), CEffect::TYPE_WATER_EXPLOSION);

                break;

            case ELEMENT_LIGHT:

                // 衝撃波エフェクト
                CEffect::Create(pos + BULLET_HIT_FLOOR_EFFECT_ADD_POS - m_move, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
                    BULLET_LIGHT_COLOR, CEffect::TYPE_WAVE);

                // 電撃エフェクト(ダメージによって、大きさを変える)
                CEffect::Create(pos + BULLET_HIT_FLOOR_EFFECT_ADD_POS - m_move, D3DXVECTOR3((float)m_nDamage * 1.5f, (float)m_nDamage * 1.5f, 0.0f),
                    DEFAULT_COLOR, CEffect::TYPE_SHOCK);

                break;

            case ELEMENT_GRASS:

                // 衝撃波エフェクト
                CEffect::Create(pos + BULLET_HIT_FLOOR_EFFECT_ADD_POS - m_move, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
                    BULLET_GRASS_COLOR, CEffect::TYPE_WAVE);

                // 爆発エフェクト
                CEffect::Create(pos + BULLET_HIT_FLOOR_EFFECT_ADD_POS - m_move, EXPLOSION_SIZE,
                    DEFAULT_COLOR, CEffect::TYPE_EXPLOSION);

                break;
            }
        }
    }

    return bHit;
}

//====================================================
// バレットから見た自機への角度
//====================================================
float CBullet::GetAngleToPlayer(void)
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
// 炎バースト
//====================================================
void CBullet::FireBurst(D3DXVECTOR3 pos)
{
    float fSpeed = BULLET_FIRE_BURST_CREATE_BULLET_SPEED;             // バーストで生成される炎の弾の速度
    D3DXVECTOR3 sizeBullet = BULLET_FIRE_BURST_CREATE_BULLET_SIZE;    // バーストで生成される炎の弾の大きさ

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

    CBullet::Create(pos, sizeBullet, D3DXVECTOR3(sinf(D3DX_PI / 8) * fSpeed, -cosf(D3DX_PI / 8) * fSpeed, 0.0f),
        CBullet::TYPE_FIRE, TRIBE_ENEMY);

    CBullet::Create(pos, sizeBullet, D3DXVECTOR3(sinf(D3DX_PI / 8) * fSpeed, cosf(D3DX_PI / 8) * fSpeed, 0.0f),
        CBullet::TYPE_FIRE, TRIBE_ENEMY);

    CBullet::Create(pos, sizeBullet, D3DXVECTOR3(-sinf(D3DX_PI / 8) * fSpeed, -cosf(D3DX_PI / 8) * fSpeed, 0.0f),
        CBullet::TYPE_FIRE, TRIBE_ENEMY);

    CBullet::Create(pos, sizeBullet, D3DXVECTOR3(-sinf(D3DX_PI / 8) * fSpeed, cosf(D3DX_PI / 8) * fSpeed, 0.0f),
        CBullet::TYPE_FIRE, TRIBE_ENEMY);
}

//====================================================
// 小爆発
//====================================================
void CBullet::SmallExplosion(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // エフェクト用の変数宣言
    float fAngle = 0.0f;                // 角度
    float fRadius = 0.0f;               // 半径
    float fSize = 0.0f;                 // 大きさ

    // 複数のパーティクルを一度に出す
    for (int nCnt = 0; nCnt < EFFECT_SMALL_EXPLOSION_COUNT_LOOP; nCnt++)
    {
        // ランダムな角度と半径を取得
        fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
        fRadius = float(rand() % (int)size.x) / 20.0f;
        fSize = float(rand() % (int)size.x + (int)(size.x / 2));

        // 爆発エフェクト
        CEffect::Create(D3DXVECTOR3(pos.x - sinf(fAngle)*fRadius, pos.y - cosf(fAngle)*fRadius, 0.0f),
            D3DXVECTOR3(fSize, fSize, 0.0f), EFFECT_SMALL_EXPLOSION_COLOR, CEffect::TYPE_SMALL_EXPLOSION);
    }
}
