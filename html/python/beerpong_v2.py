#!/usr/bin/env python
## Beerberry V0.1
##
## Raspberry side
## Runs the game, collects data from cuprecognition and sends commands to the matrix


## IMPOTS
import serial, sys, time, datetime, sqlite3

version = 0.3

print("PySerial Version: "+serial.VERSION)
print("Beerpong.py Version: "+ str(version) + "\n\n")


## GLOBAL VAR

beerpong_running = True
gamepoints = 10


## CLASSES
class Player:
    def __init__(self, name="player", port="unassigned", flag="unassigned"):
        self.name = name
        self.flag = flag
        self.score = 0
        self.port = port
        self.ser = serial.Serial(self.port, 9600)
        self.prev_score = 0
    
    def get_score(self):
        self.ser.flushInput()
        self.ser.write('?'.encode())
        try:
            self.score = int(self.ser.readline().strip())
        except:
            self.score = self.ser.readline().strip()
            print("ERROR parsing score into integer!")

        self.score = gamepoints - self.score


class Matrix:
    def __init__(self, port ="/dev/ttyACM0"):
        try:
            self.port = port
            self.ser = serial.Serial(self.port, 38400)
        except:
            self.port = "/dev/ttyACM1"
            self.ser = serial.Serial(self.port, 38400)

    def send_score(self, score_p1, score_p2):
        self.ser.flushOutput()
        self.ser.flushInput()
        self.ser.write( "{}:{}#".format(score_p1, score_p2).encode() )
        self.ser.flushOutput()

    def start_game(self):
        self.ser.write('start;'.encode())
        self.ser.flushOutput()

    def end_game(self):
        self.ser.write('end;'.encode())
        self.ser.flushOutput()

    def terminate_game(self):
        self.ser.write('terminate;'.encode())
        self.ser.flushOutput()

        

    def set_playernames(self, p1name, p2name):
        self.ser.flushOutput()
        self.ser.flushInput()
        string = p1name.upper() + "$" + p2name.upper() + "%"
        
        self.ser.write(string.encode())
        self.ser.flushOutput()

        


        
        


## METHODS

def get_ids():
    ports = ['/dev/ttyUSB0', '/dev/ttyUSB1', '/dev/ttyUSB2', "/dev/ttyUSB3", "/dev/ttyUSB4", "/dev/ttyUSB5"]
    for port in ports:
        print("Try port: " + port)
        try:
            ser = serial.Serial(port, 9600)
            time.sleep(1)
            ser.flushInput()
            ser.write('i'.encode())
            ser.flushOutput()
            id = ser.readline().strip().decode()
            print("got id: "+str(id))
            if id == "p2":
                global player1port
                player1port = port
                continue
            if id == "p1":
                global player2port
                player2port = port
                continue                
        except Exception as error:
            print(error)
            print("no known device")


def beerpong():

## create the game ID
    global game_id
    starttime = time.time()
    game_id = '{:%Y-%m-%d_%H:%M:%S}'.format(datetime.datetime.now())
    print("Game ID: "+game_id)
    print('Player 1: ' + player1.name )
    print('Player 2: ' + player2.name )

## connect to database beerpong.db and write new entry
    conn = sqlite3.connect('/var/www/html/database/beerpong.db')
    c = conn.cursor()
    query = 'INSERT INTO game ("id", "playerone", "playertwo", "team") VALUES ("' + game_id + '", "' + str(player1.name) + '", "' + str(player2.name) + '", "' + str(sys.argv[3]) + '")'
    c.execute(query)
    conn.commit()    

## reset scores and matrix
    matrix.end_game()
    matrix.start_game()
    matrix.send_score(player1.score, player2.score)
    matrix.set_playernames(player1.name, player2.name)

## start logfile
    logfilename = "/var/www/html/gamelog/" + game_id
    log = open(logfilename, "w") 
    log.write(player1.name + "\n" + player2.name + "\n" )
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
                timecode = time.time()-starttime
                write_log(logfilename ,str(int(timecode)), player1.score, player2.score)
                player1.prev_score = player1.score

            if player2.score != player2.prev_score:
                matrix.send_score(player1.score, player2.score)
                print(player2.name +" scored! New Score: " + str(player1.score) +" : " + str(player2.score))
                timecode = time.time()-starttime
                write_log(logfilename ,str(int(timecode)), player1.score, player2.score)
                player2.prev_score = player2.score

            


        if int(player1.score) >= 10 or int(player2.score) >= 10:
## Eine Seite hat keine Becher mehr
            try:
                query = 'UPDATE game SET playeronescore = ' + str(player1.score) + ', playertwoscore = ' + str(player2.score) + ' WHERE id = "'+game_id +'"'
                print(query)
                c.execute(query)
                conn.commit() 
                conn.close()
            except:
                print("DB connection could not be closed")
            end_game()
    

        time.sleep(0.05)
        
def write_log(logfile, timecode, score1, score2):
    log = open(str(logfile), "a")
    log.write(timecode + "," + str(score1) + ":" + str(score2) + "\n")
    log.close()

def end_game():
    matrix.end_game()  
    print("Game " + game_id + " with players " + player1.name + " and " + player2.name + " ended.")
    global beerpong_running
    beerpong_running = False


## SETUP
##player1port = "unassigned"
##player2port = "unassigned"
get_ids()



try: 
    player1 = Player(str(sys.argv[1]), player1port)
    print("player 1 initialized on port " + player1port)
except Exception as ERR:
    print(ERR)
    print("player 1 could not be initialized.")

try:
    player2 = Player(str(sys.argv[2]), player2port)
    print("player 2 initialized on port " + player2port)
except:
    print("player 2 could not be initialized.")
    
try:
    matrix = Matrix()
    print("matrix initialized on port " + matrix.port)
except:
    print("matrix could not be initialized.")


## MAIN Loop


if __name__ == "__main__":
    try:
        print("\n \nNew Game starts now!")
        beerpong()
    except (KeyboardInterrupt, SystemExit):
        matrix.end_game()
        
    
