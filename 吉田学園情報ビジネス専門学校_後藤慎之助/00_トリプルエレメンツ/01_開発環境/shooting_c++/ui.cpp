//===============================================
//
// UI処理 (ui.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "ui.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "library.h"
#include "player.h"
#include "game.h"
#include "sound.h"
#include "title.h"

//=======================================
// 静的メンバ変数宣言
//=======================================
LPDIRECT3DTEXTURE9 CUI::m_apTexture[CUI::TYPE_MAX] = {};
bool CUI::m_bChangeZoneToβ = false;
bool CUI::m_bChangeZoneToγ = false;

//========================================
// UIのコンストラクタ
//========================================
CUI::CUI() :CScene2D(OBJTYPE::OBJTYPE_UI)
{
    m_move = DEFAULT_VECTOR;
    m_col = DEFAULT_COLOR;
    m_type = CUI::TYPE_MAX;
    m_bFadeOut = false;
    m_bBlinking = false;
    m_nCntTime = 0;
}

//========================================
// UIのデストラクタ
//========================================
CUI::~CUI()
{

}

//========================================
// UIの初期化処理
//========================================
HRESULT CUI::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 初期化
    CScene2D::Init(pos, size);

    // テクスチャを割り当て
    BindTexture(m_apTexture[m_type]);

    return S_OK;
}

//========================================
// UIの終了処理
//========================================
void CUI::Uninit(void)
{
    // 終了処理
    CScene2D::Uninit();
}

//========================================
// UIの更新処理
//========================================
void CUI::Update(void)
{
    // 変数宣言
    D3DXVECTOR3 pos = GetPosition();        // 位置を取得
    D3DXVECTOR3 size = GetSize();           // サイズを取得
    int nPatternAnim = 0;                   // パターンアニメのカウンタを利用するとき
    bool bUse = true;                       // 使うかどうか

    // プレイヤー関連の変数宣言
    CPlayer *pPlayer = NULL;                // プレイヤーを取得
    CScene2D *pPlayerScene2D = NULL;        // プレイヤーをシーン2Dにキャスト
    D3DXVECTOR3 playerPos = DEFAULT_VECTOR; // プレイヤーの位置
    D3DXVECTOR3 playerSize = DEFAULT_VECTOR;// プレイヤーの大きさ
    int nPlayerBarrier = 0;                 // プレイヤーのバリア数
    int nPlayerStcok = 0;                   // プレイヤーのストック数
    int nPlayerSpeedRank = 0;               // プレイヤーのスピードランク
    float fBarrierAlpha = 0.0f;             // バリアの濃さ
    float fPlayerShotAngle = 0.0f;          // 水の発射角度
    int nCnt = 0;                           // カウンタ

    // ゲームなら
    if (CManager::GetMode() == CManager::MODE_GAME)
    {
        nPlayerStcok = CGame::GetStock();

        // プレイヤーが存在しているなら
        if (CGame::GetDispPlayer() == true)
        {
            // プレイヤーの情報を取得
            pPlayer = CGame::GetPlayer();
            pPlayerScene2D = (CScene2D*)pPlayer;
            playerPos = pPlayerScene2D->GetPosition();
            playerSize = pPlayerScene2D->GetSize();
            nPlayerBarrier = pPlayer->GetBarrier();
            nPlayerSpeedRank = pPlayer->GetSpeedRank();
            fPlayerShotAngle = pPlayer->GetShotAngle();
        }
    }

    //キーボードの確保したメモリを取得
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // コントローラを取得
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(PLAYER_1);

    // サウンドを取得
    CSound *pSound = CManager::GetSound();

    //=========================================================
    // プレイヤーの情報を参照する、UIの乱れを制御
    if (nPlayerBarrier < 0)
    {
        nPlayerBarrier = 0;
    }
    else if (nPlayerBarrier > PLAYER_MAX_BARRIER)
    {
        nPlayerBarrier = PLAYER_MAX_BARRIER;
    }

    if (nPlayerStcok < 0)
    {
        nPlayerStcok = 0;
    }
    else if (nPlayerStcok > PLAYER_MAX_STOCK)
    {
        nPlayerStcok = PLAYER_MAX_STOCK;
    }

    if (nPlayerSpeedRank < 0)
    {
        nPlayerSpeedRank = 0;
    }
    else if (nPlayerSpeedRank > PLAYER_MAX_SPEED_RANK)
    {
        nPlayerSpeedRank = PLAYER_MAX_SPEED_RANK;
    }
    //=========================================================

    // バリアの濃さを取得
    switch (nPlayerBarrier)
    {
    case 1:
        fBarrierAlpha = BARRIER_MAX_ALPHA_1;
        break;
    case 2:
        fBarrierAlpha = BARRIER_MAX_ALPHA_2;
        break;
    case 3:
        fBarrierAlpha = BARRIER_MAX_ALPHA_3;
        break;
    case 4:
        fBarrierAlpha = BARRIER_MAX_ALPHA_4;
        break;
    case 5:
        fBarrierAlpha = BARRIER_MAX_ALPHA_5;
        break;
    }

    // 種類に応じた場合分け
    switch (m_type)
    {
        // 矢印
    case CUI::TYPE_ARROW_SYMBOL:

        // 位置を自機の上に設定
        pos = D3DXVECTOR3(playerPos.x, playerPos.y - (playerSize.y / 2), 0.0f);

        // だんだん透明に
        m_col.a -= 0.1f;

        // 右スティックが倒れているなら透明度を上げる
        if (Controller.lZ != 0 || Controller.lRz != 0)
        {
            // 透明度を1.0fに
            m_col.a = 1.0f;
        }

        // 左スティック押し込みで、発射角度が背面にリセットされたことを示す
        if (pInputJoypad->GetJoypadPress(PLAYER_1, CInputJoypad::BUTTON_L3))
        {
            // 透明度を1.0fに
            m_col.a = 1.0f;
        }

        // 右スティック押し込みで、発射角度が正面にリセットされたことを示す
        if (pInputJoypad->GetJoypadPress(PLAYER_1, CInputJoypad::BUTTON_R3))
        {
            // 透明度を1.0fに
            m_col.a = 1.0f;
        }

        // 透明度が0.0f以下になったら
        if (m_col.a <= 0.0f)
        {
            // 0.0fで固定
            m_col.a = 0.0f;
        }

        // 暗転状態で、透明度を0.0fに
        if (CGame::GetDarkening() == true)
        {
            // 0.0fで固定
            m_col.a = 0.0f;
        }

        // 色を変更
        CScene2D::SetColor(m_col);

        break;

        // ストックのバー
    case CUI::TYPE_STOCK_BAR:

        // 横サイズ調整
        size.x = STOCK_BAR_UI_SIZE_X / ((float)PLAYER_MAX_STOCK / (float)nPlayerStcok);

        // 画像の描画範囲を調整
        SetTextureRange(nPlayerStcok, PLAYER_MAX_STOCK);

        break;

        // バリアのバー
    case CUI::TYPE_BARRIER_BAR:

        // 横サイズ調整
        size.x = BARRIER_BAR_UI_SIZE_X / ((float)PLAYER_MAX_BARRIER / (float)nPlayerBarrier);

        // 画像の描画範囲を調整
        SetTextureRange(nPlayerBarrier, PLAYER_MAX_BARRIER);

        break;

        // スピードのバー
    case CUI::TYPE_SPEED_BAR:

        // 横サイズ調整
        size.x = SPEED_BAR_UI_SIZE_X / ((float)PLAYER_MAX_SPEED_RANK / (float)nPlayerSpeedRank);

        // 画像の描画範囲を調整
        SetTextureRange(nPlayerSpeedRank, PLAYER_MAX_SPEED_RANK);

        // ショック状態なら、赤色にする
        if (CGame::GetShockPlayer() == true)
        {
            m_col.g = 0.0f;
            m_col.b = 0.0f;
        }
        else
        {
            m_col.g = 1.0f;
            m_col.b = 1.0f;
        }

        // 色を設定
        SetColor(m_col);

        break;

        // ゾーンα
    case CUI::TYPE_ZONEα:

        // ゾーンが切り替わるなら
        if (m_bChangeZoneToβ == true)
        {
            // 未使用に
            bUse = false;

            // フラグを戻す
            m_bChangeZoneToβ = false;
        }

        break;

        // ゾーンβ
    case CUI::TYPE_ZONEβ:

        // ゾーンが切り替わるなら
        if (m_bChangeZoneToγ == true)
        {
            // 未使用に
            bUse = false;

            // フラグを戻す
            m_bChangeZoneToγ = false;
        }

        break;

        // ゾーンγ
    case CUI::TYPE_ZONEγ:


        break;

        // ステージ開始1
    case CUI::TYPE_START_STAGE1:

        // 横サイズ拡大
        size.x += STAGE_START_EXTEND_RATE_X;

        // 縦サイズ拡大
        size.y += STAGE_START_EXTEND_RATE_Y;

        // 横サイズの制限
        if (size.x >= STAGE_START_MAX_SIZE_X)
        {
            size.x = STAGE_START_MAX_SIZE_X;
        }

        // 縦サイズの制限
        if (size.y >= STAGE_START_MAX_SIZE_Y)
        {
            size.y = STAGE_START_MAX_SIZE_Y;
        }

        // アニメーションのカウンタを利用して、UIのフェードアウトにつなげる
        nCnt = CountAnimation(1, MAX_ANIM_COUNTER);

        // フェードアウトを始めるカウントを超えたら、α値を下げていく
        if (nCnt >= STAGE_START_FADE_OUT_COUNT)
        {
            m_col.a -= STAGE_START_FADE_OUT_RATE;
        }

        // 色を変更
        CScene2D::SetColor(m_col);

        // 透明度が0を下回ったら、消す
        if (m_col.a <= 0.0f)
        {
            bUse = false;
        }

        break;

        // ステージ開始2
    case CUI::TYPE_START_STAGE2:

        // 横サイズ拡大
        size.x += STAGE_START_EXTEND_RATE_X;

        // 縦サイズ拡大
        size.y += STAGE_START_EXTEND_RATE_Y;

        // 横サイズの制限
        if (size.x >= STAGE_START_MAX_SIZE_X)
        {
            size.x = STAGE_START_MAX_SIZE_X;
        }

        // 縦サイズの制限
        if (size.y >= STAGE_START_MAX_SIZE_Y)
        {
            size.y = STAGE_START_MAX_SIZE_Y;
        }

        // アニメーションのカウンタを利用して、UIのフェードアウトにつなげる
        nCnt = CountAnimation(1, MAX_ANIM_COUNTER);

        // フェードアウトを始めるカウントを超えたら、α値を下げていく
        if (nCnt >= STAGE_START_FADE_OUT_COUNT)
        {
            m_col.a -= STAGE_START_FADE_OUT_RATE;
        }

        // 色を変更
        CScene2D::SetColor(m_col);

        // 透明度が0を下回ったら、消す
        if (m_col.a <= 0.0f)
        {
            bUse = false;
        }

        break;

        // ステージ開始3
    case CUI::TYPE_START_STAGE3:

        // 横サイズ拡大
        size.x += STAGE_START_EXTEND_RATE_X;

        // 縦サイズ拡大
        size.y += STAGE_START_EXTEND_RATE_Y;

        // 横サイズの制限
        if (size.x >= STAGE_START_MAX_SIZE_X)
        {
            size.x = STAGE_START_MAX_SIZE_X;
        }

        // 縦サイズの制限
        if (size.y >= STAGE_START_MAX_SIZE_Y)
        {
            size.y = STAGE_START_MAX_SIZE_Y;
        }

        // アニメーションのカウンタを利用して、UIのフェードアウトにつなげる
        nCnt = CountAnimation(1, MAX_ANIM_COUNTER);

        // フェードアウトを始めるカウントを超えたら、α値を下げていく
        if (nCnt >= STAGE_START_FADE_OUT_COUNT)
        {
            m_col.a -= STAGE_START_FADE_OUT_RATE;
        }

        // 色を変更
        CScene2D::SetColor(m_col);

        // 透明度が0を下回ったら、消す
        if (m_col.a <= 0.0f)
        {
            bUse = false;
        }

        break;

        // ワーニング
    case CUI::TYPE_WARNING:

        // 透明度を上げるか下げるかを場合分け
        if (m_col.a >= 1.0f)
        {
            m_bFadeOut = true;
        }
        else if (m_col.a <= 0.0f)
        {
            m_bFadeOut = false;
        }

        // 透明度を、フラグによって変える
        if (m_bFadeOut == true)
        {
            m_col.a -= WARNING_FADE_RATE;
        }
        else
        {
            m_col.a += WARNING_FADE_RATE;
        }

        // アニメーションのカウンタを利用して、UIのフェードアウトにつなげる
        nCnt = CountAnimation(1, MAX_ANIM_COUNTER);

        // フェードアウトを始めるカウントを超えたら
        if (nCnt >= WARNING_FADE_OUT_COUNT)
        {
            // 透明の時に未使用にする
            if (m_col.a <= 0.0f)
            {
                // ワーニング音を止める
                pSound->Stop(CSound::LABEL_SE_WARNING);
                bUse = false;
            }
        }

        // 色を変更
        CScene2D::SetColor(m_col);

        break;

        // バリアをまとう
    case CUI::TYPE_GET_BARRIER:

        // 自機の位置に表示
        pos = playerPos;

        // バリアがないなら
        if (nPlayerBarrier <= 0)
        {
            // だんだん透明に
            m_col.a -= BARRIER_FADE_RATE;
        }
        // バリアがあるなら
        else
        {
            // だんだん表示
            m_col.a += BARRIER_FADE_RATE;
        }

        // 暗転状態で、バリアの透明度を大幅に下げる
        if (CGame::GetDarkening() == true)
        {
            // うっすら見える値で固定
            m_col.a = 0.001f;
        }

        //透明度の上限、下限を設定
        if (m_col.a >= fBarrierAlpha)
        {
            m_col.a = fBarrierAlpha;
        }
        else if (m_col.a <= 0.0f)
        {
            m_col.a = 0.0f;
        }

        // アニメーション
        SetAnimation(3, 8);

        // 色を変更
        CScene2D::SetColor(m_col);

        break;

        // 2P参加待ち
    case CUI::TYPE_2P_TO_START:

        // ヘルパーがいないなら
        if (CGame::GetDispHelper() == false)
        {
            // アニメーション
            SetParagraphAnimation(1, 2, 60, 2);
        }
        // ヘルパーがいるなら
        else
        {
            // 参加中
            SetParagraphTexturePlace(1, 2, 2, 2);
        }

        break;

        // αクリア
    case CUI::TYPE_αCLEAR:

        // だんだん見えるように
        m_col.a += CLEAR_FADE_RATE;

        // α値を一定以上いかなくする
        if (m_col.a > 1.0f)
        {
            m_col.a = 1.0f;
        }

        // アニメーションのカウンタを利用して、UIのフェードアウトにつなげる
        nCnt = CountAnimation(1, MAX_ANIM_COUNTER);

        // フェードアウトを始めるカウントを超えたら
        if (nCnt >= CLEAR_FADE_OUT_COUNT)
        {
            // すぐ未使用に
            bUse = false;
        }

        // 色を変更
        CScene2D::SetColor(m_col);

        break;

        // βクリア
    case CUI::TYPE_βCLEAR:

        // だんだん見えるように
        m_col.a += CLEAR_FADE_RATE;

        // α値を一定以上いかなくする
        if (m_col.a > 1.0f)
        {
            m_col.a = 1.0f;
        }

        // アニメーションのカウンタを利用して、UIのフェードアウトにつなげる
        nCnt = CountAnimation(1, MAX_ANIM_COUNTER);

        // フェードアウトを始めるカウントを超えたら
        if (nCnt >= CLEAR_FADE_OUT_COUNT)
        {
            // すぐ未使用に
            bUse = false;
        }

        // 色を変更
        CScene2D::SetColor(m_col);

        break;

        // γクリア
    case CUI::TYPE_γCLEAR:

        // だんだん見えるように
        m_col.a += CLEAR_FADE_RATE;

        // α値を一定以上いかなくする
        if (m_col.a > 1.0f)
        {
            m_col.a = 1.0f;
        }

        // アニメーションのカウンタを利用して、UIのフェードアウトにつなげる
        nCnt = CountAnimation(1, MAX_ANIM_COUNTER);

        // フェードアウトを始めるカウントを超えたら
        if (nCnt >= CLEAR_FADE_OUT_COUNT)
        {
            // すぐ未使用に
            bUse = false;
        }

        // 色を変更
        CScene2D::SetColor(m_col);

        break;

        // ゲームオーバー
    case CUI::TYPE_GAMEOVER:

        // だんだん見えるように
        m_col.a += GAMEOVER_FADE_RATE;

        // α値を一定以上いかなくする
        if (m_col.a > 1.0f)
        {
            m_col.a = 1.0f;
        }

        // アニメーションのカウンタを利用して、UIのフェードアウトにつなげる
        nCnt = CountAnimation(1, MAX_ANIM_COUNTER);

        // フェードアウトを始めるカウントを超えたら
        if (nCnt >= CLEAR_FADE_OUT_COUNT)
        {
            // すぐ未使用に
            bUse = false;
        }

        // 色を変更
        CScene2D::SetColor(m_col);

        break;

    case TYPE_TITLE_LOGO:

        // 流れ落ちる
        pos.y += TITLE_LOGO_FLOW_DOWN_SPEED;

        // 一定の値で止めておく
        if (pos.y >= TITLE_LOGO_TARGET_POS_Y)
        {
            pos.y = TITLE_LOGO_TARGET_POS_Y;
        }

        // エンター、またはスタートボタンを押したら
        if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START))
        {
            // タイトルロゴをすぐに下す
            pos.y = TITLE_LOGO_TARGET_POS_Y;
        }

        break;

    case TYPE_PRESS_START:

        // タイトルの時間をカウント
        m_nCntTime++;

        // タイトルの最大時間
        if (m_nCntTime > TITLE_MAX_TIME)
        {
            m_nCntTime = TITLE_MAX_TIME;
        }

        // 色を変える
        if (m_bFadeOut == true)
        {
            m_col.r -= PRESS_START_FADE_RATE;
            m_col.g -= PRESS_START_FADE_RATE;
            m_col.b -= PRESS_START_FADE_RATE;
        }
        else
        {
            m_col.r += PRESS_START_FADE_RATE;
            m_col.g += PRESS_START_FADE_RATE;
            m_col.b += PRESS_START_FADE_RATE;
        }
        if (m_col.r <= PRESS_START_FADE_OUT_MIN)
        {
            m_bFadeOut = false;
        }
        else if (m_col.r >= 1.0f)
        {
            m_bFadeOut = true;
        }

        // エンター、またはスタートボタンを押したら
        if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START))
        {
            // 一定時間経過で点滅可能に
            if (m_nCntTime >= TITLE_NEXT_SCENE)
            {
                // 点滅のフラグをtrueに
                m_bBlinking = true;
            }
            // それ以外は、カウンタを遷移可能まで一気に進める
            else
            {
                m_nCntTime = TITLE_NEXT_SCENE;
            }
        }

        // 点滅するなら
        if (m_bBlinking == true)
        {
            // カウンターを進めて、パターンを切り替える
            nPatternAnim = CountAnimation(2, 2);

            // 色を2パターンで切り替える
            if (nPatternAnim == 1)
            {
                m_col.r = PRESS_START_FADE_OUT_MIN;
                m_col.g = PRESS_START_FADE_OUT_MIN;
                m_col.b = PRESS_START_FADE_OUT_MIN;
            }
            else
            {
                m_col.r = 1.0f;
                m_col.g = 1.0f;
                m_col.b = 1.0f;
            }
        }

        // 色を変更
        CScene2D::SetColor(m_col);

        break;

    case TYPE_CAREFUL_UPPER:

        bUse = Careful();

        break;

    case TYPE_CAREFUL_LEFT:

        bUse = Careful();

        break;

    case TYPE_CAREFUL_RIGHT:

        bUse = Careful();

        break;

    case TYPE_CAREFUL_DOWN:

        bUse = Careful();

        break;

    case TYPE_DANGER:

        bUse = Danger();

        break;
    }

    // 位置を反映
    SetPosition(pos);

    // サイズを反映
    SetSize(size);

    // 矢印のみ、回転する頂点座標
    if (m_type == CUI::TYPE_ARROW_SYMBOL)
    {
        // プレイヤーが右スティックで倒した角度をもとに、頂点座標を変更
        SetRotVertex(fPlayerShotAngle);
    }
    else
    {
        // 頂点座標を変更
        SetVertex();
    }

    // 使用するフラグがfalseなら消す
    if (bUse == false)
    {
        Uninit();
    }
}

//========================================
// UIの描画処理
//========================================
void CUI::Draw(void)
{
    // 描画処理
    CScene2D::Draw();
}

//========================================
// UIの画像ロード処理
//========================================
HRESULT CUI::Load(void)
{
    // レンダラーからデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    //テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui000.png", &m_apTexture[CUI::TYPE_ARROW_SYMBOL]);	    // 矢印
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui001.png", &m_apTexture[CUI::TYPE_PLAYER1]);	        // プレイヤー1
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui002.png", &m_apTexture[CUI::TYPE_STOCK]);	        // ストック
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui003.png", &m_apTexture[CUI::TYPE_BARRIER]);	        // バリア
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui004.png", &m_apTexture[CUI::TYPE_SPEED]);	        // スピード
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui005.png", &m_apTexture[CUI::TYPE_STOCK_BAR]);	    // ストックのバー
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui005.png", &m_apTexture[CUI::TYPE_BARRIER_BAR]);	    // バリアのバー
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui006.png", &m_apTexture[CUI::TYPE_ZONEα]);	        // ゾーンα
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui013.png", &m_apTexture[CUI::TYPE_ZONEβ]);	        // ゾーンβ
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui014.png", &m_apTexture[CUI::TYPE_ZONEγ]);	        // ゾーンγ
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui007.png", &m_apTexture[CUI::TYPE_SPEED_BAR]);	    // スピードのバー
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui019.png", &m_apTexture[CUI::TYPE_TITLE_LOGO]);       // タイトルのロゴ
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui020.png", &m_apTexture[CUI::TYPE_PRESS_START]);      // スタートボタンを押してね
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/result000.png", &m_apTexture[CUI::TYPE_RESULT_SOLO]);	// リザルト(ソロ)
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/result001.png", &m_apTexture[CUI::TYPE_RESULT_DUO]);	// リザルト(デュオ)
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui008.png", &m_apTexture[CUI::TYPE_START_STAGE1]);	    // ステージ1の開始
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui009.png", &m_apTexture[CUI::TYPE_WARNING]);	        // ワーニング
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui010.png", &m_apTexture[CUI::TYPE_GET_BARRIER]);	    // バリアをまとう
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui011.png", &m_apTexture[CUI::TYPE_2P_TO_START]);	    // 2P参加待ち
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/manual000.png", &m_apTexture[CUI::TYPE_MANUAL]);	    // 遊び方
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui012.png", &m_apTexture[CUI::TYPE_αCLEAR]);	        // αクリア
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui015.png", &m_apTexture[CUI::TYPE_START_STAGE2]);	    // ステージ2の開始
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui016.png", &m_apTexture[CUI::TYPE_START_STAGE3]);	    // ステージ3の開始
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui017.png", &m_apTexture[CUI::TYPE_βCLEAR]);	        // βクリア
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui018.png", &m_apTexture[CUI::TYPE_γCLEAR]);	        // γクリア
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui021.png", &m_apTexture[CUI::TYPE_GAMEOVER]);	        // ゲームオーバー
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui022.png", &m_apTexture[CUI::TYPE_CAREFUL_UPPER]);	// 危険！(上)
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui023.png", &m_apTexture[CUI::TYPE_CAREFUL_LEFT]);	    // 危険！(左)
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui024.png", &m_apTexture[CUI::TYPE_CAREFUL_RIGHT]);	// 危険！(右)
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui025.png", &m_apTexture[CUI::TYPE_DANGER]);	        // デンジャー
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui026.png", &m_apTexture[CUI::TYPE_CAREFUL_DOWN]);	    // 危険！(下)

    return S_OK;
}

//========================================
// UIの画像破棄処理
//========================================
void CUI::Unload(void)
{
    // テクスチャの破棄
    for (int nCntTexture = 0; nCntTexture < CUI::TYPE_MAX; nCntTexture++)
    {
        if (m_apTexture[nCntTexture] != NULL)
        {
            m_apTexture[nCntTexture]->Release();
            m_apTexture[nCntTexture] = NULL;
        }
    }
}

//================================
// UIの作成
//================================
CUI *CUI::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, CUI::TYPE type)
{
    CUI *pUI = NULL;

    // メモリを確保
    pUI = new CUI;

    // タイプのみ、テクスチャを割り当てのために結びつけておく
    pUI->m_type = type;

    // 初期化
    pUI->Init(pos, size);

    // メンバ変数を結びつける
    pUI->m_col = col;

    return pUI;
}

//================================
// 注意喚起の処理
//================================
bool CUI::Careful(void)
{
    // 変数宣言
    bool bUse = true;

    // カウンターを進めて、パターンを切り替える
    int nPatternAnim = CountAnimation(2, 2);

    // 色を2パターンで切り替える
    if (nPatternAnim == 1)
    {
        m_col.a = 0.1f;
    }
    else
    {
        m_col.a = 1.0f;
    }

    // 時間をカウント
    m_nCntTime++;

    // 一定時間経過で消す
    if (m_nCntTime > CAREFUL_FADE_OUT_COUNT)
    {
        m_col.a = -1.0f;

        bUse = false;
    }

    // 色を変更
    CScene2D::SetColor(m_col);

    return bUse;
}

//================================
// デンジャー表記の処理
//================================
bool CUI::Danger(void)
{
    // 変数宣言
    bool bUse = true;

    // カウンターを進めて、パターンを切り替える
    int nPatternAnim = CountAnimation(2, 2);

    // 色を2パターンで切り替える
    if (nPatternAnim == 1)
    {
        m_col.a = 0.1f;
    }
    else
    {
        m_col.a = 1.0f;
    }

    // 時間をカウント
    m_nCntTime++;

    // 一定時間経過で消す
    if (m_nCntTime > DANGER_FADE_OUT_COUNT)
    {
        m_col.a = -1.0f;

        bUse = false;
    }

    // 色を変更
    CScene2D::SetColor(m_col);

    return bUse;
}
