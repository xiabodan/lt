# -*- coding: utf-8 -*-
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

train_df = pd.read_csv('train.csv', index_col=0)
test_df = pd.read_csv('test.csv', index_col=0)


# step 2: 合并数据
# fig = plt.figure()
train_df.head()
prices = pd.DataFrame({"price":train_df["SalePrice"], "log(price + 1)":np.log1p(train_df["SalePrice"])})
prices.hist()
# plt.show()

y_train = np.log1p(train_df.pop('SalePrice'))
all_df = pd.concat((train_df, test_df), axis=0)
print all_df.shape
print y_train.head()

# step 2: 变量转换
all_df['MSSubClass'].dtypes
all_df['MSSubClass'] = all_df['MSSubClass'].astype(str)
# all_df['MSSubClass'].value_counts()
# pd.get_dummies(all_df['MSSubClass'], prefix='MSSubClass').head()

all_dummy_df = pd.get_dummies(all_df)
all_dummy_df.head()
