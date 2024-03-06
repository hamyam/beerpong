<!DOCTYPE html >
<html>
<head>
	<?php include 'php/header.php'; ?>
</head>
<body>
	<header>
			<h1 id="headtext">Spieler</h1>
			<?php include 'php/navbar.php'; ?>
	</header>
	<div class="content" id="content">
		<div class="container" style="text-align:center">
			<h2>Spieler auswählen</h2>
			<form action="spieler.php" method="GET">
				</br>
				<p>Spiel auswählen:</p>
				<?php include 'php/playerselect.php'; ?>
				</br>
				</br>
				<button class="button" type="submit" name="submit">Details</button>
			</form>
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

</script>
</body>
</html>
