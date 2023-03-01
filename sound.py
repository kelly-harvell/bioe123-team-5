import winsound


def main():
    print("Cycle Finished!")
    winsound.Beep(880,800)
    winsound.Beep(880, 300)
    winsound.Beep(880, 300)
    winsound.Beep(880, 800)

if __name__ == '__main__':
    main()

