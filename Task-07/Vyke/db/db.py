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

    cursor.execute("""
        CREATE TABLE IF NOT EXISTS inventory (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id TEXT NOT NULL,
            item_id TEXT NOT NULL,
            item_name TEXT NOT NULL,
            item_type TEXT,
            worth INTEGER NOT NULL
        )
        """)

    cursor.execute("""
            CREATE TABLE IF NOT EXISTS fleets (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                name TEXT NOT NULL UNIQUE,
                captain_id TEXT NOT NULL
            )
            """)

    cursor.execute("""
            CREATE TABLE IF NOT EXISTS war_history (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                attacker_id TEXT NOT NULL,
                victim_id TEXT NOT NULL,
                attacker_fleet_id INTEGER NOT NULL,
                victim_fleet_id INTEGER NOT NULL,
                reward INTEGER NOT NULL,
                success INTEGER NOT NULL,
                timestamp TEXT NOT NULL,
                event TEXT NOT NULL
            )
            """)

    connection.commit()
    connection.close()

#Pirate (Player)
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

#Shop
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

def get_shop_item(slot):
    connection = get_con()
    cursor = connection.cursor()
    cursor.execute(
        "SELECT * FROM shop WHERE slot = ?",
        (slot,)
    )
    row = cursor.fetchone()
    connection.close()
    return row

def remove_shop_item(slot):
    connection = get_con()
    cursor = connection.cursor()
    cursor.execute(
        "DELETE FROM shop WHERE slot = ?",
        (slot,)
    )
    connection.commit()
    connection.close()

def restock_shop(slot):
    connection = get_con()
    cursor = connection.cursor()
    cursor.execute(
        "SELECT * FROM shop WHERE slot = 10"
    )
    row = cursor.fetchone()
    if row:
        cursor.execute("""
            UPDATE shop
            SET fruit_id = ?,
                name = ?,
                type = ?,
                price = ?,
                shop_date = ?
            WHERE slot = ?
        """, (
            row[1],
            row[2],
            row[3],
            row[4],
            row[5],
            slot
        ))
        cursor.execute(
            "DELETE FROM shop WHERE slot = 10"
        )
        cursor.execute("""
            UPDATE shop
            SET slot = slot - 1
            WHERE slot > 10
        """)
    connection.commit()
    connection.close()

#Inventory
def add_inventory(user_id,item_id,item_name,item_type,worth):
    connection = get_con()
    cursor = connection.cursor()

    cursor.execute("""
        INSERT INTO inventory
        (user_id,item_id,item_name,item_type,worth)
        VALUES (?,?,?,?,?)
    """, (
        user_id,
        item_id,
        item_name,
        item_type,
        worth
    ))

    connection.commit()
    connection.close()

def get_inventory(user_id):
    connection = get_con()
    cursor = connection.cursor()

    cursor.execute(
        "SELECT * FROM inventory WHERE user_id = ?",
        (user_id,)
    )
    rows = cursor.fetchall()
    connection.close()
    return rows

def get_inventory_item(user_id,item_id):
    connection = get_con()
    cursor = connection.cursor()
    cursor.execute(
        "SELECT * FROM inventory WHERE user_id = ?",
        (user_id,)
    )
    rows = cursor.fetchall()
    connection.close()
    if item_id < 1 or item_id > len(rows):
        return None
    return rows[item_id-1]

def remove_inventory_item(item_id):
    connection = get_con()
    cursor = connection.cursor()
    cursor.execute(
        "DELETE FROM inventory WHERE id = ?",
        (item_id,)
    )
    connection.commit()
    connection.close()

#WorstGen
def get_worst_generation():
    connection = get_con()
    cursor = connection.cursor()
    cursor.execute(
        "SELECT name,berries,rank FROM pirates ORDER BY berries DESC LIMIT 5"
    )
    rows = cursor.fetchall()
    connection.close()
    return rows

#Fleet
def create_fleet(name,captain_id):
    connection = get_con()
    cursor = connection.cursor()
    cursor.execute("""
        INSERT INTO fleets
        (name,captain_id)
        VALUES (?,?)
    """, (
        name,
        captain_id
    ))
    fleet_id = cursor.lastrowid
    connection.commit()
    connection.close()
    return fleet_id

def get_fleet(field,is_id):
    connection = get_con()
    cursor = connection.cursor()
    if is_id:
        cursor.execute(
            "SELECT * FROM fleets WHERE id = ?",
            (field,)
        )
    else:
        cursor.execute(
            "SELECT * FROM fleets WHERE name = ?",
            (field,)
        )
    row = cursor.fetchone()
    connection.close()
    return row

def update_fleet(fleet_id,captain_id):
    connection = get_con()
    cursor = connection.cursor()
    if captain_id is None:
        cursor.execute(
            "UPDATE pirates SET fleet_id = NULL WHERE fleet_id = ?",
            (fleet_id,)
        )
        cursor.execute(
            "DELETE FROM fleets WHERE id = ?",
            (fleet_id,)
        )
    else:
        cursor.execute(
            "UPDATE fleets SET captain_id = ? WHERE id = ?",
            (captain_id,fleet_id)
        )
    connection.commit()
    connection.close()

def get_fleet_members(fleet_id):
    connection = get_con()
    cursor = connection.cursor()
    cursor.execute(
        "SELECT name FROM pirates WHERE fleet_id = ?",
        (fleet_id,)
    )
    rows = cursor.fetchall()
    connection.close()
    return rows

def get_captain(fleet_id):
    connection = get_con()
    cursor = connection.cursor()
    cursor.execute(
        "SELECT captain_id FROM fleets WHERE id = ?",
        (fleet_id,)
    )
    row = cursor.fetchone()
    connection.close()
    if not row:
        return None
    return int(row[0])

def get_fleet_leaderboard(fleet_id,stat):
    connection = get_con()
    cursor = connection.cursor()
    if stat == "bounty":
        cursor.execute(
            "SELECT name,bounty FROM pirates WHERE fleet_id = ? ORDER BY bounty DESC",
            (fleet_id,)
        )
    else:
        cursor.execute(
            "SELECT name,berries FROM pirates WHERE fleet_id = ? ORDER BY berries DESC",
            (fleet_id,)
        )
    rows = cursor.fetchall()
    connection.close()
    return rows

#Wars
def add_war_history(attacker_id,victim_id,attacker_fleet_id,victim_fleet_id,reward,success,timestamp,event):
    connection = get_con()
    cursor = connection.cursor()
    cursor.execute("""
        INSERT INTO war_history
        (attacker_id,victim_id,attacker_fleet_id,victim_fleet_id,reward,success,timestamp,event)
        VALUES (?,?,?,?,?,?,?,?)
    """, (
        attacker_id,
        victim_id,
        attacker_fleet_id,
        victim_fleet_id,
        reward,
        success,
        timestamp,
        event
    ))
    connection.commit()
    connection.close()

def get_war_history(fleet_id):
    connection = get_con()
    cursor = connection.cursor()
    cursor.execute("""
        SELECT * FROM war_history
        WHERE attacker_fleet_id = ? OR victim_fleet_id = ?
        ORDER BY id ASC
    """, (
        fleet_id,
        fleet_id
    ))
    rows = cursor.fetchall()
    connection.close()
    return rows

def are_at_war(fleet1_id,fleet2_id):
    connection = get_con()
    cursor = connection.cursor()
    cursor.execute("""
        SELECT event FROM war_history
        WHERE (
            (attacker_fleet_id = ? AND victim_fleet_id = ?)
            OR
            (attacker_fleet_id = ? AND victim_fleet_id = ?)
        )
        AND event IN ('WAR','END_WAR')
        ORDER BY id DESC
        LIMIT 1
    """, (
        fleet1_id,
        fleet2_id,
        fleet2_id,
        fleet1_id
    ))
    row = cursor.fetchone()
    connection.close()
    if not row:
        return False
    return row[0] == "WAR"
