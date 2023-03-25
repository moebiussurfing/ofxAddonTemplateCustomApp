#!/bin/bash

# Clean_OF_Project.sh :
# This script cleans the OF VS Project folder.
# Then we need to re run the OF Project Generator.

echo "---------------------------------------------------------------------------------------------------------"
echo -e "\n"
echo "RUNNING SCRIPT TO REMOVE PROJECT FILES."
echo "Then we need to re run the OF Project Generator."
# Get the directory where the script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Get the directory where the script was called from
CALLING_DIR="$(pwd)"
echo -e "\n"

echo "Script directory: $SCRIPT_DIR"
echo "Calling directory: $CALLING_DIR"
echo -e "\n"


# Get dir
# folder_path="$(dirname "$0")"
# Assign the value of SCRIPT_DIR to folder_path
folder_path="$CALLING_DIR"

echo "WORKING ON PATH: &folder_path"
echo -e "\n"


# List
find "$folder_path" -type d | while read folder; do
    echo "$folder"
done


# Removing .vs y obj
echo "Removing /.vs and /obj..."
if ! find "$folder_path" \( -name ".vs" -o -name "obj" \) -type d -exec rm -r "{}" \; ; then
    echo "Error: Could not find .vs or obj directory."
fi


# Removing addons/
echo "Removing /addons..."
if ! find "$folder_path" \( -name "addons" \) -type d -exec rm -r "{}" \; ; then
    echo "Error: Could not find addons directory."
fi


# Remove .sln, .vcxproj, .filters, .user
if ! find . \( -iname "*.sln" -o -iname "*.vcxproj" -o -iname "*.filters" -o -iname "*.user" \) -type f -delete ; then
    echo "Error: Could not find Visual Studio project files."
fi
echo "Removed VS Project files."


# Clean /bin
echo "Clean `/bin/`. Remove all except `/bin/data`"
cd "$folder_path/bin"
# Delete all files and directories except the data directory
shopt -s extglob
rm -rf !(data)
echo "Deleted all files and directories in `bin/` except `bin/data`"


# Clean /bin/data
echo "Clean `/data`. Remove all except `/bin/data/assets/`"
# Go to the bin/data directory
cd "$folder_path/bin/data"
# Delete all files and directories except the assets directory
shopt -s extglob
rm -rf !(assets)
echo "Deleted all files and directories in `bin/data` except `data/assets.`"


# back
cd "$folder_path"

echo -e "\n"
echo -e "Done!\n"
echo "---------------------------------------------------------------------------------------------------------"
