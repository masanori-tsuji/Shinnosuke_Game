//====================================================================
//
// シーン上のオブジェクト処理 (scene.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"

//================================================
// マクロ定義
//================================================
#define MAX_POLYGON 2048   // ポリゴンの最大数

//================================================
// クラス宣言
//================================================

// オブジェクトのクラス
class CScene
{
public:
    // オブジェクトの種類(描画順)
    typedef enum
    {
        OBJTYPE_NULL,      // なし
        OBJTYPE_BG,        // 背景
        OBJTYPE_LOGO,      // ロゴ
        OBJTYPE_RANKING,   // ランキング
        OBJTYPE_BLOCK,     // ブロック
        OBJTYPE_AFTERIMAGE,// 残像
        OBJTYPE_ENEMY,     // エネミー
        OBJTYPE_HELPER,    // ヘルパー
        OBJTYPE_PLAYER,    // プレイヤー
        OBJTYPE_BULLET,    // 弾
        OBJTYPE_EFFECT,    // エフェクト
        OBJTYPE_ITEM,      // アイテム
        OBJTYPE_FOREGROUND,// 前景
        OBJTYPE_BUBBLE,    // 泡
        OBJTYPE_LIGHT2D,   // ライト2D
        OBJTYPE_WAVE,      // 波
        OBJTYPE_UI,        // UI
        OBJTYPE_SCORE,     // スコア
        OBJTYPE_REVERSE,   // 反転合成
        OBJTYPE_WHITEFADE, // 白フェード
        OBJTYPE_MAX        // オブジェクトの種類の最大数
    }OBJTYPE;

    CScene();
    CScene(OBJTYPE objType);    // イニシャライザでオブジェクトタイプを紐づけ
    virtual ~CScene();  // 親クラスのデストラクタにvirtualをつけることで、メモリリークを抑える
    virtual HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size) = 0;
    virtual void Uninit(void) = 0;
    virtual void Update(void) = 0;
    virtual void Draw(void) = 0;
    static void UpdateAll(void);        // 全て更新
    static void DrawAll(void);          // 全て描画
    static void ReleaseAll(void);       // 全て開放
    static void ReleaseStage(void);     // 全ての背景、敵、ブロック、アイテムを開放
    OBJTYPE GetObjType(void) { return m_objType; }  // オブジェクトタイプを取得
    static CScene *GetScene(int nCntScene);         // シーンを取得

protected:
    void Release(void);                 // 単体を開放

private:
    static CScene *m_apScene[MAX_POLYGON];  // シーン上のオブジェクト情報
    static int m_nNumAll;                   // シーン上のオブジェクトの最大数
    int m_nID;   // 開放する番号を記憶(staticはインスタンス関係なく持っているが、普通のはインスタンスごとにメンバ変数を持っている)
    OBJTYPE m_objType;                      // オブジェクトの種類
};

#endif