<?php 
	$newplayer = isset($_POST['newplayer']) ? $_POST['newplayer'] : false;
	
	$db = new SQLite3('../database/beerpong.db');	
	
	$result = $db->query("SELECT name FROM player");
	
	$i = 0;
	
	while($row = $result->fetchArray()){
		$playerlist[$i] = $row['name'];
		$i++;
	}
	
	if($newplayer){
		if(in_array($newplayer, $playerlist)){
			echo '<script type="text/javascript" language="Javascript">  
			alert("Spieler existiert bereits!")  
			</script> ';
		}
		else{
			echo '<script type="text/javascript" language="Javascript">  
			alert("neuer Spieler eingetragen!")  
			</script> ';
			
			$query = "INSERT INTO player (name) VALUES ('" . $newplayer . "')";
						
			$db->exec($query);					
		}
	}
	echo '<meta http-equiv="refresh" content="1; URL=/admin.php">';
?>

