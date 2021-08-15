
<?php 
	$db = new SQLite3('database/beerpong.db');	
	$result = $db->query("SELECT id, playerone, playertwo FROM game");
	
	echo '<select name="id" class="playerselect">';	
	while($row = $result->fetchArray()){
		echo '<option value="';
		echo $row['id'];
		echo '">';
		
		$strarray = explode("_", $row['id']);
		$date = $strarray[0];
		$time = $strarray[1];
		$datearray = explode("-", $date);
		$timearray = explode(":", $time);
		$str = $datearray[2] . "." . $datearray[1] . " " . $timearray[0] . ":" . $timearray[1] . " Uhr \n" . $row['playerone'] . " vs " . $row['playertwo'] ;
		echo $str;
		echo '</option>';
	}	
	echo "</select>";
?>