## Beerberry V0.4
## standalone version for quick games
## Raspberry side
## Runs the game, collects data from cuprecognition and sends commands to the matrix


## IMPOTS
import serial
import time
import serial.tools as serial_tools

version = 0.4

print("PySerial Version: "+serial.VERSION)
print("Beerpong.py Version: "+ str(version) + "\n\n")
print("Python Version " + sys.version)

## GLOBAL VAR

beerpong_running = True


## CLASSES
class Player:
    def __init__(self, ser, name="player"):
        self.name = name
        self.score = 0
        self.ser = ser
        self.prev_score = 0
    
    def get_score(self):
        self.ser.reset_input_buffer()
        self.ser.write(b'?')
        self.score = int(self.ser.readline().strip().decode('ascii'))

    def reset_score(self):
        self.ser.write(b'X')
        self.get_score()

class Matrix:
    def __init__(self, ser):
        self.ser = ser

    def send_score(self, score_p1, score_p2):
        self.ser.reset_output_buffer()
        self.ser.reset_input_buffer()
        self.ser.write("{}:{}".format(score_p1, score_p2))
        self.ser.write(b'!')
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

def init_ser():
    ports = serial_tools.list_ports
    sers = {}
    for port in ports: 
        print('found: {}'.format(port.device))





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
    
    try: 
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
                beerpong_running = end_game()

            if int(player2.score) >= 10:
                print(player2.name + "'s score >= 10")
                beerpong_running = end_game()

        
            time.sleep(0.05)
        except KeyboardInterrupt:
            print('KeyboardInterrupt sent.')
            beerpong_running = False


def write_log(logfile, score1, score2):
    log = open(logfile, "a")
    log.write("{:%Y-%m-%d_%H:%M:%S }".format(datetime.datetime.now()) + str(score1) + ":" + str(score2) + "\n")
    log.close()

def end_game():
    player1.reset_score()
    player2.reset_score()
    print("Game " + game_id + " with players " + player1.name + " and " + player2.name + " ended.")
    return False


## MAIN Loop


if __name__ == "__main__":
    print("\n \nNew Game starts now!")
    # beerpong()
    init_ser()

        
    