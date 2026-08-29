import sqlite3
from db.data_rep import Pirate

DB = "db/vyke.db"

def get_con():
    return sqlite3.connect(DB)

def setup():
    connection = get_con()
    cursor = connection.cursor()

    cursor.execute("""
        CREATE TABLE IF NOT EXISTS pirates (
            user_id TEXT PRIMARY KEY,
            name TEXT NOT NULL,
            berries INTEGER DEFAULT 1000,
            bounty INTEGER DEFAULT 0,
            rank TEXT DEFAULT 'Rookie',
            fleet_id INTEGER,
            last_claim TEXT,
            devil_fruit_type TEXT
        )
        """)

    cursor.execute("""
        CREATE TABLE IF NOT EXISTS shop (
            slot INTEGER PRIMARY KEY,
            fruit_id TEXT NOT NULL,
            name TEXT NOT NULL,
            type TEXT NOT NULL,
            price INTEGER NOT NULL,
            shop_date TEXT NOT NULL
        )
        """)

    connection.commit()
    connection.close()

def create_pirate(pirate):
    connection = get_con()
    cursor = connection.cursor()

    cursor.execute("""
            INSERT INTO pirates
            (user_id,name,berries,bounty,rank,fleet_id,last_claim,devil_fruit_type)
            VALUES (?,?,?,?,?,?,?,?)
        """, (
            pirate.user_id,
            pirate.name,
            pirate.berries,
            pirate.bounty,
            pirate.rank,
            pirate.fleet_id,
            pirate.last_claim,
            pirate.devil_fruit_type
        ))

    connection.commit()
    connection.close()

def get_pirate(user_id):
    connection = get_con()
    cursor = connection.cursor()

    cursor.execute(
        "SELECT * FROM pirates WHERE user_id = ?",
        (user_id,)
    )

    row = cursor.fetchone()
    connection.close()

    if row is None:
        return None

    return Pirate(row[0],row[1],row[6],row[2],row[3],row[4],row[5],row[7])

def update_pirate(pirate):
    connection = get_con()
    cursor = connection.cursor()

    cursor.execute("""
        UPDATE pirates
        SET name = ?,
            berries = ?,
            bounty = ?,
            rank = ?,
            fleet_id = ?,
            last_claim = ?,
            devil_fruit_type = ?
        WHERE user_id = ?
    """, (
        pirate.name,
        pirate.berries,
        pirate.bounty,
        pirate.rank,
        pirate.fleet_id,
        pirate.last_claim,
        pirate.devil_fruit_type,
        pirate.user_id
    ))

    connection.commit()
    connection.close()

def get_shop():
    connection = get_con()
    cursor = connection.cursor()

    cursor.execute(
        "SELECT * FROM shop ORDER BY slot"
    )

    rows = cursor.fetchall()
    connection.close()

    return rows

def update_shop(stock):
    connection = get_con()
    cursor = connection.cursor()

    cursor.execute("DELETE FROM shop")

    for slot,fruit in enumerate(stock,1):
        cursor.execute("""
            INSERT INTO shop
            (slot,fruit_id,name,type,price,shop_date)
            VALUES (?,?,?,?,?,?)
        """, (
            slot,
            fruit["id"],
            fruit["name"]["en"],
            fruit["type"],
            fruit["price"],
            fruit["shop_date"]
        ))

    connection.commit()
    connection.close()
