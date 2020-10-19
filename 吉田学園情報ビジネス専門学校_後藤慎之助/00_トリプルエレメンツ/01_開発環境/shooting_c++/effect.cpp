//===============================================
//
// エフェクト処理 (effect.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "effect.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "library.h"
#include "player.h"
#include "bullet.h"
#include "game.h"
#include "enemy.h"
#include "bg.h"

//=======================================
// マクロ定義
//=======================================
#define ESCAPE_CHARGE_LOOP 1000 // プレイヤーがチャージしながらやられた時の、チャージのループを抜ける

//=======================================
// 静的メンバ変数宣言
//=======================================
LPDIRECT3DTEXTURE9 CEffect::m_apTexture[CEffect::TYPE_MAX] = {};

//========================================
// エフェクトのコンストラクタ
//========================================
CEffect::CEffect() :CScene2D(OBJTYPE::OBJTYPE_EFFECT)
{
    m_move = DEFAULT_VECTOR;
    m_col = DEFAULT_COLOR;
    m_nLife = 1;    // 0だと消えるため
    m_type = CEffect::TYPE_MAX;
    m_bAlphaDown = false;

    m_fAngle = 0.0f;
    m_fRotSpeed = 0.0f;
}

//========================================
// エフェクトのデストラクタ
//========================================
CEffect::~CEffect()
{

}

//========================================
// エフェクトの初期化処理
//========================================
HRESULT CEffect::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 初期化
    CScene2D::Init(pos, size);

    // テクスチャを割り当て
    BindTexture(m_apTexture[m_type]);

    return S_OK;
}

//========================================
// エフェクトの終了処理
//========================================
void CEffect::Uninit(void)
{
    // 終了処理
    CScene2D::Uninit();
}

//========================================
// エフェクトの更新処理
//========================================
void CEffect::Update(void)
{
    // キーボードを取得
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // コントローラを取得
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(PLAYER_1);

    // 変数宣言
    D3DXVECTOR3 pos = GetPosition();                   // 位置を取得
    D3DXVECTOR3 size = GetSize();                      // サイズを取得
    float fTargetPosX = 0.0f, fTargetPosY = 0.0f;      // 目標の座標
    float fMyPosX = 0.0f, fMyPosY = 0.0f;		       // 自分の座標
    float fAngle = 0.0f;							   // 角度
    D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 移動量
    bool bUse = true;                                  // 使用しているかどうか

    // プレイヤー関連の変数宣言
    CPlayer *pPlayer = NULL;                        // プレイヤーを取得
    CScene2D *pPlayerScene2D = NULL;                // プレイヤーをシーン2Dにキャスト
    D3DXVECTOR3 playerPos = pos;                    // プレイヤーの位置(チャージしながらやられた時のため、エフェクトの位置で初期化する)
    D3DXVECTOR3 playerSize = DEFAULT_VECTOR;        // プレイヤーの大きさ
    int nPlayerChargeTime = ESCAPE_CHARGE_LOOP;     // プレイヤーのチャージ時間
    float fPlayerAngle = 0.0f;                      // プレイヤーの角度

    // ゲームなら
    if (CManager::GetMode() == CManager::MODE_GAME)
    {
        // 空の中なら、プレイヤーの角度の値を変えておく
        if (CBg::GetInTheSky() == true)
        {
            fPlayerAngle = D3DX_PI / 4;
        }

        // プレイヤーが存在しているなら
        if (CGame::GetDispPlayer() == true)
        {
            // プレイヤーの情報を取得
            pPlayer = CGame::GetPlayer();
            pPlayerScene2D = (CScene2D*)pPlayer;
            playerPos = pPlayerScene2D->GetPosition();
            playerSize = pPlayerScene2D->GetSize();
            nPlayerChargeTime = pPlayer->GetChargeTime();
            fPlayerAngle = pPlayer->GetPlayerAngle();

            // プレイヤーが動けないなら
            if (CGame::GetMovePlayer() == false)
            {
                nPlayerChargeTime = ESCAPE_CHARGE_LOOP; // チャージを強制的に発射
            }

            // 暗転時も
            if (CGame::GetDarkening() == true)
            {
                nPlayerChargeTime = ESCAPE_CHARGE_LOOP; // チャージを強制的に発射
            }
        }
    }

    // 種類に応じた場合分け
    switch (m_type)
    {
        // パーティクル
    case CEffect::TYPE_PARTICLE:

        // サイズをだんだん縮小
        size.x -= 5.0f;
        size.y -= 5.0f;

        // だんだん透明に
        m_col.a -= 0.1f;

        // 色を変更
        CScene2D::SetColor(m_col);

        break;

        // ウェーブ
    case CEffect::TYPE_WAVE:

        // サイズをだんだん拡大
        size.x += 3.0f;
        size.y += 3.0f;

        // だんだん透明に
        m_col.a -= 0.015f;

        // 色を変更
        CScene2D::SetColor(m_col);

        break;

        // 爆発
    case CEffect::TYPE_EXPLOSION:

        // サイズをだんだん拡大
        size.x += 5.0f;
        size.y += 5.0f;

        // アニメーションを設定
        if (SetAnimation(3, 8) == true)
        {
            // アニメーションが一周したら、未使用に
            bUse = false;
        }

        // 色を変更
        CScene2D::SetColor(m_col);

        break;

        // チャージ
    case CEffect::TYPE_CHARGE:

        // サイズを変えつつ、自機に位置を結びつける
        size.x += CHARGE_RATE_SIZE;
        size.y += CHARGE_RATE_SIZE;
        pos = playerPos;

        // サイズは最大値を超えたらそれで固定
        if (size.x > CHARGE_SIZE)
        {
            size.x = CHARGE_SIZE;
            size.y = CHARGE_SIZE;
        }

        // 最大チャージになったら、点滅
        if (nPlayerChargeTime == MAX_CHARGE_TIME)
        {
            // カウンターを進めて、パターンを切り替える
            int nPatternAnim = CountAnimation(2, 2);

            // 透明度を2パターンで切り替える
            if (nPatternAnim == 1)
            {
                CScene2D::SetColor(D3DXCOLOR(m_col.r, m_col.g, m_col.b, 0.7f));
            }
            else
            {
                CScene2D::SetColor(D3DXCOLOR(m_col.r, m_col.g, m_col.b, 0.9f));
            }
        }
        else
        {
            // 色を変更
            CScene2D::SetColor(m_col);
        }

        // 弾を打ち終わったら徐々に透明にするフラグをtrueに
        if (nPlayerChargeTime == 0)
        {
            m_bAlphaDown = true;
        }
        // チャージしながらやられたら、すぐ消す
        else if (nPlayerChargeTime == ESCAPE_CHARGE_LOOP)
        {
            m_col.a = -0.1f;
            CScene2D::SetColor(m_col);
        }

        // 弾を打ち終わったら徐々に透明に
        if (m_bAlphaDown == true)
        {
            m_col.a -= 0.05f;
            CScene2D::SetColor(m_col);
        }

        break;

        // チャージのパーティクル
    case CEffect::TYPE_CHARGE_PART:

        // サイズ縮小、だんだん透明に
        size.x -= 0.5f;
        size.y -= 0.5f;
        m_col.a -= 0.1f;

        // 目標の座標と自身の座標
        fTargetPosX = playerPos.x, fTargetPosY = playerPos.y;
        fMyPosX = pos.x, fMyPosY = pos.y;

        // 角度を決める
        fAngle = atan2f((fMyPosX - fTargetPosX), (fMyPosY - fTargetPosY));

        // 移動量を決める
        move = D3DXVECTOR3(-sinf(fAngle)*CHARGE_PART_SPEED, -cosf(fAngle)*CHARGE_PART_SPEED, 0.0f);

        // 位置に移動量を足す
        pos.x += move.x;
        pos.y += move.y;

        // 色を変更
        CScene2D::SetColor(m_col);

        break;

        // 雷バレット
    case CEffect::TYPE_LIGHT_BULLET:

        // サイズ拡大しつつ、自機に位置を結びつける(自機の角度も考慮)
        size.x += BULLET_LIGHT_EXTEND_RATE;
        size.y += BULLET_LIGHT_EXTEND_RATE;
        pos = D3DXVECTOR3(playerPos.x + (cosf(fPlayerAngle) *playerSize.x), 
            playerPos.y - (sinf(fPlayerAngle) *(playerSize.y)), 0.0f);

        // サイズは最大値を超えたらそれで固定
        if (size.x > BULLET_LIGHT_MAX_SIZE)
        {
            size.x = BULLET_LIGHT_MAX_SIZE;
            size.y = BULLET_LIGHT_MAX_SIZE;
        }

        // カウンターを進めて、パターンを切り替える
        SetAnimation(2, 5);

        // 離したら発射(チャージしながらやられた時も発射)(ポーズボタンでも発射)
        if (pInputKeyboard->GetKeyboardRelease(DIK_B) || pInputJoypad->GetJoypadRelease(PLAYER_1, CInputJoypad::BUTTON_Y) ||
            nPlayerChargeTime == ESCAPE_CHARGE_LOOP || nPlayerChargeTime == 0
            || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START))
        {
            // 雷の弾
            CBullet::Create(pos, size, 
                D3DXVECTOR3(cosf(fPlayerAngle) * BULLET_LIGHT_SPEED, sinf(fPlayerAngle) * (-BULLET_LIGHT_SPEED), 0.0f),
                CBullet::TYPE_LIGHT, TRIBE_PLAYER);

            // 未使用に
            bUse = false;
        }

        // 色を変更
        CScene2D::SetColor(m_col);

        break;

        // 電撃
    case CEffect::TYPE_SHOCK:

        // サイズ拡大
        size.x += 5.0f;
        size.y += 5.0f;

        // アニメーションを設定
        if (SetAnimation(3, 8) == true)
        {
            // アニメーションが一周したら、未使用に
            bUse = false;
        }

        // 色を変更
        CScene2D::SetColor(m_col);

        break;

        // 大爆発
    case CEffect::TYPE_BIG_EXPLOSION:

        // アニメーションを設定
        if (SetAnimation(3, 10) == true)
        {
            // アニメーションが一周したら、未使用に
            bUse = false;
        }

        // 色を変更
        CScene2D::SetColor(m_col);

        break;

        // 赤火花
    case CEffect::TYPE_RED_SPARK:

        // アニメーションを設定
        if (SetAnimation(4, 10) == true)
        {
            // アニメーションが一周したら、未使用に
            bUse = false;
        }

        // 色を変更
        CScene2D::SetColor(m_col);

        break;

        // 緑火花
    case CEffect::TYPE_GREEN_SPARK:

        // アニメーションを設定
        if (SetAnimation(4, 10) == true)
        {
            // アニメーションが一周したら、未使用に
            bUse = false;
        }

        // 色を変更
        CScene2D::SetColor(m_col);

        break;

        // 青火花
    case CEffect::TYPE_BLUE_SPARK:

        // アニメーションを設定
        if (SetAnimation(4, 10) == true)
        {
            // アニメーションが一周したら、未使用に
            bUse = false;
        }

        // 色を変更
        CScene2D::SetColor(m_col);

        break;

        // 黒火花
    case CEffect::TYPE_BLACK_SPARK:

        // アニメーションを設定
        if (SetAnimation(4, 10) == true)
        {
            // アニメーションが一周したら、未使用に
            bUse = false;
        }

        // 色を変更
        CScene2D::SetColor(m_col);

        break;

        // 水爆発
    case CEffect::TYPE_WATER_EXPLOSION:

        // アニメーションを設定
        if (SetAnimation(3, 8) == true)
        {
            // アニメーションが一周したら、未使用に
            bUse = false;
        }

        // 色を変更
        CScene2D::SetColor(m_col);

        break;

        // 反転チャージ
    case CEffect::TYPE_REVERSE_CHARGE:

        fAngle = GetAngleToDragon();

        // 移動量を決める
        move = D3DXVECTOR3(-sinf(fAngle)*CHARGE_PART_SPEED, -cosf(fAngle)*CHARGE_PART_SPEED, 0.0f);

        // サイズ縮小
        size.x -= 2.0f;
        size.y -= 2.0f;

        // 位置に移動量を足す
        pos.x += move.x;
        pos.y += move.y;

        // 色を変更
        CScene2D::SetColor(m_col);

        break;

        // ドラゴンオーラ
    case CEffect::TYPE_DRAGON_AURA:

        // 移動量分、上へ
        pos.y -= m_move.y;

        // だんだん透明に
        m_col.a -= 0.02f;

        // 色を変更
        CScene2D::SetColor(m_col);

        break;

        // 二重ウェーブ
    case CEffect::TYPE_DOUBLE_WAVE:

        // サイズ拡大、だんだん透明に
        size.x += EFFECT_DOUBLE_WAVE_EXTEND_RATE;
        size.y += EFFECT_DOUBLE_WAVE_EXTEND_RATE;
        m_col.a -= EFFECT_DOUBLE_WAVE_FADE_OUT_RATE;

        // 色を変更
        CScene2D::SetColor(m_col);

        break;

        // パーティクル(jpg)
    case CEffect::TYPE_PARTICLE_JPG:

        // だんだん透明に
        m_col.a -= 0.1f;

        // 色を変更
        CScene2D::SetColor(m_col);

        break;

        // 炎
    case CEffect::TYPE_FIRE:

        if (m_bAlphaDown == true)
        {
            // だんだん透明に
            m_col.a -= EFFECT_FIRE_FADE_OUT_RATE;

            // 移動量分、上へ
            pos.y -= m_move.y;

        }
        else
        {
            // だんだん見えるように
            m_col.a += EFFECT_FIRE_FADE_IN_RATE;

            if (m_col.a >= EFFECT_FIRE_ALPHA_START_MOVE)
            {
                // 移動量分、上へ
                pos.y -= m_move.y;
            }
        }

        // 赤に近づけていく
        m_col.g -= EFFECT_FIRE_TO_RED_RATE;

        // 一定の値で透明度を下げるをtrueに
        if (m_col.a >= EFFECT_FIRE_ALPHA_START_FADE_OUT)
        {
            m_bAlphaDown = true;
        }

        // 回転
        m_fAngle += m_fRotSpeed;

        // 色を変更
        CScene2D::SetColor(m_col);

        break;

        // マグマ
    case CEffect::TYPE_MAGMA:

        // 移動量を反映
        pos.x += m_move.x;

        // 空の中にいるなら
        if (CBg::GetInTheSky() == true)
        {
            pos.y += FLOWING_STAGE;
        }

        if (m_bAlphaDown == true)
        {
            // 赤に近づけていく
            m_col.g -= m_fRotSpeed;

        }
        else
        {
            // 黄に近づけていく
            m_col.g += m_fRotSpeed;
        }

        if (m_col.g >= 1.0f)
        {
            m_bAlphaDown = true;
        }
        else if (m_col.g <= 0.0f)
        {
            m_bAlphaDown = false;
        }

        // 色を変更
        CScene2D::SetColor(m_col);

        break;

        // 葉っぱ
    case CEffect::TYPE_LEAF:

        // だんだん消す
        m_col.a -= EFFECT_LEAF_FADE_RATE;

        // 重力(少し軽め)
        m_move.y += GRAVITY / 2;

        // 移動量反映
        pos += m_move;

        // 回転
        m_fAngle += m_fRotSpeed;

        // 色を変更
        CScene2D::SetColor(m_col);

        break;

        // 枝
    case CEffect::TYPE_BRANCH:

        // だんだん消す
        m_col.a -= EFFECT_BRANCH_FADE_RATE;

        // 重力(少し軽め)
        m_move.y += GRAVITY / 2;

        // 移動量反映
        pos += m_move;

        // 回転
        m_fAngle += m_fRotSpeed;

        // 色を変更
        CScene2D::SetColor(m_col);

        break;

        // 赤い炎
    case CEffect::TYPE_RED_FIRE:

        // だんだん透明に
        m_col.a -= EFFECT_RED_FIRE_FADE_OUT_RATE;

        // 移動量分、上へ
        pos.y -= m_move.y;

        // 赤に近づけていく
        m_col.g -= EFFECT_YELLOW_FIRE_TO_RED_RATE;

        // 回転
        m_fAngle += m_fRotSpeed;

        // 色を変更
        CScene2D::SetColor(m_col);

        break;

        // 青い炎
    case CEffect::TYPE_BLUE_FIRE:

        // だんだん透明に
        m_col.a -= EFFECT_BLUE_FIRE_FADE_OUT_RATE;

        // 移動量分、上へ
        pos.y -= m_move.y;

        // 青に近づけていく
        m_col.g -= EFFECT_WHITE_FIRE_TO_BLUE_RATE;
        m_col.r -= EFFECT_WHITE_FIRE_TO_BLUE_RATE;

        // 回転
        m_fAngle += m_fRotSpeed;

        // 色を変更
        CScene2D::SetColor(m_col);

        break;

        // 黄色い炎
    case CEffect::TYPE_YELLOW_FIRE:

        // だんだん透明に
        m_col.a -= EFFECT_YELLOW_FIRE_FADE_OUT_RATE;

        // 移動量分、上へ
        pos.y -= m_move.y;

        // 緑に近づけていく
        m_col.r -= EFFECT_YELLOW_FIRE_TO_GREEN_RATE;

        // 回転
        m_fAngle += m_fRotSpeed;

        // 色を変更
        CScene2D::SetColor(m_col);

        break;

        // 紫の炎
    case CEffect::TYPE_PURPLE_FIRE:

        // だんだん透明に
        m_col.a -= EFFECT_PURPLE_FIRE_FADE_OUT_RATE;

        // 移動量分、上へ
        pos.y -= m_move.y;

        // 紫に近づけていく
        m_col.r -= EFFECT_WHITE_FIRE_TO_PURPLE_RATE_R;
        m_col.g -= EFFECT_WHITE_FIRE_TO_PURPLE_RATE_G;

        // 回転
        m_fAngle += m_fRotSpeed;

        // 色を変更
        CScene2D::SetColor(m_col);

        break;

        // パーティクルによる爆発
    case CEffect::TYPE_PART_EXPLOSION:

        // だんだん透明に
        m_col.a -= EFFECT_EXPLOSION_FADE_OUT_RATE;

        // 移動量分、上へ
        pos.y -= m_move.y;

        // 赤に近づけていく
        m_col.g -= EFFECT_EXPLOSION_TO_RED_RATE;
        m_col.b -= EFFECT_EXPLOSION_TO_RED_RATE;

        // 回転
        m_fAngle += m_fRotSpeed;

        // 色を変更
        CScene2D::SetColor(m_col);

        break;

        // 小爆発
    case CEffect::TYPE_SMALL_EXPLOSION:

        // だんだん透明に
        m_col.a -= EFFECT_SMALL_EXPLOSION_FADE_OUT_RATE;

        // 移動量分、上へ
        pos.y -= m_move.y;

        // 回転
        m_fAngle += m_fRotSpeed;

        // 色を変更
        CScene2D::SetColor(m_col);

        break;

        // 雲
    case CEffect::TYPE_CLOUD:

        // 移動量を加算
        pos += m_move;

        // 回転
        m_fAngle += m_fRotSpeed;

        // 左端、下端に達したら透明度を下げる
        if (pos.x <= 0.0f || pos.y >= SCREEN_HEIGHT)
        {
            m_col.a -= EFFECT_CLOUD_FADE_OUT_RATE;
        }

        // 色を変更
        CScene2D::SetColor(m_col);

        break;

        // ボーナスポイント
    case CEffect::TYPE_BONUS_POINT:

        // 徐々に上に
        pos.y -= BONUS_POINT_MOVE_RATE;

        // だんだん透明に
        m_col.a -= BONUS_POINT_FADE_RATE;

        // 色を変更
        CScene2D::SetColor(m_col);

        break;
    }

    // ％表示の処理
    if (m_type == TYPE_25 || m_type == TYPE_50 || m_type == TYPE_75 || m_type == TYPE_100)
    {
        // α値を下げるフラグがfalseなら下げる、trueなら上げる
        if (m_bAlphaDown == false)
        {
            m_col.a += PERCENTAGE_FADE_RATE;
        }
        else
        {
            m_col.a -= PERCENTAGE_FADE_RATE;
        }

        // 最大まで表示したら、α値を下げる
        if (m_col.a >= 1.0f)
        {
            m_col.a = 1.0f;
            m_bAlphaDown = true;
        }

        // 色を変更
        CScene2D::SetColor(m_col);
    }

    // 位置を反映
    SetPosition(pos);

    // サイズを反映
    SetSize(size);

    // 頂点座標を変更
    // 回転とそうでないもので分ける(タイトルの炎系は、結局回転させていない)
    if (m_type == TYPE_FIRE || m_type == TYPE_LEAF || m_type == TYPE_BRANCH 
        || m_type == TYPE_CLOUD || m_type == TYPE_PART_EXPLOSION || m_type == TYPE_SMALL_EXPLOSION)
    {
        SetRotVertex(m_fAngle);
    }
    else
    {
        SetVertex();
    }

    // サイズがなくなる、透明になる、ライフがなくなったら未使用に
    if (size.x < 0 || size.y < 0 || m_col.a < 0 || m_nLife <= 0)
    {
        bUse = false;
    }

    // 雷の弾、ステージ3の炎、雲以外は、画面外で未使用に
    if (m_type == TYPE_LIGHT_BULLET || m_type == TYPE_FIRE || m_type == TYPE_CLOUD)
    {
        // ゲーム外なら
        if (OutGame2D(&pos, &size) == true)
        {
            // 未使用に
            bUse = false;
        }
    }
    else
    {
        // 画面外なら
        if (OutScreen2D(&pos, &size) == true)
        {
            // 未使用に
            bUse = false;
        }
    }

    // 使用のフラグがfalseなら消滅
    if (bUse == false)
    {
        Uninit(); // これより下にコードは書かない
    }
}

//========================================
// エフェクトの描画処理
//========================================
void CEffect::Draw(void)
{
    switch (m_type)
    {
        // パーティクルは加算合成
    case CEffect::TYPE_PARTICLE:

        SetAdditiveSynthesis();

        break;

        // チャージは加算合成&αテスト
    case CEffect::TYPE_CHARGE:

        SetAdditiveSynthesis();
        SetAlphaTest();

        break;

        // チャージのパーティクルは加算合成
    case CEffect::TYPE_CHARGE_PART:

        SetAdditiveSynthesis();

        break;

        // 反転チャージは反転合成
    case CEffect::TYPE_REVERSE_CHARGE:

        SetNega();

        break;

        // ドラゴンオーラは加算合成
    case CEffect::TYPE_DRAGON_AURA:

        SetAdditiveSynthesis();

        break;

        // 二重ウェーブは加算合成
    case CEffect::TYPE_DOUBLE_WAVE:

        SetAdditiveSynthesis();

        break;

        // 白パーティクル(JPG)は加算合成
    case CEffect::TYPE_PARTICLE_JPG:

        SetAdditiveSynthesis();

        break;

        // 炎は加算合成
    case CEffect::TYPE_FIRE:

        SetAdditiveSynthesis();

        break;

        // マグマは加算合成
    case CEffect::TYPE_MAGMA:

        SetAdditiveSynthesis();

        break;

        // 赤い炎は加算合成
    case CEffect::TYPE_RED_FIRE:

        SetAdditiveSynthesis();

        break;

        // 青い炎は加算合成
    case CEffect::TYPE_BLUE_FIRE:

        SetAdditiveSynthesis();

        break;

        // 黄色い炎は加算合成
    case CEffect::TYPE_YELLOW_FIRE:

        SetAdditiveSynthesis();

        break;

        // 紫の炎は加算合成
    case CEffect::TYPE_PURPLE_FIRE:

        SetAdditiveSynthesis();

        break;

        // 雲は加算合成
    case CEffect::TYPE_CLOUD:

        SetAdditiveSynthesis();

        break;

        // パーティクルによる爆発は加算合成
    case CEffect::TYPE_PART_EXPLOSION:

        SetAdditiveSynthesis();

        break;

        // 小爆発は加算合成
    case CEffect::TYPE_SMALL_EXPLOSION:

        SetAdditiveSynthesis();

        break;
    }

    // 描画処理
    CScene2D::Draw();
}

//========================================
// エフェクトの画像ロード処理
//========================================
HRESULT CEffect::Load(void)
{
    // レンダラーからデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    //テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/particle001.png", &m_apTexture[CEffect::TYPE_PARTICLE]);	    // パーティクル
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect001.png", &m_apTexture[CEffect::TYPE_WAVE]);	            // 衝撃波
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect002.png", &m_apTexture[CEffect::TYPE_EXPLOSION]);	    // 爆発
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/particle002.png", &m_apTexture[CEffect::TYPE_CHARGE]);	        // チャージ
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/particle001.png", &m_apTexture[CEffect::TYPE_CHARGE_PART]);    // チャージのパーティクル
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet002.png", &m_apTexture[CEffect::TYPE_LIGHT_BULLET]);	    // 雷バレット
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect004.png", &m_apTexture[CEffect::TYPE_SHOCK]);	        // 電撃
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect005.png", &m_apTexture[CEffect::TYPE_BIG_EXPLOSION]);    // 大爆発
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect006.png", &m_apTexture[CEffect::TYPE_RED_SPARK]);        // 赤火花
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect007.png", &m_apTexture[CEffect::TYPE_GREEN_SPARK]);      // 緑火花
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect008.png", &m_apTexture[CEffect::TYPE_BLUE_SPARK]);       // 青火花
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect009.png", &m_apTexture[CEffect::TYPE_BLACK_SPARK]);      // 黒火花
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect010.png", &m_apTexture[CEffect::TYPE_WATER_EXPLOSION]);  // 水爆発
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/reverse000.png", &m_apTexture[CEffect::TYPE_REVERSE_CHARGE]);	// 反転チャージ
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect011.png", &m_apTexture[CEffect::TYPE_25]);               // 25％
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect012.png", &m_apTexture[CEffect::TYPE_50]);               // 50％
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect013.png", &m_apTexture[CEffect::TYPE_75]);               // 75％
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect014.png", &m_apTexture[CEffect::TYPE_100]);              // 100％
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/particle001.png", &m_apTexture[CEffect::TYPE_DRAGON_AURA]);	// ドラゴンオーラ
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect101.jpg", &m_apTexture[CEffect::TYPE_DOUBLE_WAVE]);	    // 二重ウェーブ
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect000.jpg", &m_apTexture[CEffect::TYPE_PARTICLE_JPG]);	    // パーティクル(jpg)
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect100.jpg", &m_apTexture[CEffect::TYPE_FIRE]);	            // 炎
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect000.jpg", &m_apTexture[CEffect::TYPE_MAGMA]);	        // マグマ
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect015.png", &m_apTexture[CEffect::TYPE_LEAF]);             // 葉っぱ
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect016.png", &m_apTexture[CEffect::TYPE_BRANCH]);           // 枝
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect100.jpg", &m_apTexture[CEffect::TYPE_RED_FIRE]);	        // 赤い炎
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect100.jpg", &m_apTexture[CEffect::TYPE_BLUE_FIRE]);	    // 青い炎
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect100.jpg", &m_apTexture[CEffect::TYPE_YELLOW_FIRE]);	    // 黄色い炎
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect100.jpg", &m_apTexture[CEffect::TYPE_PURPLE_FIRE]);	    // 紫の炎
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect100.jpg", &m_apTexture[CEffect::TYPE_PART_EXPLOSION]);	// パーティクルによる爆発
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect100.jpg", &m_apTexture[CEffect::TYPE_SMALL_EXPLOSION]);	// 小爆発
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect100.jpg", &m_apTexture[CEffect::TYPE_CLOUD]);	        // 雲
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect017.png", &m_apTexture[CEffect::TYPE_BONUS_POINT]);      // ボーナスポイント

    return S_OK;
}

//========================================
// エフェクトの画像破棄処理
//========================================
void CEffect::Unload(void)
{
    // テクスチャの破棄
    for (int nCntTexture = 0; nCntTexture < CEffect::TYPE_MAX; nCntTexture++)
    {
        if (m_apTexture[nCntTexture] != NULL)
        {
            m_apTexture[nCntTexture]->Release();
            m_apTexture[nCntTexture] = NULL;
        }
    }
}

//================================
// エフェクトの作成
//================================
CEffect *CEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, CEffect::TYPE type)
{
    // 変数宣言
    CEffect *pEffect = NULL;

    // メモリを確保
    pEffect = new CEffect;

    // タイプのみ、テクスチャを割り当てのために結びつけておく
    pEffect->m_type = type;

    // 初期化
    pEffect->Init(pos, size);

    // メンバ変数を結びつける
    pEffect->m_col = col;

    // 種類ごとに、ランダムな移動量を取得
    float fSpeedX = 0.0f;
    float fSpeedY = 0.0f;
    switch (type)
    {
    case TYPE_DRAGON_AURA:

        // 縦移動の値を決める
        fSpeedY = float(rand() % 800 + 300) / 100.0f;

        // 移動量に結びつける
        pEffect->m_move.y = fSpeedY;

        break;

    case TYPE_FIRE:

        // 縦移動の値を決める
        fSpeedY = float(rand() % 150 + 100) / 100.0f;

        // 移動量に結びつける
        pEffect->m_move.y = fSpeedY;

        // ランダムに回転
        pEffect->m_fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
        pEffect->m_fRotSpeed = float(rand() % 100) / 1000.0f - float(rand() % 100) / 1000.0f;

        break;

    case TYPE_MAGMA:

        // 移動量に結びつける
        pEffect->m_move.x = -FLOWING_STAGE;

        // 赤に近づけるため
        pEffect->m_bAlphaDown = true;

        // フェード割合を、ランダムに取得
        pEffect->m_fRotSpeed = float(rand() % 1000) / 20000.0f;

        break;

    case TYPE_LEAF:

        // 移動の値を決める
        fSpeedX = float(rand() % 700 + 200) / 100.0f;
        fSpeedY = float(rand() % 500 + 300) / 100.0f;
        pEffect->m_move.x = -fSpeedX;   // 右から左へ飛ばすため、マイナス
        pEffect->m_move.y = -fSpeedY;   // 下から上に飛ばすため、マイナス

        // ランダムに回転
        pEffect->m_fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
        pEffect->m_fRotSpeed = float(rand() % 100) / 1000.0f - float(rand() % 100) / 1000.0f;

        break;

    case TYPE_BRANCH:

        // 移動の値を決める
        fSpeedX = float(rand() % 700 + 200) / 100.0f;
        fSpeedY = float(rand() % 500 + 300) / 100.0f;
        pEffect->m_move.x = -fSpeedX;   // 右から左へ飛ばすため、マイナス
        pEffect->m_move.y = -fSpeedY;   // 下から上に飛ばすため、マイナス

        // ランダムに回転
        pEffect->m_fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
        pEffect->m_fRotSpeed = float(rand() % 100) / 1000.0f - float(rand() % 100) / 1000.0f;

        break;

    case TYPE_RED_FIRE:

        // サイズ拡大
        size.x += EFFECT_RED_FIRE_EXTEND_RATE;
        size.y += EFFECT_RED_FIRE_EXTEND_RATE;

        // 縦移動の値を決める
        fSpeedY = float(rand() % 150 + 100) / 100.0f;

        // 移動量に結びつける
        pEffect->m_move.y = fSpeedY;

        // ランダムに回転
        pEffect->m_fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
        pEffect->m_fRotSpeed = float(rand() % 100) / 1000.0f - float(rand() % 100) / 1000.0f;

        break;


    case TYPE_BLUE_FIRE:

        // 縦移動の値を決める
        fSpeedY = float(rand() % 150 + 100) / 100.0f;

        // 移動量に結びつける
        pEffect->m_move.y = fSpeedY;

        // ランダムに回転
        pEffect->m_fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
        pEffect->m_fRotSpeed = float(rand() % 100) / 1000.0f - float(rand() % 100) / 1000.0f;

        break;


    case TYPE_YELLOW_FIRE:

        // 縦移動の値を決める
        fSpeedY = float(rand() % 150 + 100) / 100.0f;

        // 移動量に結びつける
        pEffect->m_move.y = fSpeedY;

        // ランダムに回転
        pEffect->m_fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
        pEffect->m_fRotSpeed = float(rand() % 100) / 1000.0f - float(rand() % 100) / 1000.0f;

        break;

    case TYPE_PURPLE_FIRE:

        // 縦移動の値を決める
        fSpeedY = float(rand() % 150 + 100) / 100.0f;

        // 移動量に結びつける
        pEffect->m_move.y = fSpeedY;

        // ランダムに回転
        pEffect->m_fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
        pEffect->m_fRotSpeed = float(rand() % 100) / 1000.0f - float(rand() % 100) / 1000.0f;

        break;

    case TYPE_PART_EXPLOSION:

        // 縦移動の値を決める
        fSpeedY = float(rand() % 150 + 100) / 100.0f;

        // 移動量に結びつける
        pEffect->m_move.y = fSpeedY;

        // ランダムに回転
        pEffect->m_fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
        pEffect->m_fRotSpeed = float(rand() % 25) / 1000.0f - float(rand() % 25) / 1000.0f;

        break;

    case TYPE_SMALL_EXPLOSION:

        // サイズによって、上昇するスピードを変える
        if (size.x < EFFECT_SMALL_EXPLOSION_BORDER_SIZE_THAT_CHANGE_SPEED)
        {
            // 縦移動の値を決める
            fSpeedY = float(rand() % 75 + 50) / 100.0f;
        }
        else
        {
            // 縦移動の値を決める
            fSpeedY = float(rand() % 150 + 100) / 100.0f;
        }

        // 移動量に結びつける
        pEffect->m_move.y = fSpeedY;

        // ランダムに回転
        pEffect->m_fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
        pEffect->m_fRotSpeed = float(rand() % 100) / 1000.0f - float(rand() % 100) / 1000.0f;

        break;

    case TYPE_CLOUD:

        // 空の中なら
        if (CBg::GetInTheSky() == true)
        {
            // 左下に流れる
            pEffect->m_move.x = -sinf(D3DX_PI / 4) * EFFECT_CLOUD_FLOWING_SPEED;
            pEffect->m_move.y = cosf(D3DX_PI / 4) * EFFECT_CLOUD_FLOWING_SPEED;
        }
        // それ以外は
        else
        {
            // ボス戦時かどうかで速度を変える
            if (CGame::GetBossBattle() == false)
            {
                // 通常は、左へ流れる
                pEffect->m_move.x = -EFFECT_CLOUD_FLOWING_SPEED;
            }
            else
            {
                // ボス戦時は、左へゆっくり流れる
                pEffect->m_move.x = -EFFECT_CLOUD_FLOWING_SPEED_BOSS;
            }
        }

        // ランダムに回転
        pEffect->m_fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
        pEffect->m_fRotSpeed = float(rand() % 25) / 1000.0f - float(rand() % 25) / 1000.0f;

        break;
    }

    return pEffect;
}

//====================================================
// エフェクトから見たドラゴンへの角度
//====================================================
float CEffect::GetAngleToDragon(void)
{
    // 変数宣言
    D3DXVECTOR3 pos = GetPosition();           // 位置を取得
    float fTargetPosX = 0.0f;                  // 目標のX座標
    float fTargetPosY = 0.0f;                  // 目標のY座標
    float fMyPosX = 0.0f;                      // 自身のX座標
    float fMyPosY = 0.0f;                      // 自身のY座標
    float fAngle = 0.0f;                       // 求める角度

    // 目標の座標と自身の座標
    fTargetPosX = ENEMY_BOSS_DRAGON_STOP_POS_X - 10.0f, fTargetPosY = ENEMY_BOSS_DRAGON_STOP_POS_Y;
    fMyPosX = pos.x, fMyPosY = pos.y;

    // 角度を決める
    fAngle = atan2f((fMyPosX - fTargetPosX), (fMyPosY - fTargetPosY));

    return fAngle;
}
