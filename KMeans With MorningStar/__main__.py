import os
from Company import Company
import KMeans
import pandas as pd
import collections

def download_data():
    # mkdir
    folder = os.path.exists(r".\\data\\")
    if not folder:
        os.makedirs(".\\data")

    company_list = Company.get_code_list()
    company_list = [Company(x) for x in company_list]
    [i.start() for i in company_list]
    [i.join() for i in company_list]


def data_prepare():
    # Company.save_to_year()
    return Company.load_factor()


def get_cluster():
    cluster_list = dict()
    for k in range(3, 9):
        k_temp = KMeans.KMeans(k)
        for j in range(2010, 2013):
            print("Fitting, k=", k, "Year=", j)
            k_temp.fit(data2test[j])
            s_key = str(k) + "_" + str(j)
            cluster_list[s_key] = k_temp.cluster

    return cluster_list


if __name__ == '__main__':
    # download_data()
    data2test, company_list = data_prepare()
    clusters = get_cluster()

    clustered = pd.DataFrame(columns=range(3, 9), index=range(len(data2test[2010])))
    for company in range(len(data2test[2010])):
        for k in range(3, 9):
            temp = []
            for year in range(2010, 2013):
                s_key = str(k) + "_" + str(year)
                temp.append([0 in x.member for x in clusters[s_key]].index(True))

            counter = collections.Counter(temp)
            if max(counter.values()) >= 0.8 * 3:
                clustered.loc[company, k] = counter.most_common(1)[0][0]
            else:
                clustered.loc[company, k] = -1

    for k in range(3, 9):
        for group in range(0, k):
            temp = company_list.loc[clustered[k] == group]
            if not temp.empty:
                print("thesse companies are similar when k=", k, list(temp))

    clustered.index = company_list
    print(clustered)