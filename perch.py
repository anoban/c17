import pandas as pd
from sklearn.linear_model import LinearRegression

df = pd.read_csv(r"./perch.csv")
predictors = df.iloc[:, 1:]
target = df.Weight

model = LinearRegression()
model.fit(predictors, target)

print(model.coef_)
print(model.intercept_)
