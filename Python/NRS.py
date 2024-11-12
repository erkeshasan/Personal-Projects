# 27/01/2022

from random import randrange
from threading import Thread
import os
import sys
import math
import time

def clear():
    os.system('clear')

def is_number(x):
  try:
    int(x)
    return True
  except:
    return False

coreTemps = [300, 300, 300, 300] # Kelvin
coreFuels = [100, 100, 100, 100] # Remaining Percent
coreRods = [0, 0, 0, 0] # Raised Percent
coreIntegrity = [100, 100, 100, 100] # Percent

totalScore = 0 # Electricity Generated (Megawatts)
points = 0 # Electricity that will be sold
money = 20 # Million USD

def attemptRodPercent(percent, core):
  if percent == coreRods[core-1]:
    print("Nothing has changed.")
  elif percent > coreRods[core-1]:
    print("Attempting to raise control rods of Core #" + str(core) + ".")
    time.sleep(randrange(1,3))
    coreRods[core-1] = percent
    print("Successfully raised control rods of Core #" + str(core) + " to " + str(percent) + "%.")
  else:
    print("Attempting to lower control rods of Core #" + str(core) + ".")
    time.sleep(randrange(1,3))
    if randrange(math.floor(coreTemps[core-1]) - 1000, math.floor(coreTemps[core-1])) < 2000 and coreIntegrity[core-1] > 50:
      coreRods[core-1] = percent
      print("Successfully lowered control rods of Core #" + str(core) + " to " + str(percent) + "%.")
    else:
      print("WARNING! Control Rods of Core #" + str(core) + " could not be lowered due to a malfunction! Structural Integrity might be damaged.")

reactorOperational = True

def game_over(msg):
  global reactorOperational
  reactorOperational = False
  clear()
  print("\n\033[1;31;40m Game Over \n  " + msg)
  print("\n\033[1;36;40m Statistics: \n  Money: " + str(money) + " Million $ \n  Total Electricity Generated: "+ str(totalScore) + " Megawatts ")
  print("\n\033[1;37;40m Restart by running this replit again. ")
  sys.exit()

def printCmds():
  print("Commands:\n  Help\n  Stats\n  Integ\n  Shutdown {1, 2, 3, 4}(Optional)\n  Rods {0-100} {1, 2, 3, 4}(Optional)\n  Fuel {1, 2, 3, 4}(Optional)\n  Fix {1, 2, 3, 4}(Optional)")

def userInput():
  global reactorOperational
  global totalScore
  global points
  global money

  while reactorOperational:
    take = input("\nControl Desk: ").split(" ")
    parameter1 = None
    parameter2 = None
    
    if len(take) == 2:
      parameter1 = take[1]
    elif len(take) == 3:
      parameter1 = take[1]
      parameter2 = take[2]
    
    cmd = take[0]
    
    if cmd.lower() == "help":
      clear()
      print("\nThis is a very simplified version of a generic nuclear fission reactor facility. Not realistic but oh well.")
      print("Uppercase/Lowercase does not matter for commands.")
      print("\nCommands:")
      print("  Help: Explain commands and values.")
      print("\n  Stats: Shows important values.")
      print("\n  Integ: Hire engineers to conduct an integrity check on the nuclear power plant. Takes a bit of time.")
      print("\n  Shutdown: Lowers control rods to 0% as fast as possible. Don't specify core number to shut down all cores.")
      print("\n  Rods: Lower or raise control rods to a specified percentage. Don't specify core number to affect all cores.")
      print("\n  Fuel: Replace depleted fuel cells of a core. Don't specify core number to replace all fuel cells. 5 Million $ per fuel cell.")
      print("\n  Fix: Fix damaged structure of a core. Don't specify core number to fix all of them. Each 5 percent of missing integrity costs 1 Million $ to fix. (Min: 4 Million $ for attempt cost.)")
      print("\nValues you need to know:")
      print("\n  Core Temperatures: Current temperature of a Reactor Core. To generate electricity, core temperature needs to be atleast 600 Kelvin. You will generate more electricity and earn more money as the temperature increases. Be careful: Reactor cores will start malfunctioning and damaging integrity after 2000 Kelvin and meltdown will be unavoidable at 3000 Kelvin. (Your chances of successfully lowering control rods will start to decrease after 2000 Kelvin.)")
      print("\n  Core Fuel Percent: Percentage of fuel remaining inside a core. You should buy replacements if they deplete. If fuel percentage goes below 20%, the efficiency of the core will decrease.")
      print("\n  Core Control Rods: Neutron absorbing material that slows down fission as it gets inserted into the reactor core. Core Integrity needs to be higher than 50% to be able to successfully lower conrol rods.")
      print("\n  Core Structural Integrity: Reliableness of the reactor core, affects how much electricity a core makes. If it's damaged, fix it by paying a maintenance team.")
      print("\n  Money: Funds of the facility. If it goes below -10 Million $, the facility will be bankrupt. You earn 1 Million $ for each 4500 Megawatts of electricity you sell.")
    elif cmd.lower() == "stats":
      clear()
      for core in range(len(coreTemps)):
        print("\n  Core #" + str(core+1) + " Temperature: " + str(coreTemps[core]) + " Kelvin")
        print("  Core #" + str(core+1) + " Fuel: " + str(coreFuels[core])[0:4] + "% Remaining")
        print("  Core #" + str(core+1) + " Control Rods: " + str(coreRods[core]) + "% Raised")
      print("\n  Total Electricity Generated: " + str(totalScore) + " Megawatts")
      print("  Pending Electricity: " + str(points) + " Megawatts")
      print("\n  Money: " + str(money) + " Million $")
    elif cmd.lower() == "shutdown":
      clear()
      if parameter1:
        if parameter1 in ["1", "2", "3", "4"]:
          print("Attempting to shutdown Core #" + str(core) + ".")
          attemptRodPercent(0, core-1)
        else:
          print("Invalid Core: Must be 1, 2, 3 or 4.")
      else:
        print("Attempting to shutdown all reactor cores.")
        attemptRodPercent(0, 1)
        attemptRodPercent(0, 2)
        attemptRodPercent(0, 3)
        attemptRodPercent(0, 4)
    elif cmd.lower() == "rods":
      if parameter2:
        if parameter2 in ["1", "2", "3", "4"]:
          if is_number(parameter1):
            if int(parameter1) >= 0 and int(parameter1) <= 100:
              clear()
              attemptRodPercent(int(parameter1), int(parameter2))
            else:
              print("Invalid Percentage: Must be between 0 and 100.")
          else:
            print("Invalid Percentage: Must be a number.")
        else:
          print("Invalid Core: Must be 1, 2, 3 or 4.")
      else:
        if is_number(parameter1):
          if int(parameter1) >= 0 and int(parameter1) <= 100:
            clear()
            attemptRodPercent(int(parameter1), 1)
            time.sleep(1)
            attemptRodPercent(int(parameter1), 2)
            time.sleep(1)
            attemptRodPercent(int(parameter1), 3)
            time.sleep(1)
            attemptRodPercent(int(parameter1), 4)
          else:
            print("Invalid Percentage: Must be between 0 and 100.")
        else:
          print("Invalid Percentage: Must be a number.")
    elif cmd.lower() == "integ":
      clear()
      print("Conducting an integrity check...")
      time.sleep(randrange(2,3))
      for core in range(len(coreTemps)):
        print("\n  Core #" + str(core+1) + " Structural Integrity: " + str(coreIntegrity[core]) + "%")
    elif cmd.lower() == "fuel":
      clear()
      print("Ordering nuclear reactor fuel from ebay...")
      if parameter1:
        if parameter1 in ["1", "2", "3", "4"]:
          time.sleep(randrange(1,2))
          coreFuels[int(parameter1)-1] = 100
          money = money - 5
          print("Core #"+parameter1+" has been refueled!")
        else:
          print("Invalid Core: Must be 1, 2, 3 or 4.")
      else:
        time.sleep(randrange(2,4))
        for core in range(len(coreTemps)):
          coreFuels[core] = 100
          money = money - 5
        print("All reactor cores are refueled!")
    elif cmd.lower() == "fix":
      clear()
      print("Patching up reactor cores...")
      if parameter1:
        if parameter1 in ["1", "2", "3", "4"]:
          core = int(parameter1)-1
          time.sleep(randrange(2,3))
          cost = max(1, 20 - math.floor(coreIntegrity[core] / 5))
          money = money - cost
          coreIntegrity[core] = 100
          print("Structure of Core #"+parameter1+" has been fixed!\n  This operation cost: "+str(cost)+" Million $")
        else:
          print("Invalid Core: Must be 1, 2, 3 or 4.")
      else:
        time.sleep(randrange(3,5))
        totalcost = 0
        for core in range(len(coreTemps)):
          cost = max(1, 20 - math.floor(coreIntegrity[core] / 5))
          money = money - cost
          totalcost = totalcost + cost
          coreIntegrity[core] = 100
        print("Structure of all reactor cores have been fixed!\n  This operation cost: "+str(totalcost)+" Million $")
    else:
      print("\nUnknown command: " + cmd +"\n")
      printCmds()

thread1 = Thread(target=userInput)
thread1.daemon = True

printCmds()

thread1.start()

while reactorOperational:
  time.sleep(3)

  if money < -10:
    reactorOperational = False
    game_over("You have bankrupt the facility. ")

  for core in range(len(coreTemps)):
    tempMult = coreRods[core]

    if coreFuels[core] < 20:
      tempMult = coreRods[core] * (coreFuels[core] / 20)
    
    coreTemps[core] = max(300, math.ceil(coreTemps[core] + (coreTemps[core] / 300) * (25 * ((tempMult - 50) / 50)) + 25))
    if coreTemps[core] > 4000:
      game_over("Core #" + str(core+1) + " has undergone nuclear meltdown. ")
    else:
      coreIntegrity[core] = max(0, coreIntegrity[core] - max(0, math.floor((coreTemps[core] - 2000) / 100)))
      coreFuels[core] = max(0, coreFuels[core] - max(0, ((coreTemps[core] - 200) / 1000)))
      if coreTemps[core] > 600:
        pointsIncrement = math.ceil((coreIntegrity[core] / 100) * (coreTemps[core] / 2))
        points = points + pointsIncrement
        totalScore = totalScore + pointsIncrement
    
  money = money + (points // 4500)
  points = points % 4500
