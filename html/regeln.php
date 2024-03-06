<!DOCTYPE html >
<html>
<head>
	<?php include 'php/header.php'; ?>
</head>
<body>
	<header>
			<h1 id="headtext">Regeln</h1>
			<?php include 'php/navbar.php'; ?>
	</header>
	<div class="content" id="content" style="text-align:justify">
		<div class="container">
			<h2>Ausstatung / Equipment</h2>
			<ul class="bullet-list">
				<li>244 x 61 x 73 cm Tischformat</li>
				<li>16oz (474ml) Becher</li>
				<li>40mm Beer Pong Bälle</li>
			</ul>
		</div>
		<div class="container">
			<h2>Sportliches Verhalten</h2>
			<p>Beer Pong ist ein besonderer und einzigartiger Zeitvertreib, bei welchem es dennoch hin und wieder hoch hergehen kann. Deshalb ist es wichtig, dass Du immer daran denkst, dass Beer Pong ein Spiel ist, das in erster Linie dafür gedacht ist Spass zu haben! Respektiere deshalb nicht nur das Spiel, sondern ebenso und besonders deine Mit- und Gegenspieler, denn jeder soll beim Beer Pong Spass haben!</p>
		</div>
		<div class="container">
			<h2>Vorbereitung & Spielbeginn</h2>
			<ul class="bullet-list">
				<li>2 Spieler pro Team; 2 Bälle pro Spiel; 2 Würfe pro Team in jeder Runde (Ausnahme: Angabe)</li>
				<li>10 Becher auf jeder Seite des Tisches; angeordnet als Pyramide mit der Spitze Richtung Tischmitte; hinterste Reihe der Pyramide sollte max. 1-2 cm von der Tischkante entfernt sein; rutschen die Becher aus der Formation, sollten sie vor dem nächsten Wurf wieder in Position gebracht werden</li>
				<li>Angabe: Zur Angabe werden die Bälle geteilt; jedes Team erhält einen Ball; die Werfer werfen gleichzeitig auf die Becher und schauen sich dabei aber in die Augen. Das Team des Spielers der den Becher trifft darf das Spiel eröffnen</li>
			</ul>
		</div>
		<div class="container">
			<h2>Spielverlauf</h2>
			<ul class="bullet-list">
				<li>2 Re-Racks pro Team und pro Spiel; Re-Racks erlaubt bei 6, 4 und 3 verbleibenden Bechern; der letzte Becher muss immer mittig an der Tischkante positioniert werden.</li>
				<li>Bring-Backs: treffen beide Spieler einen Cup während einer Runde, bekommen sie die Bälle zurück und dürfen nochmals werfen.</li>
				<li>Der Ball darf nur berührt werden, wenn er zuvor den Tisch oder einen Cup berührt hat; wird der Ball zuvor berührt erfolgt eine Penalty-Strafe.</li>
				<li>Bounce Shot: Springt der Ball auf dem Tisch auf bevor er in einen Becher geht, muss das gegnerische Team 2 Becher trinken; sobald der Ball den Tisch berührt hat, darf jedoch das verteidigende Team den Ball wegschlagen.</li>
				<li>Fällt ein Cup wegen eines Balles um, muss dieser wieder aufgestellt werden (Ausnahme: Ball war offensichtlich im Cup).</li>
				<li>Ablenkungsmanöver sind erlaubt; Becher, Tisch und Gegner dürfen dabei nicht berührt werden; Erzeugen eines Windstoßes ist verboten.</li>
				<li>Sich drehende Bälle im Cup dürfen aus diesem entweder "gefingert" (Männer) oder "geblasen" werden.</li>
			</ul>
		</div>		
		<div class="container">
			<h2>Spielende</h2>
			<ul class="bullet-list">
				<li>Rebuttal: Ist der letzte Becher eines Teams getroffen, hat das gegnerische Team nochmals die Chance zu werfen bis sie keinen Becher mehr trifft.</li>
				<li>Wurde beim Rebuttal der letzte Becher getroffen geht es in die Verlängerung: 3 Becher auf jeder Seite als Pyramide; gleicher Inhalt auf beiden Seiten.</li>
				<li>Death Cup: Becher, der zuvor getroffen wurde aber nicht vom Tisch genommen oder ausgetrunken wurde; wird dieser Becher getroffen ist das Spiel sofort vorbei.</li>
				<li>Das verbleibende Bier auf dem Tisch kann vom Verlierer-Team getrunken werden.</li>
				</ul>
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
