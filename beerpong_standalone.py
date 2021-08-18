## Beerberry V0.4
## standalone version for quick games
## Raspberry side
## Runs the game, collects data from cuprecognition and sends commands to the matrix


## IMPOTS
import serial, sys, time, datetime

version = 0.4

print("PySerial Version: "+serial.VERSION)
print("Beerpong.py Version: "+ str(version) + "\n\n")
print("Python Version " + sys.version)

## GLOBAL VAR

beerpong_running = True


## CLASSES
class Player:
    def __init__(self, name="player", port="unassigned"):
        self.name = name
        self.score = 0
        self.port = port
        self.ser = serial.Serial(self.port, 9600)
        self.prev_score = 0
    
    def get_score(self):
        self.ser.reset_input_buffer()
        self.ser.write('?')
        try:
            self.score = int(self.ser.readline().strip())
        except:
            self.score = self.ser.readline().strip()
            print("ERROR parsing score into integer!")

    def reset_score(self):
        self.ser.write('X')
        self.get_score()

class Matrix:
    def __init__(self, port ="unassigned"):
        self.port = port
        self.ser = serial.Serial(self.port, 9600, timeout=5)

    def send_score(self, score_p1, score_p2):
        self.ser.reset_output_buffer()
        self.ser.reset_input_buffer()
        self.ser.write("{}:{}".format(score_p1, score_p2) )
##        time.sleep(0.1)
        self.ser.write('!')
        self.ser.reset_output_buffer()



        
        


## METHODS

def get_ids():
    ports = ["/dev/ttyUSB0", "/dev/ttyUSB1", "/dev/ttyUSB2", "/dev/ttyUSB3", "/dev/ttyUSB4", "/dev/ttyUSB5", "/dev/ttyACM0"]
    for port in ports:
        print("Try port: " + port)
        try:
            ser = serial.Serial(port, 9600, timeout=5)
            time.sleep(1)
            ser.reset_input_buffer()
            ser.write('i')
            ser.reset_output_buffer()
            id = ser.readline().strip()
            print("got id: "+id)
            if id == "p1":
                global player1port
                player1port = port
                continue
            if id == "p2":
                global player2port
                player2port = port
                continue
            if id == "ma":
                global matrixport
                matrixport = port
                continue

                
        except:
            print("no known device")



def beerpong():

## create the game ID
    global game_id
    game_id = '{:%Y-%m-%d_%H:%M:%S}'.format(datetime.datetime.now())
    print("Game ID: "+game_id)
    print('Player 1: ' + player1.name )
    print('Player 2: ' + player2.name )

## reset scores and matrix
    player1.reset_score()
    player2.reset_score()
    matrix.send_score(player1.score, player2.score)

## start logfile
    logfilename = "./logs/" + game_id + ".log"
    log = open(logfilename, "w") 
    log.write(game_id + "\n" + player1.name + "\n" + player2.name + "\n" )
    log.close()
    
    while beerpong_running:
## check scores
        player1.get_score()
        player2.get_score()

        
        if player1.score != player1.prev_score or player2.score != player2.prev_score:
## player score changed
        
        
            if player1.score != player1.prev_score:
                matrix.send_score(player1.score, player2.score)
                print(player1.name +" scored! New Score: " + str(player1.score) +" : " + str(player2.score))
                write_log(logfilename, player1.score, player2.score)
                player1.prev_score = player1.score

            if player2.score != player2.prev_score:
                matrix.send_score(player1.score, player2.score)
                print(player2.name +" scored! New Score: " + str(player1.score) +" : " + str(player2.score))
                write_log(logfilename, player1.score, player2.score)
                player2.prev_score = player2.score

            


        if int(player1.score) >= 10:
            print(player1.name + "'s score >= 10")
            end_game()

        if int(player2.score) >= 10:
            print(player2.name + "'s score >= 10")
            end_game()

    
        time.sleep(0.05)
        
def write_log(logfile, score1, score2):
    log = open(logfile, "a")
    log.write("{:%Y-%m-%d_%H:%M:%S }".format(datetime.datetime.now()) + str(score1) + ":" + str(score2) + "\n")
    log.close()

def end_game():
    player1.reset_score()
    player2.reset_score()
    print("Game " + game_id + " with players " + player1.name + " and " + player2.name + " ended.")
    global beerpong_running
    beerpong_running = False




## SETUP
player1port = "unassigned"
player2port = "unassigned"
matrixport = "unassigned"
get_ids()



try: 
    player1 = Player(str(sys.argv[1]), player1port)
    print("player 1 initialized on port " + player1port)
except:
    print("player 1 could not be initialized.")

try:
    player2 = Player(str(sys.argv[2]), player2port)
    print("player 2 initialized on port " + player2port)
except:
    print("player 2 could not be initialized.")
    
try:
    matrix = Matrix(matrixport)
    print("matrix initialized on port " + matrixport)
except:
    print("matrix could not be initialized.")


## MAIN Loop


if __name__ == "__main__":
    print("\n \nNew Game starts now!")
    beerpong()

        
    