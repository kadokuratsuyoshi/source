#!/usr/bin/python3
#-*- coding:utf-8 -*-
#
# https://www.codexa.net/pytorch-python/

# ライブラリのインポート
import numpy as np
import pandas as pd 
import matplotlib.pyplot as plt
import seaborn as sns 
# PyTorchのインポート
import torch 
import torch.nn as nn
# 評価指標（Scikit-learn）
from sklearn.metrics import mean_squared_error

# CSVファイルの読み込み
ramen = pd.read_csv('ramen-ratings.csv')

# データの最初の5行を表示
ramen.head()

# データのサイズ確認
ramen.shape

# ラーメンの国別
ramen['Country'].value_counts()[0:10]

# 未評価（Unrated）のラーメンを除外
mask = ramen.index[ramen['Stars'] == 'Unrated']
ramen = ramen.drop(index = mask)
ramen.shape

# Starsのデータ型を確認
print(ramen['Stars'].dtype)
 
# float型へ変換
ramen['Stars'] = ramen['Stars'].astype(float)
 
# 改めてデータ型を確認
print(ramen['Stars'].dtype)

# 不要なカラムをデータから削除
ramen = ramen.drop(columns=['Review #', 'Top Ten', 'Variety'])
# 確認
ramen.head()

# 特徴量のダミー変数化
Country = pd.get_dummies(ramen['Country'], prefix='Country', drop_first=True)
Brand = pd.get_dummies(ramen['Brand'], prefix='Brand',drop_first=True)
Style = pd.get_dummies(ramen['Style'], prefix='Style',drop_first=True)

# ダミー変数化した特徴量を結合
ramendf = pd.concat([Country, Brand,Style], axis=1)
# 確認
ramendf.head()

# 特徴量とターゲットへ分割
X = np.array(ramendf, dtype=np.float32) 
y = np.array(ramen[['Stars']], dtype=np.float32)

# 線形回帰モデル
model = nn.Linear(397, 1)

# 損失関数
loss = nn.MSELoss()

# 最適化関数
optimizer = torch.optim.SGD(model.parameters(), lr=0.6)

# 損失関数
loss = nn.MSELoss()
 
# 最適化関数
optimizer = torch.optim.SGD(model.parameters(), lr=0.6)

# モデル訓練
for epoch in range(1000):
    # ステージ1 Numpy配列からテンソルへ変換
    inputs = torch.from_numpy(X)
    targets = torch.from_numpy(y)
    
    # ステージ2 推測値を出力して誤差（コスト）を算出
    outputs = model(inputs)
    cost = loss(outputs, targets)
    
    # ステージ3 誤差逆伝播（バックプロパゲーション）
    optimizer.zero_grad()
    cost.backward()
    optimizer.step()
    
    # ステージ4 50回毎にコストを表示
    if (epoch+1) % 100 == 0:
        print ('Epoch [{}/{}], Loss: {:.4f}'.format(epoch+1, 1000, cost.item()))


# 予測を出力
y_pred = model(torch.from_numpy(X)).data.numpy()
print(y_pred[0:5])
print(y[0:5])


# 平均二乗誤差を確認
print(mean_squared_error(y, y_pred))

#eof
