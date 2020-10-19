//=============================================================================
//
// リザルト処理 [result.cpp]
// Author : 後藤慎之助
//
//=============================================================================
#include "result.h"
#include "input.h"
#include "manager.h"
#include "sound.h"
#include "ui.h"
#include "fade.h"
#include "ranking.h"
#include "score.h"
#include "logo.h"
#include "afterimage.h"
#include "enemy.h"

//========================================
// マクロ定義
//========================================

// 各順位に、カウンタを利用して敵を配置していく
#define ENEMY_5 5
#define ENEMY_4 95
#define ENEMY_3 215
#define ENEMY_2 335
#define ENEMY_1 455
#define STOP 1000       // カウンタのストップ

#define SET_POS_X 1400  // 配置のX座標

//=============================================================================
// リザルトのコンストラクタ
//=============================================================================
CResult::CResult()
{
    m_nCntTime = 0;
}

//=============================================================================
// リザルトのデストラクタ
//=============================================================================
CResult::~CResult()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CResult::Init(void)
{
    // 背景を生成
    CLogo::Create(LOGO_BG_POS, LOGO_BG_SIZE, DEFAULT_COLOR, CLogo::TYPE_RESULT_BG);

    // ソロ/デュオを取得
    if (CManager::GetUseHelper() == false)
    {
        // ソロ
        CUI::Create(RESULT_POS, RESULT_SIZE, DEFAULT_COLOR, CUI::TYPE_RESULT_SOLO);
    }
    else
    {
        // デュオ
        CUI::Create(RESULT_POS, RESULT_SIZE, DEFAULT_COLOR, CUI::TYPE_RESULT_DUO);
    }

    // ランキングを生成
    CRanking::Create(RANKING_FIRST_POS, SCORE_SIZE);

    // BGMを再生
    CSound *pSound = CManager::GetSound();
    pSound->Play(CSound::LABEL_BGM_RESULT);

    return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CResult::Uninit(void)
{
    // BGMを停止
    CSound *pSound = CManager::GetSound();
    pSound->Stop(CSound::LABEL_BGM_RESULT);
}

//=============================================================================
// 更新処理
//=============================================================================
void CResult::Update(void)
{
    // ランキングの演出
    RankingEffect();

    //キーボードの確保したメモリを取得
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // コントローラを取得
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(PLAYER_1);

    // 決定キーでタイトルへ
    if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START))
    {
        // フェードしていないなら
        if (CFade::GetFade() == CFade::FADE_NONE)
        {
            // 決定音再生
            CSound *pSound = CManager::GetSound();
            pSound->Play(CSound::LABEL_SE_DECISION);

            // タイトルモードに移行
            CFade::SetFade(CManager::MODE_TITLE);
        }
    }
}

//=============================================================================
// ランキングの表示演出
//=============================================================================
void CResult::RankingEffect(void)
{
    // カウンタ加算
    m_nCntTime++;

    // カウンタを利用して、敵を配置
    switch (m_nCntTime)
    {
    case ENEMY_5:

        // 5位、グソクムシ
        CAfterimage::Create(D3DXVECTOR3(SET_POS_X, 580.0f, 0.0f), ENEMY_GUSOKU_SIZE,
            0.0f, DEFAULT_COLOR, CAfterimage::TYPE_RANK_GUSOKU);

        break;

    case ENEMY_4:

        // 4位、白カラス
        CAfterimage::Create(D3DXVECTOR3(SET_POS_X, 480.0f, 0.0f), ENEMY_CROW_SIZE,
            0.0f, DEFAULT_COLOR, CAfterimage::TYPE_RANK_CROW);

        break;

    case ENEMY_3:

        // 3位、緑鳥
        CAfterimage::Create(D3DXVECTOR3(SET_POS_X, 400.0f, 0.0f), ENEMY_GREEN_BIRD_SIZE,
            0.0f, DEFAULT_COLOR, CAfterimage::TYPE_RANK_GREEN);

        break;

    case ENEMY_2:

        // 2位、青鳥
        CAfterimage::Create(D3DXVECTOR3(SET_POS_X, 300.0f, 0.0f), ENEMY_BLUE_BIRD_SIZE,
            0.0f, DEFAULT_COLOR, CAfterimage::TYPE_RANK_BLUE);

        break;

    case ENEMY_1:

        // 1位、赤鳥
        CAfterimage::Create(D3DXVECTOR3(SET_POS_X, 200.0f, 0.0f), ENEMY_RED_BIRD_SIZE,
            0.0f, DEFAULT_COLOR, CAfterimage::TYPE_RANK_RED);

        break;
    }

    // カウンタのストップ
    if (m_nCntTime > STOP)
    {
        m_nCntTime = STOP;
    }
}
