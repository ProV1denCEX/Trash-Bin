# Cal Web Simularity
A solution using Python to compute the similarity of different web pages. Specifically we are going to compute cosine similarity among web pages.

Cosine similarity is a measure of similarity between two nonzero vectors of an inner product space that is the cosine of the angle between the vectors. It can be used to measure the similarity of two arbitrary objects as long as these objects can be represented as a collection of attribute-value pairs. It is commonly used in machine learning and data mining.

For example, Let A be “you say up to you?” and	be a text of “It is up to you to do it or not to do it.”.

We can strip punctuations off so A has {you:2, say:1, up:1, to:1} and B contains {it:3, is:1, up:1, to:3, you:1, do:2, or:1, not:1}

Note the number after the word is the frequency count of that word. So document A contains 4 words and string ‘you’ appeared 2 times, etc.

You may use the following script to retrieve an html from internet which had removed html tags and styles.

``` python
import urllib

from bs4 import BeautifulSoup

#url = "https://www.jpmorgan.com/country/US/en/jpmorgan"

#url = "http://bankofamerica.com/"

url = "http://rutgers.edu/"

html = urllib.request.urlopen(url).read()

#soup = BeautifulSoup(html)

soup = BeautifulSoup(html, "lxml" )

#	remove all script and style elements for script in soup(["script", "style"]):

script.extract()

#	now retrieve text

text = soup.get_text()

print(text)
```


Note, you have to write your own Python code to further process the text retrieved from the above script.

Your program will compare 7 web pages. Output the similarities among these pages. Use a tabular format so that it is easier to see. Because of symmetry, the similarity of (page-i, page-j) is the same as (page-j, page-i) and also (page-i, page-i) has similarity of 100%, so just output (7x6)/2 = 21 entries in your table. Choose output in either an upper triangle or lower triangle form.

In addition to the above output, you will also output the most similar page of each individual page. I.e., output for each the page with highest similarity. Note page-i may be the most similar page for page-j but the reverse may not be true.
