from game import Game
import os

def main():
  # Ensure root
  if os.geteuid() != 0:
    print("You must run this program as root.")
    return

  game = Game()
  game.run()

if __name__ == "__main__":
  main()
