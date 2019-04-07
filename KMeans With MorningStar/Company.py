import numpy as np
import pandas as pd
import csv
import threading
import requests
import re
import bs4
import time
import os

semaphore = threading.Semaphore(4)


def _get_cookie():
    url = 'https://financials.morningstar.com/ratios/r.html?t=FI'
    session_temp = requests.session()
    session_temp.get(url)
    return session_temp


def _get_header(code):
    header = {
        'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; WOW64; rv:60.0) Gecko/20100101 Firefox/60.0',
        'Referer': 'https://financials.morningstar.com/ratios/r.html?t=' + code,
        'Accept-Encoding': 'gzip, deflate, br'}
    return header


class Company(threading.Thread):
    def __init__(self, code):
        threading.Thread.__init__(self)
        self._code = code
        self._data = None
        self._industry = None
        self._factor = None
        self._is_ok = False

    @property
    def is_ok(self):
        return self._is_ok

    @property
    def code(self):
        return self._code

    @property
    def data(self):
        return self._data

    @property
    def sector(self):
        return self._industry

    @property
    def indicator(self):
        return self._factor

    @staticmethod
    def get_code_list():
        param = {"pageSize": 926}
        session = _get_cookie()

        report_url = "http://www.morningstar.com/analyst-research/stock-reports.aspx"
        raw = session.post(report_url, data=param)

        soup = bs4.BeautifulSoup(raw.text, features="lxml")
        code_list = {
            re.findall(
                r'\((.*?)\)',
                i.get_text())[0] for i in soup(
                headers="cr_company")}
        return code_list

    def download_data(self):
        semaphore.acquire()

        # Getting cookies
        session = _get_cookie()
        exchange = ["XNAS", "XASE", "XNYS", "PINX"]
        is_finished = False

        for i in range(0, 5):
            for s_exchange in exchange:
                param = {
                    "callback": '?',
                    "t": s_exchange +
                    ':' +
                    self._code,
                    "region": 'usa',
                    "culture": 'en-US',
                    "cur": None,
                    "order": 'asc'}
                url = "https://financials.morningstar.com/finan/ajax/exportKR2CSV.html"
                header = _get_header(self._code)
                content = session.get(url, params=param, headers=header)

                if content.status_code == 200 and content.content:
                    s_file = ''.join([r'.\data\\', self._code, '.csv'])
                    with open(s_file, 'wb') as data:
                        data.write(content.content)

                    print(self._code, ' DONE')
                    is_finished = True
                    break

            if not is_finished:
                print(self._code + " Retrying")
                time.sleep(1.551)
            else:
                break

        semaphore.release()

    def run(self) -> None:
        self.download_data()

    def get_data(self):
        df_temp = pd.DataFrame()
        s_code = self.code + '.csv'
        with open(".\\data\\" + s_code, newline='\n', encoding='utf-8') as io:
            data = csv.reader(io)
            for row in data:
                for i in range(0, len(row)):
                    if not row[i]:
                        row[i] = np.nan

                temp = pd.DataFrame(row)
                df_temp = pd.concat([df_temp, temp], axis=1)

        df_temp.columns = range(0, df_temp.shape[1])
        if df_temp.size != 1:
            self._industry = df_temp.iloc[0, 1]
            df_temp.set_index(
                df_temp.loc[:, df_temp.iloc[0].isna()].iloc[:, 0], inplace=True)
            df_temp.drop(df_temp.columns[0:2], axis=1, inplace=True)

            temp = df_temp.loc[np.nan]
            temp[temp.isna()] = 'Date'
            self._factor = temp

            df_temp.drop([df_temp.index[0], df_temp.index[-1]],
                         axis=0, inplace=True)
            df_temp.index = [int(x[:4]) for x in df_temp.index]
            df_temp.columns = temp
            self._data = df_temp
            self._is_ok = True

    @staticmethod
    def save_to_year():
        d_company = list()
        c_codes = os.listdir(".\\data")
        for s_code in c_codes:
            c_company = Company(s_code[:-4])
            c_company.get_data()
            d_company.append(c_company)

        folder = os.path.exists(r".\\factor\\")
        if not folder:
            os.makedirs(".\\factor")

        factor_list = set()
        company_list = list()
        year_list = set()

        for company in d_company:
            if company.is_ok:
                factor_list = set(company.indicator) | factor_list
                company_list.append(company.code)
                year_list = set(company.data.index) | year_list

        factor_list = sorted(factor_list)

        data_year = dict()
        for i in year_list:
            data2write = pd.DataFrame(index=company_list, columns=factor_list)

            for company in d_company:
                if company.is_ok and i in company.data.index:
                    for j in company.indicator:
                        temp = company.data.loc[i, j]
                        if not isinstance(
                                temp, pd.Series) and not isinstance(
                                temp, pd.DataFrame):
                            data2write.loc[company.code, j] = temp
                        else:
                            if temp.shape.__len__() == 1:
                                data2write.loc[company.code, j] = temp.iloc[0]
                            else:
                                data2write.loc[company.code,
                                               j] = temp.iloc[0, 0]

            data_year[i] = data2write.to_csv(".\\factor\\" + str(i) + ".csv")
        return data_year

    @staticmethod
    def load_factor():
        data2test = dict()
        for i in range(2010, 2019):
            temp = pd.read_csv(".\\factor\\" + str(i) + ".csv")
            company_loaded = temp.iloc[:, 0]
            temp = temp.loc[:, (temp.count() > temp.shape[0] * 0.85)
                            & ((temp.dtypes == float) | (temp.dtypes == int))]

            temp = (temp - temp.min()) / (temp.max() - temp.min())
            temp.fillna(0, inplace=True)
            data2test[i] = temp

        return data2test, company_loaded


if __name__ == '__main__':
    company_list = Company.get_code_list()
    company_list = [Company(x) for x in company_list]
    [i.start() for i in company_list]
    [i.join() for i in company_list]
