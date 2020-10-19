//====================================================================
//
// 弾の処理 (bullet.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene2d.h"

//================================================
// マクロ定義
//================================================
#define BULLET_COUNTERATTACK_FIRE_SPEED 6.0f                            // 反撃弾の速さ(炎)
#define BULLET_COUNTERATTACK_WATAR_SPEED 4.5f                           // 反撃弾の速さ(水)
#define BULLET_COUNTERATTACK_LIGHT_SPEED 7.0f                           // 反撃弾の速さ(雷)
#define BULLET_COUNTERATTACK_LIFE 1000                                  // 反撃弾のライフ
#define BULLET_COUNTERATTACK_FIRE_SCORE 390                             // 反撃弾のスコア(炎)
#define BULLET_COUNTERATTACK_WATAR_SCORE 1800                           // 反撃弾のスコア(水)
#define BULLET_COUNTERATTACK_GRASS_SCORE 1200                           // 反撃弾のスコア(草)
#define BULLET_COUNTERATTACK_FIRE_SIZE D3DXVECTOR3(25.0f, 25.0f, 0.0f)	// 反撃弾のサイズ(炎)
#define BULLET_COUNTERATTACK_WATAR_SIZE D3DXVECTOR3(32.5f, 32.5f, 0.0f)	// 反撃弾のサイズ(水)
#define BULLET_COUNTERATTACK_LIGHT_SIZE D3DXVECTOR3(27.5f, 27.5f, 0.0f)	// 反撃弾のサイズ(雷)

#define BULLET_FIRE_SIZE D3DXVECTOR3(25.0f, 25.0f, 0.0f)	// 炎の弾のサイズ
#define BULLET_FIRE_SPEED 16.0f                             // 炎の弾の速さ
#define BULLET_FIRE_LIFE 1000                               // 炎の弾のライフ
#define BULLET_FIRE_DAMAGE 15                               // 炎の弾のダメージ
#define BULLET_FIRE_COOL_TIME 6                             // 炎の弾のクールタイム
#define BULLET_FIRE_CHANGE_ENEMY_COLOR_FRAME 15             // 炎の弾が敵の色を変えるフレーム
#define BULLET_FIRE_COLOR D3DXCOLOR(1.0f,0.0f,0.0f,0.7f)    // 炎の弾の色

#define BULLET_WATER_SIZE D3DXVECTOR3(80.0f, 80.0f, 0.0f)	     // 水の弾のサイズ
#define BULLET_WATER_SPEED 12.5f                                 // 水の弾の速さ
#define BULLET_WATER_LIFE 1000                                   // 水の弾のライフ
#define BULLET_WATER_DAMAGE 100                                  // 水の弾のダメージ
#define BULLET_WATAR_HELPER_DAMAGE 20                            // ヘルパーの水の弾のダメージ
#define BULLET_WATER_COOL_TIME 25                                // 水の弾のクールタイム
#define BULLET_WATER_CHANGE_ENEMY_COLOR_FRAME 20                 // 水の弾が敵の色を変えるフレーム
#define BULLET_WATER_AVOID_MULTIPLE_HITS_FRAME 24                // 水の弾の多段ヒット回避フレーム(クールタイム考慮)
#define BULLET_WATER_COLOR D3DXCOLOR(0.2f, 0.858f, 0.98f, 1.0f)  // 水の弾の色
#define BULLET_WATER_AIR_RESISTANCE 0.4f                         // 水の弾の空気抵抗

#define BULLET_LIGHT_SPEED 22.5f                            // 雷の弾の速さ
#define BULLET_LIGHT_LIFE 1000                              // 雷の弾のライフ
#define BULLET_LIGHT_EXTEND_RATE 1.0f                       // 雷の弾の拡大割合
#define BULLET_LIGHT_HELPER_DAMAGE 20                       // ヘルパーの雷の弾のダメージ
#define BULLET_LIGHT_MIN_SIZE 50.0f                         // 雷の最小サイズ
#define BULLET_LIGHT_MAX_SIZE 150.0f                        // 雷の最大サイズ
#define BULLET_LIGHT_MAX_DAMAGE 250                         // 雷の最大ダメージ
#define BULLET_LIGHT_COOL_TIME 20                           // 雷の弾のクールタイム
#define BULLET_LIGHT_CHANGE_ENEMY_COLOR_FRAME 25            // 雷の弾が敵の色を変えるフレーム
#define BULLET_LIGHT_AVOID_MULTIPLE_HITS_FRAME 19           // 雷の弾の多段ヒット回避フレーム(クールタイム考慮)
#define BULLET_LIGHT_COLOR D3DXCOLOR(1.0f,0.99f,0.0f,1.0f)  // 雷のエフェクトの色

#define BULLET_GRASS_LIFE 1000                              // 草の弾のライフ
#define BULLET_GRASS_DAMAGE 10                              // 草の弾のダメージ
#define BULLET_GRASS_COLOR D3DXCOLOR(0.0f,1.0f,0.0f,0.9f)   // 草の弾のエフェクトの色

// 弾が床に当たった時のエフェクトの位置調整
#define BULLET_HIT_FLOOR_EFFECT_ADD_POS D3DXVECTOR3(0.0f,-5.0f,0.0f)

// ライフをカウンタ代わりに使用
#define BULLET_LIFE_COUNTER 1000

// 貫通を考慮しない、状態カウンタの値
#define DONT_GO_THROUGH 19

// レイニーバレット
#define BULLET_RAINY_FIRST_SPEED 2.0f                                           // 初速
#define BULLET_RAINY_ACCEL_SPEED 10.0f                                          // 加速後スピード
#define BULLET_RAINY_LIFE_ACCEL 820                                             // 加速するカウンタ
#define BULLET_SET_FIRE_EFFECT_COUNT 2                                          // 炎をセットするカウンタ
#define CHANGE_LIGHT (D3DXVECTOR3(-1.0f, -1.0f, 0.0f))                          // 雷に変える
#define BULLET_BULE_FIRE_COLOR D3DXCOLOR(D3DXCOLOR(0.588f,0.937f,1.0f,0.6f))    // 青い炎の色

// 水ホーミングショット
#define BULLET_HOMING_WATAR_SPEED 7.5f        // ホーミング速さ
#define BULLET_HOMING_LIFE_STOP_HOMING 907    // ホーミングをやめるカウンタ

// 炎バースト
#define BULLET_FIRE_BURST_SPEED 8.75f                                       // 初速(エネミーの発射時と同じ値にする)
#define BULLET_FIRE_BURST_LIFE_ACCEL 880                                    // バーストするカウンタ
#define BULLET_FIRE_BURST_DECELERATE 0.075f                                 // 減速の割合
#define BULLET_FIRE_BURST_CREATE_BULLET_SIZE D3DXVECTOR3(30.0f,30.0f,0.0f)  // 生成する炎バレットの大きさ
#define BULLET_FIRE_BURST_CREATE_BULLET_SPEED 5.0f                          // 生成する炎バレットの速さ
#define BULLET_FIRE_BURST_SCORE 2100                                        // 打ち消した際のスコア

// バーニングバレット
#define BULLET_BURNING_FIRST_SPEED 2.0f                                     // 初速
#define BULLET_BURNING_ACCEL_SPEED 11.0f                                    // 加速後スピード
#define BULLET_BURNING_LIFE_ACCEL 817                                       // 加速するカウンタ
#define BULLET_BURNING_BURST_SCORE 400                                      // 打ち消した際のスコア
#define CHANGE_MULTI (D3DXVECTOR3(-0.1f, -0.1f, 0.0f))                      // マルチに変える

// ウィンドバレット
#define BULLET_WIND_FIRST_SPEED 8.75f                                       // 初速(エネミーの発射時と同じ値にする)
#define BULLET_WIND_DECELERATE 0.075f                                       // 減速の割合
#define BULLET_WIND_ACCEL_SPEED 17.5f                                       // 加速後スピード
#define BULLET_WIND_LIFE_ACCEL 880                                          // 急加速するカウンタ

//================================================
// クラス宣言
//================================================

// 弾クラス
class CBullet : public CScene2D
{
public:
    // 弾の種類
    typedef enum
    {
        TYPE_FIRE = 0,      // 炎
        TYPE_WATER,         // 水
        TYPE_HELPER_WATER,  // ヘルパーの水
        TYPE_LIGHT,         // 雷
        TYPE_GRASS,         // 草
        TYPE_RAINY,         // レイニーバレット
        TYPE_WATAR_HOMING,  // 水ホーミング
        TYPE_FIRE_BURST,    // 炎バースト
        TYPE_BURNING,       // バーニングバレット
        TYPE_WIND,          // ウィンドバレット
        TYPE_MAX            // 種類の最大数
    }TYPE;
    CBullet();
    ~CBullet();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CBullet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, TYPE type, TRIBE tribe);
    TRIBE GetTribe(void) { return m_tribe; }                  // 種族を取得
    ELEMENT GetElement(void) { return m_element; }            // 属性を取得
    TYPE GetType(void) { return m_type; }                     // 種類を取得
    bool HitFloor(D3DXVECTOR3 pos, D3DXVECTOR3 size);         // 床に当たったかどうか
    float GetAngleToPlayer(void);                             // 自機への角度を取得
    void FireBurst(D3DXVECTOR3 pos);                          // 炎バースト
    void SmallExplosion(D3DXVECTOR3 pos, D3DXVECTOR3 size);   // 小爆発

private:
    static LPDIRECT3DTEXTURE9 m_apTexture[TYPE_MAX];   // 読み込んだテクスチャを保存する
    D3DXVECTOR3 m_posOld;                              // 1F前の位置
    D3DXVECTOR3 m_move;                                // 移動量
    float m_fSpeed;                                    // 速度
    int m_nLife;                                       // ライフ
    int m_nDamage;                                     // ダメージ
    TYPE m_type;                                       // 弾の種類
    ELEMENT m_element;                                 // 弾の属性
    TRIBE m_tribe;                                     // 弾の種族
};

#endif