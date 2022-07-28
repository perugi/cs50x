import os
import sys

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Query the transaction database for all trades by the currently logged in user and sum them up per symbol.
    stocks = db.execute(
        "SELECT user_id, symbol, SUM(quantity)\
           FROM transactions\
          GROUP BY user_id, symbol\
         HAVING user_id = ?",
        session["user_id"],
    )

    for stock in stocks:
        # Get the current info on the stock from the IEX API.
        quote = lookup(stock["symbol"])

        stock["name"] = quote["name"]
        stock["price"] = quote["price"]
        stock["value"] = stock["SUM(quantity)"] * stock["price"]

    # Query the users database for the current cash in the account.
    rows = db.execute(
        "SELECT cash FROM users WHERE id = ?",
        session["user_id"],
    )
    cash = rows[0]["cash"]

    return render_template("index.html", stocks=stocks, cash=cash)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":

        if request.form.get("symbol") and request.form.get("shares").isdigit():
            # Use the IEX API to search for the symbol, get back a dict with symbol, name and price
            quote = lookup(request.form.get("symbol"))
            if quote:

                rows = db.execute(
                    "SELECT cash FROM users WHERE id = ?",
                    session["user_id"],
                )
                cash = rows[0]["cash"]

                # If the user has enough money in the account to complete a transaction, deduct the
                # funds for the transaction from his cash and register the trade.
                transaction_cost = float(request.form.get("shares")) * float(
                    quote["price"]
                )
                if transaction_cost <= cash:
                    # Update the users cash balance.
                    db.execute(
                        "UPDATE users\
                            SET cash = ?\
                          WHERE id = ?",
                        cash - transaction_cost,
                        session["user_id"],
                    )

                    # Record a successful trade.
                    db.execute(
                        "INSERT INTO transactions (user_id, symbol, quantity, price)\
                            VALUES (?, ?, ?, ?);",
                        session["user_id"],
                        quote["symbol"],
                        int(request.form.get("shares")),
                        quote["price"],
                    )

                    flash(
                        f"Bought {request.form.get('shares')} shares of {quote['symbol']} stock @ {usd(quote['price'])}"
                    )

                else:
                    return apology("not enough cash")

                return redirect("/")
            else:
                # Requested symbol was not found by IEX
                return apology("input a valid symbol")
        else:
            # No symbol was input
            return apology("input a valid symbol or quantity")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Query the transaction database for all trades by the currently logged in user.
    stocks = db.execute(
        "SELECT symbol, quantity, price, timestamp\
           FROM transactions\
          WHERE user_id = ?",
        session["user_id"],
    )

    return render_template("history.html", stocks=stocks)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "POST":

        if request.form.get("symbol"):
            # Use the IEX API to search for the symbol, get back a dict with symbol, name and price
            quote = lookup(request.form.get("symbol"))
            if quote:
                return render_template("quoted.html", quote=quote)
            else:
                # Requested symbol was not found by IEX
                return apology("input a valid symbol")
        else:
            # No symbol was input
            return apology("input a valid symbol")

    else:
        return render_template("quote.html")


@app.route("/password_change", methods=["GET", "POST"])
def password_register():
    """Allow the user to change their password"""

    if request.method == "POST":

        # Ensure username was submitted
        if (
            not request.form.get("password_old")
            or not request.form.get("password_new")
            or not request.form.get("password_confirm")
        ):
            return apology("must provide password", 403)

        # Ensure that the old password is correct.
        rows = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password_old")
        ):
            return apology("old password is not correct", 403)

        # Ensure the password and its confirmation matches
        elif request.form.get("password_new") != request.form.get("password_confirm"):
            return apology("passwords do not match", 400)

        # Store the username and hashed password in the database
        db.execute(
            "UPDATE users\
                SET hash = ?\
                WHERE id = ?;",
            generate_password_hash(request.form.get("password_new")),
            session["user_id"],
        )

        return redirect("/")

    else:
        return render_template("password_change.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password") or not request.form.get("confirmation"):
            return apology("must provide password", 400)

        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords do not match", 400)

        # Store the username and hashed password in the database
        try:
            db.execute(
                "INSERT INTO users (username, hash)\
                    VALUES (?, ?);",
                request.form.get("username"),
                generate_password_hash(request.form.get("password")),
            )
        # The username already exists in the database
        except ValueError:
            return apology("username already taken", 400)

        return redirect("/login")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # Query the transaction database for all trades by the currently logged in user and sum them up per symbol.
    stocks = db.execute(
        "SELECT user_id, symbol, SUM(quantity)\
            FROM transactions\
            GROUP BY user_id, symbol\
            HAVING user_id = ?",
        session["user_id"],
    )
    print(stocks, file=sys.stderr)

    if request.method == "POST":

        if request.form.get("symbol") and request.form.get("shares").isdigit():
            # Use the IEX API to search for the symbol, get back a dict with symbol, name and price
            quote = lookup(request.form.get("symbol"))
            if quote:

                # Find the stock with the requested symbol in the users portfolio.
                stock_to_sell = None
                for stock in stocks:
                    if stock["symbol"] == quote["symbol"]:
                        stock_to_sell = stock
                        break

                # If the user has the required number of stocks in his portfolio, add the
                # funds for the transaction to his cash and register the trade.
                if (
                    stock_to_sell
                    and int(request.form.get("shares"))
                    <= stock_to_sell["SUM(quantity)"]
                ):

                    # Update the users cash balance.
                    db.execute(
                        "UPDATE users\
                            SET cash = cash + ?\
                          WHERE id = ?",
                        int(request.form.get("shares")) * quote["price"],
                        session["user_id"],
                    )

                    # Record a successful trade (record the sale as a negative quantity).
                    db.execute(
                        "INSERT INTO transactions (user_id, symbol, quantity, price)\
                            VALUES (?, ?, ?, ?);",
                        session["user_id"],
                        quote["symbol"],
                        -int(request.form.get("shares")),
                        quote["price"],
                    )

                    flash(
                        f"Sold {request.form.get('shares')} shares of {quote['symbol']} stock @ {usd(quote['price'])}"
                    )

                else:
                    return apology("not enough stocks to complete transaction")

                return redirect("/")
            else:
                # Requested symbol was not found by IEX
                return apology("input a valid symbol")
        else:
            # No symbol was input
            return apology("input a valid symbol or quantity")

    else:

        return render_template("sell.html", stocks=stocks)
