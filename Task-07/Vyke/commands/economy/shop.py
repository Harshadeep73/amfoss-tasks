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
        pirate = db.get_pirate(ctx.author.id)
        if not pirate:
            await ctx.send(f"Start your journey first {ctx.author.display_name}! Use !embark ")
            return
        today = str(date.today())
        stock = db.get_shop()
        print("SHOP COMMAND")
        if not stock or stock[0][5] != today:
            fruits = await api.get_data("devil-fruits")
            if not fruits:
                await ctx.send("The shop is empty right now!")
                return
            new_stock = random.sample(fruits,18)
            for fruit in new_stock:
                fruit["price"] = random.randint(1,10) * 100
                fruit["shop_date"] = today
            db.update_shop(new_stock)
            stock = db.get_shop()

        slots = []
        names = []
        types = []
        prices = []

        for fruit in stock[:9]:
            slots.append(fruit[0])
            names.append(fruit[2])
            types.append(fruit[3])
            prices.append(fruit[4])

        await ctx.send(
            "```"
            f"{slots[0]:<25}{slots[1]:<25}{slots[2]:<25}\n"
            f"{names[0]:<25}{names[1]:<25}{names[2]:<25}\n"
            f"{types[0]:<25}{types[1]:<25}{types[2]:<25}\n"
            f"{prices[0]:<25}{prices[1]:<25}{prices[2]:<25}\n\n"

            f"{slots[3]:<25}{slots[4]:<25}{slots[5]:<25}\n"
            f"{names[3]:<25}{names[4]:<25}{names[5]:<25}\n"
            f"{types[3]:<25}{types[4]:<25}{types[5]:<25}\n"
            f"{prices[3]:<25}{prices[4]:<25}{prices[5]:<25}\n\n"

            f"{slots[6]:<25}{slots[7]:<25}{slots[8]:<25}\n"
            f"{names[6]:<25}{names[7]:<25}{names[8]:<25}\n"
            f"{types[6]:<25}{types[7]:<25}{types[8]:<25}\n"
            f"{prices[6]:<25}{prices[7]:<25}{prices[8]:<25}"
            "```"
        )

    @commands.command()
    async def buy(self,ctx,slot: int):
        pirate = db.get_pirate(ctx.author.id)
        if not pirate:
            await ctx.send(f"Start your journey first {ctx.author.display_name}! Use !embark ")
            return
        if slot < 1 or slot > 9:
            await ctx.send("That fruit isn't available right now!")
            return
        item = db.get_shop_item(slot)
        if not item:
            await ctx.send("That fruit isn't in the shop!")
            return
        if pirate.berries < item[4]:
            await ctx.send("You don't have enough Berries!")
            return
        pirate.berries -= item[4]
        db.update_pirate(pirate)
        db.add_inventory(pirate.user_id,item[1],item[2],item[3],item[4])
        db.restock_shop(slot)
        await ctx.send(f"You bought {item[2]} for {item[4]} Berries!")


async def setup(bot):
    await bot.add_cog(Shop(bot))
    print("SHOP LOADED")
