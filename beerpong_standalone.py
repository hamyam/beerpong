## Beerberry V0.4
## standalone version for quick games
## Raspberry side
## Runs the game, collects data from cuprecognition and sends commands to the matrix


## IMPOTS
import serial
import time
import serial.tools.list_ports as serial_tools
import sys
import datetime

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
		self.prev_cups = 10
		self.cups = 10
	
	def get_cups(self):
		self.ser.reset_input_buffer()
		self.ser.reset_output_buffer()
		self.ser.write(b'?')
		self.cups = int(self.ser.read().strip().decode('ascii'))

	def reset_score(self):
		self.ser.write(b'X')
		self.get_cups()

class Matrix:
	def __init__(self, ser):
		self.ser = ser

	def send_score(self, score_p1, score_p2):
		self.ser.reset_output_buffer()
		self.ser.reset_input_buffer()
		self.ser.write('{}:'.format(score_p1).encode())
		self.ser.write('{}#'.format(score_p2).encode())
		print(str(score_p1) + ': ' + str(score_p2) + '#')
		# self.ser.write(b'!')
		self.ser.reset_output_buffer()
		print('matrix sent score')

	def start_game(self, name1='name1', name2='name2'):
		self.ser.write(b'start;')

	def quick_start(self):
		self.ser.reset_input_buffer()
		self.ser.write(b'faststart;')

	def terminate(self):
		self.ser.write(b'terminate;')

	def end(self):
		self.ser.write(b'end;')

	def set_playernames(self, p1name, p2name):
		self.ser.flushOutput()
		self.ser.flushInput()
		string = p1name.upper() + "$" + p2name.upper() + "%"
		
		self.ser.write(string.encode())
		self.ser.flushOutput()

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
	ports = serial_tools.comports()
	sers = {}
	for port in ports: 
		if port.name == 'ttyAMA0':
			continue
		print('found: {}'.format(port.device))
		baudrate = 38400 if port.name == 'ttyACM0' else 9600
		ser = serial.Serial(port.device, baudrate, timeout=1)
		time.sleep(2)
		if port.name.startswith('ttyUSB'):
			ser.write(b'i')
			uid = ser.readline().decode('ascii').strip()
		else:
			uid = 'ma'
		print("id " + uid)
		sers[uid] = ser 
	print(sers)
	return sers



def beerpong(player1, player2, matrix):
	## create the game ID
	beerpong_running = True
	global game_id
	game_id = '{:%Y-%m-%d_%H:%M:%S}'.format(datetime.datetime.now())
	print("Game ID: "+game_id)
	print('Player 1: ' + player1.name )
	print('Player 2: ' + player2.name )

	## reset scores and matrix
	player1.reset_score()
	player2.reset_score()
	matrix.end()

	matrix.quick_start()
	matrix.send_score(player1.score, player2.score)

	## start logfile
	# logfilename = "./logs/" + game_id + ".log"
	# log = open(logfilename, "w") 
	# log.write(game_id + "\n" + player1.name + "\n" + player2.name + "\n" )
	# log.close()
	
	try: 
		while beerpong_running:
		## check scores
			player1.get_cups()
			player2.get_cups()

			
			if player1.cups != player1.prev_cups or player2.cups != player2.prev_cups:
			## player score changed
			
			
				if player1.cups != player1.prev_cups:
					# anzahl becher bei p1 hat sich geandert -> p2 punktet
					player2.score = 10 - player1.cups
					matrix.send_score(player1.score, player2.score)
					# write_log(logfilename, player1.score, player2.score)
					player1.prev_cups = player1.cups
					print(player2.name +" scored! New Score: " + str(player1.score) +" : " + str(player2.score))

				if player2.score != player2.prev_cups:
					# p2 geandert -> p1 gepunktet
					player1.score = 10 - player2.cups
					matrix.send_score(player1.score, player2.score)
					# write_log(logfilename, player1.score, player2.score)
					player2.prev_cups = player2.cups
					print(player1.name +" scored! New Score: " + str(player1.score) +" : " + str(player2.score))

				


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


# def write_log(logfile, score1, score2):
# 	log = open(logfile, "a")
# 	log.write("{:%Y-%m-%d_%H:%M:%S }".format(datetime.datetime.now()) + str(score1) + ":" + str(score2) + "\n")
# 	log.close()

def end_game():
	player1.reset_score()
	player2.reset_score()
	print("Game " + game_id + " with players " + player1.name + " and " + player2.name + " ended.")
	matrix.end()
	return False


## MAIN Loop


if __name__ == "__main__":
	print("\n \nNew Game starts now!")	
	sers = init_ser()
	player1 = Player(sers['p1'], name='player1')
	player2 = Player(sers['p2'], name='player2')
	matrix = Matrix(sers['ma'])

	beerpong(player1, player2, matrix)
		
	
