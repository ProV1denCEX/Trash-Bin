import urllib
from bs4 import BeautifulSoup


def get_url_text(s_url):
    # Get url contents
    temp = urllib.request.urlopen(s_url).read()
    soup = BeautifulSoup(temp, "lxml")
    for script in soup(["script", "style"]):
        script.extract()

    s_text = soup.get_text(' ')

    # replace all non-al-num char to ' '
    #   Will cause some differences with the result without replacing them
    # Split them to get a dict and count num of words
    s_text = "".join([x if x.isalnum() else ' ' for x in s_text])
    c_word = s_text.split()
    d_word = {}
    
    # actually we can use conter in collections
    for i in c_word:
        if i in d_word.keys():
            d_word[i] += 1
        else:
            d_word[i] = 1

    return d_word


def cal_cos_sim(d_web_1, d_web_2):
    n_same = sum([d_web_1[i] * d_web_2[i] if i in d_web_2.keys() else 0 for i in d_web_1.keys()])
    n_total = (sum([x ** 2 for x in d_web_1.values()]) ** 0.5) * (sum([x ** 2 for x in d_web_2.values()]) ** 0.5)

    if 1 - n_same / n_total < 1e-6:
        return 1
    else:
        return n_same / n_total


def write_output(d_sim):
    s_data2write = ""
    for i in d_sim:
        for j in range(i.__len__()):
            if j == 0:
                s_data2write = s_data2write + str(i[j])
            elif j != i.__len__() - 1:
                s_data2write = s_data2write + ", " + str(i[j])
            else:
                s_data2write = s_data2write + ", " + str(i[j]) + "\n"

    with open('output.txt', 'w') as io:
        io.write(s_data2write)


def get_input_webs(s_file):
    with open(s_file, 'r') as io:
        d_url = io.readlines()

    return d_url


if __name__ == "__main__":
    d_url = get_input_webs('input.txt')
    d_web_dict = []
    for i in d_url:
        d_web_dict.append(get_url_text(i))

    d_web_sim = [[0 for i in range(d_web_dict.__len__())] for i in range(d_web_dict.__len__())]

    for i in range(d_web_dict.__len__()):
        for j in range(d_web_dict.__len__()):
            d_web_sim[i][j] = cal_cos_sim(d_web_dict[i], d_web_dict[j])

    write_output(d_web_sim)
