<?php
    $Temp=$_POST["temperature"];
    //Create a variable that will receive the temperature
    $Humidity=$_POST["humidity"];
    //Create a variable that will receive the humidity
    $Write="<p>Temperature:" . $Temp . "Celcius </p>". "<p>Humidity: " . $Humidity . "% </p>";
    //Create a html file to display the data within
    file_put_contents('sensor.html', $Write);
?>