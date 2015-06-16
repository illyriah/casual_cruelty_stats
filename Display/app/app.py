#all the imports
import sqlite3
from flask import Flask, request, session, g, redirect, url_for, \
     abort, render_template, flash

from contextlib import closing
import psycopg2

#configuration
DATABASE = '/tmp/flaskr.db'
DEBUG = True
SECRET_KEY = 'development key'
USERNAME = 'admin'
PASSWORD = 'default'

#create our little application :)
app = Flask(__name__)
app.config.from_object(__name__)

app.config.from_envvar('FLASKR_SETTINGS', silent=True)

@app.route('/')
def show_entries():
    cur = g.conn.cursor()
    cur.execute("select * from get_match_summaries();")

    entries = [dict(max_players=row[0], min_players=row[1], players_start=row[2], players_end=row[3], map_name=row[4], match_start=row[5], match_end=row[6], total_connections=row[7]) for row in cur.fetchall()]

    cur.execute("select * from get_match_averages();")
    averages = [dict(avg_max_players=row[0], avg_min_players=row[1], avg_players_start=row[2], avg_players_end=row[3], map_name=row[4], times_played=row[5], avg_total_connections=row[6]) for row in cur.fetchall()]

    return render_template('show_entries.html', entries=entries, averages=averages) 


@app.before_request
def before_request():
    g.conn = psycopg2.connect(database="casual_cruelty_stats", user="server", password="server", host="127.0.0.1", port="5432")

@app.teardown_request
def teardown_request(exception):
    conn = getattr(g, 'conn', None)
    if conn is not None:
        conn.close()

if __name__ == '__main__':
    app.debug = True
    app.run(host='0.0.0.0', port=5000)

