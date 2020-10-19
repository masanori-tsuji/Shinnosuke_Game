//====================================================================
//
// エフェクトの処理 (effect.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene2d.h"

//================================================
// マクロ定義
//================================================
#define EXPLOSION_SIZE D3DXVECTOR3(50.0f, 50.0f, 0.0f)          // 爆発の大きさ
#define WATAR_EXPLOSION_SIZE D3DXVECTOR3(100.0f, 100.0f, 0.0f)  // 水爆発の大きさ

#define CHARGE_COLOR D3DXCOLOR(0.0f,1.0f,0.0f,0.9f)             // チャージのカラー
#define CHARGE_PART_SIZE 30.0f                                  // チャージのパーティクルの大きさ
#define CHARGE_PART_SPEED 6.0f                                  // チャージのパーティクルの速さ
#define CHARGE_SIZE 200.0f                                      // チャージのサイズ
#define CHARGE_RATE_SIZE 2.0f                                   // チャージの拡大割合

#define PERCENTAGE_SIZE D3DXVECTOR3(200.0f, 100.0f, 0.0f)   // ％表示の大きさ
#define PERCENTAGE_COLOR D3DXCOLOR(1.0f,1.0f,1.0f,0.1f)     // ％表示の色
#define PERCENTAGE_FADE_RATE 0.01f                          // ％表示のフェード割合

#define BONUS_POINT_SIZE D3DXVECTOR3(200.0f, 100.0f, 0.0f)   // ボーナスポイントの大きさ
#define BONUS_POINT_MOVE_RATE 1.0f                           // ボーナスポイントの上へ行く移動量
#define BONUS_POINT_FADE_RATE 0.025f                         // ボーナスポイントの消えていく量
#define BONUS_POINT_ADD_POS_Y -50.0f                         // ボーナスポイントの加算位置Y

#define EFFECT_DOUBLE_WAVE_SHOT_SIZE D3DXVECTOR3(200.0f, 200.0f, 0.0f)                // 二重ウェーブの大きさ(ショット時)
#define EFFECT_DOUBLE_WAVE_SHOT_COLOR D3DXCOLOR(1.0f,0.99f,0.0f,1.0f)                 // 二重ウェーブの色(ショット時)
#define EFFECT_DOUBLE_WAVE_ITEM_SIZE D3DXVECTOR3(150.0f, 150.0f, 0.0f)                // 二重ウェーブの大きさ(アイテム取得時)
#define EFFECT_DOUBLE_WAVE_ITEM_BARRIER_COLOR D3DXCOLOR(0.2f, 0.858f, 0.98f, 1.0f)    // 二重ウェーブの色(バリア取得時)
#define EFFECT_DOUBLE_WAVE_ITEM_STOCK_COLOR D3DXCOLOR(1.0f, 0.262f, 0.447f, 1.0f)     // 二重ウェーブの色(ストック取得時)
#define EFFECT_DOUBLE_WAVE_BLAZE_SIZE D3DXVECTOR3(250.0f, 250.0f, 0.0f)               // 二重ウェーブの大きさ(ボスブレイズ)
#define EFFECT_DOUBLE_WAVE_BLAZE_COLOR D3DXCOLOR(0.627f,0.007f,0.827f, 1.0f)          // 二重ウェーブの色(ボスブレイズ)
#define EFFECT_DOUBLE_WAVE_EXTEND_RATE 20.0f                                          // 二重ウェーブの拡大割合
#define EFFECT_DOUBLE_WAVE_FADE_OUT_RATE 0.05f                                        // 二重ウェーブのフェードアウト割合

#define EFFECT_FIRE_FIRST_COLOR D3DXCOLOR(1.0f,0.99f,0.0f,0.1f)      // 炎の色
#define EFFECT_FIRE_TO_RED_RATE 0.015f                               // 赤に近づけていく割合
#define EFFECT_FIRE_FADE_IN_RATE 0.02f                               // 炎のフェードイン割合
#define EFFECT_FIRE_FADE_OUT_RATE 0.003f                             // 炎のフェードアウト割合
#define EFFECT_FIRE_START_POS_Y 30.0f                                // 炎の開始位置Y
#define EFFECT_FIRE_START_POS_Y_BOSS -70.0f                          // ボス戦時の、炎の開始位置Y
#define EFFECT_FIRE_ALPHA_START_MOVE 0.3f                            // 炎の動きだすα値
#define EFFECT_FIRE_ALPHA_START_FADE_OUT 0.6f                        // 炎の消えていくα値
#define EFFECT_FIRE_MIN_SIZE 2000                                    // 炎の最小サイズ
#define EFFECT_FIRE_MAX_SIZE 5000                                    // 炎の最大サイズ

#define EFFECT_MAGMA_COLOR D3DXCOLOR(1.0f,0.99f,0.0f,0.8f)           // マグマの色
#define EFFECT_MAGMA_MIN_SIZE 3000                                   // マグマの最小サイズ
#define EFFECT_MAGMA_MAX_SIZE 5000                                   // マグマの最大サイズ

#define EFFECT_LEAF_SIZE D3DXVECTOR3(50.0f, 50.0f ,0.0f)          // 葉っぱの大きさ
#define EFFECT_BRANCH_SIZE D3DXVECTOR3(80.0f, 80.0f ,0.0f)        // 木の枝の大きさ
#define EFFECT_LEAF_FADE_RATE 0.02f                               // 葉っぱのフェードアウト割合
#define EFFECT_BRANCH_FADE_RATE 0.03f                             // 木の枝のフェードアウト割合

#define EFFECT_RED_FIRE_FIRST_SIZE D3DXVECTOR3(50.0f, 50.0f, 0.0f)       // 赤い炎の最初の大きさ
#define EFFECT_RED_FIRE_FIRST_COLOR D3DXCOLOR(1.0f,0.99f,0.0f,1.0f)      // 赤い炎の色
#define EFFECT_RED_FIRE_EXTEND_RATE 0.5f                                 // 赤い炎の拡大割合
#define EFFECT_YELLOW_FIRE_TO_RED_RATE 0.05f                             // 赤に近づけていく割合
#define EFFECT_RED_FIRE_RED D3DXCOLOR(1.0f,0.0f,0.0f,1.0f)               // 赤
#define EFFECT_RED_FIRE_OVER_SIZE 70.0f                                  // 大きすぎるものは、赤にする
#define EFFECT_RED_FIRE_FADE_OUT_RATE 0.01f                              // 赤い炎のフェードアウト割合
#define EFFECT_RED_FIRE_POS_X 1070.0f
#define EFFECT_RED_FIRE_POS_Y 460.0f

#define EFFECT_BLUE_FIRE_FIRST_SIZE D3DXVECTOR3(50.0f, 50.0f, 0.0f)       // 青い炎の最初の大きさ
#define EFFECT_BLUE_FIRE_FIRST_COLOR D3DXCOLOR(0.588f,0.937f,1.0f,1.0f)   // 青い炎の色
#define EFFECT_BLUE_FIRE_EXTEND_RATE 0.5f                                 // 青い炎の拡大割合
#define EFFECT_WHITE_FIRE_TO_BLUE_RATE 0.05f                              // 青に近づけていく割合
#define EFFECT_BLUE_FIRE_BLUE D3DXCOLOR(0.0f,0.0f,1.0f,1.0f)              // 青
#define EFFECT_BLUE_FIRE_OVER_SIZE 70.0f                                  // 大きすぎるものは、青にする
#define EFFECT_BLUE_FIRE_FADE_OUT_RATE 0.01f                              // 青い炎のフェードアウト割合
#define EFFECT_BLUE_FIRE_POS_X 200.0f
#define EFFECT_BLUE_FIRE_POS_Y 460.0f

#define EFFECT_YELLOW_FIRE_FIRST_SIZE D3DXVECTOR3(50.0f, 50.0f, 0.0f)       // 雷の炎の最初の大きさ
#define EFFECT_YELLOW_FIRE_FIRST_COLOR D3DXCOLOR(1.0f,0.99f,0.0f,1.0f)      // 雷の炎の色
#define EFFECT_YELLOW_FIRE_EXTEND_RATE 0.5f                                 // 雷の炎の拡大割合
#define EFFECT_YELLOW_FIRE_TO_GREEN_RATE 0.05f                              // 緑に近づけていく割合
#define EFFECT_YELLOW_FIRE_GREEN D3DXCOLOR(0.0f,1.0f,0.0f,1.0f)             // 緑
#define EFFECT_YELLOW_FIRE_OVER_SIZE 70.0f                                  // 大きすぎるものは、緑にする
#define EFFECT_YELLOW_FIRE_FADE_OUT_RATE 0.01f                              // 雷の炎のフェードアウト割合
#define EFFECT_YELLOW_FIRE_POS_X 640.0f
#define EFFECT_YELLOW_FIRE_POS_Y 460.0f

#define EFFECT_PURPLE_FIRE_FIRST_COLOR D3DXCOLOR(0.835f,0.901f,1.0f,0.5f)         // 紫の炎の色
#define EFFECT_PURPLE_FIRE_FIRST_COLOR_BLAZE D3DXCOLOR(0.835f,0.901f,1.0f,0.35f)  // ブレイズの紫の炎の色
#define EFFECT_WHITE_FIRE_TO_PURPLE_RATE_R 0.025f                                 // 紫に近づけていく割合R
#define EFFECT_WHITE_FIRE_TO_PURPLE_RATE_G 0.05f                                  // 紫に近づけていく割合G
#define EFFECT_PURPLE_FIRE_FADE_OUT_RATE 0.005f                                   // 紫の炎のフェードアウト割合

#define EFFECT_CLOUD_COUNT_LOOP 40                                          // 雲生成時に、一度に回すfor文
#define EFFECT_CLOUD_SIZE_X 10000                                           // 横の大きさ(雲中心からの大きさなので、実際は2倍になる)
#define EFFECT_CLOUD_SIZE_Y 2500                                            // 縦の大きさ(雲中心からの大きさなので、実際は2倍になる)
#define EFFECT_CLOUD_EDGE 60.0f                                             // 雲の端
#define EFFECT_CLOUD_PARTICLES_MAX_SIZE 4000                                // 雲の粒の最大サイズ
#define EFFECT_CLOUD_PARTICLES_MID1_SIZE 3000                               // 雲の粒の中間サイズ1
#define EFFECT_CLOUD_PARTICLES_MID2_SIZE 1500                               // 雲の粒の中間サイズ2
#define EFFECT_CLOUD_PARTICLES_MIN_SIZE 1000                                // 雲の粒の最小サイズ
#define EFFECT_CLOUD_PARTICLES_MAX_ALPHA 15                                 // 雲の粒の最大α値
#define EFFECT_CLOUD_PARTICLES_MID_ALPHA 12                                 // 雲の粒の中間α値
#define EFFECT_CLOUD_PARTICLES_MIN_ALPHA 7                                  // 雲の粒の最小α値
#define EFFECT_CLOUD_FLOWING_SPEED 7.5f                                     // 雲が流れる速度
#define EFFECT_CLOUD_FLOWING_SPEED_BOSS 3.525f                              // ボス戦時、雲が流れる速度
#define EFFECT_CLOUD_FADE_OUT_RATE 0.1f                                     // 雲のフェードアウト割合

#define EFFECT_EXPLOSION_COUNT_LOOP 50                                      // 爆発生成時に、一度に回すfor文
#define EFFECT_EXPLOSION_FADE_OUT_RATE 0.0075f                              // 爆発のフェードアウト割合
#define EFFECT_EXPLOSION_FIRST_COLOR D3DXCOLOR(0.898f,0.968f,0.529f,1.0f)   // 爆発の炎の色
#define EFFECT_EXPLOSION_TO_RED_RATE 0.025f                                 // 赤に近づけていく割合
#define EFFECT_EXPLOSION_MAX_SIZE 10000                                     // 爆発の粒の最大サイズ
#define EFFECT_EXPLOSION_MIN_SIZE 7500                                      // 爆発の粒の最小サイズ

#define EFFECT_SMALL_EXPLOSION_COUNT_LOOP 10                                // 小爆発生成時に、一度に回すfor文
#define EFFECT_SMALL_EXPLOSION_FADE_OUT_RATE 0.005f                         // 小爆発のフェードアウト割合
#define EFFECT_SMALL_EXPLOSION_COLOR D3DXCOLOR(1.0f,1.0f,1.0f,0.3f)         // 小爆発の炎の色
#define EFFECT_SMALL_EXPLOSION_BORDER_SIZE_THAT_CHANGE_SPEED 50.0f          // 小爆発のスピードを変える境目の大きさ

//================================================
// クラス宣言
//================================================

// エフェクトクラス
class CEffect : public CScene2D
{
public:
    // エフェクトの種類
    typedef enum
    {
        TYPE_PARTICLE = 0,	    // 白パーティクル
        TYPE_WAVE,              // 白衝撃波
        TYPE_EXPLOSION,         // 爆発
        TYPE_CHARGE,            // チャージ
        TYPE_CHARGE_PART,       // チャージのパーティクル
        TYPE_LIGHT_BULLET,      // 雷バレット
        TYPE_SHOCK,             // 電撃
        TYPE_BIG_EXPLOSION,     // 大爆発
        TYPE_RED_SPARK,         // 赤火花
        TYPE_GREEN_SPARK,       // 緑火花
        TYPE_BLUE_SPARK,        // 青火花
        TYPE_BLACK_SPARK,       // 黒火花
        TYPE_WATER_EXPLOSION,   // 水爆発
        TYPE_REVERSE_CHARGE,    // 反転チャージ
        TYPE_25,                // 25%
        TYPE_50,                // 50%
        TYPE_75,                // 75%
        TYPE_100,               // 100%
        TYPE_DRAGON_AURA,       // ドラゴンオーラ
        TYPE_DOUBLE_WAVE,       // 二重ウェーブ
        TYPE_PARTICLE_JPG,	    // 白パーティクル(jpg)
        TYPE_FIRE,              // 炎
        TYPE_MAGMA,             // マグマ
        TYPE_LEAF,              // 葉っぱ
        TYPE_BRANCH,            // 枝
        TYPE_RED_FIRE,          // 赤い炎
        TYPE_BLUE_FIRE,         // 青い炎
        TYPE_YELLOW_FIRE,       // 黄色い炎
        TYPE_PURPLE_FIRE,       // 紫の炎
        TYPE_CLOUD,             // 雲
        TYPE_PART_EXPLOSION,    // パーティクルによる爆発
        TYPE_BONUS_POINT,       // ボーナスポイント
        TYPE_SMALL_EXPLOSION,   // 弾を撃ち消された時の、小さな爆発
        TYPE_MAX			    // 種類の最大数
    }TYPE;
    CEffect();
    ~CEffect();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CEffect *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, TYPE type);
    float GetAngleToDragon(void);   // ドラゴンへの角度を取得

private:
    static LPDIRECT3DTEXTURE9 m_apTexture[TYPE_MAX];   // 読み込んだテクスチャを保存する
    D3DXVECTOR3 m_move; // 移動量
    D3DXCOLOR m_col;    // 色
    int m_nLife;        // ライフ
    TYPE m_type;        // エフェクトの種類

    bool m_bAlphaDown;  // 透明度を下げるフラグ

    float m_fRotSpeed;  // 回転速度
    float m_fAngle;     // 角度
};

#endif