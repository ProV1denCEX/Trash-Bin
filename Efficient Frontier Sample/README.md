# Efficient Frontier Sample
Download monthly returns observations from CRSP for the SP500 value-weighted average index, the Nasdaq equally-weighted average index, 10 randomly-selected firms and the risk-free rate (30-day T-bill) for the past 10 years (2008.1-2017.12). To make your job easier, make sure that each stock has continuous observations for the 10 years. Also, try to select firms from different industries.


1. Assume that stock returns are serially uncorrelated. Construct the portfolio frontier in the mean- standard deviation space and in the mean-variance space using the 10 risky stocks only. Show graphically.

2. Is the SP500 value-weighted portfolio efficient compared to the portfolio frontier that you constructed in question 1 above? How about the Nasdaq equally-weighted portfolio?

3. Continue to use the 10 risky stocks. Construct the minimum variance portfolio “mvp” and the
~	~
equally weighted portfolio “ewp” of the 10 stocks. Compute E(rmvp ) ,	mvp , E(rewp ) ,	ewp , and

mvp,ewp . Verify numerically that  mvp , ewp      mvp2 .

4.	Suppose that you are a portfolio manager. Your client who has $10 million to invest in the stocks is relatively aggressive. She wants her target expected monthly returns to be 0.5% higher than the equally weighted portfolio while minimizing her portfolio risk. How should you advise her to invest the $10 million in these 10 stocks to achieve her goal? You should advise her that in order to obtain a higher expected return, she will have to bear more risk. What is the minimum amount of risk that she has to bear if she just needs an expected return equal to that of the equally weighted portfolio? What is the additional risk that she has to take if she wants an additional 0.5% return per month? Do you think the return is worth the additional risk? How do you convince her?

5.	You should advise your client that past return does not necessarily guarantee future success. To convince yourself, you can take the data for the first 5 years to construct your target portfolio, and then use the data for the last 5 years to do a so-called “out-of-sample” test. Does your client’s portfolio for the last 5 years achieve the expected return of the target portfolio constructed using data from the first 5 years?

6.	Add the risk-free asset to your universe of assets. Construct the portfolio frontier with the 10 stocks and the risk-free rate in the mean-standard deviation space. Superimpose the portfolio frontier constructed without the risk-free rate on that constructed with the risk-free rate. Show graphically.

7.	Randomly pick a stock in your universe (in general this stock is unlikely the minimum variance portfolio or even a frontier portfolio). Find the minimum variance portfolio that is uncorrelated with this stock.
 


1
 

8.	The value at risk (VaR) is an estimate of the amount by which an investor’s position in a risky category could decline due to general market movements during a given period of time. We define the VaR of a long position over time horizon l with probability p as

p	P[  V (l )	VaR ]	Fl (VaR)

where, V (l) is the change in the value of the assets in the financial position (in dollar terms) and Fl (x) is its cumulative distribution function (cdf).

Similarly, the VaR of a short position is defined as

p	P[  V (l )	VaR ]	1	P[  V (l )	VaR ]	1	Fl (VaR) .

Assume that the returns of the equally weighted portfolio, your client’s first target portfolio, and her second target portfolio that you have constructed in problem 3 above are each serially independent and identically distributed. (Note that this is a strong assumption that does not hold in most data, but we make this assumption to greatly simplify the analysis). The VaR for a given long position for the next period can be calculated as follows:

VaR	(position)	+ (p% critical value) x


where	and	are the mean and standard deviation of your portfolio return. For each of the

three portfolios you constructed in problem 3 above, calculate the VaR’s for the next month at the 1% probability under the following respective distributional assumptions: (i) normal distribution; (ii) t(3) distribution; and (iii) nonparametric bootstrap distribution with 10,000 replications with replacement. From this exercise, you should see the significant impact on risk if the return distribution is not normal. (Note: we usually use daily data to compute VaR. But you can just use the monthly data you already have for this exercise.)



