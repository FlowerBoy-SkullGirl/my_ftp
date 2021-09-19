<html>
<p>Hello login page</p>

<?php
$hostname = "localhost";
$username = "damean";
$password = "4414";
$db = "dweb";

$conn = new mysqli_connect($hostname, $username, $password, $db);

if ($conn->connect_error){
	die("Connection failed: " . $dbconnect->connect_error);
}
echo "Connected successfully";

/*
$usrname = $_POST["usrname"];
$passwd = $_POST["passwd"];

$stmt = $sql->prepare( "SELECT uid, uname, password FROM logins WHERE uname=? AND password=?");
$stmt -> bind_param("ss", $usrname, $passwd);
$stmt -> execute();
$result = $conn->query($sql); 
*/
?>



</html>