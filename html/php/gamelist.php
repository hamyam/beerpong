<?php 
	$db = new SQLite3('database/beerpong.db');	
	$result = $db->query("
		SELECT id, playerone, playertwo, playeronescore, playertwoscore 
		FROM game 
		ORDER BY id DESC
		");
	
	$counter = 1;
	while($row = $result->fetchArray()){
		$strarray = explode("_", $row['id']);
		$date = $strarray[0];
		$time = $strarray[1];
		$datearray = explode("-", $date);
		$timearray = explode(":", $time);
		$str = $datearray[2] . "." . $datearray[1] . "." . $datearray[0] . " " . $timearray[0] . ":" . $timearray[1] . " Uhr";
		
		echo '<div class="listentry" onclick="toggleDisplay(';
		echo "'";
		echo $row['id'];
		echo "'";
		echo ')" >';
		echo $str;
		echo '<div class="listdetail" id="';
		echo $row['id'];
		echo '">';	
		echo $row['playerone'];
		echo ' vs ';
		echo $row['playertwo'];
		echo '</br>';
		echo $row['playeronescore'];
		echo ' : ';
		echo $row['playertwoscore'];
		echo '</br>';
		echo '<a class="button" id="linkbutton" href="spiel.php?id=';
		echo $row['id'];
		echo '">Details</a>';		
		
		echo '</div></div>';
		$counter++;
	}	
?>