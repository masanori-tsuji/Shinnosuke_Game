//====================================================================
//
// UIの処理 (ui.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _UI_H_
#define _UI_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene2d.h"

//================================================
// マクロ定義
//================================================
#define ARROW_SYMBOL_SIZE (D3DXVECTOR3(80.0f,80.0f,0.0f))   // 矢印の大きさ

#define PLAYER1_UI_POS (D3DXVECTOR3(100.0f,40.0f,0.0f))     // プレイヤー1の位置
#define PLAYER1_UI_SIZE (D3DXVECTOR3(150.0f,40.0f,0.0f))    // プレイヤー1サイズ

#define STOCK_UI_POS (D3DXVECTOR3(80.0f,90.0f,0.0f))     // ストックの位置
#define STOCK_UI_SIZE (D3DXVECTOR3(120.0f,40.0f,0.0f))    // ストックサイズ

#define BARRIER_UI_POS (D3DXVECTOR3(210.0f,90.0f,0.0f))     // バリアの位置
#define BARRIER_UI_SIZE (D3DXVECTOR3(120.0f,40.0f,0.0f))    // バリアサイズ

#define SPEED_UI_POS (D3DXVECTOR3(340.0f,90.0f,0.0f))     // スピードの位置
#define SPEED_UI_SIZE (D3DXVECTOR3(120.0f,40.0f,0.0f))    // スピードサイズ

#define STOCK_BAR_UI_POS (D3DXVECTOR3(100.0f,100.0f,0.0f))                    // ストックのバーの位置
#define STOCK_BAR_UI_SIZE_X (75.0f)                                           // ストックバーの横サイズ
#define STOCK_BAR_UI_SIZE (D3DXVECTOR3(STOCK_BAR_UI_SIZE_X,15.0f,0.0f))       // ストックバーのサイズ

#define BARRIER_BAR_UI_POS (D3DXVECTOR3(230.0f,100.0f,0.0f))                  // バリアのバーの位置
#define BARRIER_BAR_UI_SIZE_X (75.0f)                                         // バリアバーの横サイズ
#define BARRIER_BAR_UI_SIZE (D3DXVECTOR3(BARRIER_BAR_UI_SIZE_X,15.0f,0.0f))   // バリアバーのサイズ

#define SPEED_BAR_UI_POS (D3DXVECTOR3(360.0f,100.0f,0.0f))                  // スピードのバーの位置
#define SPEED_BAR_UI_SIZE_X (75.0f)                                         // スピードバーの横サイズ
#define SPEED_BAR_UI_SIZE (D3DXVECTOR3(SPEED_BAR_UI_SIZE_X,15.0f,0.0f))     // スピードバーのサイズ

#define TITLE_LOGO_FIRST_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, -150.0f, 0.0f))  // タイトルロゴの最初の位置
#define TITLE_LOGO_TARGET_POS_Y 150.0f                                       // タイトルロゴの目標位置 
#define TITLE_LOGO_SIZE (D3DXVECTOR3(500.0f, 250.0f, 0.0f))                  // タイトルロゴのサイズ
#define TITLE_LOGO_FLOW_DOWN_SPEED 2.0f                                      // タイトルロゴの流れ落ちる速度

#define MANUAL_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f))  // 遊び方の位置
#define MANUAL_SIZE (D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f))         // 遊び方のサイズ

#define RESULT_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f)) // リザルトの位置
#define RESULT_SIZE (D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f))        // リザルトのサイズ

#define STAGE_START_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 25.0f, 0.0f))  // ステージ開始の位置
#define STAGE_START_MAX_SIZE_X (400.0f)                                                   // ステージ開始の横の最大サイズ
#define STAGE_START_MAX_SIZE_Y (200.0f)                                                   // ステージ開始の縦の最大サイズ
#define STAGE_START_EXTEND_RATE_X (10.0f)                                                 // ステージ開始の横の拡大割合
#define STAGE_START_EXTEND_RATE_Y (5.0f)                                                  // ステージ開始の縦の拡大割合
#define STAGE_START_FADE_OUT_COUNT (180)                                                  // ステージ開始のフェードが始まるカウンタ
#define STAGE_START_FADE_OUT_RATE (0.05f)                                                 // ステージ開始のフェードアウト割合

#define WARNING_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 25.0f, 0.0f))  // ワーニングの位置
#define WARNING_SIZE (D3DXVECTOR3(600.0f, 200.0f, 0.0f))                              // ワーニングのサイズ
#define WARNING_FADE_RATE (0.025f)                                                    // ワーニングのフェードアウト割合
#define WARNING_FADE_OUT_COUNT (360)                                                  // ワーニングのフェードアウトが始まるカウンタ

#define BARRIER_SIZE (D3DXVECTOR3(144.0f, 120.0f, 0.0f))    // バリアのサイズ
#define BARRIER_FADE_RATE (0.025f)                          // バリアのフェードアウト割合
#define BARRIER_START_COLOR D3DXCOLOR(1.0f,1.0f,1.0f,0.0f)  // バリアの開始時の色
#define BARRIER_MAX_ALPHA_1 0.3f                            // バリアの最大α値1
#define BARRIER_MAX_ALPHA_2 0.4f                            // バリアの最大α値2
#define BARRIER_MAX_ALPHA_3 0.5f                            // バリアの最大α値3
#define BARRIER_MAX_ALPHA_4 0.6f                            // バリアの最大α値4
#define BARRIER_MAX_ALPHA_5 0.7f                            // バリアの最大α値5

#define ZONE_POS D3DXVECTOR3(SCREEN_WIDTH / 2, 65.0f, 0.0f) // ゾーンUIの位置
#define ZONE_SIZE D3DXVECTOR3(110.0f,110.0f,0.0f)           // ゾーンUIのサイズ

#define TO_START_POS (D3DXVECTOR3(1000.0f, 65.0f, 0.0f))    // 2P TO STARTの位置
#define TO_START_SIZE (D3DXVECTOR3(280.0f, 70.0f, 0.0f))    // 2P TO STARTのサイズ

#define CLEAR_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 25.0f, 0.0f))  // CLEARの位置
#define CLEAR_SIZE (D3DXVECTOR3(600.0f, 100.0f, 0.0f))                              // CLEARのサイズ
#define CLEAR_FADE_RATE (0.005f)                                                    // CLEARのフェード割合
#define CLEAR_FADE_OUT_COUNT (480)                                                  // CLEARのフェードアウトが始まるカウンタ

#define GAMEOVER_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 25.0f, 0.0f))  // GAMEOVERの位置
#define GAMEOVER_SIZE (D3DXVECTOR3(700.0f, 150.0f, 0.0f))                              // GAMEOVERのサイズ
#define GAMEOVER_FADE_RATE (0.0075f)                                                   // GAMEOVERのフェード割合

#define PRESS_START_POS D3DXVECTOR3(SCREEN_WIDTH / 2, 650.0f, 0.0f)                 // スタートボタンを押してねの位置
#define PRESS_START_SIZE D3DXVECTOR3(500.0f,150.0f,0.0f)                            // スタートボタンを押してねのサイズ
#define PRESS_START_INACTIVE_COLOR D3DXCOLOR(0.431f, 0.431f, 0.388f, 1.0f)          // スタートボタンを押してねの非アクティブ色
#define PRESS_START_FADE_RATE 0.01f                                                 // スタートボタンを押してねの点滅割合
#define PRESS_START_FADE_OUT_MIN 0.6f                                               // スタートボタンを押してねのフェードアウトの最小値

#define CAREFUL_UPPER_POS D3DXVECTOR3(SCREEN_WIDTH / 2 - 80.0f, 300.0f, 0.0f)       // 危険！(上)の位置
#define CAREFUL_UPPER_SIZE D3DXVECTOR3(650.0f,200.0f,0.0f)                          // 危険！(上)のサイズ
#define CAREFUL_DOWN_POS D3DXVECTOR3(SCREEN_WIDTH / 2 , 580.0f, 0.0f)               // 危険！(下)の位置
#define CAREFUL_DOWN_SIZE D3DXVECTOR3(600.0f,150.0f,0.0f)                           // 危険！(下)のサイズ
#define CAREFUL_LEFT_POS D3DXVECTOR3(150.0f, 360.0f, 0.0f)                          // 危険！(左)の位置
#define CAREFUL_LEFT_SIZE D3DXVECTOR3(125.0f,500.0f,0.0f)                           // 危険！(左)のサイズ
#define CAREFUL_RIGHT_POS D3DXVECTOR3(SCREEN_WIDTH - 150.0f, 360.0f, 0.0f)          // 危険！(右)の位置
#define CAREFUL_RIGHT_SIZE D3DXVECTOR3(125.0f,500.0f,0.0f)                          // 危険！(右)のサイズ
#define CAREFUL_INACTIVE_COLOR D3DXCOLOR(0.431f, 0.431f, 0.388f, 1.0f)              // 危険！の非アクティブ色
#define CAREFUL_FADE_OUT_COUNT (195)                                                // 危険！のフェードアウトが始まるカウンタ

#define DANGER_SIZE D3DXVECTOR3(250.0f,125.0f,0.0f)                                 // デンジャーのサイズ
#define DANGER_FADE_OUT_COUNT (90)                                                  // デンジャーのフェードアウトが始まるカウンタ

//================================================
// クラス宣言
//================================================

// UIクラス
class CUI : public CScene2D
{
public:
    // UIの種類
    typedef enum
    {
        TYPE_ARROW_SYMBOL = 0,	// 矢印
        TYPE_PLAYER1,           // プレイヤー1
        TYPE_STOCK,             // ストック
        TYPE_BARRIER,           // バリア
        TYPE_SPEED,             // スピード
        TYPE_STOCK_BAR,         // ストックのバー
        TYPE_BARRIER_BAR,       // バリアのバー
        TYPE_ZONEα,            // ゾーンα
        TYPE_ZONEβ,            // ゾーンβ
        TYPE_ZONEγ,            // ゾーンγ
        TYPE_SPEED_BAR,         // スピードのバー
        TYPE_TITLE_LOGO,        // タイトルのロゴ
        TYPE_PRESS_START,       // スタートボタンを押してね
        TYPE_RESULT_SOLO,       // リザルト(ソロ)
        TYPE_RESULT_DUO,        // リザルト(デュオ)
        TYPE_START_STAGE1,      // ステージ1の開始
        TYPE_WARNING,           // ワーニング
        TYPE_GET_BARRIER,       // バリアをまとう
        TYPE_2P_TO_START,       // 2P参加待ち
        TYPE_MANUAL,            // 遊び方の背景
        TYPE_αCLEAR,           // αクリア
        TYPE_START_STAGE2,      // ステージ2の開始
        TYPE_START_STAGE3,      // ステージ3の開始
        TYPE_βCLEAR,           // βクリア
        TYPE_γCLEAR,           // γクリア
        TYPE_GAMEOVER,          // ゲームオーバー
        TYPE_CAREFUL_UPPER,     // 危険！(上)
        TYPE_CAREFUL_LEFT,      // 危険！(左)
        TYPE_CAREFUL_RIGHT,     // 危険！(右)
        TYPE_DANGER,            // デンジャー
        TYPE_CAREFUL_DOWN,      // 危険！(下)
        TYPE_MAX			    // 種類の最大数
    }TYPE;

    CUI();
    ~CUI();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CUI *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, TYPE type);
    static void ChangeZoneToβ(void) { m_bChangeZoneToβ = true; }
    static void ChangeZoneToγ(void) { m_bChangeZoneToγ = true; }
    bool Careful(void);
    bool Danger(void);

private:
    static LPDIRECT3DTEXTURE9 m_apTexture[TYPE_MAX];   // 読み込んだテクスチャを保存する
    static bool m_bChangeZoneToβ;  // ゾーンのUIを変えるかどうか
    static bool m_bChangeZoneToγ;  // ゾーンのUIを変えるかどうか
    D3DXVECTOR3 m_move; // 移動量
    D3DXCOLOR m_col;    // 色
    TYPE m_type;        // UIの種類
    bool m_bFadeOut;    // フェードアウトかどうか
    bool m_bBlinking;   // 点滅中かどうか
    int m_nCntTime;     // 時間をカウント
};

#endif