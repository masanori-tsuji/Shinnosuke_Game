//=============================================================================
//
// タイトル処理 [title.cpp]
// Author : 後藤慎之助
//
//=============================================================================
#include "title.h"
#include "input.h"
#include "manager.h"
#include "sound.h"
#include "ui.h"
#include "fade.h"
#include "effect.h"
#include "logo.h"

//=============================================================================
// タイトルのコンストラクタ
//=============================================================================
CTitle::CTitle()
{
    m_nCntTime = 0;
    m_bNextScene = false;
}

//=============================================================================
// タイトルのデストラクタ
//=============================================================================
CTitle::~CTitle()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTitle::Init(void)
{
    // 背景を生成
    CLogo::Create(LOGO_BG_POS, LOGO_BG_SIZE, DEFAULT_COLOR, CLogo::TYPE_TITLE_BG);

    // ロゴを生成
    CUI::Create(TITLE_LOGO_FIRST_POS, TITLE_LOGO_SIZE, DEFAULT_COLOR, CUI::TYPE_TITLE_LOGO);

    // プレススタート
    CUI::Create(PRESS_START_POS, PRESS_START_SIZE, DEFAULT_COLOR, CUI::TYPE_PRESS_START);

    // BGMを再生
    CSound *pSound = CManager::GetSound();
    pSound->Play(CSound::LABEL_BGM_TITLE);

    return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTitle::Uninit(void)
{

}

//=============================================================================
// 更新処理
//=============================================================================
void CTitle::Update(void)
{
    // タイトルの時間をカウント
    m_nCntTime++;

    // タイトルの最大時間
    if (m_nCntTime > TITLE_MAX_TIME)
    {
        m_nCntTime = TITLE_MAX_TIME;
    }

    //キーボードの確保したメモリを取得
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // コントローラを取得
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(PLAYER_1);

    // 遷移のフラグがtrueなら
    if (m_bNextScene == true)
    {
        // エンター、またはスタートボタンを押したら
        if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START))
        {
            // フェードしていないなら
            if (CFade::GetFade() == CFade::FADE_NONE)
            {
                // 決定音再生
                CSound *pSound = CManager::GetSound();
                pSound->Play(CSound::LABEL_SE_DECISION);

                // 遊び方モードに移行
                CFade::SetFade(CManager::MODE_MANUAL);
            }
        }
    }
    // 遷移のフラグがfalseなら
    else
    {
        // 一定時間経過で遷移可能に
        if (m_nCntTime >= TITLE_NEXT_SCENE)
        {
            m_bNextScene = true;
        }
        // 一定時間経過していなくても
        else
        {
            // エンター、またはスタートボタンを押したら
            if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START))
            {
                // 遷移可能に
                m_bNextScene = true;
            }
        }
    }

    // 炎を生成する位置
    D3DXVECTOR3 pos = DEFAULT_VECTOR;
    float fAngle = 0.0f;
    float fRadius = 0.0f;
    float fSize = 0.0f;
    D3DXCOLOR color = DEFAULT_COLOR;

    // 赤い炎
    //==========================================================================================
    // エフェクト用に、角度と半径を求める
    fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
    fRadius = float(rand() % 1000) / 100.0f;
    fSize = float(rand() % 7500 + 2500) / 100.0f;

    // 位置を反映
    pos = D3DXVECTOR3(EFFECT_RED_FIRE_POS_X -sinf(fAngle)*fRadius, EFFECT_RED_FIRE_POS_Y - cosf(fAngle)*fRadius, 0.0f);

    if (fSize >= EFFECT_RED_FIRE_OVER_SIZE)
    {
        color = EFFECT_RED_FIRE_RED;
    }
    else
    {
        color = EFFECT_RED_FIRE_FIRST_COLOR;
    }

    // 炎を生成
    CEffect::Create(pos, D3DXVECTOR3(fSize, fSize, 0.0f), color, CEffect::TYPE_RED_FIRE);
    //===========================================================================================


    // 青い炎
    //==========================================================================================
    // エフェクト用に、角度と半径を求める
    fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
    fRadius = float(rand() % 1000) / 100.0f;
    fSize = float(rand() % 7500 + 2500) / 100.0f;

    // 位置を反映
    pos = D3DXVECTOR3(EFFECT_BLUE_FIRE_POS_X - sinf(fAngle)*fRadius, EFFECT_BLUE_FIRE_POS_Y - cosf(fAngle)*fRadius, 0.0f);

    if (fSize >= EFFECT_BLUE_FIRE_OVER_SIZE)
    {
        color = EFFECT_BLUE_FIRE_BLUE;
    }
    else
    {
        color = EFFECT_BLUE_FIRE_FIRST_COLOR;
    }

    // 炎を生成
    CEffect::Create(pos, D3DXVECTOR3(fSize, fSize, 0.0f), color, CEffect::TYPE_BLUE_FIRE);
    //===========================================================================================


    // 黄色い炎
    //==========================================================================================
    // エフェクト用に、角度と半径を求める
    fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
    fRadius = float(rand() % 1000) / 100.0f;
    fSize = float(rand() % 7500 + 2500) / 100.0f;

    // 位置を反映
    pos = D3DXVECTOR3(EFFECT_YELLOW_FIRE_POS_X - sinf(fAngle)*fRadius, EFFECT_YELLOW_FIRE_POS_Y - cosf(fAngle)*fRadius, 0.0f);

    if (fSize >= EFFECT_YELLOW_FIRE_OVER_SIZE)
    {
        color = EFFECT_YELLOW_FIRE_GREEN;
    }
    else
    {
        color = EFFECT_YELLOW_FIRE_FIRST_COLOR;
    }

    // 炎を生成
    CEffect::Create(pos, D3DXVECTOR3(fSize, fSize, 0.0f), color, CEffect::TYPE_YELLOW_FIRE);
    //===========================================================================================
}
