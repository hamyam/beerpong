<?php 
	$player = isset($_POST['playerselect']) ? $_POST['playerselect'] : false;
	
	$db = new SQLite3('../database/beerpong.db');
	
	if($player){
		echo '<script type="text/javascript" language="Javascript">
		alert("entferne Player: ';
		echo $player;
		echo '")</script>';
		// HERE GOES THE SQL EXEC!
		
		$query = "DELETE FROM player WHERE name = '" . $player . "'";
		
		//echo $query;
		
		$db->exec($query);	
		
	}
	else{
		echo '<script type="text/javascript" language="Javascript">  
		alert("Kein Spieler ausgewählt")  
		</script> ';
		$playerone = false;
	}
	
	echo '<meta http-equiv="refresh" content="1; URL=/admin.php">';
?>

