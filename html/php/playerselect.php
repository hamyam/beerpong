
<?php 
	$db = new SQLite3('database/beerpong.db');	
	$result = $db->query("SELECT name FROM player");
	
	echo '<select name="playerselect" class="playerselect">';	
	while($row = $result->fetchArray()){
		echo '<option value="';
		echo $row['name'];
		echo '">';
		echo $row['name'];
		echo '</option>';
	}	
	echo "</select>";
?>