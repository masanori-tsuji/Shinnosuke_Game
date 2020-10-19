//=============================================================================
//
// 遊び方処理 [manual.cpp]
// Author : 後藤慎之助
//
//=============================================================================
#include "manual.h"
#include "input.h"
#include "manager.h"
#include "sound.h"
#include "ui.h"
#include "fade.h"

//=============================================================================
// 遊び方のコンストラクタ
//=============================================================================
CManual::CManual()
{
}

//=============================================================================
// 遊び方のデストラクタ
//=============================================================================
CManual::~CManual()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CManual::Init(void)
{
    // 仮の背景を生成
    CUI::Create(MANUAL_POS, MANUAL_SIZE, DEFAULT_COLOR, CUI::TYPE_MANUAL);

    //// 背景のインスタンス生成
    //CBG::Create(CBG::BG_TYPE_MANUAL);

    //// UIのインスタンス生成
    //CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 250.0f, 0.0f), CTexture::TEXTURE_UI06, D3DXVECTOR3(600.0f, 300.0f, 0.0f), false);
    //CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 100.0f, 0.0f), CTexture::TEXTURE_UI07, D3DXVECTOR3(400.0f, UI_SIZE_Y, 0.0f), true);

    //// BGMを再生
    //CSound *pSound = CManager::GetSound();
    //pSound->Play(CSound::SOUND_LABEL_BGM002);

    return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CManual::Uninit(void)
{
    // BGMを停止
    CSound *pSound = CManager::GetSound();
    pSound->Stop(CSound::LABEL_BGM_TITLE);
}

//=============================================================================
// 更新処理
//=============================================================================
void CManual::Update(void)
{
    //キーボードの確保したメモリを取得
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // コントローラを取得
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(PLAYER_1);

    if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START))
    {
        // フェードしていないなら
        if (CFade::GetFade() == CFade::FADE_NONE)
        {
            // 決定音再生
            CSound *pSound = CManager::GetSound();
            pSound->Play(CSound::LABEL_SE_DECISION);

            // ステージαから
            CManager::SetSetStage(CManager::STAGE_α);

            // ゲームモードに移行
            CFade::SetFade(CManager::MODE_GAME);
        }
    }
}
