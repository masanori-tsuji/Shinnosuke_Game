//====================================================================
//
// プレイヤーの処理 (player.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene2d.h"

//================================================
// マクロ定義
//================================================
#define PLAYER_START_POS_X 200.0f                                           // 自機の開始X座標
#define PLAYER_START_POS_Y (SCREEN_HEIGHT / 2)                              // 自機の開始Y座標
#define PLAYER_POS D3DXVECTOR3(PLAYER_START_POS_X, PLAYER_START_POS_Y, 0.0f)// 自機の位置
#define PLAYER_SIZE D3DXVECTOR3(100.0f, 100.0f, 0.0f)		                // 自機の描画サイズ
#define PLAYER_COLLISION_SIZE D3DXVECTOR3(60.0f, 60.0f, 0.0f)	            // 自機の衝突サイズ
#define PLAYER_COLLISION_ITEM_SIZE D3DXVECTOR3(75.0f, 75.0f, 0.0f)	        // 自機のアイテムに対する衝突サイズ
#define PLAYER_START_STOCK 3                                                // 開始時のストック数
#define PLAYER_INVINCIBLE_FRAME 150                                         // プレイヤーの無敵時間
#define PLAYER_ENABLE_SHOT_FRAME 110                                        // プレイヤーが弾を撃つことができるようになるフレーム
#define PLAYER_MAX_STOCK 5                                                  // プレイヤーの最大ストック
#define PLAYER_MAX_BARRIER 5                                                // プレイヤーの最大バリア
#define PLAYER_ADD_WATAR_POS -45.0f                                         // プレイヤーの水弾の発射位置を中心から加算
#define PLAYER_MOVE_LIMITED_BY_UI 120.0f                                    // UIによる、プレイヤーの移動範囲制限

// スピード周り
#define PLAYER_SPEED_1 3.5f                                     // 自機の速さ1
#define PLAYER_SPEED_2 6.0f                                     // 自機の速さ2
#define PLAYER_SPEED_3 10.0f                                    // 自機の速さ3
#define PLAYER_SPEED_4 15.0f                                    // 自機の速さ4
#define PLAYER_FIRST_SPEED_RANK 2                               // 自機の最初のスピードランク
#define PLAYER_MAX_SPEED_RANK 4                                 // 自機の最大スピードランク
#define PLAYER_POS_Y_DOWN_IN_WATAR 0.5f                         // 水の中で、徐々に位置が下がる

#define MAX_CHARGE_TIME 100                                     // 最大チャージ時間
#define PLAYER_TILT_RATE 0.01f                                  // プレイヤーが傾く割合

//================================================
// クラス宣言
//================================================

// プレイヤークラス
class CPlayer : public CScene2D
{
public:
    CPlayer();
    ~CPlayer();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    void ChangeShotAngle(void);                         // 水の発射角度調整
    void ChangeSpeed(void);                             // スピード変更
    D3DXVECTOR3 ControlPlayer(D3DXVECTOR3 pos);         // 移動操作
    void ShotBullet(D3DXVECTOR3 pos, D3DXVECTOR3 size); // 攻撃操作
    static HRESULT Load(void);
    static void Unload(void);
    static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, float fAngle, int nCntInvincibleFrame);
    float GetShotAngle(void) { return m_fShotAngle; }               // メンバ変数の発射角度を返す
    int GetChargeTime(void) { return m_nChargeTime; }               // メンバ変数のチャージ時間を返す
    int GetBarrier(void) { return m_nBarrier; }                     // メンバ変数のバリアを返す
    int GetSpeedRank(void) { return m_nSpeedRank; }                 // メンバ変数のスピードランクを返す
    int GetInvincibleFrame(void) { return m_nCntInvincibleFrame; }  // メンバ変数の無敵時間を返す
    void Hit(void);                                                 // 攻撃を受けた時
    D3DXVECTOR3 HitFloor(D3DXVECTOR3 pos, D3DXVECTOR3 size);        // 床に当たったかどうか
    void SetStartPos(void);                                         // スタート位置に設定

    float GetPlayerAngle(void) { return m_fPlayerAngle; }           // プレイヤーの角度を取得

private:
    int m_nAttackCoolTime;                  // 攻撃のクールタイム
    float m_fShotAngle;                     // 水の発射角度
    float m_fPlayerAngle;                   // 自機の角度
    int m_nChargeTime;                      // 雷のチャージ時間
    int m_nBarrier;                         // バリアの枚数
    int m_nCntInvincibleFrame;              // 無敵時間
    float m_fSpeed;                         // 速さ
    int m_nSpeedRank;                       // スピードランク
    bool m_bUse;                            // 使用フラグ
    static LPDIRECT3DTEXTURE9 m_pTexture;   // 読み込んだテクスチャを保存する
};

#endif