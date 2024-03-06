<!DOCTYPE html >
<html>
<head>
	<?php include 'php/header.php'; ?>
</head>
<body onLoad="changeProgressBar()">
	<header>
			<h1 id="headtext">Spieler</h1>
			<?php include 'php/navbar.php'; ?>
	</header>
	<div class="content" id="content">
		<div class="container" style="text-align:center; font-size:130%">
			<?php $player = isset($_GET['playerselect']) ? $_GET['playerselect'] : false;
			
			if($player){
				$db = new SQLite3('database/beerpong.db');
				
				$query = "SELECT COUNT(*) as count 
				FROM game
				WHERE 
				(playerone = '" . $player . "' AND playeronescore >= 10 ) OR
				(playertwo = '" . $player . "' AND playertwoscore >= 10 )
				";
				
				$result = $db->query($query);
				$siege = $result->fetchArray();
				
				$query = "SELECT COUNT(*) as count 
				FROM game
				WHERE 
				(playerone = '" . $player . "' AND playeronescore < 10 ) OR
				(playertwo = '" . $player . "' AND playertwoscore < 10 )
				";
				
				$result = $db->query($query);
				$niederlagen = $result->fetchArray();
				
				echo "Spieler <strong>" . $player . "</strong> ausgewählt.";
				echo "</br></br>";
				echo "Siege: <strong>" . $siege[0];
				echo "</strong></br></br>";
				echo "Niederlagen: <strong>" . $niederlagen[0];
				echo "</strong></br></br></br>";
				
				if($siege[0] + $niederlagen[0] == 0){
					echo "Noch keine Spiele gespielt!";
				}
				else{
										
					$command = '		
					<div class="progressbar">
					  <div class="progressbar_content" id="progressbar_content">25%
					  </div>
					  <div class="progressbar_background" id="progressbar_background" >
						75%</div>
					</div>';
					echo $command;
					
				}
				
				echo "</br></br></br>";
				
				echo "Letzte Spiele:</br></br>";
				
				$query = "
				SELECT id, playerone, playertwo, playeronescore, playertwoscore
				FROM game
				WHERE (playerone = '" . $player . "') OR
				(playertwo = '" . $player . "')
				ORDER BY id DESC
				LIMIT 3				
				";

				$result = $db->query($query);
				
				$counter = 1;
				while($row = $result->fetchArray()){
					$strarray = explode("_", $row['id']);
					$date = $strarray[0];
					$time = $strarray[1];
					$datearray = explode("-", $date);
					$timearray = explode(":", $time);
					echo '<div class="listentry" style="font-weight:normal" onclick="toggleDisplay(';
					echo "'";
					echo $row['id'];
					echo "'";
					echo ')" >Spiel ';
					echo $counter;
					echo '<div class="listdetail" style="font-size:80%" id="';
					echo $row['id'];
					echo '">';
					$str = $datearray[2] . "." . $datearray[1] . "." . $datearray[0] . " " . $timearray[0] . ":" . $timearray[1] . " Uhr";
					echo $str;
					echo '</br>';
					echo $row['playerone'];
					echo ' vs ';
					echo $row['playertwo'];
					echo '</br>';
					echo $row['playeronescore'];
					echo ' : ';
					echo $row['playertwoscore'];
					echo '</div></div>';
					$counter++;
				}			

				
			}
			else{
				echo "Kein Spieler ausgewählt!</br>
					  Zurück zur Auswahl";
			}
			?>
		</div>
	</div>
	<footer>
		<p>copyright Jannik Schmitt &copy; </p>
	</footer>
	<script language="JavaScript">
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
	
	function changeProgressBar(){
	var siege = <?php echo $siege[0]; ?>;
	var niederlagen = <?php echo $niederlagen[0]; ?>;

	var anteilsiege = siege / (siege + niederlagen);
	anteilsiege = anteilsiege.toFixed(2) * 100;
	var str_anteilsiege = String(anteilsiege) + "%";
	var bar = document.getElementById("progressbar_content")
	bar.innerHTML = str_anteilsiege;
	bar.style.width = str_anteilsiege;

	var anteilniederlagen = 100 - anteilsiege;
	var str_anteilniederlagen = String(anteilniederlagen) + "%";

	bar = document.getElementById("progressbar_background")
	bar.innerHTML = str_anteilniederlagen;
	bar.style.width = str_anteilniederlagen;
	}
	
	</script>
</body>
</html>
