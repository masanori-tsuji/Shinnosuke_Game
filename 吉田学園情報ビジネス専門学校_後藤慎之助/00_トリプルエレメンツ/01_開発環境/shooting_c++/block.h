//====================================================================
//
// ブロックの処理 (block.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene2d.h"

//================================================
// マクロ定義
//================================================
#define BLOCK_APPLE_SIZE D3DXVECTOR3(95.0f, 95.0f, 0.0f)	        // リンゴのサイズ
#define BLOCK_APPLE_COLLISION_SIZE D3DXVECTOR3(65.0f, 65.0f, 0.0f)  // リンゴの衝突サイズ
#define BLOCK_APPLE_ROT_SPEED 0.2f                                  // 回転スピード
#define BLOCK_APPLE_FALL_COUNT 90                                   // 落下するまでのカウンタ
#define BLOCK_APPLE_RIGHT_BOUND D3DXVECTOR3(3.0f, -10.0f, 0.0f)     // 右にバウンド
#define BLOCK_APPLE_LEFT_BOUND D3DXVECTOR3(-3.0f, -10.0f, 0.0f)     // 左にバウンド

#define BLOCK_ROCK_SIZE D3DXVECTOR3(300.0f, 300.0f, 0.0f)	        // 岩のサイズ
#define BLOCK_ROCK_COLLISION_SIZE D3DXVECTOR3(130.0f, 130.0f, 0.0f) // 岩の衝突サイズ
#define BLOCK_ROCK_ROT_SPEED 0.05f                                  // 岩の回転スピード
#define BLOCK_ROCK_MAX_FALL_SPEED 9.0f                              // 岩の最大落下スピード
#define BLOCK_ROCK_RIGHT_BOUND D3DXVECTOR3(3.0f, -7.0f, 0.0f)       // 右にバウンド
#define BLOCK_ROCK_LEFT_BOUND D3DXVECTOR3(-3.0f, -7.0f, 0.0f)       // 左にバウンド

#define BLOCK_METEO_SIZE D3DXVECTOR3(300.0f, 300.0f, 0.0f)	        // メテオのサイズ
#define BLOCK_METEO_COLLISION_SIZE D3DXVECTOR3(150.0f, 150.0f, 0.0f)// メテオの衝突サイズ
#define BLOCK_METEO_ROT_SPEED 0.05f                                 // メテオの回転スピード
#define BLOCK_METEO_FALL_SPEED 20.0f                                // メテオの落下スピード

//================================================
// クラス宣言
//================================================

// ブロッククラス
class CBlock : public CScene2D
{
public:
    // ブロックの種類
    typedef enum
    {
        TYPE_APPLE,     // リンゴ
        TYPE_ROCK,      // 岩
        TYPE_METEO,     // メテオ
        TYPE_MAX        // 種類の最大数
    }TYPE;
    CBlock();
    ~CBlock();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CBlock *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, bool bRotRight, TYPE type);
    D3DXVECTOR3 Bound(D3DXVECTOR3 pos, D3DXVECTOR3 size);                   // 床に当たった時のバウンド
    D3DXVECTOR3 GetCollisionSize(void) { return m_collisionSize; }          // 衝突の大きさを返す
    D3DXVECTOR3 GetPosOld(void) { return m_posOld; }                        // 1F前の位置を返す
    float GetAngleToPlayer(D3DXVECTOR3 pos);
    bool MeteoInpact(D3DXVECTOR3 pos, D3DXVECTOR3 size);                    // 隕石の衝突

private:
    static LPDIRECT3DTEXTURE9 m_apTexture[TYPE_MAX];   // 読み込んだテクスチャを保存する
    D3DXVECTOR3 m_posOld;                              // 1F前の位置
    D3DXVECTOR3 m_collisionSize;                       // 当たり判定の大きさ
    D3DXVECTOR3 m_move;                                // 移動量
    int m_nCntChangeMove;                              // 移動量が変わりだすまでのカウント
    float m_fRotAngle;                                 // 回転角度
    bool m_bRotRight;                                  // 右回転かどうか
    TYPE m_type;                                       // ブロックの種類
};

#endif