//=============================================================================
//
// ゲーム処理 [game.h]
// Author : 後藤慎之助
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "mode.h"
#include "manager.h"

//================================================
// 前方宣言(不完全なクラスは、cppの方でヘッダを呼び出す)
//================================================
class CPlayer;
class CScore;
class CPause;
class CHelper;

//================================================
// マクロ定義
//================================================

//ゲーム状態の管理フレーム
#define COUNT_WAIT_GAME_OVER 180	    // ゲームオーバー時に、リザルト画面へ行くまでの時間
#define COUNT_WAIT_GAME_CLEAR 90        // ゲームクリア時に、リザルト画面へ行くまでの時間
#define CREATE_PLAYER_COUNT 90          // プレイヤーを生成するまでのカウンタ

#define FLOOR_COLLISION (SCREEN_HEIGHT - 35.0f) // 床の判定
#define FLOWING_STAGE (3.195f)                  // ステージが流れる速度

//================================================
// クラス宣言
//================================================
class CGame : public CMode
{
public:

    // ゲームの状態
    typedef enum
    {
        STATE_NORMAL = 0,   // 通常
        STATE_PAUSE,	    // ポーズ
        STATE_MAX			// 状態の最大数
    }STATE;

    CGame();
    ~CGame();
    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    static CScore *GetScore(void) { return m_pScore; }                      // スコアを取得
    static CPlayer *GetPlayer(void) { return m_pPlayer; }                   // プレイヤーを取得 
    static int GetStock(void) { return m_nStock; }                          // ストック数を取得
    static bool GetDispPlayer(void) { return m_bDispPlayer; }               // プレイヤーがいるかどうかの取得
    static void DefeatPlayer(void);                                         // プレイヤーがやられた
    static void AddStock(void);                                             // ストックを加算
    static bool GetBossBattle(void){return m_bBossBattle; }                 // ボスバトル中かどうか取得
    static bool GetFloor(void) { return m_bFloor; }                         // 床の判定があるかどうかを取得
    static void StopGameCount(void);                                        // ゲームのカウンタを止める
    static void SetState(STATE state) { m_state = state; }                  // ゲームの状態を設定
    static STATE GetState(void) { return m_state; }                         // ゲームの状態を取得
    static void SetStage(CManager::STAGE stage);                            // ステージを設定
    static CManager::STAGE GetStage(void) { return m_stage; }               // ステージを取得
    static void RespawnPlayer(void);                                        // プレイヤーをリスポーン
    static void Continue(void);                                             // コンティニュー
    static bool GetInvinciblePlayer(void) { return m_bInvinciblePlayer; }   // プレイヤーが無敵かどうかを取得
    static void SetInvinciblePlayer(void) { m_bInvinciblePlayer = true; }   // プレイヤーを無敵にする
    static bool GetMovePlayer(void) { return m_bMovePlayer; }               // プレイヤーが動けるかどうかを取得
    static void SetMovePlayer(bool bMove);                                  // プレイヤーが動けるかどうかを設定
    static void SetShockPlayer(bool bShock) { m_bShockPlayer = bShock; }    // プレイヤーのショック状態を調整
    static bool GetShockPlayer(void) { return m_bShockPlayer; }             // プレイヤーがショック状態かどうかを取得
    static bool GetDarkening(void) { return m_bDarkening; }                 // 暗転状態かどうか取得
    static bool GetDispHelper(void) { return m_bCreateHelper; }             // ヘルパーがいるかどうか取得
    static void SetGameClear(void) { m_bGameClear = true; }                 // ゲームクリア
    static void SetGameOver(void){ m_bFinishGame = true; m_nCntFinishGame = COUNT_WAIT_GAME_OVER;}  // ゲームオーバー
    void Stageα(void);      // ステージαの処理
    void Stageβ(void);      // ステージβの処理
    void Stageγ(void);      // ステージγの処理
    void SetRandBubble(void);// ランダムに泡を生成
    void SetRandFire(void);  // ランダムに炎を生成
    void SetRandMagma(void); // ランダムにマグマを生成
    void SetRandCloud(void); // ランダムに雲を生成

private:
    int m_nCntCreatePlayer;              // プレイヤーを生成するまでのカウンタ

    static CScore *m_pScore;             // スコアのポインタ
    static CPlayer *m_pPlayer;           // プレイヤーのポインタ
    static CHelper *m_pHelper;           // ヘルパーのポインタ
    static CPause *m_pPause;             // ポーズのポインタ
    static STATE m_state;                // 状態
    static CManager::STAGE m_stage;      // ステージ
    static int m_nCntGameTime;           // ゲーム時間のカウンタ
    static int m_nStock;                 // ストック数
    static bool m_bCreatePlayer;         // プレイヤーを生成するかどうか
    static bool m_bDispPlayer;           // プレイヤーがいるかどうか
    static bool m_bBossBattle;           // ボスバトル中かどうか
    static bool m_bFloor;                // 床の判定を使うかどうか
    static bool m_bInvinciblePlayer;     // プレイヤーが無敵かどうか
    static bool m_bMovePlayer;           // プレイヤーが動けるかどうか
    static bool m_bShockPlayer;          // プレイヤーがショック状態かどうか
    static bool m_bDarkening;            // 暗転状態かどうか
    static bool m_bCreateHelper;         // ヘルパーが生成されているかどうか
    static bool m_bGameClear;            // ゲームクリアしたかどうか
    static int m_nCntFinishGame;         // ゲームを終わるまでのカウンタ
    static bool m_bFinishGame;           // ゲームが終わったかどうか
};

#endif