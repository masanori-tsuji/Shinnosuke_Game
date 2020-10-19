//====================================================================
//
// ヘルパーの処理 (helper.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _HELPER_H_
#define _HELPER_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene2d.h"

//================================================
// マクロ定義
//================================================
#define HELPER_START_POS_X 200.0f                                           // ヘルパーの開始X座標
#define HELPER_START_POS_Y (SCREEN_HEIGHT / 2 - 120.0f)                     // ヘルパーの開始Y座標
#define HELPER_POS D3DXVECTOR3(HELPER_START_POS_X, HELPER_START_POS_Y, 0.0f)// ヘルパーの位置
#define HELPER_SIZE D3DXVECTOR3(100.0f, 100.0f, 0.0f)		                // ヘルパーの描画サイズ
#define HELPER_COLLISION_SIZE D3DXVECTOR3(40.0f, 40.0f, 0.0f)	            // ヘルパーの衝突サイズ
#define HELPER_INVINCIBLE_FRAME 120                                         // ヘルパーの無敵時間
#define HELPER_ENABLE_SHOT_FRAME 75                                         // ヘルパーが弾を撃つことができるようになるフレーム
#define HELPER_MOVE_LIMITED_BY_UI 120.0f                                    // UIによる、ヘルパーの移動範囲制限
#define HELPER_KNOCKBACK_CONTROL 0.9f                                       // ヘルパーのノックバックを制御
#define HELPER_KNOCKBACK_VALUE 27.5f                                        // ヘルパーのノックバック量
#define HELPER_TILT_RATE 0.01f                                              // ヘルパーが傾く割合

// 弾周り
#define HELPER_BULLET_SIZE D3DXVECTOR3(30.0f, 30.0f, 0.0f)		           // ヘルパーの弾サイズ
#define HELPER_BULLET_SPEED 17.5f                                          // ヘルパーの弾の速度
#define HELPER_BULLET_FIRE_COOL_TIME 23                                    // ヘルパーの炎弾のクールタイム
#define HELPER_BULLET_WATER_COOL_TIME 23                                   // ヘルパーの水弾のクールタイム
#define HELPER_BULLET_LIGHT_COOL_TIME 23                                   // ヘルパーの雷弾のクールタイム

// スピード周り
#define HELPER_SPEED 7.5f                                       // ヘルパーの速さ
#define HELPER_POS_Y_DOWN_IN_WATAR 0.5f                         // 水の中で、徐々に位置が下がる

//================================================
// クラス宣言
//================================================

// ヘルパークラス
class CHelper : public CScene2D
{
public:
    CHelper();
    ~CHelper();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    D3DXVECTOR3 ControlHelper(D3DXVECTOR3 pos);         // 移動操作
    void ShotBullet(D3DXVECTOR3 pos, D3DXVECTOR3 size); // 攻撃操作
    static HRESULT Load(void);
    static void Unload(void);
    static CHelper *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, float fAngle, int nCntInvincibleFrame);
    void Hit(D3DXVECTOR3 posOld);                                                       // 攻撃を受けた時
    D3DXVECTOR3 HitFloor(D3DXVECTOR3 pos, D3DXVECTOR3 size);                            // 床に当たったかどうか
    void SetStartPos(void);                                                             // スタート位置に設定

private:
    D3DXVECTOR3 m_move;                     // 移動量
    float m_fHelperAngle;                   // ヘルパーの角度
    int m_nAttackCoolTime;                  // 攻撃のクールタイム
    int m_nCntInvincibleFrame;              // 無敵時間
    float m_fSpeed;                         // 速さ
    static LPDIRECT3DTEXTURE9 m_pTexture;   // 読み込んだテクスチャを保存する
};

#endif