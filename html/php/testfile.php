<?php 
	$longstr = "2017.05.01 15:24:41";
	
	$strarray = explode(" ", $longstr);
	
	$date = $strarray[0];
	$time = $strarray[1];
	
	$datearray = explode(".", $date);
	$timearray = explode(":", $time);
	
	$year = $datearray[0];
	$month = $datearray[1];
	$day = $datearray[2];
	
	echo $day;
	echo "</br>";
	echo $month;
	echo "</br>";
	echo $year;
		
?>