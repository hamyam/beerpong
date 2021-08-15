<?php 
	$playerone = isset($_POST['playeroneselect']) ? $_POST['playeroneselect'] : false;
	$playertwo = isset($_POST['playertwoselect']) ? $_POST['playertwoselect'] : false;
	
	if($playerone === $playertwo){
		echo '<script type="text/javascript" language="Javascript">  
		alert("Alleine trinken ist nicht gut!")  
		</script> ';
		$playerone = false;
	}
	
	if($playerone && $playertwo){
		echo '<script type="text/javascript" language="Javascript">
		alert("Player 1: ';
		echo $playerone;
		echo '\nPlayer 2: ';
		echo $playertwo;
		echo '")</script>';
		
		$command = "sudo /var/www/html/python/beerpong_v2.py " . $playerone . " " . $playertwo . " 0";
		
		echo $command;
		
		echo shell_exec( $command);
		
	}	
	
	echo '<meta http-equiv="refresh" content="1; URL=/index.php">';
?>

