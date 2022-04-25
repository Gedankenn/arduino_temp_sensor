<!DOCTYPE html>
<html>
<body>

<?php

/**
 * php codigo para inserir na tabela do banco de dados
 */


function insert($esp, $location, $temp, $humid)
{
	$sql = 'INSERT INTO SensorData(sensor,location,value1,value2) VALUES(:sensor,:location:value1:value2)';
	$stmt = $this->pdo->prepare($sql);


	$stmt->bindValue(':sensor', $esp);
	$stmt->bindValue(':location', $location);
	$stmt->bindValue(':value1', $value1);
	$stmt->bindValue(':value2', $value2);

	$stmt->execute();
}

function connect()
{
	$host 		= "localhost";
	$port 		= 5432;
	$dbname 	= "APS_BANCO";
	$user 		= "esp";
	$pass		= "espBostinha";
	$conStr = sprintf("pgsql:host=%s;port=%d;dbname=%s;user=%s;password=%s",
						$host,
						$port,
						$dbname,
						$user,
						$pass);

	$pdo = new\PDO($conStr);
	$pdo->SetAttribute(\PDO::ATTR_ERRMODE, \PDO::ERRMODE_EXCEPTION);

	return $pdo;
}

$api_key= $sensor = $location = $value1 = $value2 = "";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $api_key = test_input($_POST["api_key"]);
    if($api_key == $api_key_value) {
        $sensor = test_input($_POST["sensor"]);
        $location = test_input($_POST["location"]);
        $value1 = test_input($_POST["value1"]);
        $value2 = test_input($_POST["value2"]);
        
        // Create connection
        try
        {
        	$pdo = connect()
        	insert($sensor,$location,$value1,$value2);
        } catch(\PDOException $e)
        {
        	echo $e->getMessage();
        }

    }
    else {
        echo "Wrong API Key provided.";
    }

}
else {
    echo "No data posted with HTTP POST.";
}

?>

</body>
</html>