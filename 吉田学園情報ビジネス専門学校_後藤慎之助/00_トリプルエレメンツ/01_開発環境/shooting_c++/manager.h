//====================================================================
//
// 生成の管理処理 (manager.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"

//================================================
// 前方宣言(不完全なクラスは、cppの方でヘッダを呼び出す)
//================================================
class CRenderer;
class CInputKeyboard;
class CInputJoypad;
class CSound;
class CMode;
class CFade;

//================================================
// クラス宣言
//================================================

// マネージャークラス
class CManager
{
public:

    // モードの種類
    typedef enum
    {
        MODE_NONE =0,   // なし
        MODE_TITLE,     // タイトル
        MODE_MANUAL,    // 遊び方
        MODE_GAME,      // ゲーム
        MODE_RESULT,    // リザルト
        MODE_MAX
    }MODE;

    // ステージ
    typedef enum
    {
        STAGE_NONE = 0,     // なし
        STAGE_α,	        // α
        STAGE_β,	        // β
        STAGE_γ,           // γ
        STAGE_MAX			// ステージの最大数
    }STAGE;

    CManager();
    ~CManager();
    HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
    void Uninit(void);
    void Update(void);
    void Draw(void);  
    static CRenderer *GetRenderer(void) { return m_pRenderer; }
    static CInputKeyboard *GetInputKeyboard(void) { return m_pInputKeyboard; }
    static CInputJoypad *GetInputJoypad(void) { return m_pInputJoypad; }
    static CSound *GetSound(void) { return m_pSound; }
    static MODE GetMode(void);
    static void SetMode(MODE mode);

    static void SetScore(int nScore) { m_nScore = nScore; }
    static int GetScore(void) { return m_nScore; }
    static void SetUseHelper(bool bUse) { m_bUseHelper = bUse; }
    static bool GetUseHelper(void) { return m_bUseHelper; }
    static STAGE GetSetStage(void) { return m_stage; }
    static void SetSetStage(STAGE setStage) { m_stage = setStage; }

private:
    static CRenderer *m_pRenderer;              // レンダラーのポインタ
    static CInputKeyboard *m_pInputKeyboard;    // キーボードのポインタ
    static CInputJoypad *m_pInputJoypad;        // コントローラーのポインタ
    static CSound *m_pSound;                    // サウンドのポインタ
    static CMode *m_pMode;                      // モードのポインタ
    static CFade *m_pFade;                      // フェードのポインタ

    static int m_nScore;                        // スコア
    static bool m_bUseHelper;                   // ヘルパーを使ったかどうか
    static STAGE m_stage;                // セットするステージ
};

#endif