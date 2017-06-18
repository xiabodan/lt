# -*- coding: utf-8 -*-
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np  # 科学计算
from numpy import *
from pandas import Series,DataFrame
from sklearn.ensemble import RandomForestRegressor
import sklearn.preprocessing as preprocessing
from sklearn import linear_model
from sklearn import cross_validation
from sklearn.learning_curve import learning_curve
from sklearn.ensemble import BaggingRegressor

data_train = pd.read_csv("train.csv")

def loadData():
    data_train = pd.read_csv("train.csv")
    return data_train

def statistical():
    fig = plt.figure()
    fig.set(alpha=0.2)

    plt.subplot2grid((2,3),(0,0))
    data_train.Survived.value_counts().plot(kind='bar')
    plt.title("survied")
    plt.ylabel(u"发的")

    plt.subplot2grid((2,3),(0,1))
    data_train.Pclass.value_counts().plot(kind="bar")
    plt.ylabel(u"人数")
    plt.title(u"乘客等级分布")

    plt.subplot2grid((2,3),(0,2))
    plt.scatter(data_train.Survived, data_train.Age)
    plt.ylabel(u"年龄")                         # 设定纵坐标名称
    plt.grid(b=True, which='major', axis='y')
    plt.title(u"按年龄看获救分布 (1为获救)")

    # plt.subplot2grid((2,3),(1,0), colspan=2)
    # data_train.Age[data_train.Pclass == 1].plot(kind='kde')
    # data_train.Age[data_train.Pclass == 2].plot(kind='kde')
    # data_train.Age[data_train.Pclass == 3].plot(kind='kde')
    # plt.xlabel(u"年龄") # plots an axis lable
    # plt.ylabel(u"密度")
    # plt.title(u"各等级的乘客年龄分布")
    # plt.legend((u'头等舱', u'2等舱',u'3等舱'),loc='best') # sets our legend for our graph.

    plt.subplot2grid((2,3),(1,2))
    data_train.Embarked.value_counts().plot(kind='bar')
    plt.title(u"各登船口岸上船人数")
    plt.ylabel(u"人数")

    plt.show()

def Pclass():
    fig = plt.figure()
    fig.set(alpha=0.2)  # 设定图表颜色alpha参数

    Survived_0 = data_train.Pclass[data_train.Survived == 0].value_counts()
    Survived_1 = data_train.Pclass[data_train.Survived == 1].value_counts()
    df=pd.DataFrame({u'获救':Survived_1, u'未获救':Survived_0})
    df.plot(kind='bar', stacked=True)
    plt.title(u"各乘客等级的获救情况")
    plt.xlabel(u"乘客等级")
    plt.ylabel(u"人数")
    plt.show()

def DataFrame():
    fig = plt.figure()
    fig.set(alpha=0.2)  # 设定图表颜色alpha参数

    Survived_m = data_train.Survived[data_train.Sex == 'male'].value_counts()
    Survived_f = data_train.Survived[data_train.Sex == 'female'].value_counts()
    df=pd.DataFrame({u'男性':Survived_m, u'女性':Survived_f})
    df.plot(kind='bar', stacked=True)
    plt.title(u"按性别看获救情况")
    plt.xlabel(u"性别")
    plt.ylabel(u"人数")
    plt.show()

def PclassOrSex():
     #然后我们再来看看各种舱级别情况下各性别的获救情况
     fig=plt.figure()
     fig.set(alpha=0.65) # 设置图像透明度，无所谓
     plt.title(u"根据舱等级和性别的获救情况")

     ax1=fig.add_subplot(141)
     data_train.Survived[data_train.Sex == 'female'][data_train.Pclass != 3].value_counts().plot(kind='bar', label="female highclass", color='#FA2479')
     ax1.set_xticklabels([u"获救", u"未获救"], rotation=0)
     ax1.legend([u"女性/高级舱"], loc='best')

     ax2=fig.add_subplot(142, sharey=ax1)
     data_train.Survived[data_train.Sex == 'female'][data_train.Pclass == 3].value_counts().plot(kind='bar', label='female, low class', color='pink')
     ax2.set_xticklabels([u"未获救", u"获救"], rotation=0)
     plt.legend([u"女性/低级舱"], loc='best')

     ax3=fig.add_subplot(143, sharey=ax1)
     data_train.Survived[data_train.Sex == 'male'][data_train.Pclass != 3].value_counts().plot(kind='bar', label='male, high class',color='lightblue')
     ax3.set_xticklabels([u"未获救", u"获救"], rotation=0)
     plt.legend([u"男性/高级舱"], loc='best')

     ax4=fig.add_subplot(144, sharey=ax1)
     data_train.Survived[data_train.Sex == 'male'][data_train.Pclass == 3].value_counts().plot(kind='bar', label='male low class', color='steelblue')
     ax4.set_xticklabels([u"未获救", u"获救"], rotation=0)
     plt.legend([u"男性/低级舱"], loc='best')

     plt.show()

def Embarked():
    fig = plt.figure()
    fig.set(alpha=0.2)  # 设定图表颜色alpha参数

    Survived_0 = data_train.Embarked[data_train.Survived == 0].value_counts()
    Survived_1 = data_train.Embarked[data_train.Survived == 1].value_counts()
    df=pd.DataFrame({u'获救':Survived_1, u'未获救':Survived_0})
    df.plot(kind='bar', stacked=True)
    plt.title(u"各登录港口乘客的获救情况")
    plt.xlabel(u"登录港口")
    plt.ylabel(u"人数")

    plt.show()

def groupby():
    g = data_train.groupby(['SibSp','Survived'])
    df = pd.DataFrame(g.count()['PassengerId'])
    # print df

    g = data_train.groupby(['SibSp','Survived'])
    df = pd.DataFrame(g.count()['PassengerId'])
    # print df

def Cabin():
    fig = plt.figure()
    fig.set(alpha=0.2)  # 设定图表颜色alpha参数

    Survived_cabin = data_train.Survived[pd.notnull(data_train.Cabin)].value_counts()
    Survived_nocabin = data_train.Survived[pd.isnull(data_train.Cabin)].value_counts()
    df=pd.DataFrame({u'有':Survived_cabin, u'无':Survived_nocabin}).transpose()
    df.plot(kind='bar', stacked=True)
    plt.title(u"按Cabin有无看获救情况")
    plt.xlabel(u"Cabin有无")
    plt.ylabel(u"人数")
    plt.show()


#------------------------------------------------------------------------------
def set_Cabin_type(df):
    df.loc[ (df.Cabin.notnull()), 'Cabin' ] = "Yes"
    df.loc[ (df.Cabin.isnull()), 'Cabin' ] = "No"
    return df

### 使用 RandomForestClassifier 填补缺失的年龄属性
def set_missing_ages(df):

    # 把已有的数值型特征取出来丢进Random Forest Regressor中
    age_df = df[['Age','Fare', 'Parch', 'SibSp', 'Pclass']]

    # 乘客分成已知年龄和未知年龄两部分
    known_age = age_df[age_df.Age.notnull()].as_matrix()
    unknown_age = age_df[age_df.Age.isnull()].as_matrix()

    # y即目标年龄
    y = known_age[:, 0]

    # X即特征属性值
    X = known_age[:, 1:]

    # fit到RandomForestRegressor之中
    rfr = RandomForestRegressor(random_state=0, n_estimators=2000, n_jobs=-1)
    rfr.fit(X, y)

    # 用得到的模型进行未知年龄结果预测
    predictedAges = rfr.predict(unknown_age[:, 1::])

    # 用得到的预测结果填补原缺失数据
    df.loc[ (df.Age.isnull()), 'Age' ] = predictedAges

    return df, rfr

def addMiss(data_train):
    data_train, rfr = set_missing_ages(data_train)
    data_train = set_Cabin_type(data_train)
    return data_train, rfr

def dumm(data_train):
    dummies_Cabin = pd.get_dummies(data_train['Cabin'], prefix= 'Cabin')
    dummies_Embarked = pd.get_dummies(data_train['Embarked'], prefix= 'Embarked')
    dummies_Sex = pd.get_dummies(data_train['Sex'], prefix= 'Sex')
    dummies_Pclass = pd.get_dummies(data_train['Pclass'], prefix= 'Pclass')

    df = pd.concat([data_train, dummies_Cabin, dummies_Embarked, dummies_Sex, dummies_Pclass], axis=1)
    df.drop(['Pclass', 'Name', 'Sex', 'Ticket', 'Cabin', 'Embarked'], axis=1, inplace=True)
    return df

def normalized(df):
    import sklearn.preprocessing as preprocessing
    scaler = preprocessing.StandardScaler()
    age_scale_param = scaler.fit(df['Age'])
    df['Age_scaled'] = scaler.fit_transform(df['Age'], age_scale_param)
    fare_scale_param = scaler.fit(df['Fare'])
    df['Fare_scaled'] = scaler.fit_transform(df['Fare'], fare_scale_param)
    # print df
    return df, scaler, age_scale_param, fare_scale_param

def lr(df):
    # 用正则取出我们要的属性值
    train_df = df.filter(regex='Survived|Age_.*|SibSp|Parch|Fare_.*|Cabin_.*|Embarked_.*|Sex_.*|Pclass_.*')
    train_np = train_df.as_matrix()

    # y即Survival结果
    y = train_np[:, 0]
    # X即特征属性值
    X = train_np[:, 1:]

    # fit到RandomForestRegressor之中
    clf = linear_model.LogisticRegression(C=1.0, penalty='l1', tol=1e-6)
    clf.fit(X, y)
    print clf
    print pd.DataFrame({"columns":list(train_df.columns)[1:], "coef":list(clf.coef_.T)})
    return clf

def createModle():
    data = loadData()
    data, rfr = addMiss(data)
    df = dumm(data)
    df, scaler, age_scale_param, fare_scale_param = normalized(df)
    clf = lr(df)
    return clf, rfr, scaler, age_scale_param, fare_scale_param

def cross():
    data = loadData()
    data, rfr = addMiss(data)
    df = dumm(data)
    df, scaler, age_scale_param, fare_scale_param = normalized(df)
    clf = lr(df)
    f = linear_model.LogisticRegression(C=1.0, penalty='l1', tol=1e-6)
    all_data = df.filter(regex='Survived|Age_.*|SibSp|Parch|Fare_.*|Cabin_.*|Embarked_.*|Sex_.*|Pclass_.*')
    X = all_data.as_matrix()[:,1:]
    y = all_data.as_matrix()[:,0]
    print cross_validation.cross_val_score(clf, X, y, cv=5)

def badCase():
    data = loadData()
    data, rfr = addMiss(data)
    df = dumm(data)
    df, scaler, age_scale_param, fare_scale_param = normalized(df)
    # 分割数据，按照 训练数据:cv数据 = 7:3的比例
    split_train, split_cv = cross_validation.train_test_split(df, test_size=0.3, random_state=0)
    train_df = split_train.filter(regex='Survived|Age_.*|SibSp|Parch|Fare_.*|Cabin_.*|Embarked_.*|Sex_.*|Pclass_.*')
    # 生成模型
    clf = linear_model.LogisticRegression(C=1.0, penalty='l1', tol=1e-6)
    clf.fit(train_df.as_matrix()[:,1:], train_df.as_matrix()[:,0])

    # 对cross validation数据进行预测

    cv_df = split_cv.filter(regex='Survived|Age_.*|SibSp|Parch|Fare_.*|Cabin_.*|Embarked_.*|Sex_.*|Pclass_.*')
    predictions = clf.predict(cv_df.as_matrix()[:,1:])

    origin_data_train = pd.read_csv("train.csv")
    bad_cases = origin_data_train.loc[origin_data_train['PassengerId'].isin(split_cv[predictions != cv_df.as_matrix()[:,0]]['PassengerId'].values)]
    # print bad_cases

# 用sklearn的learning_curve得到training_score和cv_score，使用matplotlib画出learning curve
def plot_learning_curve(estimator, title, X, y, ylim=None, cv=None, n_jobs=1,
        train_sizes=np.linspace(.05, 1., 20), verbose=0, plot=True):
    """
    画出data在某模型上的learning curve.
    参数解释
    ----------
    estimator : 你用的分类器。
    title : 表格的标题。
    X : 输入的feature，numpy类型
    y : 输入的target vector
    ylim : tuple格式的(ymin, ymax), 设定图像中纵坐标的最低点和最高点
    cv : 做cross-validation的时候，数据分成的份数，其中一份作为cv集，其余n-1份作为training(默认为3份)
    n_jobs : 并行的的任务数(默认1)
    """
    train_sizes, train_scores, test_scores = learning_curve(
             estimator, X, y, cv=cv, n_jobs=n_jobs, train_sizes=train_sizes, verbose=verbose)

    train_scores_mean = np.mean(train_scores, axis=1)
    train_scores_std = np.std(train_scores, axis=1)
    test_scores_mean = np.mean(test_scores, axis=1)
    test_scores_std = np.std(test_scores, axis=1)

    if plot:
        plt.figure()
        plt.title(title)
        if ylim is not None:
            plt.ylim(*ylim)
        plt.xlabel(u"训练样本数")
        plt.ylabel(u"得分")
        plt.gca().invert_yaxis()
        plt.grid()

        plt.fill_between(train_sizes, train_scores_mean - train_scores_std, train_scores_mean + train_scores_std,
        alpha=0.1, color="b")
        plt.fill_between(train_sizes, test_scores_mean - test_scores_std, test_scores_mean + test_scores_std,
        alpha=0.1, color="r")
        plt.plot(train_sizes, train_scores_mean, 'o-', color="b", label=u"训练集上得分")
        plt.plot(train_sizes, test_scores_mean, 'o-', color="r", label=u"交叉验证集上得分")

        plt.legend(loc="best")

        plt.draw()
        plt.show()
        plt.gca().invert_yaxis()
    midpoint = ((train_scores_mean[-1] + train_scores_std[-1]) + (test_scores_mean[-1] - test_scores_std[-1])) / 2
    diff = (train_scores_mean[-1] + train_scores_std[-1]) - (test_scores_mean[-1] - test_scores_std[-1])
    return midpoint, diff

def plotLearning():
    data = loadData()
    data, rfr = addMiss(data)
    df = dumm(data)
    df, scaler, age_scale_param, fare_scale_param = normalized(df)
    train_df = df.filter(regex='Survived|Age_.*|SibSp|Parch|Fare_.*|Cabin_.*|Embarked_.*|Sex_.*|Pclass_.*')
    train_np = train_df.as_matrix()
    # y即Survival结果
    y = train_np[:, 0]
    # X即特征属性值
    X = train_np[:, 1:]
    # fit到RandomForestRegressor之中
    clf = linear_model.LogisticRegression(C=1.0, penalty='l1', tol=1e-6)

    plot_learning_curve(clf, u"学习曲线", X, y)

# Test
def loadTestData():
    data_test = pd.read_csv("test.csv")
    return data_test

def normalizedTest(data_test, rfr, scaler, age_scale_param, fare_scale_param):
    data_test.loc[ (data_test.Fare.isnull()), 'Fare' ] = 0
    # 接着我们对test_data做和train_data中一致的特征变换
    # 首先用同样的RandomForestRegressor模型填上丢失的年龄
    tmp_df = data_test[['Age','Fare', 'Parch', 'SibSp', 'Pclass']]
    null_age = tmp_df[data_test.Age.isnull()].as_matrix()
    # 根据特征属性X预测年龄并补上
    X = null_age[:, 1:]
    predictedAges = rfr.predict(X)
    data_test.loc[ (data_test.Age.isnull()), 'Age' ] = predictedAges

    data_test = set_Cabin_type(data_test)
    dummies_Cabin = pd.get_dummies(data_test['Cabin'], prefix= 'Cabin')
    dummies_Embarked = pd.get_dummies(data_test['Embarked'], prefix= 'Embarked')
    dummies_Sex = pd.get_dummies(data_test['Sex'], prefix= 'Sex')
    dummies_Pclass = pd.get_dummies(data_test['Pclass'], prefix= 'Pclass')


    df_test = pd.concat([data_test, dummies_Cabin, dummies_Embarked, dummies_Sex, dummies_Pclass], axis=1)
    df_test.drop(['Pclass', 'Name', 'Sex', 'Ticket', 'Cabin', 'Embarked'], axis=1, inplace=True)
    df_test['Age_scaled'] = scaler.fit_transform(df_test['Age'], age_scale_param)
    df_test['Fare_scaled'] = scaler.fit_transform(df_test['Fare'], fare_scale_param)
    # print df_test
    return df_test

def result(df_test, clf, data_test):
    test = df_test.filter(regex='Age_.*|SibSp|Parch|Fare_.*|Cabin_.*|Embarked_.*|Sex_.*|Pclass_.*')
    predictions = clf.predict(test)
    result = pd.DataFrame({'PassengerId':data_test['PassengerId'].as_matrix(), 'Survived':predictions.astype(np.int32)})
    result.to_csv("logistic_regression_predictions.csv", index=False)

def main():
    clf, rfr, scaler, age_scale_param, fare_scale_param = createModle()
    data_test = loadTestData()
    df_test = normalizedTest(data_test, rfr, scaler, age_scale_param, fare_scale_param)
    result(df_test, clf, data_test)

def bagging():
    data = loadData()
    data, rfr = addMiss(data)
    df = dumm(data)
    df, scaler, age_scale_param, fare_scale_param = normalized(df)

    train_df = df.filter(regex='Survived|Age_.*|SibSp|Parch|Fare_.*|Cabin_.*|Embarked_.*|Sex_.*|Pclass.*|Mother|Child|Family|Title')
    train_np = train_df.as_matrix()
    # y即Survival结果
    y = train_np[:, 0]

    # X即特征属性值
    X = train_np[:, 1:]

    # fit到BaggingRegressor之中
    clf = linear_model.LogisticRegression(C=1.0, penalty='l1', tol=1e-6)
    bagging_clf = BaggingRegressor(clf, n_estimators=20, max_samples=0.8, max_features=1.0, bootstrap=True, bootstrap_features=False, n_jobs=-1)
    bagging_clf.fit(X, y)

    data_test = loadTestData()
    df_test = normalizedTest(data_test, rfr, scaler, age_scale_param, fare_scale_param)

    test = df_test.filter(regex='Age_.*|SibSp|Parch|Fare_.*|Cabin_.*|Embarked_.*|Sex_.*|Pclass.*|Mother|Child|Family|Title')
    predictions = bagging_clf.predict(test)
    result = pd.DataFrame({'PassengerId':data_test['PassengerId'].as_matrix(), 'Survived':predictions.astype(np.int32)})
    result.to_csv("logistic_regression_bagging_predictions.csv", index=False)

