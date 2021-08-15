<!DOCTYPE html >
<html>
<head>
	<?php include 'php/header.php'; ?>
</head>
<body>
	<header>
			<h1 id="headtext">Spieldetails</h1>
			<?php include 'php/navbar.php'; ?>
	</header>
	<div class="content" id="content">
		<div class="container" style="text-align:center; font-size:130%">
			<?php
				$spiel = isset($_GET['id']) ? $_GET['id'] : false;
							
				if($spiel){
					
					$db = new SQLite3('database/beerpong.db');
					
					$query = "SELECT playerone, playeronescore, playertwo, playertwoscore 
					FROM game
					WHERE 
					id = '" . $spiel . "'";
					
					$result = $db->query($query);

					$data = $result->fetchArray();
					
					$strarray = explode("_", $spiel);
					$date = $strarray[0];
					$time = $strarray[1];
					$datearray = explode("-", $date);
					$timearray = explode(":", $time);
					$str = $datearray[2] . "." . $datearray[1] . "." . $datearray[0] . " " . $timearray[0] . ":" . $timearray[1] . " Uhr";	
					echo "<h4>Spiel: ";
					echo $str;
					echo "</h4	>";
					
					$str = "<p>Spieler 1: " . $data['playerone'] . 
						   "</p><p>Spieler 2: " . $data['playertwo'] . 
						   "</p><p>Endstand:</br>" . $data['playeronescore'] . ":" . $data['playertwoscore'] .
						   "</p>";			
					echo $str;
					
					$logpath = "gamelog/" . $spiel;
					$handle = fopen($logpath, "r");
					
					if($handle){
						while(($buffer = fgets($handle, 1024)) !== false){

							$strarray = explode(",", $buffer);
							$sec[$counter] = intval($strarray[0]);
							
							$scorearray = $strarray[1];

							$score = explode(":", $scorearray);
							$scorep1[$counter] = intval($score[0]);
							$scorep2[$counter] = intval($score[1]);	
							
							$counter ++;
						}	
						if(!feof($handle)){
							echo "Fehler: unerwarteter fgets() Fehlschlag\n";
						}
						fclose($handle);
					}
				}
				else{
					echo "<h2>Kein Spiel ausgewaehlt.</h2>";
				}
			?>
			
			<div class="spielverlauf">
			<svg height="100%" width="100%">

			<rect id="blue" width="15%" height="110%" x="35%" y="-5%"></rect>
			<rect id="green" width="15%" height="110%" x="50%" y="-5%"></rect>
			<!-- Player 1 Hits -->
			<circle id="p1_1" cy="3%" cx="42.5%" r="4%"></circle>
			<circle id="p1_2" cy="6.2%" cx="42.5%" r="4%"></circle>
			<circle id="p1_3" cy="12%" cx="42.5%" r="4%"></circle>
			<circle id="p1_4" cy="22%" cx="42.5%" r="4%"></circle>
			<circle id="p1_5" cy="45%" cx="42.5%" r="4%"></circle>
			<circle id="p1_6" cy="56%" cx="42.5%" r="4%"></circle>
			<circle id="p1_7" cy="70%" cx="42.5%" r="4%"></circle>
			<circle id="p1_8" cy="72%" cx="42.5%" r="4%"></circle>
			<circle id="p1_9" cy="80%" cx="42.5%" r="4%"></circle>
			<circle id="p1_10" cy="100%" cx="42.5%" r="4%"></circle>

			<!-- Player 2 Hits -->
			<circle id="p2_1" cy="1%" cx="57.5%" r="4%"></circle>
			<circle id="p2_2" cy="9.2%" cx="57.5%" r="4%"></circle>
			<circle id="p2_3" cy="13%" cx="57.5%" r="4%"></circle>
			<circle id="p2_4" cy="24%" cx="57.5%" r="4%"></circle>
			<circle id="p2_5" cy="29%" cx="57.5%" r="4%"></circle>
			<circle id="p2_6" cy="38%" cx="57.5%" r="4%"></circle>
			<circle id="p2_7" cy="42%" cx="57.5%" r="4%"></circle>

			</svg>
			</div>
			
		</div>
	</div>
	<footer>
		<p>copyright Jannik Schmitt &copy; </p>
	</footer>
	<script>
	function showNavbar(){
		var x = document.getElementById("navbar");
		if (x.className === "topnav"){
			x.className += " responsive";
		}
		else {
			x.className = "topnav";
		}
	}

	function toggleDisplay(id){
	var x = document.getElementById(id);
	if(x.style.display === 'none'){
		x.style.display = 'block';
	}
	else {
		x.style.display = 'none';
	}
	}
	
	</script>
</body>
</html>
