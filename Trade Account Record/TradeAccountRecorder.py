import re


class Account(object):

    def __init__(self, s_name):
        self.name = s_name
        self.balance = 0

    def deposit(self, n_cash):
        if n_cash >= 0:
            self.balance += n_cash
        else:
            print("Cannot deposit money less than 0!")

    def withdraw(self, n_cash):
        if n_cash >= 0:
            self.balance -= n_cash
        else:
            print("Cannot withdraw money less than 0!")

    def transfer(self, c_account, n_cash):
        if n_cash >= 0:
            self.balance -= n_cash
            c_account.balance += n_cash

        elif n_cash > self.balance:
            print("Cannot transfer money more than current balance!")

        else:
            print("Cannot withdraw money less than 0!")

    def __str__(self):
        return self.name + ", " + str(self.balance)


class Asset(object):
    def __init__(self, s_symbol, n_num, n_price):
        self.symbol = s_symbol
        self.num = n_num
        self.cost_total = n_num * n_price

    def add(self):
        pass

    def reduce(self):
        pass


class Stock(Asset):
    def add(self, n_num, n_price):
        self.num += n_num
        self.cost_total += n_num * n_price
        return n_num * n_price

    def reduce(self, n_num, n_price):
        self.num -= n_num
        self.cost_total -= self.cost_total / self.num * n_num
        return n_num * n_price

    def __str__(self):
        return self.symbol + ", " + str(self.num) + ", " + str(self.cost_total)


class RealEstate(Asset):
    def __init__(self, s_symbol, n_price):
        Asset.__init__(self, s_symbol, 1, n_price)
        self.address_ID = self.symbol
        self.market_value = self.cost_total

    def add(self):
        return self.cost_total

    def reduce(self, n_price):
        if n_price > 0:
            return n_price
        else:
            return self.cost_total

    def __str__(self):
        return self.address_ID + ", " + str(self.market_value)


class Portfolio(object):
    def __init__(self, s_name):
        self.name = s_name
        self.stock = list()
        self.real_estate = list()
        self.account = Account(s_name)

    def buy_stock(self, s_symbol, n_num, n_price):
        if n_num > 0 and n_price > 0:
            if self.account.balance >= n_num * n_price:
                c_list = [i.symbol for i in self.stock]
                if s_symbol in c_list:
                    self.account.balance -= self.stock[c_list.index(
                        s_symbol)].add(n_num, n_price)
                else:
                    self.stock.append(Stock(s_symbol, n_num, n_price))
                    self.account.balance -= n_num * n_price

            else:
                raise ValueError("no enough money!")

        else:
            raise ValueError("Cannot buy stock less than 0")

    def sell_stock(self, s_symbol, n_num, n_price=0.0):
        if n_num > 0 and n_price > 0:
            c_list = [i.symbol for i in self.stock]
            if s_symbol in c_list:
                dlocated = c_list.index(s_symbol)
                if n_num <= self.stock[dlocated].num:
                    self.account.balance += self.stock[dlocated].reduce(
                        n_num, n_price)
                    if self.stock[dlocated].num == 0:
                        self.stock.pop(dlocated)

                else:
                    raise ValueError("Cannot sell asset more than you have!")

            else:
                raise ValueError("no such asset in account!")

        else:
            raise ValueError("Cannot sell stock less than 0")

    def buy_real_estate(self, s_address_ID, n_market_value):
        if self.account.balance >= n_market_value:
            c_list = [i.address_ID for i in self.real_estate]
            if s_address_ID in c_list:
                raise ValueError("Cannot buy owned real estate!")
            else:
                self.real_estate.append(
                    RealEstate(s_address_ID, n_market_value))
                self.account.balance -= n_market_value

        else:
            raise ValueError("no enough money!")

    def sell_real_estate(self, s_address_ID, n_price=0.0):
        c_list = [i.address_ID for i in self.real_estate]
        if s_address_ID in c_list:
            dlocated = c_list.index(s_address_ID)
            self.account.balance += self.real_estate[dlocated].reduce(n_price)
            self.stock.pop(dlocated)

        else:
            raise ValueError("Cannot sell unowned real estate!")

    def __str__(self):
        return "Portfolio Status: " + self.name + "\n" + "  " + "Account: " + self.account.__str__() + "\n" + "  " + "Stock: " + "".join(
            ["\n" + "    " + i.__str__() for i in self.stock]) + "\n" + "  " + "Real Estate: " + "".join(["\n" + "    " + i.__str__() for i in self.real_estate])

    @staticmethod
    def transfer(portfolio_in, portfolio_out, obj, n_num=1):
        if isinstance(obj, str):
            c_list_stock_symbol = [i.symbol for i in portfolio_out.stock]
            c_list_real_ID = [
                i.address_ID for i in portfolio_out.real_estate]

            if obj in c_list_stock_symbol and obj not in c_list_real_ID:
                dlocated_out = c_list_stock_symbol.index(obj)
                c_temp_in = [i.symbol for i in portfolio_in.stock]

                if obj in c_temp_in:
                    dlocated_in = c_temp_in.index(obj)
                    portfolio_in.stock[dlocated_in].num += n_num
                    portfolio_in.stock[dlocated_in].cost_total += portfolio_out.stock[dlocated_out].cost_total * \
                        n_num / portfolio_out.stock[dlocated_out].num

                else:
                    portfolio_in.stock.append(
                        Stock(
                            obj,
                            n_num,
                            portfolio_out.stock[dlocated_out].cost_total /
                            portfolio_out.stock[dlocated_out].num))

                portfolio_out.stock[dlocated_out].num -= n_num
                portfolio_out.stock[dlocated_out].cost_total -= portfolio_out.stock[dlocated_out].cost_total * \
                    n_num / portfolio_out.stock[dlocated_out].num
                if not portfolio_out.stock[dlocated_out].num:
                    portfolio_out.stock.pop(dlocated_out)

            elif obj not in c_list_stock_symbol and obj in c_list_real_ID:
                dlocated_out = c_list_real_ID.index(obj)
                c_temp_in = [i.symbol for i in portfolio_in.real_estate]

                if obj in c_temp_in:
                    raise ValueError("Error: Duplicated real estate")

                else:
                    portfolio_in.real_estate.append(
                        RealEstate(
                            obj,
                            portfolio_out.real_estate[dlocated_out].market_value))

                portfolio_out.real_estate.pop(dlocated_out)

            elif obj not in c_list_stock_symbol and obj not in c_list_real_ID:
                raise RuntimeError(
                    "Cannot find %s in %s" % (obj, portfolio_out.name))

            elif obj in c_list_stock_symbol and obj in c_list_real_ID:
                raise RuntimeError(
                    "find %s both in stock list and real estate list" % obj)

        elif isinstance(obj, (int, float)):
            if portfolio_out.account.balance >= obj:
                portfolio_in.account.balance += obj
                portfolio_out.account.balance -= obj

            else:
                raise ValueError("No enough money!")

        else:
            raise ValueError("Unexpected transfer type!")

        return portfolio_in, portfolio_out

    @staticmethod
    def merge(portfolio_1, portfolio_2, portfolio_new):
        portfolio_new, portfolio_1 = Portfolio.transfer(
            portfolio_new,
            portfolio_1,
            portfolio_1.account.balance)
        for i in portfolio_1.stock:
            portfolio_new, portfolio_1 = Portfolio.transfer(
                portfolio_new, portfolio_1, i.symbol)

        for i in portfolio_1.real_estate:
            portfolio_new, portfolio_1 = Portfolio.transfer(
                portfolio_new, portfolio_1, i.address_ID)

        portfolio_new, portfolio_2 = Portfolio.transfer(
            portfolio_new,
            portfolio_2,
            portfolio_2.account.balance)
        for i in portfolio_2.stock:
            portfolio_new, portfolio_2 = Portfolio.transfer(
                portfolio_new, portfolio_2, i.symbol)

        for i in portfolio_2.real_estate:
            portfolio_new, portfolio_2 = Portfolio.transfer(
                portfolio_new, portfolio_2, i.address_ID)

        return portfolio_new


def clean_commend(s_commend):
    temp = re.findall(r'(.*?)#', s_commend)
    return temp[0].replace("\t", "").strip().split(", ")


def main():
    s_output = ""
    c_list_portfolio = list()
    with open("input.txt", 'r') as io:
        for s_commend in io:
            c_list_commend = clean_commend(s_commend)

            c_list_portfolio_name = [i.name for i in c_list_portfolio]
            s_commend_type = c_list_commend[0].lower()
            try:
                if s_commend_type == "cp":
                    s_name = c_list_commend[1]

                    if s_name in c_list_portfolio_name:
                        raise ValueError("Already has a portfolio %s" % s_name)

                    else:
                        c_list_portfolio.append(Portfolio(s_name))

                elif s_commend_type == "dp":
                    s_name = c_list_commend[1]
                    n_cash = float(c_list_commend[2])

                    if s_name in c_list_portfolio_name:
                        dlocated = c_list_portfolio_name.index(s_name)
                        c_list_portfolio[dlocated].account.deposit(n_cash)

                    else:
                        raise ValueError("No such portfolio %s" % s_name)

                elif s_commend_type == "bs":
                    s_name = c_list_commend[4]
                    s_symbol = c_list_commend[1]
                    n_amount = float(c_list_commend[2])
                    n_price = float(c_list_commend[3])

                    if s_name in c_list_portfolio_name:
                        dlocated = c_list_portfolio_name.index(s_name)
                        c_list_portfolio[dlocated].buy_stock(
                            s_symbol, n_amount, n_price)

                    else:
                        raise ValueError("No such portfolio %s" % s_name)

                elif s_commend_type == "br":
                    s_name = c_list_commend[3]
                    s_symbol = c_list_commend[1]
                    n_price = float(c_list_commend[2])

                    if s_name in c_list_portfolio_name:
                        dlocated = c_list_portfolio_name.index(s_name)
                        c_list_portfolio[dlocated].buy_real_estate(
                            s_symbol, n_price)

                    else:
                        raise ValueError("No such portfolio %s" % s_name)

                elif s_commend_type == "wd":
                    s_name = c_list_commend[1]
                    n_cash = float(c_list_commend[2])

                    if s_name in c_list_portfolio_name:
                        dlocated = c_list_portfolio_name.index(s_name)
                        c_list_portfolio[dlocated].account.withdraw(n_cash)

                    else:
                        raise ValueError("No such portfolio %s" % s_name)

                elif s_commend_type == "ss":
                    s_name = c_list_commend[4]
                    s_symbol = c_list_commend[1]
                    n_amount = float(c_list_commend[2])
                    n_price = float(c_list_commend[3])

                    if s_name in c_list_portfolio_name:
                        dlocated = c_list_portfolio_name.index(s_name)
                        c_list_portfolio[dlocated].sell_stock(
                            s_symbol, n_amount, n_price)

                    else:
                        raise ValueError("No such portfolio %s" % s_name)

                elif s_commend_type == "sr":
                    s_name = c_list_commend[3]
                    s_symbol = c_list_commend[1]
                    n_price = float(c_list_commend[2])

                    if s_name in c_list_portfolio_name:
                        dlocated = c_list_portfolio_name.index(s_name)
                        c_list_portfolio[dlocated].sell_real_estate(
                            s_symbol, n_price)

                    else:
                        raise ValueError("No such portfolio %s" % s_name)

                elif s_commend_type == "xf" or s_commend_type == "xs" or s_commend_type == "xr":

                    if s_commend_type == "xf":
                        s_portfolio_in = c_list_commend[1]
                        s_portfolio_out = c_list_commend[2]
                        obj = float(c_list_commend[3])
                    else:
                        s_portfolio_in = c_list_commend[2]
                        s_portfolio_out = c_list_commend[1]
                        obj = c_list_commend[3]

                    if c_list_commend.__len__() == 5:
                        n_num = float(c_list_commend[4])
                    else:
                        n_num = 0

                    if s_portfolio_in in c_list_portfolio_name and s_portfolio_out in c_list_portfolio_name:
                        dlocated_in = c_list_portfolio_name.index(
                            s_portfolio_in)
                        dlocated_out = c_list_portfolio_name.index(
                            s_portfolio_out)
                        c_list_portfolio[dlocated_in], c_list_portfolio[dlocated_out] = Portfolio.transfer(
                            c_list_portfolio[dlocated_in], c_list_portfolio[dlocated_out], obj, n_num)

                    else:
                        raise ValueError(
                            "No such portfolio %s or %s" %
                            (s_portfolio_in, s_portfolio_out))

                elif s_commend_type == "mg":
                    s_portfolio_1 = c_list_commend[1]
                    s_portfolio_2 = c_list_commend[2]
                    s_portfolio_new = c_list_commend[3]

                    dlocated_1 = c_list_portfolio_name.index(s_portfolio_1)
                    dlocated_2 = c_list_portfolio_name.index(s_portfolio_2)

                    portfolio_new = Portfolio.merge(
                        c_list_portfolio[dlocated_1],
                        c_list_portfolio[dlocated_2],
                        Portfolio(s_portfolio_new))

                    c_list_portfolio.pop(dlocated_1)
                    c_list_portfolio_name = [i.name for i in c_list_portfolio]
                    dlocated_2 = c_list_portfolio_name.index(s_portfolio_2)
                    c_list_portfolio.pop(dlocated_2)
                    c_list_portfolio.append(portfolio_new)

            except IndexError:
                raise IndexError(
                    "No enough inputs for commend %s" %
                    c_list_commend[0])

            s_output += s_commend + \
                "".join([i.__str__() for i in c_list_portfolio]) + "\r\n"
            print(s_commend + "".join([i.__str__()
                                       for i in c_list_portfolio]) + "\n")

    with open("output.txt", 'w') as io:
        io.writelines(s_output)


if __name__ == '__main__':
    main()
