import random, discord
from datetime import date
from discord.ext import commands
from db import db
from db import data_rep
from commands import api

class Player(commands.Cog):
    def __init__(self,bot):
        self.bot = bot

    @commands.command()
    async def embark(self,ctx):
            user_id = ctx.author.id
            name = ctx.author.display_name
            if db.get_pirate(user_id):
                await ctx.send("You're already a pirate, buddy!")
                return
            pirate = data_rep.Pirate(user_id,name,None)
            db.create_pirate(pirate)
            await ctx.send(
                f"Welcome aboard, {name}!\n"
                f"You've started your journey with {pirate.berries} Berries as a gift!\n"
                f"Claim your daily berries with !setsail"
            )

    @commands.command()
    async def treasury(self,ctx):
        user_id = ctx.author.id
        name = ctx.author.display_name
        pirate = db.get_pirate(user_id)
        if not pirate:
            await ctx.send(f"Start your journey first {name}! Use !embark")
            return
        await ctx.send(f"{name}'s Treasury: {pirate.berries}")

    @commands.command()
    async def bounty(self,ctx):
        user_id = ctx.author.id
        name = ctx.author.display_name
        pirate = db.get_pirate(user_id)
        if not pirate:
            await ctx.send(f"Start your journey first {name}! Use !embark !")
            return
        await ctx.send( f"Bounty: {pirate.bounty}\n"
                        f"Rank: {pirate.rank}\n"
                        f"bounty needed for next Rank: {pirate.next_rank()}"
                      )
        return

    @commands.command()
    async def setsail(self,ctx):
        user_id = ctx.author.id
        name = ctx.author.display_name
        pirate = db.get_pirate(user_id)
        if not pirate:
            await ctx.send(f"Start your journey first {name}! Use !embark")
            return
        today = str(date.today())
        if pirate.last_claim == today:
            await ctx.send("The marines are looking! Its too dangerous to raid innocents now! Come back tomorrow!")
            return
        luck = random.randint(1,4)
        pirate.berries += luck * 500
        pirate.last_claim = today
        db.update_pirate(pirate)
        looted = list(data_rep.LOOTED.keys())[luck-1]
        await ctx.send(f"You've Looted {looted} and gained {data_rep.LOOTED[looted]} berries."
                       f"Use !treasury to check your current berry count!")
        return

    @commands.command()
    async def trade(self,ctx,member: discord.Member,amount: int):
        sender = db.get_pirate(ctx.author.id)
        receiver = db.get_pirate(member.id)
        today = str(date.today())
        if not sender:
            await ctx.send(f"Start your journey first {ctx.author.display_name}! Use !embark")
            return
        if not receiver:
            await ctx.send(f"{member.display_name} hasn't embarked yet!")
            return

        if amount <= 0:
            await ctx.send("You can't send that amount!")
            return
        if sender.berries < amount:
            await ctx.send("You don't have the required amount of berries!")
            if today != sender.last_claim:
                await ctx.send(f"{sender.name} can claim today's berries through !setsail")
            return

        sender.berries -= amount
        receiver.berries += amount

        db.update_pirate(sender)
        db.update_pirate(receiver)

        await ctx.send(
            f"{sender.name} traded {amount} Berries "
            f"to {receiver.name}!"
        )

    @commands.command()
    async def logpose(self,ctx):
        user_id = ctx.author.id
        pirate = db.get_pirate(user_id)
        if not pirate:
            await ctx.send(f"Start your journey first {ctx.author.display_name}! Use !embark")
            return
        fact_type = random.randint(1,3)
        match fact_type:
            case 1:
                data = await api.get_random("characters")
                await ctx.send(data_rep.character_str(data))
            case 2:
                data = await api.get_random("devil-fruits")
                await ctx.send(data_rep.devil_fruit_str(data))
            case 3:
                data = await api.get_random("islands")
                await ctx.send(data_rep.island_str(data))

    @commands.command()
    async def inventory(self,ctx):
        pirate = db.get_pirate(ctx.author.id)
        if not pirate:
            await ctx.send(f"Start your journey first {ctx.author.display_name}! Use !embark ")
            return
        inventory = db.get_inventory(ctx.author.id)
        if not inventory:
            await ctx.send("Your inventory is empty!")
            return
        message = "Inventory:\n"
        for i,item in enumerate(inventory):
            message += f"{i+1}.{item[3]} - {item[5]} Berries\n"
        await ctx.send(message)

    @commands.command()
    async def sell(self,ctx,item_id: int):
        pirate = db.get_pirate(ctx.author.id)
        if not pirate:
            await ctx.send(f"Start your journey first {ctx.author.display_name}! Use !embark ")
            return
        item = db.get_inventory_item(ctx.author.id,item_id)
        if not item:
            await ctx.send("That item isn't in your inventory!")
            return
        worth = int(item[5] * 0.7)
        pirate.berries += worth
        db.update_pirate(pirate)
        db.remove_inventory_item(item[0])
        await ctx.send(f"You sold {item[3]} for {worth} Berries!")

    @commands.command()
    async def eat(self,ctx,item_id: int):
        pirate = db.get_pirate(ctx.author.id)
        if not pirate:
            await ctx.send(f"Start your journey first {ctx.author.display_name}! Use !embark ")
            return
        item = db.get_inventory_item(ctx.author.id,item_id)
        if not item:
            await ctx.send("That item isn't in your inventory!")
            return
        pirate.devil_fruit_type = item[4]
        db.update_pirate(pirate)
        db.remove_inventory_item(item[0])
        await ctx.send(f"You ate the {item[3]}!")

    @commands.command()
    async def fruit_power(self,ctx):
        pirate = db.get_pirate(ctx.author.id)
        if not pirate:
            await ctx.send(f"Start your journey first {ctx.author.display_name}! Use !embark ")
            return
        if not pirate.devil_fruit_type:
            await ctx.send("You haven't eaten a Devil Fruit yet!")
            return
        match pirate.devil_fruit_type:
            case "Paramecia":
                await ctx.send(
                    "Paramecia\n"
                    "Grants the user a unique supernatural ability.\n"
                    "Raid Bonus: Chance to multiply your raid reward."
                )
            case "Zoan":
                await ctx.send(
                    "Zoan\n"
                    "Allows the user to transform into an animal or hybrid form.\n"
                    "Raid Bonus: Provides a modest return but has a higher chance of success."
                )
            case "Logia":
                await ctx.send(
                    "Logia\n"
                    "Allows the user to create, control and transform into a natural element.\n"
                    "Raid Bonus: Chance to steal the opponent's Devil Fruit."
                )

    @commands.command()
    async def worstgeneration(self,ctx):
        worst_gen = db.get_worst_generation()
        if not worst_gen:
            await ctx.send("There are no Pirates sailing the seas!")
            return
        await ctx.send("The Worst Generation: ")
        for i,pirate in enumerate(worst_gen):
            await ctx.send(f"{i+1}. {pirate[0]} - {pirate[1]} Berries - {pirate[2]}\n")

    @commands.command()
    async def raid(self,ctx,member: discord.Member):
        attacker = db.get_pirate(ctx.author.id)
        victim = db.get_pirate(member.id)
        if not attacker:
            await ctx.send(f"Start your journey first {ctx.author.display_name}! Use !embark ")
            return False, 0
        if not victim:
            await ctx.send(f"{member.display_name} hasn't embarked yet!")
            return False, 0

        if victim.berries == 0:
            await ctx.send(f"{victim.name} has no berries left to be raided!")
            return False, 0
        if victim == attacker:
            await ctx.send("You can't raid yourself!")
            return False, 0

        take = 0.4
        chance = 0.5
        bounty_rate = 0.4
        multiplier = False
        steal = False
        stolen_fruit = ""
        match attacker.devil_fruit_type:
            case "Paramecia":
                take = 0.35
                chance = 0.4
                bounty_rate = 0.5
                if random.random() < 0.45:
                    take = 0.7
                    multiplier = True
            case "Zoan":
                take = 0.3
                chance = 0.7
                bounty_rate = 0.85
            case "Logia":
                take = 0.35
                chance = 0.4
                bounty_rate = 0.6
        if random.random() > chance:
            await ctx.send(f"{attacker.name}'s raid against {victim.name} failed!")
            return False, 0
        reward = int(victim.berries * take)
        bounty = int(reward * bounty_rate)
        victim.berries -= reward
        attacker.berries += reward
        attacker.bounty += bounty
        attacker.update_rank()
        if attacker.devil_fruit_type == "Logia":
            if random.random() < 0.2:
                inventory = db.get_inventory(victim.user_id)
                if inventory:
                    fruit = random.choice(inventory)
                    db.remove_inventory_item(fruit[0])
                    db.add_inventory(attacker.user_id,fruit[2],fruit[3],fruit[4],fruit[5])
                    stolen_fruit = fruit[3]
                    steal = True
        db.update_pirate(attacker)
        db.update_pirate(victim)
        message = f"{attacker.name} raided {victim.name} and took {reward} Berries! {attacker.name} gained {bounty} Bounty."
        if multiplier:
            message += " Paramecia multiplier activated!"
        if steal:
            message += f" {attacker.name} stole {stolen_fruit}!"
        await ctx.send(message)
        return True, reward


async def setup(bot):
    await bot.add_cog(Player(bot))
    print("PLAYER LOADED")
