<?php      // puts the text data on the text document
header('Content-Type: application/json');

// Check if the request method is PUT
if ($_SERVER['REQUEST_METHOD'] === 'PUT') {
    // Read the raw input stream
    $input = file_get_contents('php://input');
    // Decode the JSON data
    $data = json_decode($input, true); // true for associative array

    if ($data === null) {
        http_response_code(400); // Bad Request
        echo json_encode(['message' => 'Invalid JSON data']);
        exit;
    }

    $newName = $data['Node1'] ?? null;
    $newStatus = $data['Node1'] ?? null;

    $response = [
        'htmlText'=> "Last time you clicked was: {$newStatus}",
        'received_data' => $jsonData];
    $trimmed_input = trim($input, '"');
     echo json_encode($trimmed_double);

    $myfile = fopen("status.txt","w") or die("Unable to open file!");
    fwrite($myfile, $trimmed_input);
    fclose($myfile);
    
    
} else {
    http_response_code(405); // Method Not Allowed
    echo json_encode(['message' => 'Only PUT requests are allowed']);
}
