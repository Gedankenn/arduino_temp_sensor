<?php

$db = pg_connect("host=localhost port=5432 dbname=APS_BANCO user=esp password=espBostinha");
$api_key_value = tPmAT5Ab3j7F9;

if($_SERVER["REQUEST_METHOD"] == "POST")
{
	$api_key = test_input($_POST["api_key"]);
	if($api_key_value == $api_key)
	{
		$sensor = test_input($_POST["sensor"]);
		$location = test_input($_POST["location"]);
		$value1 = test_input($_POST["value1"]);
		$value2 = test_input($_POST["value2"]);
		$query = "INSERT INTO SensorData VALUES ($sensor, $location, $value1, $value2])";
		$result = pg_query($query);
	}
	function test_input($data) {
	    $data = trim($data);
	    $data = stripslashes($data);
	    $data = htmlspecialchars($data);
	    return $data;
	}
}

?>