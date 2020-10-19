//=============================================================================
//
// ゲーム処理 [game.cpp]
// Author : 後藤慎之助
//
//=============================================================================
#include "game.h"
#include <time.h>
#include "manager.h"
#include "sound.h"
#include "player.h"
#include "bullet.h"
#include "effect.h"
#include "enemy.h"
#include "bg.h"
#include "number.h"
#include "score.h"
#include "ui.h"
#include "item.h"
#include "bubble.h"
#include "block.h"
#include "afterimage.h"
#include "fade.h"
#include "light2d.h"
#include "pause.h"
#include "input.h"
#include "wave.h"
#include "helper.h"
#include "foreground.h"

//========================================
// マクロ定義
//========================================

// ウェーブのカウンタ
#define STAGE_WAVE_STOP 10007           // 泡や炎で割り切れないように(素数)

// ステージ1
#define STAGE1_START 2
#define STAGE1_WAVE1 200
#define STAGE1_WAVE2 560
#define STAGE1_WAVE3 860
#define STAGE1_WAVE4 1500
#define STAGE1_WAVE5 2200
#define STAGE1_WAVE6 3000
#define STAGE1_WAVE_BOSS_START 3500 // ワーニング開始
#define STAGE1_WAVE_SET_FLOOR 3800  // 床とボスの音楽と注意表記
#define STAGE1_WAVE_BLUE_BIRD 4500  // 青い鳥

// ステージ2
#define STAGE2_START 90
#define STAGE2_SET_BUBBLE_COUNT 20  // 泡を生成するカウンタ
#define STAGE2_WAVE1 200
#define STAGE2_WAVE2 500
#define STAGE2_WAVE3 920
#define STAGE2_WAVE4 1340
#define STAGE2_WAVE5 1760
#define STAGE2_WAVE6 2120
#define STAGE2_WAVE7 2520
#define STAGE2_WAVE8 3300
#define STAGE2_WAVE_BOSS_START 3700     // ワーニング開始
#define STAGE2_WAVE_DARKENING 3800      // 暗転
#define STAGE2_WAVE_BOSS_SET 3820       // ボス生成
#define STAGE2_WAVE_STOP_FLOWING 3945   // 背景の流れを止める
#define STAGE2_WAVE_PLAY_SOUND 4080     // ボスの音楽
#define STAGE2_WAVE_LIGHTNING 4200      // 明るくなる

// ステージ3
#define STAGE3_START 90
#define STAGE3_SET_FIRE_COUNT 2         // 炎を生成するカウンタ
#define STAGE3_SET_MAGMA_COUNT 2        // マグマを生成する
#define STAGE3_SET_CLOUD_COUNT 45       // 雲を生成するカウンタ
#define STAGE3_SET_CLOUD_COUNT_BOSS 75  // ボス戦時、雲を生成するカウンタ
#define STAGE3_WAVE1 360
#define STAGE3_WAVE2 480
#define STAGE3_WAVE2_DANGER 700         // 炎の鳥の警告
#define STAGE3_WAVE3 1200
#define STAGE3_WAVE4 1680
#define STAGE3_WAVE5 1980
#define STAGE3_WAVE6_DANGER 2205        // コウモリの洗礼の警告
#define STAGE3_WAVE6 2400
#define STAGE3_WAVE7 2730
#define STAGE3_TAKE_OFF 2850            // 空に飛び立つフレーム(bg.cppと調整)
#define STAGE3_SET_CLOUD_START 2940     // 雲を設置し始めるカウンタ
#define STAGE3_WAVE8 2970
#define STAGE3_WAVE9 3970
#define STAGE3_WAVE10 4570
#define STAGE3_WAVE_END_SKY_MODE 4850   // 空モードの終了
#define STAGE3_WAVE_BOSS_START 5420     // ワーニング開始
#define STAGE3_WAVE_PLAY_SOUND 5780     // ボスの音楽開始
#define STAGE3_WAVE_SET_FLOOR 5870      // 床の判定を設定

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CScore *CGame::m_pScore = NULL;
CPlayer *CGame::m_pPlayer = NULL;
CHelper *CGame::m_pHelper = NULL;
CPause *CGame::m_pPause = NULL;
CGame::STATE CGame::m_state = STATE_NORMAL;
CManager::STAGE CGame::m_stage = CManager::STAGE_NONE;
int CGame::m_nStock = PLAYER_START_STOCK;
int CGame::m_nCntGameTime = 0;
bool CGame::m_bCreatePlayer = false;
bool CGame::m_bDispPlayer = true;
bool CGame::m_bBossBattle = false;
bool CGame::m_bFloor = false;
bool CGame::m_bInvinciblePlayer = false;
bool CGame::m_bMovePlayer = true;
bool CGame::m_bShockPlayer = false;
bool CGame::m_bDarkening = false;
bool CGame::m_bCreateHelper = false;
bool CGame::m_bGameClear = false;
int CGame::m_nCntFinishGame = 0;
bool CGame::m_bFinishGame = false;

//=============================================================================
// ゲームのコンストラクタ
//=============================================================================
CGame::CGame()
{
    m_nStock = PLAYER_START_STOCK;
    m_bCreatePlayer = false;
    m_bDispPlayer = true;   // 最初はプレイヤーがいる
    m_pScore = NULL;
    m_pPlayer = NULL;
    m_pHelper = NULL;
    m_pPause = NULL;

    m_state = STATE_NORMAL;
    m_stage = CManager::STAGE_NONE;
    m_nCntGameTime = 0;
    m_nCntCreatePlayer = 0;
    m_nCntFinishGame = 0;
    m_bFinishGame = false;
    m_bBossBattle = false;
    m_bFloor = false;
    m_bInvinciblePlayer = false;
    m_bMovePlayer = true;
    m_bShockPlayer = false;
    m_bDarkening = false;
    m_bCreateHelper = false;
    m_bGameClear = false;
}

//=============================================================================
// ゲームのデストラクタ
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CGame::Init(void)
{
    // 乱数の初期化
    srand((unsigned int)time(NULL));

    // ステージをαに設定
    SetStage(CManager::GetSetStage());

    // ポーズの生成
    m_pPause = CPause::Create();

    // プレイヤーの生成(最初の角度、無敵時間は0)
    m_pPlayer = CPlayer::Create(PLAYER_POS, PLAYER_SIZE, 0.0f, 0);

    // スコアの生成
    m_pScore = CScore::Create(SCORE_POS, SCORE_SIZE);

    // UIの生成
    CUI::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), ARROW_SYMBOL_SIZE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), CUI::TYPE_ARROW_SYMBOL);
    CUI::Create(PLAYER1_UI_POS, PLAYER1_UI_SIZE, DEFAULT_COLOR, CUI::TYPE_PLAYER1);
    CUI::Create(STOCK_UI_POS, STOCK_UI_SIZE, DEFAULT_COLOR, CUI::TYPE_STOCK);
    CUI::Create(BARRIER_UI_POS, BARRIER_UI_SIZE, DEFAULT_COLOR, CUI::TYPE_BARRIER);
    CUI::Create(SPEED_UI_POS, SPEED_UI_SIZE, DEFAULT_COLOR, CUI::TYPE_SPEED);
    CUI::Create(STOCK_BAR_UI_POS, STOCK_BAR_UI_SIZE, DEFAULT_COLOR, CUI::TYPE_STOCK_BAR);
    CUI::Create(BARRIER_BAR_UI_POS, BARRIER_BAR_UI_SIZE, DEFAULT_COLOR, CUI::TYPE_BARRIER_BAR);
    CUI::Create(SPEED_BAR_UI_POS, SPEED_BAR_UI_SIZE, DEFAULT_COLOR, CUI::TYPE_SPEED_BAR);
    CUI::Create(DEFAULT_VECTOR, BARRIER_SIZE, BARRIER_START_COLOR, CUI::TYPE_GET_BARRIER);
    CUI::Create(TO_START_POS, TO_START_SIZE, DEFAULT_COLOR, CUI::TYPE_2P_TO_START);

    return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CGame::Uninit(void)
{
    // BGMやループするSEを停止
    CSound *pSound = CManager::GetSound();
    pSound->Stop(CSound::LABEL_BGM_GAME1);
    pSound->Stop(CSound::LABEL_BGM_BOSS1);
    pSound->Stop(CSound::LABEL_BGM_GAME2);
    pSound->Stop(CSound::LABEL_BGM_BOSS2);
    pSound->Stop(CSound::LABEL_BGM_GAME3);
    pSound->Stop(CSound::LABEL_BGM_BOSS3);
    pSound->Stop(CSound::LABEL_SE_WARNING);

    // ポーズを破棄
    if (m_pPause != NULL)
    {
        m_pPause->Uninit();
        delete m_pPause;
        m_pPause = NULL;
    }
}

//=============================================================================
// 更新処理
//=============================================================================
void CGame::Update(void)
{
    // キーボードを取得
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // コントローラを取得
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(PLAYER_1);
    DIJOYSTATE2 Controller2 = pInputJoypad->GetController(PLAYER_2);

    // サウンドを取得
    CSound *pSound = CManager::GetSound();

    // 状態ごとに場合分け
    switch (m_state)
    {
    case STATE_NORMAL:

        // スタートボタンが押されたら
        if (pInputKeyboard->GetKeyboardTrigger(DIK_P) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START))
        {
            // ゲーム中なら
            if (CGame::GetState() == CGame::STATE_NORMAL)
            {
                // ゲームが続いているなら
                if (m_bFinishGame == false)
                {
                    // ポーズ音
                    pSound->Play(CSound::LABEL_SE_PAUSE);

                    // ポーズ状態にする
                    CPause::SetState(CPause::STATE_PAUSE);
                    CGame::SetState(CGame::STATE_PAUSE);
                }
            }
        }

        // プレイヤー2のスタートボタンが押されたら
        if (pInputJoypad->GetJoypadTrigger(PLAYER_2, CInputJoypad::BUTTON_START))
        {
            // ヘルパーが生成されていないなら
            if (m_bCreateHelper == false)
            {
                // ゲーム中なら
                if (CGame::GetState() == CGame::STATE_NORMAL)
                {
                    // ゲームが続いているなら
                    if (m_bFinishGame == false)
                    {
                        // ポーズ音(参戦音の代わり)
                        pSound->Play(CSound::LABEL_SE_PAUSE);

                        // ヘルパーの生成(初期角度を変えて配置するのは、不自然にみえたのでやめた)
                        m_pHelper = CHelper::Create(HELPER_POS, HELPER_SIZE, 0.0f, HELPER_INVINCIBLE_FRAME);

                        // ヘルパーがいるかどうかのフラグをtrueに
                        m_bCreateHelper = true;
                    }
                }
            }
        }

        // ゲーム時間を加算
        m_nCntGameTime++;

        // ステージごとに切り替え
        switch (m_stage)
        {
        case CManager::STAGE_α:
            Stageα();
            break;

        case CManager::STAGE_β:
            Stageβ();
            break;

        case CManager::STAGE_γ:
            Stageγ();
            break;
        }

        // プレイヤー生成のフラグがtrueなら
        if (m_bCreatePlayer == true)
        {
            // プレイヤーを生成するカウンタ
            m_nCntCreatePlayer++;

            // 一定時間経過したら
            if (m_nCntCreatePlayer >= CREATE_PLAYER_COUNT)
            {
                // プレイヤーを生成するカウンタをリセット
                m_nCntCreatePlayer = 0;

                // プレイヤーの残像から、プレイヤーを配置(初期角度を変えて配置するのは、不自然にみえたのでやめた)
                CAfterimage::Create(AFTERIMAGE_PLAYER_DISP_POS, PLAYER_SIZE, 0.0f, DEFAULT_COLOR, CAfterimage::TYPE_PLAYER_DISP);

                // プレイヤー生成のフラグをfalseに
                m_bCreatePlayer = false;
            }
        }

        // ゲームが続いているなら
        if (m_bFinishGame == false)
        {
            // コンティニュー判定(ストックが0未満)
            if (m_nStock < 0)
            {
                // コンティニュー状態にする
                CPause::SetState(CPause::STATE_CONTINUE);
                CGame::SetState(CGame::STATE_PAUSE);
            }
            // ゲームクリア判定
            else
            {
                // ゲームクリアの判定がtrueなら
                if (m_bGameClear == true)
                {
                    // ゲーム終了のフラグをtrueに
                    m_bFinishGame = true;

                    // リザルトに行くまでの時間を設定
                    m_nCntFinishGame = COUNT_WAIT_GAME_CLEAR;
                }
            }
        }

        // リザルトに行くまでのカウンタ
        if (m_nCntFinishGame > 0)
        {
            m_nCntFinishGame--;

            // 0になった時に
            if (m_nCntFinishGame == 0)
            {
                // フェードしていないなら
                if (CFade::GetFade() == CFade::FADE_NONE)
                {
                    // スコアをマネージャに渡す
                    CManager::SetScore(m_pScore->GetScore());

                    // ヘルパーがいるかどうかをマネージャに渡す
                    if (m_pHelper == NULL)
                    {
                        CManager::SetUseHelper(false);
                    }
                    else
                    {
                        CManager::SetUseHelper(true);
                    }

                    // リザルトモードに移行
                    CFade::SetFade(CManager::MODE_RESULT);
                }
            }
        }

        break;

    case STATE_PAUSE:

        // ポーズの更新
        m_pPause->Update();

        break;
    }
}

//========================================
// プレイヤー撃破処理
//========================================
void CGame::DefeatPlayer(void)
{
    // プレイヤーがいるかどうかのフラグをtrueに
    m_bDispPlayer = false;

    // ストック数を下げる
    m_nStock--;

    // 0以上のストックがあるなら
    if (m_nStock >= 0)
    {
        // プレイヤーの生成
        m_bCreatePlayer = true;
    }
}

//========================================
// コンティニュー
//========================================
void CGame::Continue(void)
{
    // ストックを、開始時のものにする
    m_nStock = PLAYER_START_STOCK;

    // プレイヤーの生成のフラグをtrueに
    m_bCreatePlayer = true;

    //================================================
    // スコアを半分にする処理
    // 変数宣言
    int nScore = 0;     // スコア
    int nAnswer = 0;    // 最終的な答え

    // スコアを取得
    if (m_pScore != NULL)
    {
        nScore = m_pScore->GetScore();
    }
    
    // 0以下か、それ以外で場合分け
    if (nScore > 0)
    {
        // 偶数なら
        if (nScore % 2 == 0)
        {
            if (m_pScore != NULL)
            {
                // そのまま半分にする
                nAnswer = nScore / 2;
                m_pScore->SetScore(nAnswer);
            }
        }
        // 奇数なら
        else
        {
            if (m_pScore != NULL)
            {
                // 小数点以下切り捨てで半分にする
                nAnswer = (nScore - 1) / 2;
                m_pScore->SetScore(nAnswer);
            }
        }
    }
    else
    {
        // 0以下は、0
        if (m_pScore != NULL)
        {
            nAnswer = 0;
            m_pScore->SetScore(nAnswer);
        }
    }
    //================================================
}

//========================================
// ストック加算処理
//========================================
void CGame::AddStock(void)
{
    // ストックを加算
    m_nStock++;

    // 最大数を超えたら、最大数にする
    if (m_nStock > PLAYER_MAX_STOCK)
    {
        m_nStock = PLAYER_MAX_STOCK;
    }
}

//========================================
// ゲームのカウンタを止める
//========================================
void CGame::StopGameCount(void)
{
    m_nCntGameTime = STAGE_WAVE_STOP - 1;
}

//========================================
// ステージを設定する
//========================================
void CGame::SetStage(CManager::STAGE stage)
{
    // サウンドを取得
    CSound *pSound = CManager::GetSound();

    // ステージを変える
    m_stage = stage;

    // ゲームのカウンタをリセット
    m_nCntGameTime = 0;

    // ボスバトル中のフラグをリセット
    m_bBossBattle = false;

    // ショック状態をリセット
    m_bShockPlayer = false;

    // 暗転状態をリセット
    m_bDarkening = false;

    // UIと、背景と、床の判定を設定
    switch (m_stage)
    {
    case CManager::STAGE_α:

        // UIの生成
        CUI::Create(ZONE_POS, ZONE_SIZE, DEFAULT_COLOR, CUI::TYPE_ZONEα);

        // 背景の生成
        CBg::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
            D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f),CBg::SET_α);

        // 前景の生成
        CForeground::Create(FOREGROUND_FOREST_POS, FOREGROUND_FOREST_SIZE, DEFAULT_COLOR, CForeground::TYPE_FOREST);

        // BGM再生
        pSound->Play(CSound::LABEL_BGM_GAME1);

        // 床は未使用
        m_bFloor = false;

        break;

    case CManager::STAGE_β:

        // UIの生成
        CUI::Create(ZONE_POS, ZONE_SIZE, DEFAULT_COLOR, CUI::TYPE_ZONEβ);

        // 背景の生成
        CBg::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
            D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), CBg::SET_β);

        // 前景の生成
        CForeground::Create(FOREGROUND_CORAL_POS, FOREGROUND_CORAL_SIZE, DEFAULT_COLOR, CForeground::TYPE_CORAL);

        // 波の生成
        CWave::Create(WAVE_POS, WAVE_SIZE, WAVE_COLOR, CWave::TYPE_BIG_WAVE);
        // 波の生成
        CWave::Create(WAVE_POS, WAVE_SIZE2, WAVE_COLOR2, CWave::TYPE_BIG_WAVE2);

        // BGM再生
        pSound->Play(CSound::LABEL_BGM_GAME2);

        // 床を使用
        m_bFloor = true;

        break;

    case CManager::STAGE_γ:

        // UIの生成
        CUI::Create(ZONE_POS, ZONE_SIZE, DEFAULT_COLOR, CUI::TYPE_ZONEγ);

        // 背景の生成
        CBg::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
            D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), CBg::SET_γ);

        // BGM再生
        pSound->Play(CSound::LABEL_BGM_GAME3);

        // 床を使用
        m_bFloor = true;

        break;
    }

    // プレイヤーの無敵解除
    m_bInvinciblePlayer = false;
}

//========================================
// プレイヤーをリスポーン
//========================================
void CGame::RespawnPlayer(void)
{
    // プレイヤーの生成(初期角度を変えて配置するのは、不自然にみえたのでやめた)
    m_pPlayer = CPlayer::Create(PLAYER_POS, PLAYER_SIZE, 0.0f, PLAYER_INVINCIBLE_FRAME);

    // プレイヤーがいるかどうかのフラグをtrueに
    m_bDispPlayer = true;
}

//========================================
// プレイヤーが動けるかどうかを設定
//========================================
void CGame::SetMovePlayer(bool bMove)
{
    // プレイヤーが動けるかどうかを設定
    m_bMovePlayer = bMove;

    // プレイヤーが生存しているなら、スタート位置にする
    if (m_bDispPlayer == true)
    {
        m_pPlayer->SetStartPos();
    }

    // ヘルパーが存在しているなら、スタート位置にする
    if (m_bCreateHelper == true)
    {
        m_pHelper->SetStartPos();
    }
}

//========================================
// ステージα
//========================================
void CGame::Stageα(void)
{
    // サウンドを取得
    CSound *pSound = CManager::GetSound();

    // 敵の生成
    switch (m_nCntGameTime)
    {
    case STAGE1_START:
        CUI::Create(STAGE_START_POS, DEFAULT_VECTOR, DEFAULT_COLOR, CUI::TYPE_START_STAGE1);
        break;

    case STAGE1_WAVE1:

        // 第1ウェーブ
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 400.0f, 200.0f, 0.0f), ENEMY_FLY_SIZE,
            210, false, CEnemy::TYPE_FLY);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 560.0f, 200.0f, 0.0f), ENEMY_FLY_SIZE,
            245, false, CEnemy::TYPE_FLY);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 720.0f, 200.0f, 0.0f), ENEMY_FLY_SIZE,
            280, false, CEnemy::TYPE_FLY);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 880.0f, 200.0f, 0.0f), ENEMY_FLY_SIZE,
            315, false, CEnemy::TYPE_FLY);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 1040.0f, 200.0f, 0.0f), ENEMY_FLY_SIZE,
            350, false, CEnemy::TYPE_FLY);

        break;

    case STAGE1_WAVE2:

        // 第2ウェーブ(下からくるハエ)
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 400.0f, 550.0f, 0.0f), ENEMY_FLY_SIZE,
            210, true, CEnemy::TYPE_FLY);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 560.0f, 550.0f, 0.0f), ENEMY_FLY_SIZE,
            245, true, CEnemy::TYPE_FLY);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 720.0f, 550.0f, 0.0f), ENEMY_FLY_SIZE,
            280, true, CEnemy::TYPE_FLY);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 880.0f, 550.0f, 0.0f), ENEMY_FLY_SIZE,
            315, true, CEnemy::TYPE_FLY);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 1040.0f, 550.0f, 0.0f), ENEMY_FLY_SIZE,
            350, true, CEnemy::TYPE_FLY);

        break;

    case STAGE1_WAVE3:

        // 第3ウェーブ(骸骨)
        // 上の段
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 400.0f, SCREEN_HEIGHT / 2 - 65.0f, 0.0f), ENEMY_SKELETON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_SKELETON);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 560.0f, SCREEN_HEIGHT / 2 - 65.0f, 0.0f), ENEMY_SKELETON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_SKELETON);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 720.0f, SCREEN_HEIGHT / 2 - 65.0f, 0.0f), ENEMY_SKELETON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_SKELETON);
        // 下の段
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 400.0f, SCREEN_HEIGHT / 2 + 65.0f, 0.0f), ENEMY_SKELETON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_SKELETON);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 560.0f, SCREEN_HEIGHT / 2 + 65.0f, 0.0f), ENEMY_SKELETON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_SKELETON);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 720.0f, SCREEN_HEIGHT / 2 + 65.0f, 0.0f), ENEMY_SKELETON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_SKELETON);

        break;

    case STAGE1_WAVE4:

        // 第4ウェーブ(緑鳥)
        // 緑鳥
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 100.0f, 200.0f, 0.0f), ENEMY_GREEN_BIRD_SIZE,
            0, false, CEnemy::TYPE_GREEN_BIRD);
        // 骸骨3体
        CEnemy::Create(D3DXVECTOR3(-500.0f, SCREEN_HEIGHT / 2 + 165.0f, 0.0f), ENEMY_SKELETON_SIZE,
            DONT_CHANGE_MOVE, false, CEnemy::TYPE_SKELETON);
        CEnemy::Create(D3DXVECTOR3(-660.0f, SCREEN_HEIGHT / 2 + 165.0f, 0.0f), ENEMY_SKELETON_SIZE,
            DONT_CHANGE_MOVE, false, CEnemy::TYPE_SKELETON);
        CEnemy::Create(D3DXVECTOR3(-820.0f, SCREEN_HEIGHT / 2 + 165.0f, 0.0f), ENEMY_SKELETON_SIZE,
            DONT_CHANGE_MOVE, false, CEnemy::TYPE_SKELETON);

        break;

    case STAGE1_WAVE5:

        // 第5ウェーブ(白カラス)
        // 白カラス(上段)
        CEnemy::Create(D3DXVECTOR3(-100.0f, 180.0f, 0.0f), ENEMY_CROW_SIZE,
            DONT_CHANGE_MOVE, false, CEnemy::TYPE_CROW);
        // ハエ(下段)
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 300.0f, SCREEN_HEIGHT / 2 + 265.0f, 0.0f), ENEMY_FLY_SIZE,
            210, true, CEnemy::TYPE_FLY);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 460.0f, SCREEN_HEIGHT / 2 + 265.0f, 0.0f), ENEMY_FLY_SIZE,
            245, true, CEnemy::TYPE_FLY);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 620.0f, SCREEN_HEIGHT / 2 + 265.0f, 0.0f), ENEMY_FLY_SIZE,
            280, true, CEnemy::TYPE_FLY);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 780.0f, SCREEN_HEIGHT / 2 + 265.0f, 0.0f), ENEMY_FLY_SIZE,
            315, true, CEnemy::TYPE_FLY);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 940.0f, SCREEN_HEIGHT / 2 + 265.0f, 0.0f), ENEMY_FLY_SIZE,
            350, true, CEnemy::TYPE_FLY);
        // 二列の骸骨(中段)
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 700.0f, SCREEN_HEIGHT / 2 - 65.0f, 0.0f), ENEMY_SKELETON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_SKELETON);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 860.0f, SCREEN_HEIGHT / 2 - 65.0f, 0.0f), ENEMY_SKELETON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_SKELETON);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 1020.0f, SCREEN_HEIGHT / 2 - 65.0f, 0.0f), ENEMY_SKELETON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_SKELETON);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 700.0f, SCREEN_HEIGHT / 2 + 65.0f, 0.0f), ENEMY_SKELETON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_SKELETON);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 860.0f, SCREEN_HEIGHT / 2 + 65.0f, 0.0f), ENEMY_SKELETON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_SKELETON);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 1020.0f, SCREEN_HEIGHT / 2 + 65.0f, 0.0f), ENEMY_SKELETON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_SKELETON);

        break;

    case STAGE1_WAVE6:

        // 第6ウェーブ(くの字隊列 ハエ)
        // ハエ(くの字)
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 100.0f, SCREEN_HEIGHT / 2 + 60.0f, 0.0f), ENEMY_FLY_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_FLY);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 175.0f, SCREEN_HEIGHT / 2 - 75.0f + 60.0f, 0.0f), ENEMY_FLY_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_FLY);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 250.0f, SCREEN_HEIGHT / 2 - 150.0f + 60.0f, 0.0f), ENEMY_FLY_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_FLY);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 325.0f, SCREEN_HEIGHT / 2 - 225.0f + 60.0f, 0.0f), ENEMY_FLY_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_FLY);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 425.0f, SCREEN_HEIGHT / 2 - 225.0f + 60.0f, 0.0f), ENEMY_FLY_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_FLY);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 175.0f, SCREEN_HEIGHT / 2 + 75.0f + 60.0f, 0.0f), ENEMY_FLY_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_FLY);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 250.0f, SCREEN_HEIGHT / 2 + 150.0f + 60.0f, 0.0f), ENEMY_FLY_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_FLY);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 325.0f, SCREEN_HEIGHT / 2 + 225.0f + 60.0f, 0.0f), ENEMY_FLY_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_FLY);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 425.0f, SCREEN_HEIGHT / 2 + 225.0f + 60.0f, 0.0f), ENEMY_FLY_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_FLY);

        break;

    case STAGE1_WAVE_BOSS_START:

        // ゲームのBGMを止める
        pSound->Stop(CSound::LABEL_BGM_GAME1);

        // ワーニング音
        pSound->Play(CSound::LABEL_SE_WARNING);

        // ワーニング
        CUI::Create(WARNING_POS, WARNING_SIZE, DEFAULT_COLOR, CUI::TYPE_WARNING);

        // ボス森
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 825.0f, 660.0f, 0.0f), ENEMY_BOSS_FOREST_SIZE,
            0, false, CEnemy::TYPE_BOSS_FOREST);

        // ボスバトルのフラグをtrueに
        m_bBossBattle = true;

        break;

    case STAGE1_WAVE_SET_FLOOR:

        // 危険！(上)
        CUI::Create(CAREFUL_UPPER_POS, CAREFUL_UPPER_SIZE, DEFAULT_COLOR, CUI::TYPE_CAREFUL_UPPER);

        // ボスBGM
        pSound->Play(CSound::LABEL_BGM_BOSS1);

        // 床の判定のフラグをtrueに
        m_bFloor = true;

        break;

    case STAGE1_WAVE_BLUE_BIRD:

        // 青鳥
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 150.0f, -300.0f, 0.0f), ENEMY_BLUE_BIRD_SIZE,
            0, true, CEnemy::TYPE_BLUE_BIRD);

        break;

    case STAGE_WAVE_STOP:

        // ゲーム時間を最後のウェーブが終わった後に戻す
        m_nCntGameTime = STAGE1_WAVE_BLUE_BIRD + 1;

        break;
    }
}

//========================================
// ステージβ
//========================================
void CGame::Stageβ(void)
{
    // サウンドを取得
    CSound *pSound = CManager::GetSound();

    // ランダムに泡を生成
    SetRandBubble();

    switch (m_nCntGameTime)
    {
    case STAGE2_START:
        CUI::Create(STAGE_START_POS, DEFAULT_VECTOR, DEFAULT_COLOR, CUI::TYPE_START_STAGE2);
        break;

    case STAGE2_WAVE1:

        // 危険！(下)
        CUI::Create(CAREFUL_DOWN_POS, CAREFUL_DOWN_SIZE, DEFAULT_COLOR, CUI::TYPE_CAREFUL_DOWN);

        // グソクムシ(下段)
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 100.0f, 635.0f, 0.0f), ENEMY_GUSOKU_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_GUSOKU);

        break;

    case STAGE2_WAVE2:

        // サーディン(中段)
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 100.0f, SCREEN_HEIGHT / 2, 0.0f), ENEMY_SARDINE_SIZE,
            CEnemy::SARDINE_MOVE_MIDDLE, true, CEnemy::TYPE_SARDINE);

        // サーディン(中段)
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 250.0f, SCREEN_HEIGHT / 2 + 20.0f, 0.0f), ENEMY_SARDINE_SIZE,
            CEnemy::SARDINE_MOVE_MIDDLE, true, CEnemy::TYPE_SARDINE);

        // サーディン(中段)
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 400.0f, SCREEN_HEIGHT / 2 + 40.0f, 0.0f), ENEMY_SARDINE_SIZE,
            CEnemy::SARDINE_MOVE_MIDDLE, true, CEnemy::TYPE_SARDINE);

        // サーディン(中段)
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 550.0f, SCREEN_HEIGHT / 2 + 60.0f, 0.0f), ENEMY_SARDINE_SIZE,
            CEnemy::SARDINE_MOVE_MIDDLE, true, CEnemy::TYPE_SARDINE);

        // サーディン(中段)
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 700.0f, SCREEN_HEIGHT / 2 + 80.0f, 0.0f), ENEMY_SARDINE_SIZE,
            CEnemy::SARDINE_MOVE_MIDDLE, true, CEnemy::TYPE_SARDINE);

        break;

    case STAGE2_WAVE3:

        // サーディン(上段)
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 100.0f, 150.0f, 0.0f), ENEMY_SARDINE_SIZE,
            CEnemy::SARDINE_MOVE_UPPER, true, CEnemy::TYPE_SARDINE);

        // サーディン(上段)
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 250.0f, 150.0f + 20.0f, 0.0f), ENEMY_SARDINE_SIZE,
            CEnemy::SARDINE_MOVE_UPPER, true, CEnemy::TYPE_SARDINE);

        // サーディン(上段)
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 400.0f, 150.0f + 40.0f, 0.0f), ENEMY_SARDINE_SIZE,
            CEnemy::SARDINE_MOVE_UPPER, true, CEnemy::TYPE_SARDINE);

        // サーディン(上段)
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 550.0f, 150.0f + 60.0f, 0.0f), ENEMY_SARDINE_SIZE,
            CEnemy::SARDINE_MOVE_UPPER, true, CEnemy::TYPE_SARDINE);

        // サーディン(上段)
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 700.0f, 150.0f + 80.0f, 0.0f), ENEMY_SARDINE_SIZE,
            CEnemy::SARDINE_MOVE_UPPER, true, CEnemy::TYPE_SARDINE);

        break;

    case STAGE2_WAVE4:

        // サーディン(下段)
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 100.0f, 550.0f, 0.0f), ENEMY_SARDINE_SIZE,
            CEnemy::SARDINE_MOVE_LOWER, true, CEnemy::TYPE_SARDINE);

        // サーディン(下段)
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 250.0f, 550.0f + 20.0f, 0.0f), ENEMY_SARDINE_SIZE,
            CEnemy::SARDINE_MOVE_LOWER, true, CEnemy::TYPE_SARDINE);

        // サーディン(下段)
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 400.0f, 550.0f + 40.0f, 0.0f), ENEMY_SARDINE_SIZE,
            CEnemy::SARDINE_MOVE_LOWER, true, CEnemy::TYPE_SARDINE);

        // サーディン(下段)
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 550.0f, 550.0f + 60.0f, 0.0f), ENEMY_SARDINE_SIZE,
            CEnemy::SARDINE_MOVE_LOWER, true, CEnemy::TYPE_SARDINE);

        // サーディン(下段)
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 700.0f, 550.0f + 80.0f, 0.0f), ENEMY_SARDINE_SIZE,
            CEnemy::SARDINE_MOVE_LOWER, true, CEnemy::TYPE_SARDINE);

        break;

    case STAGE2_WAVE5:

        // 危険！(右)
        CUI::Create(CAREFUL_RIGHT_POS, CAREFUL_RIGHT_SIZE, DEFAULT_COLOR, CUI::TYPE_CAREFUL_RIGHT);

        // 岩
        CBlock::Create(D3DXVECTOR3(SCREEN_WIDTH + 200.0f, 75.0f, 0.0f), BLOCK_ROCK_SIZE, false, CBlock::TYPE_ROCK);

        // 岩
        CBlock::Create(D3DXVECTOR3(SCREEN_WIDTH + 250.0f, 550.0f, 0.0f), BLOCK_ROCK_SIZE, false, CBlock::TYPE_ROCK);

        // 砲台
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 200.0f, 450.0f, 0.0f), ENEMY_CANNON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_CANNON);

        // 岩
        CBlock::Create(D3DXVECTOR3(SCREEN_WIDTH + 750.0f, 125.0f, 0.0f), BLOCK_ROCK_SIZE, false, CBlock::TYPE_ROCK);

        // 砲台
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 715.0f, 235.0f, 0.0f), ENEMY_CANNON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_CANNON);

        // 岩
        CBlock::Create(D3DXVECTOR3(SCREEN_WIDTH + 850.0f, 650.0f, 0.0f), BLOCK_ROCK_SIZE, false, CBlock::TYPE_ROCK);

        // 砲台
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 810.0f, 550.0f, 0.0f), ENEMY_CANNON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_CANNON);

        break;

    case STAGE2_WAVE6:

        // サーディン(中段)
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 550.0f, SCREEN_HEIGHT / 2 + 20.0f, 0.0f), ENEMY_SARDINE_SIZE,
            CEnemy::SARDINE_MOVE_MIDDLE, true, CEnemy::TYPE_SARDINE);

        // サーディン(中段)
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 700.0f, SCREEN_HEIGHT / 2 + 40.0f, 0.0f), ENEMY_SARDINE_SIZE,
            CEnemy::SARDINE_MOVE_MIDDLE, true, CEnemy::TYPE_SARDINE);

        // サーディン(中段)
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 850.0f, SCREEN_HEIGHT / 2 + 60.0f, 0.0f), ENEMY_SARDINE_SIZE,
            CEnemy::SARDINE_MOVE_MIDDLE, true, CEnemy::TYPE_SARDINE);

        // サーディン(中段)
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 1000.0f, SCREEN_HEIGHT / 2 + 80.0f, 0.0f), ENEMY_SARDINE_SIZE,
            CEnemy::SARDINE_MOVE_MIDDLE, true, CEnemy::TYPE_SARDINE);

        // 岩
        CBlock::Create(D3DXVECTOR3(SCREEN_WIDTH + 200.0f, 600.0f, 0.0f), BLOCK_ROCK_SIZE, false, CBlock::TYPE_ROCK);

        // 岩
        CBlock::Create(D3DXVECTOR3(SCREEN_WIDTH + 500.0f, 135.0f, 0.0f), BLOCK_ROCK_SIZE, false, CBlock::TYPE_ROCK);

        // 岩
        CBlock::Create(D3DXVECTOR3(SCREEN_WIDTH + 900.0f, 600.0f, 0.0f), BLOCK_ROCK_SIZE, false, CBlock::TYPE_ROCK);

        // 砲台
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 875.0f, 500.0f, 0.0f), ENEMY_CANNON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_CANNON);

        // 岩
        CBlock::Create(D3DXVECTOR3(SCREEN_WIDTH + 950.0f, 115.0f, 0.0f), BLOCK_ROCK_SIZE, false, CBlock::TYPE_ROCK);

        break;

    case STAGE2_WAVE7:

        // 岩
        CBlock::Create(D3DXVECTOR3(SCREEN_WIDTH + 400.0f, 700.0f, 0.0f), BLOCK_ROCK_SIZE, false, CBlock::TYPE_ROCK);

        // 砲台
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 425.0f, 600.0f, 0.0f), ENEMY_CANNON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_CANNON);

        // 砲台
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 875.0f, 640.0f, 0.0f), ENEMY_CANNON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_CANNON);

        // トーピード
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 300.0f, -250.0f, 0.0f), ENEMY_TORPEDO_SIZE,
            DONT_CHANGE_MOVE, false, CEnemy::TYPE_TORPEDO);

        // トーピード(バリアアイテム)
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 450.0f, -250.0f, 0.0f), ENEMY_TORPEDO_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_TORPEDO);

        // トーピード
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 600.0f, -250.0f, 0.0f), ENEMY_TORPEDO_SIZE,
            DONT_CHANGE_MOVE, false, CEnemy::TYPE_TORPEDO);

        // トーピード
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 800.0f, -500.0f, 0.0f), ENEMY_TORPEDO_SIZE,
            DONT_CHANGE_MOVE, false, CEnemy::TYPE_TORPEDO);

        // トーピード(バリアアイテム)
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 950.0f, -500.0f, 0.0f), ENEMY_TORPEDO_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_TORPEDO);

        // トーピード
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 1100.0f, -500.0f, 0.0f), ENEMY_TORPEDO_SIZE,
            DONT_CHANGE_MOVE, false, CEnemy::TYPE_TORPEDO);

        break;

    case STAGE2_WAVE8:

        // 第8ウェーブ(くの字隊列 サーディン)
        // サーディン(くの字)
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 200.0f, SCREEN_HEIGHT / 2 + 60.0f, 0.0f), ENEMY_SARDINE_SIZE,
            CEnemy::SARDINE_MOVE_STRAIGHT, true, CEnemy::TYPE_SARDINE);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 300.0f, SCREEN_HEIGHT / 2 - 75.0f + 60.0f, 0.0f), ENEMY_SARDINE_SIZE,
            CEnemy::SARDINE_MOVE_STRAIGHT, true, CEnemy::TYPE_SARDINE);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 400.0f, SCREEN_HEIGHT / 2 - 150.0f + 60.0f, 0.0f), ENEMY_SARDINE_SIZE,
            CEnemy::SARDINE_MOVE_STRAIGHT, true, CEnemy::TYPE_SARDINE);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 500.0f, SCREEN_HEIGHT / 2 - 225.0f + 60.0f, 0.0f), ENEMY_SARDINE_SIZE,
            CEnemy::SARDINE_MOVE_STRAIGHT, true, CEnemy::TYPE_SARDINE);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 600.0f, SCREEN_HEIGHT / 2 - 225.0f + 60.0f, 0.0f), ENEMY_SARDINE_SIZE,
            CEnemy::SARDINE_MOVE_STRAIGHT, true, CEnemy::TYPE_SARDINE);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 300.0f, SCREEN_HEIGHT / 2 + 75.0f + 60.0f, 0.0f), ENEMY_SARDINE_SIZE,
            CEnemy::SARDINE_MOVE_STRAIGHT, true, CEnemy::TYPE_SARDINE);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 400.0f, SCREEN_HEIGHT / 2 + 150.0f + 60.0f, 0.0f), ENEMY_SARDINE_SIZE,
            CEnemy::SARDINE_MOVE_STRAIGHT, true, CEnemy::TYPE_SARDINE);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 500.0f, SCREEN_HEIGHT / 2 + 225.0f + 60.0f, 0.0f), ENEMY_SARDINE_SIZE,
            CEnemy::SARDINE_MOVE_STRAIGHT, true, CEnemy::TYPE_SARDINE);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 600.0f, SCREEN_HEIGHT / 2 + 225.0f + 60.0f, 0.0f), ENEMY_SARDINE_SIZE,
            CEnemy::SARDINE_MOVE_STRAIGHT, true, CEnemy::TYPE_SARDINE);

        break;

    case STAGE2_WAVE_BOSS_START:

        // ゲームのBGMを止める
        pSound->Stop(CSound::LABEL_BGM_GAME2);

        // ワーニング音
        pSound->Play(CSound::LABEL_SE_WARNING);

        // ワーニング
        CUI::Create(WARNING_POS, WARNING_SIZE, DEFAULT_COLOR, CUI::TYPE_WARNING);

        // ライトの演出
        CLight2D::Create(LIGHT_FIRST_POS, LIGHT_SIZE);

        break;

    case STAGE2_WAVE_DARKENING:

        // 暗転
        m_bDarkening = true;

        // プレイヤーの移動制限&位置変える
        SetMovePlayer(false);

        break;

    case STAGE2_WAVE_BOSS_SET:

        // ボスドラゴン
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 300.0f, ENEMY_BOSS_DRAGON_STOP_POS_Y, 0.0f), ENEMY_BOSS_DRAGON_SIZE,
            ENEMY_BOSS_DRAGON_FIRST_ATTACK_COUNTER, false, CEnemy::TYPE_BOSS_DRAGON);

        break;

    case STAGE2_WAVE_STOP_FLOWING:

        // ボスバトルのフラグをtrueに
        m_bBossBattle = true;

        break;

    case STAGE2_WAVE_PLAY_SOUND:

        // ボスBGM
        pSound->Play(CSound::LABEL_BGM_BOSS2);

        break;

    case STAGE2_WAVE_LIGHTNING:

        // 暗転解除
        m_bDarkening = false;

        // プレイヤーの移動制限解除
        CGame::SetMovePlayer(true);

        break;

    case STAGE_WAVE_STOP:

        // ゲーム時間を最後のウェーブが終わった後に戻す
        m_nCntGameTime = STAGE2_WAVE_LIGHTNING + 1;

        break;
    }
}

//========================================
// ステージγ
//========================================
void CGame::Stageγ(void)
{
    // サウンドを取得
    CSound *pSound = CManager::GetSound();

    // 飛び立つ前まで
    if (m_nCntGameTime < STAGE3_TAKE_OFF)
    {
        // ランダムに炎の床を生成
        SetRandMagma();

        // ランダムに炎を生成
        SetRandFire();
    }
    // 空の中
    else if (m_nCntGameTime >= STAGE3_TAKE_OFF && m_nCntGameTime < STAGE3_WAVE_SET_FLOOR)
    {
        // 雲を生成
        SetRandCloud();

        // 床の判定を消す
        m_bFloor = false;
    }
    // ボス戦時
    else if (m_nCntGameTime >= STAGE3_WAVE_SET_FLOOR)
    {
        // ランダムに炎を生成
        SetRandFire();

        // 床の判定を復活
        m_bFloor = true;
    }

    switch (m_nCntGameTime)
    {
    case STAGE3_START:
        CUI::Create(STAGE_START_POS, DEFAULT_VECTOR, DEFAULT_COLOR, CUI::TYPE_START_STAGE3);
        break;

    case STAGE3_WAVE1:

        // 白カラス(上段)
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 200.0f, 180.0f, 0.0f), ENEMY_CROW_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_CROW);

        break;

    case STAGE3_WAVE2:

        // 青鳥
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 160.0f, -300.0f, 0.0f), ENEMY_BLUE_BIRD_SIZE,
            0, false, CEnemy::TYPE_BLUE_BIRD);

        // 緑鳥
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 110.0f, 200.0f, 0.0f), ENEMY_GREEN_BIRD_SIZE,
            0, false, CEnemy::TYPE_GREEN_BIRD);

        // 炎鳥
        CEnemy::Create(D3DXVECTOR3(-700.0f, 330.0f, 0.0f), ENEMY_RED_BIRD_SIZE,
            0, false, CEnemy::TYPE_RED_BIRD);

        break;

    case STAGE3_WAVE2_DANGER:
        // 危険！(左)
        CUI::Create(CAREFUL_LEFT_POS, CAREFUL_LEFT_SIZE, DEFAULT_COLOR, CUI::TYPE_CAREFUL_LEFT);
        break;

    case STAGE3_WAVE3:

        // 骸骨ラッシュ
        // 上段
        // 上列
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 100.0f, SCREEN_HEIGHT / 2 - 65.0f - 100.0f, 0.0f), ENEMY_SKELETON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_SKELETON);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 260.0f, SCREEN_HEIGHT / 2 - 65.0f - 100.0f, 0.0f), ENEMY_SKELETON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_SKELETON);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 420.0f, SCREEN_HEIGHT / 2 - 65.0f - 100.0f, 0.0f), ENEMY_SKELETON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_SKELETON);
        // 下列
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 100.0f, SCREEN_HEIGHT / 2 + 35.0f - 100.0f, 0.0f), ENEMY_SKELETON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_SKELETON);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 260.0f, SCREEN_HEIGHT / 2 + 35.0f - 100.0f, 0.0f), ENEMY_SKELETON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_SKELETON);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 420.0f, SCREEN_HEIGHT / 2 + 35.0f - 100.0f, 0.0f), ENEMY_SKELETON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_SKELETON);

        // 下段
        // 上列
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 200.0f, SCREEN_HEIGHT / 2 - 65.0f + 200.0f, 0.0f), ENEMY_SKELETON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_SKELETON);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 360.0f, SCREEN_HEIGHT / 2 - 65.0f + 200.0f, 0.0f), ENEMY_SKELETON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_SKELETON);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 520.0f, SCREEN_HEIGHT / 2 - 65.0f + 200.0f, 0.0f), ENEMY_SKELETON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_SKELETON);
        // 下列
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 200.0f, SCREEN_HEIGHT / 2 + 35.0f + 200.0f, 0.0f), ENEMY_SKELETON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_SKELETON);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 360.0f, SCREEN_HEIGHT / 2 + 35.0f + 200.0f, 0.0f), ENEMY_SKELETON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_SKELETON);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 520.0f, SCREEN_HEIGHT / 2 + 35.0f + 200.0f, 0.0f), ENEMY_SKELETON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_SKELETON);

        // 中段
        // 上列
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 700.0f, SCREEN_HEIGHT / 2 - 65.0f + 50.0f, 0.0f), ENEMY_SKELETON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_SKELETON);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 860.0f, SCREEN_HEIGHT / 2 - 65.0f + 50.0f, 0.0f), ENEMY_SKELETON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_SKELETON);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 1020.0f, SCREEN_HEIGHT / 2 - 65.0f + 50.0f, 0.0f), ENEMY_SKELETON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_SKELETON);
        // 下列
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 700.0f, SCREEN_HEIGHT / 2 + 35.0f + 50.0f, 0.0f), ENEMY_SKELETON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_SKELETON);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 860.0f, SCREEN_HEIGHT / 2 + 35.0f + 50.0f, 0.0f), ENEMY_SKELETON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_SKELETON);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 1020.0f, SCREEN_HEIGHT / 2 + 35.0f + 50.0f, 0.0f), ENEMY_SKELETON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_SKELETON);

        break;

    case STAGE3_WAVE4:

        // 岩と砲台ゾーン
        // 岩
        CBlock::Create(D3DXVECTOR3(SCREEN_WIDTH + 200.0f, 700.0f, 0.0f), BLOCK_ROCK_SIZE, false, CBlock::TYPE_ROCK);

        // 岩
        CBlock::Create(D3DXVECTOR3(SCREEN_WIDTH + 1200.0f, 675.0f, 0.0f), BLOCK_ROCK_SIZE, false, CBlock::TYPE_ROCK);

        // 岩
        CBlock::Create(D3DXVECTOR3(SCREEN_WIDTH + 700.0f, 650.0f, 0.0f), BLOCK_ROCK_SIZE, false, CBlock::TYPE_ROCK);

        // 砲台
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 225.0f, 600.0f, 0.0f), ENEMY_CANNON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_CANNON);

        // 砲台
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 725.0f, 550.0f, 0.0f), ENEMY_CANNON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_CANNON);

        // 砲台
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 1225.0f, 575.0f, 0.0f), ENEMY_CANNON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_CANNON);

        break;

    case STAGE3_WAVE5:

        // コウモリの挨拶
        // コウモリ
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 725.0f, 360.0f, 0.0f), ENEMY_BAT_SIZE,
            0, false, CEnemy::TYPE_BAT);

        break;

    case STAGE3_WAVE6_DANGER:

        // 危険！(左)
        CUI::Create(CAREFUL_LEFT_POS, CAREFUL_LEFT_SIZE, DEFAULT_COLOR, CUI::TYPE_CAREFUL_LEFT);
        // 危険！(右)
        CUI::Create(CAREFUL_RIGHT_POS, CAREFUL_RIGHT_SIZE, DEFAULT_COLOR, CUI::TYPE_CAREFUL_RIGHT);

        break;

    case STAGE3_WAVE6:

        // コウモリの洗礼
        // コウモリ
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 50.0f, 680.0f, 0.0f), ENEMY_BAT_SIZE,
            0, false, CEnemy::TYPE_BAT);

        // コウモリ
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 250.0f, 180.0f, 0.0f), ENEMY_BAT_SIZE,
            0, false, CEnemy::TYPE_BAT);

        // コウモリ
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 850.0f, 360.0f, 0.0f), ENEMY_BAT_SIZE,
            0, false, CEnemy::TYPE_BAT);

        // コウモリ
        CEnemy::Create(D3DXVECTOR3(-550.0f, 360.0f, 0.0f), ENEMY_BAT_SIZE,
            0, false, CEnemy::TYPE_BAT);

        // コウモリ
        CEnemy::Create(D3DXVECTOR3(-1150.0f, 360.0f, 0.0f), ENEMY_BAT_SIZE,
            0, false, CEnemy::TYPE_BAT);

        break;

    case STAGE3_WAVE7:

        // コウモリゾーン終了
        // コウモリ
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 50.0f, 480.0f, 0.0f), ENEMY_BAT_SIZE,
            0, false, CEnemy::TYPE_BAT);

        break;

    case STAGE3_WAVE8:

        // マルチバード
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 180.0f, -320.0f, 0.0f), ENEMY_MULTI_BIRD_SIZE,
            0, false, CEnemy::TYPE_MULTI_BIRD);

        break;

    case STAGE3_WAVE9:

        // トーピード
        CEnemy::Create(D3DXVECTOR3(125.0f, -100.0f, 0.0f), ENEMY_TORPEDO_SIZE,
            DONT_CHANGE_MOVE, false, CEnemy::TYPE_TORPEDO);

        // トーピード
        CEnemy::Create(D3DXVECTOR3(275.0f, -200.0f, 0.0f), ENEMY_TORPEDO_SIZE,
            DONT_CHANGE_MOVE, false, CEnemy::TYPE_TORPEDO);

        // トーピード
        CEnemy::Create(D3DXVECTOR3(425.0f, -300.0f, 0.0f), ENEMY_TORPEDO_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_TORPEDO);

        // トーピード
        CEnemy::Create(D3DXVECTOR3(575.0f, -400.0f, 0.0f), ENEMY_TORPEDO_SIZE,
            DONT_CHANGE_MOVE, false, CEnemy::TYPE_TORPEDO);

        // トーピード
        CEnemy::Create(D3DXVECTOR3(725.0f, -500.0f, 0.0f), ENEMY_TORPEDO_SIZE,
            DONT_CHANGE_MOVE, false, CEnemy::TYPE_TORPEDO);

        break;

    case STAGE3_WAVE10:

        // トーピード
        CEnemy::Create(D3DXVECTOR3(525.0f, -100.0f, 0.0f), ENEMY_TORPEDO_SIZE,
            DONT_CHANGE_MOVE, false, CEnemy::TYPE_TORPEDO);

        // トーピード
        CEnemy::Create(D3DXVECTOR3(675.0f, -200.0f, 0.0f), ENEMY_TORPEDO_SIZE,
            DONT_CHANGE_MOVE, false, CEnemy::TYPE_TORPEDO);

        // トーピード
        CEnemy::Create(D3DXVECTOR3(825.0f, -300.0f, 0.0f), ENEMY_TORPEDO_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_TORPEDO);

        // トーピード
        CEnemy::Create(D3DXVECTOR3(975.0f, -400.0f, 0.0f), ENEMY_TORPEDO_SIZE,
            DONT_CHANGE_MOVE, false, CEnemy::TYPE_TORPEDO);

        // トーピード
        CEnemy::Create(D3DXVECTOR3(1125.0f, -500.0f, 0.0f), ENEMY_TORPEDO_SIZE,
            DONT_CHANGE_MOVE, false, CEnemy::TYPE_TORPEDO);

        break;

    case STAGE3_WAVE_END_SKY_MODE:

        // 骸骨(くの字)
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 200.0f, SCREEN_HEIGHT / 2 + 60.0f, 0.0f), ENEMY_SKELETON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_SKELETON);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 275.0f, SCREEN_HEIGHT / 2 - 75.0f + 60.0f, 0.0f), ENEMY_SKELETON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_SKELETON);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 350.0f, SCREEN_HEIGHT / 2 - 150.0f + 60.0f, 0.0f), ENEMY_SKELETON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_SKELETON);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 425.0f, SCREEN_HEIGHT / 2 - 225.0f + 60.0f, 0.0f), ENEMY_SKELETON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_SKELETON);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 525.0f, SCREEN_HEIGHT / 2 - 225.0f + 60.0f, 0.0f), ENEMY_SKELETON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_SKELETON);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 275.0f, SCREEN_HEIGHT / 2 + 75.0f + 60.0f, 0.0f), ENEMY_SKELETON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_SKELETON);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 350.0f, SCREEN_HEIGHT / 2 + 150.0f + 60.0f, 0.0f), ENEMY_SKELETON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_SKELETON);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 425.0f, SCREEN_HEIGHT / 2 + 225.0f + 60.0f, 0.0f), ENEMY_SKELETON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_SKELETON);
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 525.0f, SCREEN_HEIGHT / 2 + 225.0f + 60.0f, 0.0f), ENEMY_SKELETON_SIZE,
            DONT_CHANGE_MOVE, true, CEnemy::TYPE_SKELETON);

        break;

    case STAGE3_WAVE_BOSS_START:

        // ゲームのBGMを止める
        pSound->Stop(CSound::LABEL_BGM_GAME3);

        // ワーニング音
        pSound->Play(CSound::LABEL_SE_WARNING);

        // ワーニング
        CUI::Create(WARNING_POS, WARNING_SIZE, DEFAULT_COLOR, CUI::TYPE_WARNING);

        // ボスバトルのフラグをtrueに
        m_bBossBattle = true;

        // ボスブレイズ
        CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + 1100.0f, ENEMY_BOSS_BLAZE_STOP_POS_Y, 0.0f), ENEMY_BOSS_BLAZE_SIZE,
            ENEMY_BOSS_BLAZE_FIRST_ATTACK_COUNTER, false, CEnemy::TYPE_BOSS_BLAZE);

        break;

    case STAGE3_WAVE_PLAY_SOUND:

        // ボスBGM
        pSound->Play(CSound::LABEL_BGM_BOSS3);

        break;

    case STAGE3_WAVE_SET_FLOOR:

        // 床の判定のフラグをtrueに
        m_bFloor = true;

        break;

    case STAGE_WAVE_STOP:

        // ゲーム時間を最後のウェーブが終わった後に戻す
        m_nCntGameTime = STAGE3_WAVE_SET_FLOOR + 1;

        break;
    }
}

//========================================
// ランダムに泡を生成する処理
//========================================
void CGame::SetRandBubble(void)
{
    // 泡を生成
    if (m_nCntGameTime % STAGE2_SET_BUBBLE_COUNT == 0)
    {
        // 泡を生成する位置
        D3DXVECTOR3 pos = DEFAULT_VECTOR;

        // 位置X、Yをランダムに取得(画面内全体が範囲)
        int nMaxX = (SCREEN_WIDTH * 10);
        int nMaxY = (SCREEN_HEIGHT * 10);
        float fPosX = float(rand() % nMaxX) / 10.0f;
        float fPosY = float(rand() % nMaxY) / 10.0f;

        // 位置を反映
        pos = D3DXVECTOR3(fPosX, fPosY, 0.0f);

        // 6つの泡を生成
        CBubble::Create(pos, BUBBLE_MINIMUM_SIZE, CBubble::TYPE_MINIMUM);
        CBubble::Create(pos, BUBBLE_SMALL_SIZE, CBubble::TYPE_SMALL);
        CBubble::Create(pos, BUBBLE_NORMAL_SIZE, CBubble::TYPE_NORMAL);
        CBubble::Create(pos, BUBBLE_NORMAL2_SIZE, CBubble::TYPE_NORMAL2);
        CBubble::Create(pos, BUBBLE_BIG_SIZE, CBubble::TYPE_BIG);
        CBubble::Create(pos, BUBBLE_MAXIMUM_SIZE, CBubble::TYPE_MAXIMUM);
    }
}

//========================================
// ランダムに炎を生成する処理
//========================================
void CGame::SetRandFire(void)
{
    // 炎を生成するカウンタ
    if (m_nCntGameTime % STAGE3_SET_FIRE_COUNT == 0)
    {
        // 炎を生成する位置
        D3DXVECTOR3 pos = DEFAULT_VECTOR;
        float fSize = 0.0f;
        int nMinX = (SCREEN_WIDTH - (m_nCntGameTime * 3)) * 10;    // 最小値（だんだん右からマグマが出てくるため）
        if (nMinX < 0)
        {
            nMinX = 0;  // マイナスの値にはしない
        }
        int nMaxX = (SCREEN_WIDTH * 10);            // 炎発生の最大範囲
        float fPosX = 0.0f;                         // 発生位置X
        float fPosY = 0.0f;                         // 発生位置Y
        float fPosAdjustY = 0.0f;                   // 発生位置Yの調整

        // 道中か、塔の上かで、炎の発生位置の高さを変える
        if (m_nCntGameTime < STAGE3_WAVE_SET_FLOOR)
        {
            // 道中は高め
            fPosAdjustY = EFFECT_FIRE_START_POS_Y;
        }
        else
        {
            // 塔の上は低め
            fPosAdjustY = EFFECT_FIRE_START_POS_Y_BOSS;
        }

        // 画面右端から発生させるための位置X
        float fRandPosX = 0.0f;

        // エフェクト用に、大きさと座標を求める
        fSize = float(rand() % EFFECT_FIRE_MAX_SIZE + EFFECT_FIRE_MIN_SIZE) / 100.0f;
        fPosX = float(rand() % nMaxX - nMinX) / 10.0f;
        fRandPosX = SCREEN_WIDTH - fPosX;
        fPosY = (SCREEN_HEIGHT - fPosAdjustY) - float(rand() % 10000) / 100.0f;
        // 位置を反映
        pos = D3DXVECTOR3(fRandPosX, fPosY, 0.0f);
        // 炎を生成
        CEffect::Create(pos, D3DXVECTOR3(fSize, fSize, 0.0f), EFFECT_FIRE_FIRST_COLOR, CEffect::TYPE_FIRE);

        // エフェクト用に、大きさと座標を求める
        fSize = float(rand() % EFFECT_FIRE_MAX_SIZE + EFFECT_FIRE_MIN_SIZE) / 100.0f;
        fPosX = float(rand() % nMaxX - nMinX) / 10.0f;
        fRandPosX = SCREEN_WIDTH - fPosX;
        fPosY = (SCREEN_HEIGHT - fPosAdjustY) - float(rand() % 10000) / 100.0f;
        // 位置を反映
        pos = D3DXVECTOR3(fRandPosX, fPosY, 0.0f);
        // 炎を生成
        CEffect::Create(pos, D3DXVECTOR3(fSize, fSize, 0.0f), EFFECT_FIRE_FIRST_COLOR, CEffect::TYPE_FIRE);

        // エフェクト用に、大きさと座標を求める
        fSize = float(rand() % EFFECT_FIRE_MAX_SIZE + EFFECT_FIRE_MIN_SIZE) / 100.0f;
        fPosX = float(rand() % nMaxX - nMinX) / 10.0f;
        fRandPosX = SCREEN_WIDTH - fPosX;
        fPosY = (SCREEN_HEIGHT - fPosAdjustY) - float(rand() % 10000) / 100.0f;
        // 位置を反映
        pos = D3DXVECTOR3(fRandPosX, fPosY, 0.0f);
        // 炎を生成
        CEffect::Create(pos, D3DXVECTOR3(fSize, fSize, 0.0f), EFFECT_FIRE_FIRST_COLOR, CEffect::TYPE_FIRE);

        //=============================================================================================
        // ゲームの進行に影響を及ぼすなら、消す
        // エフェクト用に、大きさと座標を求める
        fSize = float(rand() % EFFECT_FIRE_MAX_SIZE + EFFECT_FIRE_MIN_SIZE) / 100.0f;
        fPosX = float(rand() % nMaxX - nMinX) / 10.0f;
        fRandPosX = SCREEN_WIDTH - fPosX;
        fPosY = (SCREEN_HEIGHT - fPosAdjustY) - float(rand() % 10000) / 100.0f;
        // 位置を反映
        pos = D3DXVECTOR3(fRandPosX, fPosY, 0.0f);
        // 炎を生成
        CEffect::Create(pos, D3DXVECTOR3(fSize, fSize, 0.0f), EFFECT_FIRE_FIRST_COLOR, CEffect::TYPE_FIRE);
        //=============================================================================================
    }
}

//========================================
// ランダムにマグマを生成する処理
//========================================
void CGame::SetRandMagma(void)
{
    // マグマを生成するカウンタ
    if (m_nCntGameTime % STAGE3_SET_MAGMA_COUNT == 0)
    {
        // 変数宣言
        D3DXVECTOR3 pos = DEFAULT_VECTOR;
        int nMaxX = (SCREEN_WIDTH * 10 + 6400);
        float fPosX = 0.0f;
        float fPosY = 0.0f;
        float fSize = 0.0f;

        // ランダムに位置とサイズを得る
        fPosX = float(rand() % nMaxX) / 10.0f;
        fPosY = (SCREEN_HEIGHT)-float(rand() % 5000) / 100.0f;
        fSize = float(rand() % EFFECT_MAGMA_MAX_SIZE + EFFECT_MAGMA_MIN_SIZE) / 100.0f;
        // 位置を反映
        pos = D3DXVECTOR3(SCREEN_WIDTH + (fSize / 2) - 1, fPosY, 0.0f);
        // マグマを生成
        CEffect::Create(pos, D3DXVECTOR3(fSize, fSize, 0.0f), EFFECT_MAGMA_COLOR, CEffect::TYPE_MAGMA);

        // マグマの一番下の床
        // ランダムにサイズを得る
        fSize = float(rand() % EFFECT_MAGMA_MAX_SIZE + EFFECT_MAGMA_MIN_SIZE) / 100.0f;
        // 位置を反映
        pos = D3DXVECTOR3(SCREEN_WIDTH + (fSize / 2) - 1, SCREEN_HEIGHT - (fSize / 3), 0.0f);
        // マグマを生成
        CEffect::Create(pos, D3DXVECTOR3(fSize, fSize, 0.0f), EFFECT_MAGMA_COLOR, CEffect::TYPE_MAGMA);
    }
}

//========================================
// ランダムに雲を生成する処理
//========================================
void CGame::SetRandCloud(void)
{
    // 雲を設置できるカウンタ以上で
    if (m_nCntGameTime >= STAGE3_SET_CLOUD_START)
    {
        // 変数宣言
        int nCntSetCount = 0;   // 雲を設置するカウンタ
        
        // ボス戦時かどうかで、雲を設置するカウンタを変える
        if (m_bBossBattle == false)
        {
            // 通常時
            nCntSetCount = STAGE3_SET_CLOUD_COUNT;
        }
        else
        {
            // ボス戦時
            nCntSetCount = STAGE3_SET_CLOUD_COUNT_BOSS;
        }

        // カウンタによって、雲を生成
        if (m_nCntGameTime % nCntSetCount == 0)
        {
            // 変数宣言
            float fLocalPosX = 0.0f;            // 雲の粒の座標X
            float fLocalPosY = 0.0f;            // 雲の粒の座標Y
            float fSize = 0.0f;                 // 雲の粒の大きさ
            float fGlobalPosX = 0.0f;           // 雲自体の座標X
            float fGlobalPosY = 0.0f;           // 雲自体の座標Y
            float fAlpha = 0.0f;                // α値
            D3DXVECTOR3 pos = DEFAULT_VECTOR;   // 生成位置

            // 空の中かどうかで、雲自体の生成地点を決める
            if (m_nCntGameTime < STAGE3_WAVE_END_SKY_MODE)
            {
                // 空の中の時は、画面右上から
                fGlobalPosX = float(rand() % 16000 + 6400) / 10.0f;
                fGlobalPosY = float(rand() % 2000 + 0) / 10.0f;

                // 画面外から出したいので、Yにマイナスをかける
                fGlobalPosY *= -1;
            }
            else if (m_nCntGameTime >= STAGE3_WAVE_END_SKY_MODE)
            {
                // 空の中でないときは、右から
                fGlobalPosX = float(rand() % 16000 + 15000) / 10.0f;
                fGlobalPosY = float(rand() % 6600 + 750) / 10.0f;
            }

            for (int nCnt = 0; nCnt < EFFECT_CLOUD_COUNT_LOOP; nCnt++)
            {
                // 雲の粒の生成地点を決める
                fLocalPosX = float(rand() % EFFECT_CLOUD_SIZE_X) / 100.0f - float(rand() % EFFECT_CLOUD_SIZE_X) / 100.0f;
                fLocalPosY = float(rand() % EFFECT_CLOUD_SIZE_Y) / 100.0f - float(rand() % EFFECT_CLOUD_SIZE_Y) / 100.0f;
                pos = D3DXVECTOR3(fGlobalPosX + fLocalPosX, fGlobalPosY + fLocalPosY, 0.0f);

                // 粒の生成地点のX座標によって、雲の粒の大きさとα値を決める
                if (fabsf(fLocalPosX) < EFFECT_CLOUD_EDGE)
                {
                    // 中心に近いなら、粒は大きく
                    fSize = float(rand() % EFFECT_CLOUD_PARTICLES_MAX_SIZE + EFFECT_CLOUD_PARTICLES_MID1_SIZE) / 100.0f;

                    // α値を少し高めに
                    fAlpha = float(rand() % EFFECT_CLOUD_PARTICLES_MAX_ALPHA + EFFECT_CLOUD_PARTICLES_MID_ALPHA) / 100.0f;
                }
                else
                {
                    // 中心から遠いなら、粒は小さく
                    fSize = float(rand() % EFFECT_CLOUD_PARTICLES_MID2_SIZE + EFFECT_CLOUD_PARTICLES_MIN_SIZE) / 100.0f;

                    // α値を低めに
                    fAlpha = float(rand() % EFFECT_CLOUD_PARTICLES_MID_ALPHA + EFFECT_CLOUD_PARTICLES_MIN_ALPHA) / 100.0f;
                }

                // 雲を生成
                CEffect::Create(pos, D3DXVECTOR3(fSize, fSize, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha), CEffect::TYPE_CLOUD);
            }
        }
    }
}
