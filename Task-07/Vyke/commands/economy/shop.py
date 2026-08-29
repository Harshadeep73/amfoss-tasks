import random
from datetime import date
from discord.ext import commands
from commands.api import api
from db import db

class Shop(commands.Cog):
    def __init__(self,bot):
        self.bot = bot

    @commands.command()
    async def shop(self,ctx):
        today = str(date.today())
        stock = db.get_shop()
        print("SHOP COMMAND")
        if not stock or stock[0][5] != today:
            fruits = await api.get_data("devil-fruits")
            if not fruits:
                await ctx.send("The shop is empty right now!")
                return
            new_stock = random.sample(fruits,9)
            for fruit in new_stock:
                fruit["price"] = random.randint(1,10) * 10000
                fruit["shop_date"] = today
            db.update_shop(new_stock)
            stock = db.get_shop()
        names = []
        types = []
        prices = []
        for fruit in stock:
            names.append(fruit[2])
            types.append(fruit[3])
            prices.append(fruit[4])
        await ctx.send(
            "```"
            f"{names[0]:<25}{names[1]:<25}{names[2]:<25}\n"
            f"{types[0]:<25}{types[1]:<25}{types[2]:<25}\n"
            f"{prices[0]:<25}{prices[1]:<25}{prices[2]:<25}\n\n"

            f"{names[3]:<25}{names[4]:<25}{names[5]:<25}\n"
            f"{types[3]:<25}{types[4]:<25}{types[5]:<25}\n"
            f"{prices[3]:<25}{prices[4]:<25}{prices[5]:<25}\n\n"

            f"{names[6]:<25}{names[7]:<25}{names[8]:<25}\n"
            f"{types[6]:<25}{types[7]:<25}{types[8]:<25}\n"
            f"{prices[6]:<25}{prices[7]:<25}{prices[8]:<25}"
            "```"
        )

async def setup(bot):
    await bot.add_cog(Shop(bot))
    print("SHOP LOADED")
