<?php  // READS THE TEXT FILE WHERE I STORE THE SLIDER DATA.
$filename = "status.txt";
$content = file_get_contents($filename);

if ($content === false) {
    echo "Error: Unable to read file.";
} else {
    // echo $content;
}
?>
