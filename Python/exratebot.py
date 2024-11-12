import discord
import os
import requests
import json
import time
from keep_alive import keep_alive

client = discord.Client()

apikey = os.environ['EXRATETOKEN']

@client.event
async def on_ready():
  print('Logged in as {0.user}'.format(client))
  channel = client.get_channel(935979688730435648)

  async def send_exrate():
    response = requests.get(apikey)
    json_data = json.loads(response.text)

    embedMsg = discord.Embed()
    embedMsg.add_field(name = "US Dollars", value = str(1/json_data['data']['USD'])[0:6])
    embedMsg.add_field(name = "Euro", value = str(1/json_data['data']['EUR'])[0:6])
    embedMsg.add_field(name = "Pound Sterling", value = str(1/json_data['data']['GBP'])[0:6])
    embedMsg.title = "**Exchange Rate**"
    embedMsg.colour = 0x00FF00

    await channel.send(embed=embedMsg)
  
  await send_exrate()

  while True:
    time.sleep(300)
    await send_exrate()

@client.event
async def on_message(message):
  if message.author == client.user:
    return

keep_alive()
client.run(os.environ['TOKEN'])
