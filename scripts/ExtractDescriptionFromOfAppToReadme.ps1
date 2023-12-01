# Define the file name
$fileName = ".\src\ofApp.h"

# Read the file content
$fileContent = Get-Content $fileName -Raw

# Find the first block of text between /* and */
$pattern = "/\*.*?\*/"
$block = [regex]::Match($fileContent, $pattern).Value

# Print the block to the console before cleaning
Write-Host "Block before cleaning: $block"

# Remove the /* and */ symbols
$block = $block.TrimStart("/*").TrimEnd("*/")

# Split the block into lines
$lines = $block -split "`n"

# Process each line
for ($i = 0; $i -lt $lines.Length; $i++) {
    # If the line starts with *, remove it
    if ($lines[$i].TrimStart().StartsWith("*")) {
        $lines[$i] = $lines[$i].TrimStart("*").TrimStart()
    }
}

# Join the lines back together
$description = $lines -join "`n"

# Print the description to the console
Write-Host "Description: $description"

# Define the README file name
$readmeFileName = ".\README.md"

# Write the description to the README file
$description | Out-File $readmeFileName

# Add the image reference to the README file
$imageReference = "`n!`n"
Add-Content $readmeFileName $imageReference
