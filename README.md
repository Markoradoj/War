# War
War card game in C



 At the beginning of the program the user introduces three options, 1. start game 2, load game
 3. exit program, When I press 1, it will prompt to enter 2 to 4 players, if I type it out of the
 integer bounds, then it will trigger an error. After I type the correct amount, we get to see the
 players cards. These cards range from 13 from each of the player and is randomized each time the
 program starts. The 13 cars from the array at the start will add 2 because 2 is the minimum value 
  while ace (14) is the highest. The fucntion will also check that each of the suit (ace, queen)
 maximum is 4. Each time player picks a card, the function will pick up and stores that number but 
 if any player has the highest card, it will check the flag winner and the player wins the match.
 If there's a player thats got 2 same cards, its a tie and the points will cancel. After it, 
 the program will prompt a save function, if hit "Y" it will save called save.txt and it shows
 the players hands in that text file and it will also load on that last match. The loop lasts around
 13 rounds, after that the flag will check which player has the highest amount of points scored.
 For loading the game, the way its being updated is that it uses a pointer to ensure that we are getting
 its true valuefo everything.
  
