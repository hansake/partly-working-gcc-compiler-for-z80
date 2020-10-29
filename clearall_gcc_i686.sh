#!/bin/bash

# Clear all built files and directories
BUILD_TYPE=gcc_i686
find . -name "$BUILD_TYPE-*" -printf "%f " > temp_filenames_to_delete
FILES_TO_DELETE=( "$(cat temp_filenames_to_delete)" )
rm temp_filenames_to_delete
if [ -z "$FILES_TO_DELETE" ]; then
  echo "No files to delete"
  exit 1
fi
echo "Do you want to delete these files and directories:"
echo "  $FILES_TO_DELETE"
read -p "Are you sure? " -n 1 -r
if [[ ! $REPLY =~ ^[Yy]$ ]]
then
  echo ""
  echo "Not deleting"
  exit 1
else
  echo ""
  echo "Ok, deleting"
fi
rm -f -r $FILES_TO_DELETE
