# KMeans With MorningStar

## TODO
To reorg the download part and make it a new sdk for morningstar -> No timeline guaranteed

## Description
you will apply K-mean clustering algorithm to group companies together based on the key ratios and other financial variable of a company. To simplify your task, financial and key ratio information may be retrieved from https://www.morningstar.com

For example to retrieve key ratio of Apple Company, you may use this link. https://financials.morningstar.com/ratios/r.html?t=AAPL

Your task is to choose companies from at least four different categories: For example

Technology such as Google, Microsoft, etc

Finance such as Bank of America, JP Morgan, etc

Pharmaceutical such as Merck, Pfizer, etc

Utility such as AES Corp, FirstEnergy etc

Retailer such as Walmart, Target, etc.

Etc.

Choose at least twenty companies from each category that you chose.

Your task is to group these eighty companies into K-groups. Where 2 <=k <= 8.

Write a Python program to read in data for a particular month or year report, pre-process data if needed.

Write a python k-mean program to process these data. Do not use Python package to implement K-mean. You need to write your own code. The output would be a list of lists.

For k = 2, output contains first group companies and second group companies.

For k = 3, output three groups, each group contains some companies.

Similar for k = 4, 5, 6, 7 and 8

After process all these companies for year y, now process year y+1 for the same companies. For example, y = 2013, then y+1 will be 2014. Since the data we retrieved from Morningstar.com may not fall into the same month, to simplify your task, you many assume they are in the same month of that year

Choose some common financial variables such as earning per share, net margin, current ratio, etc (at least 7) from these data set and run the k-mean clustering Python program. Repeat the same process for y+1, and y+2. So your program will process total of three years data.

Output the companies that are 80% of times are always in the same group for k=2 to 8 and year y to y+2. We may say that these companies are very similar.
