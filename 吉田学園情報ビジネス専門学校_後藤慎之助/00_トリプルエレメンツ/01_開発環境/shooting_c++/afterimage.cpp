//===============================================
//
// 残像処理 (afterimage.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "afterimage.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "library.h"
#include "game.h"
#include "effect.h"
#include "sound.h"
#include "whiteFade.h"
#include "player.h"
#include "enemy.h"

//=======================================
// マクロ定義
//=======================================

//=======================================
// 静的メンバ変数宣言
//=======================================
LPDIRECT3DTEXTURE9 CAfterimage::m_apTexture[CAfterimage::TYPE_MAX] = {};

//========================================
// 残像のコンストラクタ
//========================================
CAfterimage::CAfterimage() :CScene2D(OBJTYPE::OBJTYPE_AFTERIMAGE)
{
    m_nCntTime = 0;
    m_fAngle = 0.0f;
    m_move = DEFAULT_VECTOR;
    m_col = DEFAULT_COLOR;
    m_type = CAfterimage::TYPE_MAX;
}

//========================================
// 残像のデストラクタ
//========================================
CAfterimage::~CAfterimage()
{

}

//========================================
// 残像の初期化処理
//========================================
HRESULT CAfterimage::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 初期化
    CScene2D::Init(pos, size);

    // テクスチャを割り当て
    BindTexture(m_apTexture[m_type]);

    return S_OK;
}

//========================================
// 残像の終了処理
//========================================
void CAfterimage::Uninit(void)
{
    // 終了処理
    CScene2D::Uninit();
}

//========================================
// 残像の更新処理
//========================================
void CAfterimage::Update(void)
{
    // 変数宣言
    D3DXVECTOR3 pos = GetPosition();                   // 位置を取得
    D3DXVECTOR3 size = GetSize();                      // サイズを取得
    float fAngle = 0.0f;                               // 角度
    float fRadius = 0.0f;                              // 半径
    float fAfterimageAngle = 0.0f;                     // 残像の角度

    // 音を取得
    CSound *pSound = CManager::GetSound();

    // 種類に応じた場合分け
    switch (m_type)
    {
        // プレイヤー
    case CAfterimage::TYPE_PLAYER:

        // だんだん透明に
        m_col.a -= AFTERIMAGE_PLAYER_FADE_RATE;

        // 色を変更
        CScene2D::SetColor(m_col);

        break;

        // プレイヤー配置
    case CAfterimage::TYPE_PLAYER_DISP:

        // 初期位置に近づける
        pos.x += PLAYER_SPEED_2;

        // 初期位置に到達したら
        if (pos.x >= PLAYER_START_POS_X)
        {
            // プレイヤーをリスポーン
            CGame::RespawnPlayer();

            // 透明にする
            m_col.a = -1.0f;
        }

        // 色を変更
        CScene2D::SetColor(m_col);

        break;

        // ボス森
    case CAfterimage::TYPE_BOSS_FOREST:

        // カウンタ加算
        m_nCntTime++;

        // だんだん透明に
        m_col.a -= AFTERIMAGE_BOSS_FADE_RATE;

        // テクスチャ座標を痛がる顔に更新
        SetTexturePlace(2, 2);

        // 色を変更(赤でだんだん透明)
        CScene2D::SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_col.a));

        // カウンタをもとに、エフェクトを設定する
        if (m_nCntTime % AFTERIMAGE_BOSS_SET_EFFECT == 0)
        {
            // エフェクト用に、角度と半径を求める
            fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
            fRadius = float(rand() % AFTERIMAGE_BOSS_RADIUS + 0) / 100.0f;

            // 爆発音再生
            pSound->Play(CSound::LABEL_SE_EXPLOSION);

            // 大爆発エフェクト (サイズは倍)
            CEffect::Create(D3DXVECTOR3(pos.x - sinf(fAngle)*fRadius, pos.y - cosf(fAngle)*fRadius, 0.0f), 
                AFTERIMAGE_BOSS_EXPLOSION_SIZE,
                DEFAULT_COLOR, CEffect::TYPE_BIG_EXPLOSION);
        }

        // 透明度が下がり切ったら、白フェードを設定
        if (m_col.a < 0)
        {
            CWhiteFade::Create(WHITE_FADE_POS, WHITE_FADE_SIZE, WHITE_FADE_END_TIME, 
                D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f), CWhiteFade::TYPE_α);
        }

        break;

        // ボスドラゴン
    case CAfterimage::TYPE_BOSS_DRAGON:

        // カウンタ加算
        m_nCntTime++;

        // だんだん透明に
        m_col.a -= AFTERIMAGE_BOSS_FADE_RATE;

        // テクスチャ座標を攻撃中のものに更新
        SetParagraphTexturePlace(3, 2, 2, 4);

        // 色を変更(赤でだんだん透明)
        CScene2D::SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_col.a));

        // カウンタをもとに、エフェクトを設定する
        if (m_nCntTime % AFTERIMAGE_BOSS_SET_EFFECT == 0)
        {
            // エフェクト用に、角度と半径を求める
            fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
            fRadius = float(rand() % AFTERIMAGE_BOSS_RADIUS) / 100.0f;

            // 爆発音再生
            pSound->Play(CSound::LABEL_SE_EXPLOSION);

            // 大爆発エフェクト (サイズは倍)
            CEffect::Create(D3DXVECTOR3(pos.x - sinf(fAngle)*fRadius, pos.y - cosf(fAngle)*fRadius, 0.0f),
                AFTERIMAGE_BOSS_EXPLOSION_SIZE,
                DEFAULT_COLOR, CEffect::TYPE_BIG_EXPLOSION);
        }

        // 透明度が下がり切ったら、白フェードを設定
        if (m_col.a < 0)
        {
            CWhiteFade::Create(WHITE_FADE_POS, WHITE_FADE_SIZE, WHITE_FADE_END_TIME,
                D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f), CWhiteFade::TYPE_β);
        }

        break;

        // ボスブレイズ
    case CAfterimage::TYPE_BOSS_BLAZE:

        // カウンタ加算
        m_nCntTime++;

        // だんだん透明に
        m_col.a -= AFTERIMAGE_BOSS_FADE_RATE;

        // テクスチャ座標を攻撃中のものに更新
        SetTexturePlace(1, 3);

        // 色を変更(赤でだんだん透明)
        CScene2D::SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_col.a));

        // カウンタをもとに、エフェクトを設定する
        if (m_nCntTime % AFTERIMAGE_BOSS_SET_EFFECT == 0)
        {
            // エフェクト用に、角度と半径を求める
            fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
            fRadius = float(rand() % AFTERIMAGE_BOSS_RADIUS) / 100.0f;

            // 爆発音再生
            pSound->Play(CSound::LABEL_SE_EXPLOSION);

            // 大爆発エフェクト (サイズは倍)
            CEffect::Create(D3DXVECTOR3(pos.x - sinf(fAngle)*fRadius, pos.y - cosf(fAngle)*fRadius, 0.0f),
                AFTERIMAGE_BOSS_EXPLOSION_SIZE,
                DEFAULT_COLOR, CEffect::TYPE_BIG_EXPLOSION);
        }

        // 透明度が下がり切ったら、ゲームクリアを設定
        if (m_col.a < 0)
        {
            // ゲームクリア
            CGame::SetGameClear();
        }

        break;

        // ゴッドバード
    case CAfterimage::TYPE_GOD_BIRD:

        // だんだん透明に
        m_col.a -= AFTERIMAGE_PLAYER_FADE_RATE;

        // 色を変更
        CScene2D::SetColor(m_col);

        break;

        // 神通力
    case CAfterimage::TYPE_MAGIC:

        // サイズ拡大、だんだん透明に
        size.x += MAGIC_EXTEND_RATE;
        size.y += MAGIC_EXTEND_RATE;
        m_col.a -= MAGIC_FADE_OUT_RATE;

        // 色を変更
        CScene2D::SetColor(m_col);

        break;

        // グソクムシ
    case CAfterimage::TYPE_RANK_GUSOKU:

        // テクスチャ座標を更新
        SetAnimation(6, 2);

        // 右から左へ
        pos.x -= ENEMY_GUSOKU_SPEED;

        // ランキング画面での処理
        RankingFadeOut(pos);

        break;

        // 白カラス
    case CAfterimage::TYPE_RANK_CROW:

        // テクスチャ座標を更新
        SetAnimation(6, 3);

        // 右から左へ
        pos.x -= ENEMY_CROW_SPEED;

        // ランキング画面での処理
        RankingFadeOut(pos);

        break;

        // 緑鳥
    case CAfterimage::TYPE_RANK_GREEN:

        // 右から左へ
        pos.x -= ENEMY_GREEN_BIRD_GOD_SPEED;

        // ランキング画面での処理
        RankingFadeOut(pos);

        // カウンタ加算
        m_nCntTime++;

        // カウンタによって、エフェクトを生成
        if (m_nCntTime % ENEMY_GREEN_BIRD_SET_EFFECT_COUNT == 0)
        {
            // エフェクトを生成(サイズは一回り小さい)
            CEffect::Create(pos + m_move, size - D3DXVECTOR3(-10.0f, -10.0f, 0.0f),
                EFFECT_YELLOW_FIRE_FIRST_COLOR, CEffect::TYPE_YELLOW_FIRE);
        }

        break;

        // 青鳥
    case CAfterimage::TYPE_RANK_BLUE:

        // 右から左へ
        pos.x -= ENEMY_GREEN_BIRD_GOD_SPEED;

        // ランキング画面での処理
        RankingFadeOut(pos);

        // カウンタ加算
        m_nCntTime++;

        // カウンタによって、エフェクトを生成
        if (m_nCntTime % ENEMY_GREEN_BIRD_SET_EFFECT_COUNT == 0)
        {
            // エフェクトを生成(サイズは一回り小さい)
            CEffect::Create(pos + m_move, size - D3DXVECTOR3(-10.0f, -10.0f, 0.0f),
                EFFECT_BLUE_FIRE_FIRST_COLOR, CEffect::TYPE_BLUE_FIRE);
        }

        break;

        // 赤鳥
    case CAfterimage::TYPE_RANK_RED:

        // 右から左へ
        pos.x -= ENEMY_GREEN_BIRD_GOD_SPEED;

        // ランキング画面での処理
        RankingFadeOut(pos);

        // カウンタ加算
        m_nCntTime++;

        // カウンタによって、エフェクトを生成
        if (m_nCntTime % ENEMY_GREEN_BIRD_SET_EFFECT_COUNT == 0)
        {
            // エフェクトを生成(サイズは一回り小さい)
            CEffect::Create(pos + m_move, size - D3DXVECTOR3(-10.0f, -10.0f, 0.0f),
                EFFECT_RED_FIRE_FIRST_COLOR, CEffect::TYPE_RED_FIRE);
        }

        break;
    }

    // 位置を反映
    SetPosition(pos);

    // サイズを反映
    SetSize(size);

    // プレイヤー系の残像のみ、回転する可能性がある
    if (m_type == TYPE_PLAYER || m_type == TYPE_PLAYER_DISP)
    {
        fAfterimageAngle = m_fAngle * (-1);   // -1をかけることで、角度を加算するほど左に回るように
        SetRotVertex(fAfterimageAngle);
    }
    else
    {
        // 頂点座標を変更
        SetVertex();
    }

    // 透明なら消滅
    if (m_col.a < 0)
    {
        Uninit(); // これより下にコードは書かない
    }
}

//========================================
// 残像の描画処理
//========================================
void CAfterimage::Draw(void)
{
    // 描画処理
    CScene2D::Draw();
}

//========================================
// 残像の画像ロード処理
//========================================
HRESULT CAfterimage::Load(void)
{
    // レンダラーからデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player000.png", &m_apTexture[CAfterimage::TYPE_PLAYER]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player000.png", &m_apTexture[CAfterimage::TYPE_PLAYER_DISP]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/afterimage000.png", &m_apTexture[CAfterimage::TYPE_BOSS_FOREST]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy011.png", &m_apTexture[CAfterimage::TYPE_BOSS_DRAGON]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy015.png", &m_apTexture[CAfterimage::TYPE_BOSS_BLAZE]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/afterimage001.png", &m_apTexture[CAfterimage::TYPE_GOD_BIRD]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/magic.png", &m_apTexture[CAfterimage::TYPE_MAGIC]);

    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy007.png", &m_apTexture[CAfterimage::TYPE_RANK_GUSOKU]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy003.png", &m_apTexture[CAfterimage::TYPE_RANK_CROW]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/afterimage002.png", &m_apTexture[CAfterimage::TYPE_RANK_GREEN]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/afterimage003.png", &m_apTexture[CAfterimage::TYPE_RANK_BLUE]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/afterimage004.png", &m_apTexture[CAfterimage::TYPE_RANK_RED]);

    return S_OK;
}

//========================================
// 残像の画像破棄処理
//========================================
void CAfterimage::Unload(void)
{
    // テクスチャの破棄
    for (int nCntTexture = 0; nCntTexture < CAfterimage::TYPE_MAX; nCntTexture++)
    {
        if (m_apTexture[nCntTexture] != NULL)
        {
            m_apTexture[nCntTexture]->Release();
            m_apTexture[nCntTexture] = NULL;
        }
    }
}

//================================
// 残像の作成
//================================
CAfterimage *CAfterimage::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, float fAngle, D3DXCOLOR col, CAfterimage::TYPE type)
{
    CAfterimage *pAfterimage = NULL;

    // メモリを確保
    pAfterimage = new CAfterimage;

    // タイプのみ、テクスチャを割り当てのために結びつけておく
    pAfterimage->m_type = type;

    // 初期化
    pAfterimage->Init(pos, size);

    // メンバ変数を結びつける
    pAfterimage->m_col = col;
    pAfterimage->m_fAngle = fAngle;

    return pAfterimage;
}

//================================
// ランキング画面で、消える処理
//================================
void CAfterimage::RankingFadeOut(D3DXVECTOR3 pos)
{
    // 左に流れきったら
    if (pos.x <= AFTERIMAGE_RANKING_FADE_OUT_POS_X)
    {
        // 透明に
        m_col.a = -1.0f;
    }
}
