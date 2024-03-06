<!DOCTYPE html >
<html>


<head>
	<?php include 'php/header.php'; ?>
</head>
<body>
	<header>
			<h1 id="headtext">Home</h1>
			<?php include 'php/navbar.php'; ?>
	</header>
	<div class="content">
	
		<div class="container" id="title">
			<img id="titleimage" src="img/beer_pong_logo.png">
		</div> 		
		<div class="container" style="text-align:center">
			<!-- Button to open New game Popup -->
				<button class="button" id="startnewgame" onclick="document.getElementById('new_game_popup').style.display='block'">Start new game!</button>
			<!-- The Popup -->
				<div id="new_game_popup" class="modal">
					<form class="modal-content animate" action="php/startgame.php" method="post">
						<!-- close Button -->
						<span class="close" onclick="document.getElementById('new_game_popup').style.display='none'" title="Close Popup">&times;</span>
						<img src="img/cup_logo.png" class="logo_medium">
						<div class="container">
							<p>Select Player 1</p>
							<?php include 'php/playeroneselect.php'; ?>
							</br>
							<p>Select Player 2</p>
							<?php include 'php/playertwoselect.php'; ?>
							</br>
							</br>
							<div class="container-small">
								<button type="submit" name="submit" class="button" id="startbutton">start</button>
							</div>
							<div class="container-small">						
								<button type="button" class="button" id="cancelbutton" onclick="document.getElementById('new_game_popup').style.display='none'">cancel</button>
							</div>
						</div>					
					</form>
				</div>	
		</div>
		
		<div class="container">
			<!-- EINTRAEGE PER PHP GENERIERT -->
			<?php include 'php/gamelist.php'; ?>
			<div style="margin:auto; text-align:center">
			<button class="button" onclick="showMoreGames(2)">> 5 mehr Spiele <</button>
			</div>
		</div>
		
		<div class="container">
			<div class="listentry" onclick="toggleDisplay('howto')">
				HowTo				
				<div class="listdetail" id='howto' style="font-size:80%; text-align:justify">
					</br>
					<ul class="bullet-list" >
						<li>Um ein neues Spiel zu starten, den Button "Start new game!" auf dem Home Screen drücken.</br>
						Im folgenden Menü die beiden Spieler auswählen die gegeneinander antreten und bestätigen.</br>
						ACHTUNG: das laufende Spiel wird dabei beendet!</li>
						<li>Falls dein Spielername noch nicht in der Liste vorhanden ist, wende dich an Jannik. Einen neuen Spieler eintragen dauert nur ein paar Sekunden!</li>
						<li>Du kannst dir deine Statistiken im "Spieler" Menü anschauen. Dort werden auch die Details der letzten 3 Spiele angezeigt.</li>
						<li>Bugreports, Featurerequests und Feeback kann gerne über das Formular "Feedback" abgegeben werden.</li>
						<li>Der Turniermodus wird derzeit noch nicht unterstützt.</li>
						
					
					</ul>
				</div>			
			</div>
		</div>
		
	</div>
	<footer>
		<p>copyright Jannik Schmitt &copy; </p>
	</footer>
	<script>
// Get the modal
var modal = document.getElementById('new_game_popup');

// When the user clicks anywhere outside of the modal, close it
window.onclick = function(event) {
    if (event.target == modal) {
        modal.style.display = "none";
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

function showNavbar(){
	var x = document.getElementById("navbar");
	if (x.className === "topnav"){
		x.className += " responsive";
	}
	else {
		x.className = "topnav";
	}
}


function showMoreGames(counter){
	var x = document.getElementsByClassName("listentry");
	
	for(var i = 0; i < x.length; i++){
		if(i <= counter){
			if(x[i].style.display == 'none'){
				x[i].style.display = "block";
				counter ++;
			}
		}
	}	
}



</script>
</body>
</html>
