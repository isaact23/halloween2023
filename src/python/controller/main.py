from game import Game
import argparse, os

def get_args():
  parser = argparse.ArgumentParser(
    prog="Halloween 2023",
    description="Python controller for Halloween 2023 project"
  )
  parser.add_argument('-a', '--audioDir')
  
  args = parser.parse_args()
  return args

def main():
  # Ensure root
  if os.geteuid() != 0:
    print("You must run this program as root.")
    return

  args = get_args()
  game = Game(args)
  game.run()

if __name__ == "__main__":
  main()
