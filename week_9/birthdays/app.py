import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
def index():
    birthdays = db.execute("SELECT * FROM birthdays;")

    if request.method == "POST":

        # Get the data from the POST request.
        id = request.form.get("id")
        if id:
            id = int(id)
        name = request.form.get("name")
        month = request.form.get("month")
        if month:
            month = int(month)
        day = request.form.get("day")
        if day:
            day = int(day)

        if (
            name
            and day
            and day >= 1
            and day <= 31
            and month
            and month >= 1
            and month <= 12
        ):

            # If the id already exists in the database, update the information.
            if id in [birthday["id"] for birthday in birthdays]:
                db.execute(
                    "UPDATE birthdays\
                        SET name = %s,\
                            month = %s,\
                            day = %s\
                      WHERE id = %s;",
                    name,
                    month,
                    day,
                    id,
                )

            # If the ID does not exist, create a new entry
            else:
                db.execute(
                    "INSERT INTO birthdays (name, month, day)\
                     VALUES (%s, %s, %s);",
                    name,
                    month,
                    day,
                )

        return redirect("/")

    else:
        # Display the entries in the database on index.html
        return render_template("index.html", birthdays=birthdays)


@app.route("/delete", methods=["POST"])
def delete():

    id = request.form.get("id")
    if id:
        db.execute(
            "DELETE FROM birthdays\
              WHERE id = %s;",
            id,
        )

    return redirect("/")
